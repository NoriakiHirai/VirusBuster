#ifndef GAME_1_GAME_LIB_UI_RENDERER_H_
#define GAME_1_GAME_LIB_UI_RENDERER_H_

#include <d3dx9.h>
#include <vector>
#include "Renderer.h"

using std::vector;

// 前方宣言
class Sprite;
class GameObject;

class UIRenderer
	: public Renderer
{
public:
	UIRenderer() : sprite_(nullptr) {}
	UIRenderer(int w, int h) : sprite_(nullptr)
	{
		scw_ = w; sch_ = h;
	}
	~UIRenderer();

	static void Initialize();
	static void SetScreenSize(int w, int h)
	{
		scw_ = w; sch_ = h;
	}
	void SetSprite(Sprite* sprite);
	Sprite* GetSprite() { return sprite_; }

	void Draw(const GameObject& obj);
	static void Finalize();

private:
	static IDirect3DVertexBuffer9* buffer_;
	static IDirect3DVertexDeclaration9* declaration_;
	static ID3DXEffect* effect_;	// シェーダープログラムへのポインタ
	static int scw_, sch_;	// スクリーンサイズ
	static D3DXVECTOR3 pos_correction_;		// 位置補正

	Sprite* sprite_;
};

#endif // !GAME_1_GAME_LIB_UI_RENDERER_H_
