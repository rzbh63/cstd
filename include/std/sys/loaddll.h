
#ifndef _LOADDLL_H_
#define _LOADDLL_H_

#include <windows.h>
/*
LOADDLL_BEGIN()
*/
static HINSTANCE hInsts[ 100 ] = {0};
static unsigned hInstslen = 0;

static void unloadalldll(void)
{
  unsigned i;

  for (i = 0; i < hInstslen; ++i) {
    if (NULL != hInsts[ i ]) {
      FreeLibrary(hInsts[ i ]);
      hInsts[ i ] = NULL;
    }
  }

  hInstslen = 0;
}

#define LOADDLL_BEGIN(_DLLNAME) do { HINSTANCE _hInst = LoadLibrary(_DLLNAME); if(_hInst) { hInsts[hInstslen++] = _hInst;
#define GETFUN(_FUNNAME)  GetProcAddress(_hInst, _FUNNAME)
#define GETDLLFUN(_FUNNAME, _FTYPE)  ((_FTYPE)GetProcAddress(_hInst, _FUNNAME))
#define LOADDLL_END()  atexit(unloadalldll);  } } while(0)


#if 0
// ֱ�������ڴ��е�DLL

��������ܲ�ϣ���ڷ�������ʱ������һ���ⲿ�� DLL����Ϊ���ܻ���Щ�û��������а� DLL ɾ���˶���� EXE ������ȷ���У�Ҳ�п��ܸ� DLL �ᱻ������ȥʹ�ã�Ҳ�п��ܣ��� DLL ���Ϊ�ƽ����ƽ���ĳ����ͻ�ƿڡ����۳��ں���ԭ����������һ�� DLL �ϲ���һ�� EXE �еĻ�����������������ַ�����

Win32 ������� DLL �Ļ���

���� Win32 EXE �ڵ���һ���ⲿ DLL �еĺ���ʱ������Ҫ���� LoadLibary ����������� DLL ������Ľ��̵�ַ�ռ䡣 ��� LoadLibary ����� DLL �ɹ���������һ���� DLL �ľ���� ������ʵ���Ͼ��Ǹ� DLL ���ڴ��е���ʼ��ַ�� ������ DLL �ɹ��󣬻�������� GetProcAddress ��������ȡҪ���õĺ����ĵ�ַ��Ȼ���ٸ��ݸõ�ַ���������������
��������ԭ�����ǿ��԰�һ�� DLL ��Ϊ��Դ�ļ��ŵ� EXE �ļ��У��ڳ�������ʱ������һ���ڴ棬Ȼ�󽫴���Դ���Ƶ��÷�����ڴ��У������ݸ��ڴ��ַ����õ���صĵ���������ַ��Ȼ�󣬵�������Ҫ����ĳһ����ʱ�������øú������ڴ��еĵ�ַ����������
����ʵ�֡�
�������ȣ���Ҫ�ϲ��� DLL ��Ϊ��Դ���뵽��Ŀ����Դ�ļ��У�Ȼ���ڳ�������ʱ������Դ���������Դ���Եõ��� DLL ���ڴ��е�λ�ã�
LPVOID sRawDll; // ��Դ�ļ����ڴ��еĵ�ַ
HRSRC hRes;
HMODULE hLibrary;
HGLOBAL hResourceLoaded;
char lib_name[MAX_PATH];
GetModuleFileName(hInstance, lib_name, MAX_PATH);  // �õ����г��������
hLibrary = LoadLibrary(lib_name);                  // ��������һ�� DLL һ���������г����ڴ���

if (NULL != hLibrary)
{
  // �õ�ָ������Դ�ļ����ڴ��е�λ��
  hRes = FindResource(hLibrary, MAKEINTRESOURCE(IDR_DATA1), RT_RCDATA);

  if (NULL != hRes) {
    // ����Դ�ļ������ڴ�
    hResourceLoaded = LoadResource(hLibrary, hRes);

    if (NULL != hResourceLoaded) {
      // �õ���Դ�ļ���С
      SizeofResource(hLibrary, hRes);

      // ������Դ�Եõ������ڴ��еĵ�ַ
      sRawDll = (LPVOID)LockResource(hResourceLoaded);
    }
  }
  else {
    return 1;
  }

  FreeLibrary(hLibrary);
}
else
{
  return 1;
}

