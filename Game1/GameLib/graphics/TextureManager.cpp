#include "TextureManager.h"
#include "DirectX.h"
#include <d3dx9.h>

map<string, IDirect3DTexture9*> TextureManager::texture_list_;


TextureManager::~TextureManager()
{
	for (const auto& v : texture_list_)
	{
		delete v.second;
	}
	texture_list_.clear();
}

void TextureManager::ReadTexture(string file_name)
{
	if (file_name == "") return;
	// 存在チェック(Textureマネージャに保持していたら、GetTextueで取得）
	if (texture_list_.count(file_name) > 0) return;
	
	IDirect3DTexture9* d3d_texture9 = (IDirect3DTexture9*)malloc(sizeof(IDirect3DTexture9));
	using Hirai::DirectX;
	DirectX::LoadTexture(file_name, &d3d_texture9);
	texture_list_.emplace(file_name, d3d_texture9);
}
