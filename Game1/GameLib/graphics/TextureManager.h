#ifndef GAME_LIB_GAME_LIB_TEXTURE_MANAGER_H_
#define GAME_LIB_GAME_LIB_TEXTURE_MANAGER_H_

#include <string>
#include <map>
#include <d3dx9.h>

using std::string;
using std::map;

/**
*	@brief
*		テクスチャの読み込みを行うユーティリティクラス
*		読み込み済みのテクスチャ保持する。
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
		//	// TODO: このエラー処理は時間があれば何とかしたい。。。
		//	MessageBox(0, TEXT("ファイル名の指定が不正です。"), NULL, MB_OK);
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
