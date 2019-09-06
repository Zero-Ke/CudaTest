// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 CPPDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// CPPDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef CPPDLL_EXPORTS
#define CPPDLL_API __declspec(dllexport)
#else
#define CPPDLL_API __declspec(dllimport)
#endif
//#include"../GPUDll/GPUDll.h"
typedef unsigned char byte;
// 此类是从 CPPDll.dll 导出的
class CPPDLL_API CCPPDll {
public:
	CCPPDll(void);
	//int Test(int a);
	//int CppRatezationGPU(byte* frn, byte* to, int Width, int Height); 
	//int CppBinaryzationGPU(CppColorRange* range, byte* fro, byte* to, int Width, int Height);
	//int CppBinaryzationCPU(CppColorRange* range, byte* bitmapFr, byte* bitmapTo, int width, int height);
	int CppRatezationCPU(byte* bitmapFr, byte* bitmapTo, int width, int height,bool re);
};

extern CPPDLL_API int nCPPDll;

CPPDLL_API int fnCPPDll(void);
