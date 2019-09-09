#include "stdafx.h"
#include "CudaBitmap.h"
#include <device_launch_parameters.h>
#include "cuda_runtime.h"
#include <cuda_runtime_api.h>
#include "CppTimer.h"
#include "iostream"
#define MR_Call(err)       __SafeCall(err, __FILE__, __LINE__)
inline void __SafeCall(cudaError err, const char *file, const int line)
{
	if (cudaSuccess != err) 
	{
		return;
		fprintf(stderr, "SafeCall() Runtime API error in file <%s>, line %i : %s.\n", file, line, cudaGetErrorString(err));
		exit(-1);
	}
}
inline void DebugLogLine(const char* str)
{
	if (false)
	{
		std::cout << str << std::endl;
	}
}
CudaBitmap::CudaBitmap()
{
	
}


CudaBitmap::~CudaBitmap()
{

}
//分配内存，使用的是锁页内存
void CudaBitmap::MR_Malloc(int w, int h, byte *host,int fovID, bool isHost)
{
	Width = w;
	Height = h;
	FovId = fovID;
	int memSize = w * h * sizeof(uchar4);
	MR_Call(cudaMalloc((void**)&Device_Data, memSize*sizeof(uchar4)));
	if (Device_Data == NULL)
		DebugLogLine("Failed to allocate device data\n");
	if (host&&host!=NULL)
	{
		MR_Call(cudaHostAlloc((void**)&Host_Data, memSize, cudaHostAllocDefault));
		MR_Call(cudaMemcpy((void**)Host_Data, host, memSize, cudaMemcpyHostToHost));
	}
}
void CudaBitmap::MR_FreeHost()
{
	if (Host_Data != NULL)
	{
		MR_Call(cudaFreeHost(Host_Data));
		Host_Data = NULL;
	}
}
void CudaBitmap::MR_FreeDevice()
{
	if (Device_Data != NULL)
	{
		MR_Call(cudaFree(Device_Data));
		Device_Data = NULL;
	}
}
void CudaBitmap::MR_HostToDevice()
{
	if (Host_Data!=NULL&&Device_Data!=NULL)
	{
		MR_Call(cudaMemcpy(Device_Data, Host_Data, Width*Height*sizeof(uchar4), cudaMemcpyHostToDevice));
	}
}
void CudaBitmap::MR_DeviceToHost()
{
	if (Host_Data != NULL&&Device_Data != NULL)
	{
		MR_Call(cudaMemcpy(Host_Data, Device_Data, Width*Height*sizeof(uchar4), cudaMemcpyDeviceToHost));
	}
}