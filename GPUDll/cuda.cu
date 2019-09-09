#include <device_launch_parameters.h>
#include "cuda_runtime.h"
#include <cuda_runtime_api.h>
#include "GpuDll.h"
#include "CppTimer.h"
#include "iostream"
#include <vector>
#define redTo dst[xy].z
#define greenTo dst[xy].y
#define blueTo dst[xy].x
#define grayTo dst[xy].w
int  TestGPU()
{
	int n;
	cudaGetDeviceCount(&n);
	//std::cout << "Cuda Device Count: " << n << std::endl;
	if (n<0)
	{
		return 0;
	}
	cudaDeviceProp prop;
	cudaGetDeviceProperties(&prop, 0);
	int memSize = 10 * sizeof(uchar4);
	uchar4* src_device;
	//Init...
	cudaMalloc((void**)&src_device, memSize);
	//int a = prop.canMapHostMemory;
	cudaFree(src_device);
	//std::cout << "Device name: " << a << std::endl;
	return n;
}
__global__ void Kernel_Ratezation(uchar4* pfr, uchar4* pto, int width, int height)
{
	int index_x = blockIdx.x*blockDim.x + threadIdx.x;
	int index_y = blockIdx.y*blockDim.y + threadIdx.y;
	int grid_width = gridDim.x*blockDim.x;
	int xy = index_y*grid_width + index_x;
	if (index_x>width || index_y>height)
	{
		return;
	}
	int r = pfr[xy].z;
	int g = pfr[xy].y;
	int b = pfr[xy].x;
	int total = r + g + b;
	if (total == 0) total = 1;
	pto[xy].w = (byte)(total / 3);
	pto[xy].z = (byte)(r * 100 / total);
	pto[xy].y = (byte)(g * 100 / total);
	pto[xy].x = (byte)(b * 100 / total);

}
__global__ void kernel_Binaryzation(CppColorRange range, uchar4* pfr, uchar4* pto, int width, int height)
{
	int index_x = blockIdx.x*blockDim.x + threadIdx.x;
	int index_y = blockIdx.y*blockDim.y + threadIdx.y;
	int grid_width = gridDim.x*blockDim.x;
	int xy = index_y*grid_width + index_x;
	if (index_x>width || index_y>height)
	{
		return;
	}
	byte redUp = range.redUp;
	byte redDown = range.redDown;
	byte greenUp = range.greenUp;
	byte greenDown = range.greenDown;
	byte blueUp = range.blueUp;
	byte blueDown = range.blueDown;
	byte grayUp = range.grayUp;
	byte grayDown = range.grayDown;
	int red = pfr[xy].z;
	int green = pfr[xy].y;
	int blue = pfr[xy].x;
	int gray = pfr[xy].w;
	bool choosed = true;
	if (range.redUsed != 0)	{ if (red> redUp) choosed = false; if (red< redDown) choosed = false; }
	if (range.greenUsed != 0)	{ if (green> greenUp) choosed = false;  if (green< greenDown) choosed = false; }
	if (range.greenUsed != 0)	{ if (blue> blueUp) choosed = false; if (blue< blueDown) choosed = false; }
	if (range.greenUsed != 0)	{ if (gray> grayUp) choosed = false; if (gray< grayDown) choosed = false; }
	switch (range.operation)
	{
	case 0:
		if (choosed)	{ pto[xy].w = 0; }
		break;
	case 1:
		if (choosed)	{ pto[xy].w = 1; }
		break;
	case 2:
		if (!choosed)	{ pto[xy].w = 1; }
		break;
	}
}
__global__ void kernel_BinaryzationShow(CppColorRange* rangeArr, int count, uchar4* dst, int width, int height, int isAll)
{
	int index_x = blockIdx.x*blockDim.x + threadIdx.x;
	int index_y = blockIdx.y*blockDim.y + threadIdx.y;
	int grid_width = gridDim.x*blockDim.x;
	int xy = index_y*grid_width + index_x;
	if (index_x>width || index_y>height)
	{
		return;
	}
	int r = redTo;
	int g = greenTo;
	int b = blueTo;
	//printf("%d %d %d\n",r,g,b);
	int total = r + g + b;
	if (total == 0) total = 1;
	int gray = (byte)(total / 3);
	int red = (byte)(r * 100 / total);
	int green = (byte)(g * 100 / total);
	int blue = (byte)(b * 100 / total);
	//比例化结束,二值化开始
	for (int i = 0; i < count; i++)
	{
		CppColorRange range = *(rangeArr + i);
		bool redUsed = range.redUsed != 0;
		byte redUp = range.redUp;
		byte redDown = range.redDown;
		bool greenUsed = range.greenUsed != 0;
		byte greenUp = range.greenUp;
		byte greenDown = range.greenDown;
		bool blueUsed = range.blueUsed != 0;
		byte blueUp = range.blueUp;
		byte blueDown = range.blueDown;
		bool grayUsed = range.grayUsed != 0;
		byte grayUp = range.grayUp;
		byte grayDown = range.grayDown;
		//printf("%d %d %d %d \n", redUp, greenDown, blueDown, grayUp);
		bool choosed = true;
		if (redUsed)	{ if (red> redUp) choosed = false; if (red< redDown) choosed = false; }
		if (greenUsed)	{ if (green> greenUp) choosed = false;  if (green< greenDown) choosed = false; }
		if (blueUsed)	{ if (blue> blueUp) choosed = false; if (blue< blueDown) choosed = false; }
		if (grayUsed)	{ if (gray> grayUp) choosed = false; if (gray< grayDown) choosed = false; }
		switch (range.operation)
		{
		case 0:	if (choosed)	{ grayTo = 0; } break;
		case 1:	if (choosed)	{ grayTo = 1; }	break;
		case 2:	if (!choosed)	{ grayTo = 1; }	break;
		}
	}
	//二值化结束 ，抽色显示
	if (isAll == 1){ if (grayTo == 0){ redTo = 255; greenTo = 128; blueTo = 128; } }
	else { if (grayTo != 255){ redTo = 0; greenTo = 255; blueTo = 0; } }grayTo = 255;
}
static vector<CudaBitmap> bmpList;
//根据FOV ID获得相对应的bitmap
CudaBitmap *GetCudaBmp(int fovId)
{
	CudaBitmap *bmp;
	bool isFind = false;
	for (size_t i = 0; i < bmpList.size(); i++)
	{
		if (bmpList.at(i).FovId == fovId)
		{
			bmp = &bmpList.at(i);
			isFind = true;
			break;
		}
	}
	return bmp;
}
void FreeGPUByFovID(int fovId, int isClear)
{
	int i = 0;
	int n = bmpList.size();
	while (i<n)
	{
		if (bmpList.at(i).FovId == fovId || isClear == 1)
		{
			bmpList.at(i).MR_FreeHost();
			bmpList.at(i).MR_FreeDevice();
			if (isClear == 0)
			{
				bmpList.erase(bmpList.begin()+i);
				break;
			}
		}
		i++;
	}
	if (isClear == 1)bmpList.clear();
}
//显示抽色信息
void BinaryShowGPU(CppColorRange*range_host, int count, byte* src_host, byte* dst_host, int width, int height, int isAll)
{
	CppTimer time;
	CudaBitmap bmp;
	bool isFind = false;
	for (size_t i = 0; i < bmpList.size(); i++)
	{
		if (bmpList.at(i).FovId == 1)
		{
			bmp = bmpList.at(i);
			isFind = true;
			break;
		}
	}
	if (!isFind)
	{
		bmp.MR_Malloc(width, height, src_host, 1, true);
		bmpList.push_back(bmp);
	}
	CppColorRange* range_device;
	bmp.MR_HostToDevice();
	if (bmp.Device_Data == NULL || bmp.Host_Data == NULL)return;
	//----------------------------------------------------------------------------/
	cudaMalloc((void**)&range_device, count*sizeof(CppColorRange));
	cudaMemcpy((void**)range_device, range_host, count*sizeof(CppColorRange), cudaMemcpyHostToDevice);
	dim3 threadsPerBlock(32, 32);
	time.Begin();
	dim3 blocksPerGrid((width + threadsPerBlock.x - 1) / threadsPerBlock.x, (height + threadsPerBlock.y - 1) / threadsPerBlock.y);
	kernel_BinaryzationShow << <blocksPerGrid, threadsPerBlock >> >(range_device, count,(uchar4*)bmp.Device_Data, width, height, isAll);
	cudaThreadSynchronize();
	//----------------------------------------------------------------------------/
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess)
	{
		return;
	}
	cudaMemcpy((void**)dst_host, bmp.Device_Data, width * height * sizeof(uchar4), cudaMemcpyDeviceToHost);
	cudaFree(range_device);
}
void Ratezation_GPU(byte* src_host, byte* dst_host, int width, int height)
{
	CppTimer time;
	int length = width * height;
	int memSize = length * sizeof(uchar4);
	uchar4* src_device;
	uchar4* dst_device;
	time.Begin("");
	//Malloc
	cudaMalloc((void**)&src_device, memSize);
	cudaMalloc((void**)&dst_device, memSize);
	time.Reset("malloc");
	cudaError_t cudastatus;
	//Copy To GPU	
	cudastatus = cudaMemcpy((void**)src_device, src_host, memSize, cudaMemcpyHostToDevice);
	cudaMemcpy((void**)dst_device, dst_host, memSize, cudaMemcpyHostToDevice);
	dim3 threadsPerBlock(32, 32);
	dim3 blocksPerGrid((width + threadsPerBlock.x - 1) / threadsPerBlock.x, (height + threadsPerBlock.y - 1) / threadsPerBlock.y);
	time.Reset("copy to device");
	cudaDeviceSynchronize();
	Kernel_Ratezation << <blocksPerGrid, threadsPerBlock >> >(src_device, dst_device, width, height);
	time.Reset("kernel calculate");
	cudaMemcpy((void**)dst_host, dst_device, memSize, cudaMemcpyDeviceToHost);
	time.Reset("copy to host");
	cudaFree(src_device);
	cudaFree(dst_device);
}

