// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

typedef unsigned char byte;

#ifndef WINVER       
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif                        

#ifndef _WIN32_WINDOWS     
#define _WIN32_WINDOWS 0x0501
#endif

#ifndef _WIN32_IE            
#define _WIN32_IE 0x0601    
#endif

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>



// TODO:  在此处引用程序需要的其他头文件