Ȼ�󣬴���Դ������ DLL ���ڴ溯�� LoadPbDllFromMemory ������ DLL ���ڴ��У� �ú�����������������һ��������ָ�� DLL ��Դ���ڴ��еĵ�ַ��ָ�룬Ҳ����ǰ������е� LockResource �����ķ���ֵ���ڶ���������һ���յ�ָ�룬������� LoadPbDllFromMemory ���гɹ�����ָ�뽫ָ��������Ϻ���ڴ��е� DLL ����ʼ��ַ���ú�������һ�����ܾ���������гɹ��������ֶ����� DLL_PROCESS_ATTACH �������� DLL ����ں��� DllMain ����ʼ���� DLL������֮�⣬�������ֶ�������ϲ��� DLL ����ڱ��е���� DLL �������������ڴ��е���Ե�ַ�������Ǹú�������:

DWORD LoadPbDllFromMemory(LPVOID lpRawDll, LPVOID lpImageDll)
{
  SYSTEM_INFO sSysInfo;
  PIMAGE_DOS_HEADER dosHeader;
  PIMAGE_NT_HEADERS pNTHeader;
  PIMAGE_SECTION_HEADER section;
  PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
  PIMAGE_IMPORT_BY_NAME pOrdinalName;
  PIMAGE_BASE_RELOCATION baseReloc;
  PDWORD lpLink;
  unsigned char Protection[4096];
  HINSTANCE hDll;
  WORD i;
  DWORD ImagePages, fOldProtect, j, MaxLen, HdrLen, Addr1, Addr2, Pg, Pg1, Pg2;
  char* sDllName;
  int HeaderSize;
  DWORD NeededMemory;
  int ii;
  DWORD OrgAddr, NewAddr, Size;
  DWORD importsStartRVA;
  PBYTE pImageBase;

  if (NULL == lpRawDll) {
    return 1 ;
  }

  dosHeader = (PIMAGE_DOS_HEADER)lpRawDll;

  // Is this the MZ header?
  if ((TRUE == IsBadReadPtr(dosHeader, sizeof(IMAGE_DOS_HEADER))) ||
      (IMAGE_DOS_SIGNATURE != dosHeader->e_magic)) {
    return 2;
  }

#define MakePtr(cast, ptr, addvalue)   (cast)((DWORD)(ptr)+(DWORD)(addvalue))

  // Get the PE header.
  pNTHeader = MakePtr(PIMAGE_NT_HEADERS, dosHeader, dosHeader->e_lfanew);

  // Is this a real PE image?
  if ((TRUE == IsBadReadPtr(pNTHeader, sizeof(IMAGE_NT_HEADERS))) ||
      (IMAGE_NT_SIGNATURE != pNTHeader->Signature)) {
    return 3 ;
  }

  if ((pNTHeader->FileHeader.SizeOfOptionalHeader !=
      sizeof(pNTHeader->OptionalHeader)) ||
      (pNTHeader->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC)) {
    return 4;
  }

  if (pNTHeader->FileHeader.NumberOfSections < 1) {
    return 5;
  }

  section = IMAGE_FIRST_SECTION(pNTHeader);
  HeaderSize = sizeof(IMAGE_SECTION_HEADER);

  // ��ͷ����
  HdrLen = (DWORD)section - (DWORD)dosHeader +
      HeaderSize * pNTHeader->FileHeader.NumberOfSections;

  // �ҳ����Ľڵĳ���,�˽�һ���Ǵ������ڵĽ�(.text ��)
  MaxLen = HdrLen;
  ii = 0;

  for (i = 0; i < (DWORD)pNTHeader->FileHeader.NumberOfSections; i++) { // find MaxLen
    if (MaxLen < section[i].VirtualAddress + section[i].SizeOfRawData) {
      MaxLen = section[i].VirtualAddress + section[i].SizeOfRawData;
    }

    if (strcmp((const char*)section[i].Name, ".rsrc") == 0) {
      ii = i;
    }
  }

  GetSystemInfo(&sSysInfo);
  ImagePages = MaxLen / sSysInfo.dwPageSize;

  if (MaxLen % sSysInfo.dwPageSize) {
    ImagePages++;
  }

  // ����������ڴ�
  NeededMemory = ImagePages * sSysInfo.dwPageSize;
  lpImageDll = VirtualAlloc(NULL, NeededMemory, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

  if (lpImageDll == NULL) {
    return 6;  // �����ڴ�ʧ��
  }

  MoveMemory(lpImageDll, lpRawDll, HdrLen);   // ���ƽ�ͷ

  OrgAddr = 0;
  NewAddr = 0;
  Size = 0;

  // ���� .text ������
  for (i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++) {
    OrgAddr = (DWORD)lpImageDll + (DWORD)section[i].VirtualAddress;
    NewAddr = (DWORD)lpRawDll + (DWORD)section[i].PointerToRawData;
    Size = (DWORD)section[i].SizeOfRawData;
    MoveMemory((void*)OrgAddr, (void*)NewAddr, Size);
  }

  // ��ָ��ָ���µ� DLL ӳ��
  dosHeader = (PIMAGE_DOS_HEADER) lpImageDll; // Switch to new image
  pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)dosHeader + dosHeader->e_lfanew);
  section = (PIMAGE_SECTION_HEADER)((DWORD)pNTHeader + sizeof(IMAGE_NT_HEADERS));
  pImageBase = (PBYTE)dosHeader;