void Binaryzation_GPU(CppColorRange* range_host, byte* src_host, byte* dst_host, int width, int height)
{
	CppTimer time;
	int length = width * height;
	int memSize = length * sizeof(uchar4);
	CppColorRange range_devive = *range_host;
	uchar4* src_device;
	uchar4* dst_device;
	time.Begin();
	//malloc
	cudaMalloc((void**)&src_device, memSize);
	cudaMalloc((void**)&dst_device, memSize);
	time.Reset("malloc");
	//copy
	cudaMemcpy((void**)src_device, src_host, memSize, cudaMemcpyHostToDevice);
	cudaMemcpy((void**)dst_device, dst_host, memSize, cudaMemcpyHostToDevice);
	//handle
	dim3 threadsPerBlock(16, 16);
	dim3 blocksPerGrid((width + threadsPerBlock.x - 1) / threadsPerBlock.x, (height + threadsPerBlock.y - 1) / threadsPerBlock.y);
	time.Reset("Binaryzation:copy to device");
	cudaDeviceSynchronize();
	kernel_Binaryzation << <blocksPerGrid, threadsPerBlock >> >(range_devive, src_device, dst_device, width, height);
	time.Reset("Binaryzation:kernel calculate");
	cudaMemcpy((void**)dst_host, dst_device, memSize, cudaMemcpyDeviceToHost);
	time.Reset("Binaryzation:copy to host");
	cudaFree(src_device);
	cudaFree(dst_device);
}