// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� CPPDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// CPPDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef CPPDLL_EXPORTS
#define CPPDLL_API __declspec(dllexport)
#else
#define CPPDLL_API __declspec(dllimport)
#endif
//#include"../GPUDll/GPUDll.h"
typedef unsigned char byte;
// �����Ǵ� CPPDll.dll ������
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
