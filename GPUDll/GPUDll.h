
#ifdef GPUDLL_EXPORTS
#define GPUDLL_API __declspec(dllexport)
#else
#define GPUDLL_API __declspec(dllimport)
#endif
#include "CudaBitmap.h"
using namespace std;

class GPUDLL_API CGPUDll {
public:
	CGPUDll(void);
	int TestGPU();
	void Ratezation_GPU(byte* src_host, byte* dst_host, long width, long height);
	void Binaryzation_GPU(CppColorRange* range_host, byte* src_host, byte* dst_host, int width, int height);
	void BinaryShowGPU(CppColorRange* range_host, int count, byte* src_host, byte* dst_host, int width, int height, int isAll);
	void FreeGPUByFovID(int fovId, int isClear=0);
};

//CppColorRange* range_host,
GPUDLL_API int fnGPUDll(void);
			extern "C"  __declspec(dllimport) int TestGPU();
			extern "C"  __declspec(dllimport) void Ratezation_GPU(byte* src_host, byte* dst_host, int width, int height);
			extern "C"  __declspec(dllimport) void Binaryzation_GPU(CppColorRange* range_host, byte* src_host, byte* dst_host, int width, int height);
			extern "C"  __declspec(dllimport) void BinaryShowGPU(CppColorRange* range_host, int count, byte* src_host, byte* dst_host, int width, int height, int isAll);
			extern "C"  __declspec(dllimport) void FreeGPUByFovID(int fovId, int isClear = 0);