#ifndef GAME_LIB_UTILITY_STRING_HELPER_H_
#define GAME_LIB_UTILITY_STRING_HELPER_H_

#include <string>

using std::string;
using std::wstring;
/**
*	@brief
*		C++�`���̕����^��C�`���̕����^�ɕϊ�����@�\��񋟂���N���X
*		DirectX��C�`���̕����^���������Ƃ������̂ŁB
*/
class StringHelper
{
public:
	// string -> wstring
	static wstring ConvertStringToWString(const string& str);
};

#endif // !GAME_LIB_UTILITY_STRING_HELPER_H_
