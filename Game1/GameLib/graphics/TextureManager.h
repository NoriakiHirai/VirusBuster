#ifndef GAME_LIB_GAME_LIB_TEXTURE_MANAGER_H_
#define GAME_LIB_GAME_LIB_TEXTURE_MANAGER_H_

#include <string>
#include <map>
#include <d3dx9.h>

using std::string;
using std::map;

/**
*	@brief
*		�e�N�X�`���̓ǂݍ��݂��s�����[�e�B���e�B�N���X
*		�ǂݍ��ݍς݂̃e�N�X�`���ێ�����B
*/
class TextureManager
{
public:
	TextureManager() {}
	~TextureManager();

	static void ReadTexture(string file_name);
	static IDirect3DTexture9* GetTexture(string file_name)
	{
		//if (!file_name)
		//{
		//	// TODO: ���̃G���[�����͎��Ԃ�����Ή��Ƃ��������B�B�B
		//	MessageBox(0, TEXT("�t�@�C�����̎w�肪�s���ł��B"), NULL, MB_OK);
		//	return (IDirect3DTexture9 * *)nullptr;
		//}
		return texture_list_.at(file_name);
	}
	static void Release(string file_name) { texture_list_.erase(file_name); }
	static void ReleaseAll()
	{
		if (texture_list_.size() > 0)
			texture_list_.clear();
	}

private:
	static map<string, IDirect3DTexture9*> texture_list_;
};

#endif // !GAME_LIB_GAME_LIB_TEXTURE_MANAGER_H_
