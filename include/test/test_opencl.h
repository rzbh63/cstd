#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

#ifdef __APPLE__
#include <opencl/cl.h>
#else
#include <CL/cl.h>
#endif

#pragma comment (lib,"OpenCL.lib")

const int ARRAY_SIZE = 10000;

//Ϊcpuƽ̨����������
cl_context createContext()
{
  cl_platform_id firstPlatformId = 0;
  cl_uint numPlatforms = 0;
  //����ѡ���һ��ƽ̨
  cl_int errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
  if (errNum != CL_SUCCESS || numPlatforms <= 0)
  {
    std::cerr << "Failed to find any opencl platforms." << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "��openclƽ̨" << std::endl;
  }

  //����ƽ̨��һ�������ģ�����ͼ����һ��gpu�ģ����û�еĻ����ʹ���cpu��
  cl_context_properties contextProperties[] =
  {
    CL_CONTEXT_PLATFORM,
    (cl_context_properties)firstPlatformId,
    0
  };
  cl_context context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU, NULL, NULL, &errNum);
  if (errNum != CL_SUCCESS)
  {
    std::cout << "���ܴ���gpu������ ������CPU..." << std::endl;
    context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU, NULL, NULL, &errNum);
    if (errNum != CL_SUCCESS)
    {
      std::cerr << "���ܴ���opencl gpu����cpu������";
      return NULL;
    }
    else
    {
      std::cout << "�ܴ���cpu������" << std::endl;
    }
  }
  else
  {
    std::cout << "�ܴ���gpu������" << std::endl;
  }
  return context;
}
//ѡ���һ�������豸��������һ���������
cl_command_queue createCommandQueue(cl_context context, cl_device_id & device)
{
  size_t deviceBufferSize = -1;
  cl_int errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
  if (errNum != CL_SUCCESS)
  {
    std::cerr << "���ܻ�ȡ�豸�����С" << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "�ɹ���ȡ�豸�����С" << std::endl;
  }

  //Ϊ�豸�������ռ�
  cl_device_id * devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
  errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
  if (errNum != CL_SUCCESS)
  {
    std::cerr << "���ܵõ��豸id����" << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "���Եõ��豸id����" << std::endl;
  }

  //����ֻѡ���һ�����õ��豸���ڸ��豸����һ���������.�������������ڽ�������Ҫִ�е��ں��Ŷӣ������ؽ��
  cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);
  if (commandQueue == NULL)
  {
    std::cerr << "����Ϊ�豸0�����������" << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "��Ϊ�豸0�����������" << std::endl;
  }
  device = devices[0];
  delete[] devices;
  return commandQueue;

}

//�Ӵ��̼����ں�Դ�ļ������͹���һ���������
cl_program createProgram(cl_context context, cl_device_id device, const char* fileName)
{
  std::ifstream kernelFile(fileName, std::ios::in);
  if (!kernelFile.is_open())
  {
    std::cerr << "���ܴ��ļ�" << fileName << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "�ɹ����ļ�" << fileName << std::endl;
  }

  std::ostringstream oss;
  oss << kernelFile.rdbuf();
  std::string srcStdStr = oss.str();
  const char * srcStr = srcStdStr.c_str();
  //�����������
  cl_program program = clCreateProgramWithSource(context, 1, (const char**)&srcStr, NULL, NULL);
  if (program == NULL)
  {
    std::cerr << "���ܴ�Դ�ļ�����opencl�������" << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "�ܴ�Դ�ļ�����opencl�������" << std::endl;
  }

  //�����ں�Դ��
  cl_int errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (errNum != CL_SUCCESS)
  {
    //�жϴ���ԭ��
    char buildLog[16384];
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
    std::cerr << "�ں˴���:" << std::endl;
    std::cerr << buildLog;
    clReleaseProgram(program);
    return NULL;
  }
  else
  {
    std::cout << "�����ں�Դ��ɹ�" << std::endl;
  }
  return program;
}

//�Ӵ��̼����ں�Դ�ļ������͹���һ���������
cl_program createProgramFromSrc(cl_context context, cl_device_id device, const char* srcStr)
{
  //�����������
  cl_program program = clCreateProgramWithSource(context, 1, (const char**)&srcStr, NULL, NULL);
  if (program == NULL)
  {
    std::cerr << "���ܴ�Դ�ļ�����opencl�������" << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "�ܴ�Դ�ļ�����opencl�������" << std::endl;
  }

  //�����ں�Դ��
  cl_int errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (errNum != CL_SUCCESS)
  {
    //�жϴ���ԭ��
    char buildLog[16384];
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
    std::cerr << "�ں˴���:" << std::endl;
    std::cerr << buildLog;
    clReleaseProgram(program);
    return NULL;
  }
  else
  {
    std::cout << "�����ں�Դ��ɹ�" << std::endl;
  }
  return program;
}
//�����ڴ�������豸�ڴ��У��������ں˺���ֱ�ӷ���
bool createMemObjects(cl_context context, cl_mem memObjects[3], float * a, float *b)
{
  memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * ARRAY_SIZE, a, NULL);
  memObjects[1] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * ARRAY_SIZE, b, NULL);
  memObjects[2] = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * ARRAY_SIZE, NULL, NULL);
  if (memObjects[0] == NULL ||
    memObjects[1] == NULL ||
    memObjects[2] == NULL
    )
  {
    std::cerr << "���󴴽��ڴ����" << std::endl;
    return false;
  }
  return true;
}

