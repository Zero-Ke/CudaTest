#pragma once
class CppTimer
{
public:
	CppTimer(void);
	~CppTimer(void);
	void Begin(const char* args = "");
	void Reset(const char* args = "");
};

	