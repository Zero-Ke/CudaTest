// ÏÂÁÐ ifdef ¿éÊÇ´´½¨Ê¹´Ó DLL µ¼³ö¸ü¼òµ¥µÄ
// ºêµÄ±ê×¼·½·¨¡£´Ë DLL ÖÐµÄËùÓÐÎÄ¼þ¶¼ÊÇÓÃÃüÁîÐÐÉÏ¶¨ÒåµÄ GPUDLL_EXPORTS
// ·ûºÅ±àÒëµÄ¡£ÔÚÊ¹ÓÃ´Ë DLL µÄ
// ÈÎºÎÆäËûÏîÄ¿ÉÏ²»Ó¦¶¨Òå´Ë·ûºÅ¡£ÕâÑù£¬Ô´ÎÄ¼þÖÐ°üº¬´ËÎÄ¼þµÄÈÎºÎÆäËûÏîÄ¿¶¼»á½«
// GPUDLL_API º¯ÊýÊÓÎªÊÇ´Ó DLL µ¼ÈëµÄ£¬¶ø´Ë DLL Ôò½«ÓÃ´Ëºê¶¨ÒåµÄ
// ·ûºÅÊÓÎªÊÇ±»µ¼³öµÄ¡£
#ifdef GPUDLL_EXPORTS
#define GPUDLL_API __declspec(dllexport)
#else
#define GPUDLL_API __declspec(dllimport)
#endif
using namespace std;
typedef int bool4;

typedef unsigned char byte;
struct CppColorRange
{
	int operation;
	bool4 redUsed;
	byte redUp;
	byte redDown;
	bool4 greenUsed;
	byte greenUp;
	byte greenDown;
	bool4 blueUsed;
	byte blueUp;
	byte blueDown;
	bool4 grayUsed;
	byte grayUp;
	byte grayDown;
};
// ´ËÀàÊÇ´Ó GPUDll.dll µ¼³öµÄ
class GPUDLL_API CGPUDll {
public:
	CGPUDll(void);
	int TestGPU();
	void Ratezation_GPU(byte* src_host, byte* dst_host, long width, long height);
	void Binaryzation_GPU(CppColorRange* range_host, byte* src_host, byte* dst_host, int width, int height);
	void BinaryShowGPU(CppColorRange* range_host, int count, byte* src_host, byte* dst_host, int width, int height, int isAll);
};

//CppColorRange* range_host,
GPUDLL_API int fnGPUDll(void);
			extern "C"  __declspec(dllimport) int TestGPU();
			extern "C"  __declspec(dllimport) void Ratezation_GPU(byte* src_host, byte* dst_host, int width, int height);
			extern "C"  __declspec(dllimport) void Binaryzation_GPU(CppColorRange* range_host, byte* src_host, byte* dst_host, int width, int height);
			extern "C"  __declspec(dllimport) void BinaryShowGPU(CppColorRange* range_host, int count, byte* src_host, byte* dst_host, int width, int height, int isAll);