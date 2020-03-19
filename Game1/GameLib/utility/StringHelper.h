#ifndef GAME_LIB_UTILITY_STRING_HELPER_H_
#define GAME_LIB_UTILITY_STRING_HELPER_H_

#include <string>

using std::string;
using std::wstring;
/**
*	@brief
*		C++形式の文字型をC形式の文字型に変換する機能を提供するクラス
*		DirectXがC形式の文字型を扱うことが多いので。
*/
class StringHelper
{
public:
	// string -> wstring
	static wstring ConvertStringToWString(const string& str);
};

#endif // !GAME_LIB_UTILITY_STRING_HELPER_H_
