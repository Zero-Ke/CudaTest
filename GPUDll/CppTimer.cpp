#include "CppTimer.h"
#include"Time.h";
#include<iostream>
clock_t beginTime;
clock_t endTime;
CppTimer::CppTimer()
{

}


CppTimer::~CppTimer()
{

}

void CppTimer::Begin(const char* res)
{
	std::cout << res << "->��ʼ��ʱ��" << std::endl;
	beginTime = clock();
}


void CppTimer::Reset(const char* res)
{
	endTime = clock();
	int n = endTime - beginTime;
	beginTime = clock();
	if (res != "")
	{
		std::cout << res << "->��ʱ:" << n << "ms" << std::endl;
	}
}	