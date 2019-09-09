#pragma once
typedef int bool4;
typedef unsigned char byte;
#include "CppTimer.h"
#include "iostream"
#include <vector>
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
//面对对象，若有需要再改
class CudaBitmap
{
public:
	int FovId;
	int Width, Height;
	byte *Host_Data;
	byte *Device_Data;
public:
	CudaBitmap();
	~CudaBitmap();
	void MR_Malloc(int w, int h, byte *host,int fovID, bool isHost=false);
	void MR_HostToDevice();
	void MR_DeviceToHost();
	void MR_FreeHost();
	void MR_FreeDevice();
};

