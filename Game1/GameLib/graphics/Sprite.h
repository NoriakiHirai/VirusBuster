#ifndef GAME_1_GAME_LIB_SPRITE_H_
#define GAME_1_GAME_LIB_SPRITE_H_

#include <common/Component.h>
#include "Math.h"
#include <d3dx9.h>

using Hirai::Vector2;

struct UV_Pos {
	float left;
	float top;
	float w; // width
	float h; // height
};

// 頂点フォーマット定義
struct TextureVertex {
	float x, y, z;
	float u, v;
};

/**
*	@brief スプライトをレンダリングするための情報を保持するクラス
*			uv座標やuv上の幅高さなどを保持する
*/
class Sprite :
	public Component
{
private:
	IDirect3DTexture9* texture;
	bool is_active;
	float pivot_x, pivot_y;
	float uv_left, uv_top;		// ピクセル指定
	float uv_w, uv_h;			// 切り取る割合
	float alpha;
	float posZ;					// プライオリティ設定に用いる

public:
	// コンストラクタ
	Sprite();
	Sprite(int polyW, int polyH);
	// コピーコンストラクタ
	Sprite(const Sprite& sprite);
	~Sprite();

	void copy(const Sprite& sprite);
	Sprite& operator=(const Sprite& sprite);

	void SetSize(int w, int h) {
		poly_w = w; poly_h = h;
	}

	void SetPivot(float x, float y) { pivot_x = x; pivot_y = y; }
	Vector2 GetPivot() { return Vector2{ pivot_x, pivot_y }; }

	void SetTexture(IDirect3DTexture9* tex, bool is_resize);
	IDirect3DTexture9& GetTexture() const { return *texture; }

	// 切り取り設定
	void SetUV(float left, float top, float width, float height)
	{
		uv_left = left; uv_top = top; uv_w = width; uv_h = height;
	}
	void SetUV(UV_Pos uv_pos_)
	{
		uv_left = uv_pos_.left; uv_top = uv_pos_.top; uv_w = uv_pos_.w; uv_h = uv_pos_.h;
	}
	UV_Pos GetUV() { return UV_Pos{ uv_left, uv_top, uv_w, uv_h }; }

	// アルファ値設定
	void SetAlpha(float a) { alpha = a; }
	float GetAlpha() const { return alpha; }

	// プライオリティ設定
	// 0.f~1.fで設定する
	// 0.f: 最前面、1.f: 最後尾
	void SetPriority(float z) { posZ = z; }
	float GetPriority() const { return posZ; }

	// アクティビティ設定
	void SetActivity(bool activity) { is_active = activity; }
	bool IsActive() const { return is_active; }
	
	int poly_w, poly_h;			// サイズ

};

#endif // !GAME_1_GAME_LIB_SPRITE_H_