#define IsNT() (TRUE)

  if ((ii != 0) && (IsNT() == TRUE)) {
    section[ii].VirtualAddress = section[ii].VirtualAddress + (DWORD)lpRawDll;
    section[ii].PointerToRawData = section[ii].PointerToRawData + (DWORD)lpRawDll;
  }


  // Look up where the imports section is (normally in the .idata section)
  // but not necessarily so. Therefore, grab the RVA from the data dir.
  importsStartRVA = GetImgDirEntryRVA(pNTHeader, IMAGE_DIRECTORY_ENTRY_IMPORT);

  if (!importsStartRVA) {
    VirtualFree(dosHeader, 0, MEM_RELEASE);
    return 7;
  }

  pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR) pNTHeader->
      OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

  if (pImportDesc != 0) {
    pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)pImportDesc + (DWORD)dosHeader);
  }
  else {
    VirtualFree(dosHeader, 0, MEM_RELEASE);
    return 8;
  }

  while (1) { // �������ڱ��е� DLL
    // ����Ƿ������˿յ� IMAGE_IMPORT_DESCRIPTOR
    if ((pImportDesc->TimeDateStamp == 0) && (pImportDesc->Name == 0)) {
      break;
    }

    // �Ӵ����������� Dll,
    // ע��,����� DLL �Ǻϲ��� DLL ����ڱ��е� DLL,
    // ���Ǻϲ��� EXE �е� DLL
    sDllName = (char*)(pImportDesc->Name + (DWORD)pImageBase);
    hDll = GetModuleHandle(sDllName);

    if (hDll == 0) {
      hDll = LoadLibrary(sDllName);
    }

    if (hDll == 0) {
      MessageBox(NULL, "Can''t find required Dll",
          "Error in LoadPbDllFromMemory()", 0);
      VirtualFree(dosHeader, 0, MEM_RELEASE);
      return 9;
    }

    DWORD* lpFuncNameRef = (DWORD*)(pImportDesc->OriginalFirstThunk +
        (DWORD)dosHeader);
    DWORD* lpFuncAddr = (DWORD*)(pImportDesc->FirstThunk +
        (DWORD)dosHeader);

    while (*lpFuncNameRef != 0) {
      pOrdinalName = (PIMAGE_IMPORT_BY_NAME)(*lpFuncNameRef +
          (DWORD)dosHeader);
      DWORD pIMAGE_ORDINAL_FLAG = 0x80000000;

      if (*lpFuncNameRef & pIMAGE_ORDINAL_FLAG)
        *lpFuncAddr = (DWORD) GetProcAddress(hDll,
            (const char*)(*lpFuncNameRef & 0xFFFF));
      else
        *lpFuncAddr = (DWORD) GetProcAddress(hDll,
            (const char*)pOrdinalName->Name);

      if (lpFuncAddr == 0) {
        VirtualFree(dosHeader, 0, MEM_RELEASE);
        return 10;// Can''t GetProcAddress
      }

      lpFuncAddr++;
      lpFuncNameRef++;
    }

    pImportDesc++;
  }

  DWORD TpOffset;
  baseReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pNTHeader->
      �������� OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

  if (baseReloc != 0) {
    baseReloc = (PIMAGE_BASE_RELOCATION)((DWORD)baseReloc + (DWORD)dosHeader);

    while (baseReloc->VirtualAddress != 0) {
      PWORD lpTypeOffset = (PWORD)((DWORD)baseReloc +
          sizeof(IMAGE_BASE_RELOCATION));

      while (lpTypeOffset < (PWORD)((DWORD)baseReloc +
          (DWORD)baseReloc->SizeOfBlock)) {
        TpOffset = *lpTypeOffset & 0xF000;

        if (TpOffset == 0x3000) {
          lpLink = (PDWORD)((DWORD)dosHeader +
              ��������������������������������������������             baseReloc->VirtualAddress +
              (*lpTypeOffset & 0xFFF));
          *lpLink = (DWORD)dosHeader +
              (*lpLink) - pNTHeader->OptionalHeader.ImageBase;
        }
        else {
          if (TpOffset != 0) {
            VirtualFree(dosHeader, 0, MEM_RELEASE);
            return 10;
          }
        }

        lpTypeOffset++;
      }

      baseReloc = (PIMAGE_BASE_RELOCATION)((DWORD)baseReloc +
          (DWORD)baseReloc->SizeOfBlock);
    }
  }

  // ȡ��ԭʼ���ڴ�״̬
  memset(Protection, 0, 4096);

  for (i = 0; i <= pNTHeader->FileHeader.NumberOfSections; i++) {
    if (i == pNTHeader->FileHeader.NumberOfSections) {
      Addr1 = 0;
      Addr2 = HdrLen;
      j = 0x60000000;
    }
    else {
      Addr1 = section[i].VirtualAddress;
      Addr2 = section[i].SizeOfRawData;
      j = section[i].Characteristics;
    }

    Addr2 += Addr1 - 1;

    Pg1 = Addr1 / sSysInfo.dwPageSize;
    Pg2 = Addr2 / sSysInfo.dwPageSize;

    for (Pg = Pg1 ; Pg <= Pg2; Pg++) {
      if (j & 0x20000000) {
        Protection[Pg] |= 1;  // Execute
      }

      if (j & 0x40000000) {
        Protection[Pg] |= 2;  // Read
      }

      if (j & 0x80000000) {
        Protection[Pg] |= 4;  // Write
      }
    }
  }

  // �ָ�ԭʼ���ڴ�״̬
  Addr1 = (DWORD)dosHeader;

  for (Pg = 0 ; Pg <= ImagePages; Pg++) {
    switch (Protection[Pg]) {
    case 2:
      fOldProtect = PAGE_READONLY;
      break;

    case 3:
      fOldProtect = PAGE_EXECUTE_READ;
      break;

    case 6:
      fOldProtect = PAGE_READWRITE;
      break;

    default:
      // Ignore strange combinations
      fOldProtect = PAGE_EXECUTE_READWRITE;
      break;
    }

    if (fOldProtect != PAGE_EXECUTE_READWRITE) {
      if (VirtualProtect((void*)Addr1,
          sSysInfo.dwPageSize,
          fOldProtect,
          &fOldProtect) == 0) {
        VirtualFree(dosHeader, 0, MEM_RELEASE);
        return 11;
      }
    }

    Addr1 += sSysInfo.dwPageSize;
  }

  EntryPoint = (LPENTRYPOINT)((DWORD)pNTHeader->OptionalHeader.AddressOfEntryPoint +
      (DWORD)dosHeader);
  LPVOID lpReserved = 0;
  EntryPoint((HINSTANCE)dosHeader, DLL_PROCESS_ATTACH, lpReserved);
  lpImageDll2 = lpImageDll;
  return 0;
}
// һ�� DLL ����ȷ�����뵽�ڴ��У����ǾͿ���ͨ���Զ��庯�� GetProcAddressDirectly ����ȡĳ�������ڴ��еĵ�ַ��
/// �����ݸõ�ַ�����øú������ú���Ҳ��������������һ��������ָ�����뵽�ڴ��е� DLL ����ʼ��ַ��ָ�룬
// �ڶ�����Ҫ���õĺ����ĺ������������� GetProcAddressDirectly ��������:
DWORD GetProcAddressDirectly(PIMAGE_DOS_HEADER dosHeader, char* FuncName)
{
  PIMAGE_NT_HEADERS pNTHeader;
  PIMAGE_EXPORT_DIRECTORY pExportDir;
  PWORD lpNameOrdinals;
  LPDWORD lpFunctions;
  DWORD* lpName;
  char* lpExpFuncName;
  DWORD i;
  DWORD j;
  char* lpFuncName;
  DWORD exportsStartRVA, exportsEndRVA;

  if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
    return 0;
  }

  pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)dosHeader + dosHeader->e_lfanew);

  if (pNTHeader->Signature != IMAGE_NT_SIGNATURE) {
    return 0;
  }

  if ((pNTHeader->FileHeader.SizeOfOptionalHeader != sizeof(pNTHeader->OptionalHeader)) ||
      (pNTHeader->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC)) {
    return 0;
  }

  pImageBase = (PBYTE)dosHeader;

  // Make pointers to 32 and 64 bit versions of the header.
  pNTHeader = MakePtr(PIMAGE_NT_HEADERS, dosHeader, dosHeader->e_lfanew);

  exportsStartRVA = GetImgDirEntryRVA(pNTHeader, IMAGE_DIRECTORY_ENTRY_EXPORT);
  exportsEndRVA = exportsStartRVA +
      GetImgDirEntrySize(pNTHeader, IMAGE_DIRECTORY_ENTRY_EXPORT);

  // Get the IMAGE_SECTION_HEADER that contains the exports. This is
  // usually the .edata section, but doesn''t have to be.
  PIMAGE_SECTION_HEADER header;
  header = GetEnclosingSectionHeader(exportsStartRVA, pNTHeader);

  if (!header) {
    return 0;
  }

  INT delta;
  delta = (INT)(header->VirtualAddress - header->PointerToRawData);
  pExportDir = (PIMAGE_EXPORT_DIRECTORY)GetPtrFromRVA(exportsStartRVA,
      pNTHeader, pImageBase);


  pExportDir = (PIMAGE_EXPORT_DIRECTORY)(pNTHeader->
      OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

  if (pExportDir == 0) {
    MessageBox(NULL, "Error in GetProcAddressDirectly()", 0, 0);
    return 0;
  }

  pExportDir = (PIMAGE_EXPORT_DIRECTORY)((DWORD)pExportDir + (DWORD)dosHeader);
  lpNameOrdinals = (PWORD)((DWORD)pExportDir->AddressOfNameOrdinals + (DWORD)dosHeader);
  lpName = (LPDWORD)(pExportDir->AddressOfNames + (DWORD)dosHeader);
  lpFunctions = (LPDWORD)(pExportDir->AddressOfFunctions + (DWORD)dosHeader);
  lpFuncName = FuncName;

  if (HIWORD(lpFuncName) != 0) {
    for (i = 0; i <= pExportDir->NumberOfFunctions - 1; i++) {
      DWORD entryPointRVA = *lpFunctions;

      // Skip over gaps in exported function
      if (entryPointRVA == 0) {
        continue;
      }

      for (j = 0; j <= pExportDir->NumberOfNames - 1; j++) {
        if (lpNameOrdinals[j] == i) {
          lpExpFuncName = (char*)(lpName[j] +
              (DWORD)dosHeader);

          if (strcmp((char*)lpExpFuncName, (char*)FuncName) == 0)
            return (DWORD)(lpFunctions[i] +
                (DWORD)dosHeader);
        }
      }
    }
  }
  else {
    for (i = 0 ; i <= pExportDir->NumberOfFunctions - 1; i++) {
      if (lpFuncName == (char*)(pExportDir->Base + i)) {
        if (lpFunctions[i]) return (unsigned long)(lpFunctions[i] +
              dosHeader);
      }
    }
  }

  return 0;
}


