#include "StringHelper.h"

#include <Windows.h>

wstring StringHelper::ConvertStringToWString(const string& str)
{
	int str_length = (int)str.length() + 1;
	int length = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str_length, nullptr, 0);
	wchar_t* buf = new wchar_t[length];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str_length, buf, length);
	wstring result{ buf };
	delete[] buf;
	return result;
}