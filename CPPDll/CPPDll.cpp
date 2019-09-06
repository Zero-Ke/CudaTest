// CPPDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <iostream>
#include"../GPUDll/GPUDll.h"
#define redTo (pto[2])
#define greenTo (pto[1])
#define blueTo (*pto)
#define grayTo (pto[3])

#define redFr (pfr[2])
#define greenFr (pfr[1])
#define blueFr (pfr[0])
#define grayFr (pfr[3])
using namespace std;

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 CPPDll.h
int Test(int a)
{
	return TestGPU();
}
int CppRatezationGPU(byte* fro, byte* to, int Width, int Height)
{
	Ratezation_GPU(fro, to, Width, Height);
	return 1;
}
int CppBinaryzationGPU(CppColorRange* range,byte* fro, byte* to, int Width, int Height)
{
	Binaryzation_GPU(range,fro, to, Width, Height);
	return 1;
}
//Change RGB color value to r\g\b rate and gray value, corresponding to the color range value.
int CppRatezationCPU(byte* bitmapFr, byte* bitmapTo, int width, int height,bool re)
{
	int bpline = (width * 4 + 4 - 1) / 4 * 4;	//Four bytes alignment
	for( int y = 0; y < height; y++)
	{
 		byte *pfr = bitmapFr + y*bpline;
		byte *pto = bitmapTo + y*bpline;
		for( int x=0;x<width;x++) 
		{
			int r = redFr;
			int g = greenFr;
			int b = blueFr;
			int total = r+g+b;
			if( total == 0 ) total = 1;
			grayTo = (byte)(total/3);
			redTo = (byte)(r*100/total);
			greenTo = (byte)(g*100/total);
			blueTo = (byte)(b*100/total);
			pfr += 4;
			pto += 4;
		}
	}
	return 0;
}

//-242741168
//Binaryzation the image use gaven color range as a threshold.
int CppBinaryzationCPU( CppColorRange* range, byte* bitmapFr, byte* bitmapTo, int width, int height)
{
	int x,y;
	int bpline = (width * 4 + 4 - 1) / 4 * 4;	//Bytes per line. Four bytes alignment
	bool redUsed = range->redUsed != 0;
	byte redUp = range->redUp;
	byte redDown = range->redDown;
	bool greenUsed = range->greenUsed != 0;
	byte greenUp = range->greenUp;
	byte greenDown = range->greenDown;
	bool blueUsed = range->blueUsed != 0;
	byte blueUp = range->blueUp;
	byte blueDown = range->blueDown;
	bool grayUsed = range->grayUsed != 0;
	byte grayUp = range->grayUp;
	byte grayDown = range->grayDown;
	//printf("11111111111111 %d-%d-%d-%d\n", (int)redUsed, (int)greenUsed, (int)blueUsed, (int)grayUsed);
	switch(range->operation)
	{
	case 0://add
		for( y = 0; y < height; y++)
		{
 			byte *pfr = bitmapFr + y*bpline;
			byte *pto = bitmapTo + y*bpline;
			for( x=0;x<width;x++) 
			{
				int gray = grayFr;
				int red = redFr;
				int green = greenFr;
				int blue = blueFr;
				
				bool choosed = true;
				if( redUsed ){ if( red> redUp) choosed = false; if( red< redDown) choosed = false; }
				if( greenUsed ){ if( green> greenUp) choosed = false; if( green< greenDown) choosed = false; }
				if( blueUsed ){ if( blue> blueUp) choosed = false; if( blue< blueDown) choosed = false; }
				if( grayUsed ){ if( gray> grayUp) choosed = false; if( gray< grayDown) choosed = false; }
				if( choosed )
				{
					grayTo = 0;
					//printf("11111111111111 %d-%d-%d-%d\n", (int)red, (int)green, (int)blue, (int)gray);
				}
				pfr += 4;
				pto += 4;
			}
		}
		break;
	case 1://sub
		for( y = 0; y < height; y++)
		{
 			byte *pfr = bitmapFr + y*bpline;
			byte *pto = bitmapTo + y*bpline;
			for( x=0;x<width;x++) 
			{
				int gray = grayFr;
				int red = redFr;
				int green = greenFr;
				int blue = blueFr;

				bool choosed = true;
				if( redUsed ){ if( red> redUp) choosed = false; if( red< redDown) choosed = false; }
				if( greenUsed ){ if( green> greenUp) choosed = false; if( green< greenDown) choosed = false; }
				if( blueUsed ){ if( blue> blueUp) choosed = false; if( blue< blueDown) choosed = false; }
				if( grayUsed ){ if( gray> grayUp) choosed = false; if( gray< grayDown) choosed = false; }
				if( choosed )
				{
					grayTo = 1;
				}
				pfr += 4;
				pto += 4;
			}
		}
		break;
	case 2://and
		for( y = 0; y < height; y++)
		{
 			byte *pfr = bitmapFr + y*bpline;
			byte *pto = bitmapTo + y*bpline;
			for( x=0;x<width;x++) 
			{
				int gray = grayFr;
				int red = redFr;
				int green = greenFr;
				int blue = blueFr;

				bool choosed = true;
				if( redUsed ){ if( red> redUp) choosed = false; if( red< redDown) choosed = false; }
				if( greenUsed ){ if( green> greenUp) choosed = false; if( green< greenDown) choosed = false; }
				if( blueUsed ){ if( blue> blueUp) choosed = false; if( blue< blueDown) choosed = false; }
				if( grayUsed ){ if( gray> grayUp) choosed = false; if( gray< grayDown) choosed = false; }
				if( !choosed )
				{
					grayTo = 1;
				}
				pfr += 4;
				pto += 4;
			}
		}
		break;
	}
	return 0;
}



int CppBinaryShowAll( byte* bitmapFr, byte* bitmapTo, int width, int height)
{
	int bpline = (width * 4 + 4 - 1) / 4 * 4;	//Four bytes alignment
	for( int y = 0; y < height; y++)
	{
		byte *pfr = bitmapFr + y*bpline;
		byte *pto = bitmapTo + y*bpline;
		for( int x=0;x<width;x++) 
		{
			if( grayFr == 0)
			{
				redTo = 255;
				greenTo = 128;
				blueTo = 128;
			}
			grayTo = 255;
			pfr += 4;
			pto += 4;
		}
	}
	return 0;
}

int CppBinaryShow(CppColorRange* range, int count, byte* bitmapFr, byte* bitmapTo, int width, int height, int isAll)
{
	for (int i = 0; i < count; i++)
	{
		CppColorRange cr = *(range + i);
		std::cout << "" << endl;
	}
	BinaryShowGPU(range, count,  bitmapFr,  bitmapTo,  width,  height,  isAll);
	return 1;
}
//Used to show one Color binarized value in screen. The bitmapTo should be the orginal bitmap.
int CppBinaryShowOne( byte* bitmapFr, byte* bitmapTo, int width, int height)
{
	int bpline = (width * 4 + 4 - 1) / 4 * 4;	//Four bytes alignment
	for( int y = 0; y < height; y++)
	{
		byte *pfr = bitmapFr + y*bpline;
		byte *pto = bitmapTo + y*bpline;
		for( int x=0;x<width;x++) 
		{
			if( grayFr != 255 )
			{
				redTo = 0;
				greenTo = 255;
				blueTo = 0;
			}
			grayTo = 255;
			pfr += 4;
			pto += 4;
		}
	}
	return 0;
}