// �ڵ����꺯���󣬲�Ҫ������ UnloadPbDllFromMemory �����ڴ�����ȥ DLL ���ͷŷ�����ڴ棬�ú��������� DLL_PROCESS_DETACH �������� DLL ����ں��� DllMain ���ӵ��ý��̵ĵ�ַ�ռ�ж�ظ� DLL�� ������ UnloadPbDllFromMemory ��������:
DWORD UnloadPbDllFromMemory(PIMAGE_DOS_HEADER dosHeader)
{
  PIMAGE_NT_HEADERS pNTHeader;
  pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)dosHeader + (DWORD)dosHeader->e_lfanew);
  EntryPoint = (LPENTRYPOINT)(pNTHeader->OptionalHeader.AddressOfEntryPoint +
      (DWORD)dosHeader);
  EntryPoint((HINSTANCE)dosHeader, DLL_PROCESS_DETACH, 0);
  return VirtualFree(dosHeader, 0, MEM_RELEASE);
}

����ʾ�������˵��
�����ڱ��ĸ�����ʾ�������У��ϲ���һ����Ϊ hardware.dll �Ķ�̬���ӿ⣬�ö�̬���ӿ���һ����ȡϵͳӲ����Ϣ�Ŀ��ļ������а��������º���:

getmac     ȡ������ MAC
VolumeNumber   ȡ��Ӳ�̾��
changeres            �ı���Ļ�ֱ���
IsDiskInDrive        ����������Ƿ������
DPGetDefaultPrinter  ȡ��Ĭ�ϵĴ�ӡ����
DPSetDefaultPrinter  ����Ĭ�ϵĴ�ӡ��
getserial            ȡ��Ӳ�̵ĳ������к�
getmetric            ȡ����ʾ�ֱ���
PrintStringDirect    ֱ�����ӡ������һ����
vfpbeep              �� PC ���ȷ���
getcpuid             ȡ�� CPU ID
getbios              ȡ������ BIOS ID
������ʾ�������У�ֻ����������һ������ getbios ����ȡ���� BIOS ID�� ����˵��һ�£��ú���ʵ���Ϻ���ֻ�ܼ�� AWARD ����� BIOS�� Ҳ����˵���Ƕ�ȡ����ϵͳ�ڴ� 0x000fex71 ����ֵ����Ϊ�������ӵ����� BIOS ��λ�����в�ͬ�����ڳ�����û�н����ⷽ��Ĵ��������ڶ��������ӵ����� BIOS ʱ���ܻ���Щ����(���������ݿ��ܲ���ȷ)�����ڴ� DLL �����ݺ�ʵ�֣�Ҳ���һ�����һƪ�������ۼ���
��

����
�������ҽ��в���ʱ�����ֶ�����Щ������Դ�� DLL���� 9x ƽ̨�¿��ܻ������⡣
���⻰
�������⣬����һЩ����δ�ἰ�ķ���Ҫ�ĺ����������вμ�Դ�����е�ע�͡�
�٣������漰 PE �ļ���ʽ�����֪ʶ�������Ѿ������˱��ĵķ�Χ��������Ϣ�ɲμ� MSDM �е�:

Peering Inside the PE:
A Tour of the Win32 Portable Executable File Format һ�ĺ�
Microsoft Portable Executable and Common Object File Format Specification һ��
�ر��л¬������Aming�����ұ�д����ʱ������һЩ��������Ľ����ָ��

#endif





#endif // _LOADDLL_H_