//�����Դ
void cleanUp(cl_context context, cl_command_queue commandQueue, cl_program program, cl_kernel theKernel, cl_mem memObjects[3])
{
  for (size_t i = 0; i < 3; i++)
  {
    if (memObjects[i] != 0)
    {
      clReleaseMemObject(memObjects[i]);
    }
  }
  if (commandQueue != 0)
  {
    clReleaseCommandQueue(commandQueue);
  }

  if (theKernel != 0)
  {
    clReleaseKernel(theKernel);
  }
  if (program != 0)
  {
    clReleaseProgram(program);
  }
  if (context != 0)
  {
    clReleaseContext(context);
  }
}


const char* test_str = //
"__kernel void hello_kernel(__global const float * a,"
"  __global const float * b,"
"  __global float* result)"
"{"
"  int gid = get_global_id(0);"
"  result[gid] = a[gid] + b[gid];"
"}";


int test_opencl(int argc, char ** argv)
{
  time_t startTime1;
  time_t endTime1;
  time_t endTime2;
  time(&startTime1);
  std::string fileName = "test.cl";
  cl_mem memObjects[3] = { 0, 0, 0 };
  cl_context theContext = createContext();
  cl_device_id  device = NULL;
  cl_command_queue theQueue = createCommandQueue(theContext, device);
#if 0
  cl_program theProgram = createProgram(theContext, device, fileName.c_str());
#else
  cl_program theProgram = createProgramFromSrc(theContext, device, test_str);
#endif
  //����opencl�ں�
  cl_kernel theKernel = clCreateKernel(theProgram, "hello_kernel", NULL);
  if (theKernel == NULL)
  {
    std::cerr << "���ܴ����ں�" << std::endl;
    cleanUp(theContext, theQueue, theProgram, theKernel, memObjects);
    return 1;
  }
  else
  {
    std::cout << "�ɹ������ں�" << std::endl;
  }

  float result[ARRAY_SIZE];
  float a[ARRAY_SIZE];
  float b[ARRAY_SIZE];
  for (size_t i = 0; i < ARRAY_SIZE; i++)
  {
    a[i] = (float)i;
    b[i] = (float)(i * 2);
  }
  if (!createMemObjects(theContext, memObjects, a, b))
  {
    std::cout << "�����ڴ����ʧ��" << std::endl;
    cleanUp(theContext, theQueue, theProgram, theKernel, memObjects);
    return 1;
  }
  else
  {
    std::cout << "�����ڴ����ɹ�" << std::endl;
  }

  //�����ں˲���
  cl_int errNum = clSetKernelArg(theKernel, 0, sizeof(cl_mem), &memObjects[0]);
  errNum |= clSetKernelArg(theKernel, 1, sizeof(cl_mem), &memObjects[1]);
  errNum |= clSetKernelArg(theKernel, 2, sizeof(cl_mem), &memObjects[2]);
  if (errNum != CL_SUCCESS)
  {
    std::cerr << "���������ں˲���" << std::endl;
    cleanUp(theContext, theQueue, theProgram, theKernel, memObjects);
    return 1;
  }
  else
  {
    std::cout << "��ȷ�����ں˲���" << std::endl;
  }

  //ʹ���������ʹ�����豸��ִ�е��ں��Ŷ�
  size_t globalWorkSize[1] = { ARRAY_SIZE };
  size_t localWorkSize[1] = { 1 };
  errNum = clEnqueueNDRangeKernel(theQueue, theKernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
  if (errNum != CL_SUCCESS)
  {
    std::cerr << "�����ִ�ж����ں�" << std::endl;
    cleanUp(theContext, theQueue, theProgram, theKernel, memObjects);
    return 1;
  }
  else
  {
    std::cout << "������ȷ��ִ�ж����ں�" << std::endl;
  }

  //���ں˶��ؽ��
  errNum = clEnqueueReadBuffer(theQueue, memObjects[2], CL_TRUE, 0, ARRAY_SIZE * sizeof(float), result, 0, NULL, NULL);
  if (errNum != CL_SUCCESS)
  {
    std::cerr << "�����ȡ�������" << std::endl;
    cleanUp(theContext, theQueue, theProgram, theKernel, memObjects);
    return 1;
  }

  cleanUp(theContext, theQueue, theProgram, theKernel, memObjects);
  time(&endTime1);
  double deltaTime1 = difftime(endTime1, startTime1);

  //����
  float result2[ARRAY_SIZE];
  float a2[ARRAY_SIZE];
  float b2[ARRAY_SIZE];
  for (size_t i = 0; i < ARRAY_SIZE; i++)
  {
    a2[i] = (float)i;
    b2[i] = (float)(i * 2);
    result2[i] = a2[i] + b2[i];
  }

  time(&endTime2);
  double deltaTime2 = difftime(endTime2, endTime1);
  std::cout << "����ʱ�䣺" << deltaTime1 << std::endl;
  std::cout << "����ʱ�䣺" << deltaTime2 << std::endl;

  return 0;
}

