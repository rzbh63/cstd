#ifndef __MEMEXEC_H__
#define __MEMEXEC_H__
//ֱ�������ڴ��еĳ���
// Windows��PE�����������������ʱ�򣬻Ὣ�����ϵ��ļ����ص��ڴ棬Ȼ�����ܶ�������纯��������ض�λ������Ԥ����֮��ġ���λ���ֵ������Լ�д��һ��PE��������ֱ�ӽ��ڴ��еĳ����������ǵ���ǰ�ġ���ɫ���롱����Ҳ����ͬ�����ԡ�
// ֱ�������ڴ��еĳ����൱�ڼ���һ���ǣ����԰ѳ�����ܱ��棬����ʱ���ܵ��ڴ棬Ȼ���������������������ƽ��ѶȻ�Ҫ��΢���ӵ㡣�����˼Ұ��ڴ��еĽ���DUMP����Ȼ���޸������ͱ��ϳ����ˡ�
#include <windows.h>
typedef IMAGE_SECTION_HEADER(*PIMAGE_SECTION_HEADERS)[1];
// ��������Ĵ�С
unsigned long GetAlignedSize(unsigned long Origin, unsigned long Alignment)
{
  return (Origin + Alignment - 1) / Alignment * Alignment;
}
// �������pe��������Ҫռ�ö����ڴ�
// δֱ��ʹ��OptionalHeader.SizeOfImage��Ϊ�������Ϊ��˵�еı��������ɵ�exe���ֵ����0
unsigned long CalcTotalImageSize(PIMAGE_DOS_HEADER MzH, unsigned long FileLen, PIMAGE_NT_HEADERS peH, PIMAGE_SECTION_HEADERS peSecH)
{
  int i;
  unsigned long res;
  // ����peͷ�Ĵ�С
  res = GetAlignedSize(peH->OptionalHeader.SizeOfHeaders, peH->OptionalHeader.SectionAlignment);
  // �������нڵĴ�С
  for (i = 0; i < peH->FileHeader.NumberOfSections; ++i) {
    // �����ļ���Χ
    if (peSecH[i]->PointerToRawData + peSecH[i]->SizeOfRawData > FileLen) {
      return 0;
    }
    else if (peSecH[i]->VirtualAddress) { //��������ĳ�ڵĴ�С
      if (peSecH[i]->Misc.VirtualSize) {
        res = GetAlignedSize(peSecH[i]->VirtualAddress + peSecH[i]->Misc.VirtualSize, peH->OptionalHeader.SectionAlignment);
      }
      else {
        res = GetAlignedSize(peSecH[i]->VirtualAddress + peSecH[i]->SizeOfRawData, peH->OptionalHeader.SectionAlignment);
      }
    }
    else if (peSecH[i]->Misc.VirtualSize < peSecH[i]->SizeOfRawData) {
      res += GetAlignedSize(peSecH[i]->SizeOfRawData, peH->OptionalHeader.SectionAlignment);
    }
    else {
      res += GetAlignedSize(peSecH[i]->Misc.VirtualSize, peH->OptionalHeader.SectionAlignment);
    }// if_else
  }// for
  return res;
}
// ����pe���ڴ沢�������н�
BOOL AlignPEToMem(void* pBuf, long Len, PIMAGE_NT_HEADERS* ppeH, PIMAGE_SECTION_HEADERS* ppeSecH, void** pMem, unsigned long* pImageSize)
{
  PIMAGE_DOS_HEADER SrcMz;// DOSͷ
  PIMAGE_NT_HEADERS SrcPeH;// PEͷ
  PIMAGE_SECTION_HEADERS SrcPeSecH;// �ڱ�
  SrcMz = (PIMAGE_DOS_HEADER)pBuf;
  if (Len < sizeof(IMAGE_DOS_HEADER)) {
    return FALSE;
  }
  if (SrcMz->e_magic != IMAGE_DOS_SIGNATURE) {
    return FALSE;
  }
  if (Len < SrcMz->e_lfanew + (long)sizeof(IMAGE_NT_HEADERS)) {
    return FALSE;
  }
  SrcPeH = (PIMAGE_NT_HEADERS)((int)SrcMz + SrcMz->e_lfanew);
  if (SrcPeH->Signature != IMAGE_NT_SIGNATURE) {
    return FALSE;
  }
  if ((SrcPeH->FileHeader.Characteristics & IMAGE_FILE_DLL) ||
      (SrcPeH->FileHeader.Characteristics & IMAGE_FILE_EXECUTABLE_IMAGE == 0) ||
      (SrcPeH->FileHeader.SizeOfOptionalHeader != sizeof(IMAGE_OPTIONAL_HEADER))) {
    return FALSE;
  }
  SrcPeSecH = (PIMAGE_SECTION_HEADERS)((int)SrcPeH + sizeof(IMAGE_NT_HEADERS));
  *pImageSize = CalcTotalImageSize(SrcMz, Len, SrcPeH, SrcPeSecH);
  if (*pImageSize == 0) {
    return FALSE;
  }
  *pMem = VirtualAlloc(NULL, *pImageSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);  // �����ڴ�
  if (*pMem != NULL) {
    // ������Ҫ���Ƶ�PEͷ�ֽ���
    int i;
    void* Pt;
    unsigned long l = SrcPeH->OptionalHeader.SizeOfHeaders;
    for (i = 0; i < SrcPeH->FileHeader.NumberOfSections; ++i) {
      if ((SrcPeSecH[i]->PointerToRawData) && (SrcPeSecH[i]->PointerToRawData < l)) {
        l = SrcPeSecH[i]->PointerToRawData;
      }
    }
    memmove(*pMem, SrcMz, l);
    *ppeH = (PIMAGE_NT_HEADERS)((int) * pMem + ((PIMAGE_DOS_HEADER) * pMem)->e_lfanew);
    *ppeSecH = (PIMAGE_SECTION_HEADERS)((int) * ppeH + sizeof(IMAGE_NT_HEADERS));
    Pt = (void*)((unsigned long)(*pMem) + GetAlignedSize((*ppeH)->OptionalHeader.SizeOfHeaders, (*ppeH)->OptionalHeader.SectionAlignment));
    for (i = 0; i < (*ppeH)->FileHeader.NumberOfSections; ++i) {
      // ��λ�ý����ڴ��е�λ��
      if ((*ppeSecH)[i]->VirtualAddress) {
        Pt = (void*)((unsigned long)(*pMem) + (*ppeSecH)[i]->VirtualAddress);
      }
      if ((*ppeSecH)[i]->SizeOfRawData) {
        // �������ݵ��ڴ�
        memmove(Pt, (const void*)((unsigned long)(SrcMz) + (*ppeSecH)[i]->PointerToRawData), (*ppeSecH)[i]->SizeOfRawData);
        if ((*ppeSecH)[i]->Misc.VirtualSize < (*ppeSecH)[i]->SizeOfRawData) {
          Pt = (void*)((unsigned long)Pt + GetAlignedSize((*ppeSecH)[i]->SizeOfRawData, (*ppeH)->OptionalHeader.SectionAlignment));
        }
        else { // pt ��λ����һ�ڿ�ʼλ��
          Pt = (void*)((unsigned long)Pt + GetAlignedSize((*ppeSecH)[i]->Misc.VirtualSize, (*ppeH)->OptionalHeader.SectionAlignment));
        }
      }
      else {
        Pt = (void*)((unsigned long)Pt + GetAlignedSize((*ppeSecH)[i]->Misc.VirtualSize, (*ppeH)->OptionalHeader.SectionAlignment));
      }
    }
  }
  return TRUE;
}
typedef void* (__stdcall* pfVirtualAllocEx)(unsigned long, void*, unsigned long, unsigned long, unsigned long);
pfVirtualAllocEx MyVirtualAllocEx = NULL;
// ��ʼ��
int CInit1()
{
  if (NULL == MyVirtualAllocEx) {
    MyVirtualAllocEx = (pfVirtualAllocEx)GetProcAddress(GetModuleHandle("Kernel32.dll"), "VirtualAllocEx");
  }
  return 0;
}
BOOL IsNT()
{
  CInit1();
  return MyVirtualAllocEx != NULL;
}
// ������ǳ���������
char* PrepareShellExe(char* CmdParam, unsigned long BaseAddr, unsigned long ImageSize)
{
  if (IsNT()) {
    char* pBuf = (char*)malloc(256);
    memset(pBuf, 0, 256);
    GetModuleFileName(0, pBuf, 256);
    if (strlen(CmdParam) > 0) {
      strcat(pBuf, " ");
      strcat(pBuf, CmdParam);
    }
    return pBuf; // ��ǵ��ͷ��ڴ�;-)
  }
  else {
    // Win98�µĴ�����ο�ԭ��;-)
    // http://community.csdn.net/Expert/topic/4416/4416252.xml?temp=8.709133E-03
    return NULL;
  }
}
// �Ƿ�������ض����б�
BOOL HasRelocationTable(PIMAGE_NT_HEADERS peH)
{
  return (peH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress)
      && (peH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);
}
#pragma pack(push, 1)
typedef struct {
  unsigned long VirtualAddress;
  unsigned long SizeOfBlock;
}* PImageBaseRelocation;
#pragma pack(pop)
// �ض���PE�õ��ĵ�ַ
void DoRelocation(PIMAGE_NT_HEADERS peH, void* OldBase, void* NewBase)
{
  unsigned long Delta = (unsigned long)NewBase - peH->OptionalHeader.ImageBase;
  PImageBaseRelocation p = (PImageBaseRelocation)((unsigned long)OldBase
      + peH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
  while (p->VirtualAddress + p->SizeOfBlock) {
    unsigned int i;
    unsigned short* pw = (unsigned short*)((int)p + sizeof(*p));
    for (i = 1; i <= (p->SizeOfBlock - sizeof(*p)) / 2; ++i) {
      if ((*pw) & 0xF000 == 0x3000) {
        unsigned long* t = (unsigned long*)((unsigned long)(OldBase) + p->VirtualAddress + ((*pw) & 0x0FFF));
        *t += Delta;
      }
      ++pw;
    }
    p = (PImageBaseRelocation)pw;
  }
}
// ж��ԭ���ռ���ڴ�
BOOL UnloadShell(HANDLE ProcHnd, unsigned long BaseAddr)
{
  typedef unsigned long(__stdcall * pfZwUnmapViewOfSection)(unsigned long, unsigned long);
  pfZwUnmapViewOfSection ZwUnmapViewOfSection = NULL;
  BOOL res = FALSE;
  HMODULE m = LoadLibrary("ntdll.dll");
  if (m) {
    ZwUnmapViewOfSection = (pfZwUnmapViewOfSection)GetProcAddress(m, "ZwUnmapViewOfSection");
    if (ZwUnmapViewOfSection) {
      res = (ZwUnmapViewOfSection((unsigned long)ProcHnd, BaseAddr) == 0);
    }
    FreeLibrary(m);
  }
  return res;
}
// ������ǽ��̲���ȡ���ַ����С�͵�ǰ����״̬
BOOL CreateChild(char* Cmd, CONTEXT* pCtx, HANDLE* pProcHnd, HANDLE* pThrdHnd,
    unsigned long* pProcId, unsigned long* pBaseAddr, unsigned long* pImageSize)
{
  BOOL res;
  STARTUPINFOA si;
  PROCESS_INFORMATION pi;
  unsigned long old;
  MEMORY_BASIC_INFORMATION MemInfo;
  memset(&si, 0, sizeof(si));
  memset(&pi, 0, sizeof(pi));
  si.cb = sizeof(si);
  res = CreateProcess(NULL, Cmd, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi); // �Թ���ʽ���н���;
  if (res) {
    void* p;
    *pProcHnd = pi.hProcess;
    *pThrdHnd = pi.hThread;
    *pProcId = pi.dwProcessId;
    // ��ȡ��ǽ�������״̬��[ctx.Ebx+8]�ڴ洦�������ǽ��̵ļ��ػ�ַ��ctx.Eax�������ǽ��̵���ڵ�ַ
    (*pCtx).ContextFlags = CONTEXT_FULL;
    GetThreadContext(*pThrdHnd, pCtx);
    ReadProcessMemory(*pProcHnd, (void*)(pCtx->Ebx + 8), pBaseAddr, sizeof(unsigned long), &old); // ��ȡ���ػ�ַ
    p = (void*)(*pBaseAddr);
    // ������ǽ���ռ�е��ڴ�
    while (VirtualQueryEx(*pProcHnd, p, &MemInfo, sizeof(MemInfo))) {
      if (MemInfo.State = MEM_FREE) {
        break;
      }
      p = (void*)((unsigned long)p + MemInfo.RegionSize);
    }
    *pImageSize = (unsigned long)p - (unsigned long)(*pBaseAddr);
  }
  return res;
}
// ������ǽ��̲���Ŀ������滻��Ȼ��ִ��
HANDLE AttachPE(char* CmdParam, PIMAGE_NT_HEADERS peH, PIMAGE_SECTION_HEADERS peSecH,
    void* Ptr, unsigned long ImageSize, unsigned long* pProcId)
{
  HANDLE res = INVALID_HANDLE_VALUE;
  CONTEXT Ctx;
  HANDLE Thrd;
  unsigned long Addr, Size;
  char* s = PrepareShellExe(CmdParam, peH->OptionalHeader.ImageBase, ImageSize);
  CInit1();
  if (s == NULL) {
    return res;
  }
  if (CreateChild(s, &Ctx, &res, &Thrd, pProcId, &Addr, &Size)) {
    void* p = NULL;
    unsigned long old;
    if ((peH->OptionalHeader.ImageBase == Addr) && (Size >= ImageSize)) {// ��ǽ��̿�������Ŀ����̲��Ҽ��ص�ַһ��
      p = (void*)Addr;
      VirtualProtectEx(res, p, Size, PAGE_EXECUTE_READWRITE, &old);
    }
    else if (IsNT()) {
      if (UnloadShell(res, Addr)) { // ж����ǽ���ռ���ڴ�
        p = MyVirtualAllocEx((unsigned long)res, (void*)peH->OptionalHeader.ImageBase, ImageSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
      }
      if ((p == NULL) && HasRelocationTable(peH)) { // �����ڴ�ʧ�ܲ���Ŀ�����֧���ض���
        p = MyVirtualAllocEx((unsigned long)res, NULL, ImageSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (p) {
          DoRelocation(peH, Ptr, p);  // �ض���
        }
      }
    }
    if (p) {
      WriteProcessMemory(res, (void*)(Ctx.Ebx + 8), &p, sizeof(DWORD), &old); // ����Ŀ��������л����еĻ�ַ
      peH->OptionalHeader.ImageBase = (unsigned long)p;
      if (WriteProcessMemory(res, p, Ptr, ImageSize, &old)) { // ����PE���ݵ�Ŀ�����
        Ctx.ContextFlags = CONTEXT_FULL;
        if ((unsigned long)p == Addr) {
          Ctx.Eax = peH->OptionalHeader.ImageBase + peH->OptionalHeader.AddressOfEntryPoint;  // �������л����е���ڵ�ַ
        }
        else {
          Ctx.Eax = (unsigned long)p + peH->OptionalHeader.AddressOfEntryPoint;
        }
        SetThreadContext(Thrd, &Ctx);// �������л���
        ResumeThread(Thrd);// ִ��
        CloseHandle(Thrd);
      }
      else {// ����ʧ��,ɱ����ǽ���
        TerminateProcess(res, 0);
        CloseHandle(Thrd);
        CloseHandle(res);
        res = INVALID_HANDLE_VALUE;
      }
    }
    else {// ����ʧ��,ɱ����ǽ���
      TerminateProcess(res, 0);
      CloseHandle(Thrd);
      CloseHandle(res);
      res = INVALID_HANDLE_VALUE;
    }
  }
  free(s);
  return res;
}
// ���ڴ��м��ز�����exe
// ������
// Buffer: �ڴ��е�exe��ַ
// Len: �ڴ���exeռ�ó���
// CmdParam: �����в���(������exe�ļ�����ʣ�������в�����
// ProcessId: ���صĽ���Id
// ����ֵ�� ����ɹ��򷵻ؽ��̵�Handle(ProcessHandle),���ʧ���򷵻�INVALID_HANDLE_VALUE
HANDLE MemExecute(void* ABuffer, long Len, char* CmdParam, unsigned long* pProcessId)
{
  HANDLE res = INVALID_HANDLE_VALUE;
  PIMAGE_NT_HEADERS peH;
  PIMAGE_SECTION_HEADERS peSecH;
  void* Ptr;
  unsigned long peSz;
  if (AlignPEToMem(ABuffer, Len, &peH, &peSecH, &Ptr, &peSz)) {
    res = AttachPE(CmdParam, peH, peSecH, Ptr, peSz, pProcessId);
    VirtualFree(Ptr, peSz, MEM_DECOMMIT);
  }
  return res;
}
#include "cfile.h"
#define EXEPATH "C:\\WINDOWS\\notepad.exe"
//#define EXEPATH ".\\Debug\\aaa.exe"
int test_memexec()
{
  DWORD dwFileSize;
  unsigned long ulProcessId = 0;
  LPBYTE pBuf = filemap(EXEPATH, &dwFileSize);
  MemExecute(pBuf, dwFileSize, "C:\\boot.ini", &ulProcessId);
  free(pBuf);
  return 0;
}
#endif // __MEMEXEC_H__

