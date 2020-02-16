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

// ���_�t�H�[�}�b�g��`
struct TextureVertex {
	float x, y, z;
	float u, v;
};

/**
*	@brief �X�v���C�g�������_�����O���邽�߂̏���ێ�����N���X
*			uv���W��uv��̕������Ȃǂ�ێ�����
*/
class Sprite :
	public Component
{
private:
	IDirect3DTexture9* texture;
	bool is_active;
	float pivot_x, pivot_y;
	float uv_left, uv_top;		// �s�N�Z���w��
	float uv_w, uv_h;			// �؂��銄��
	float alpha;
	float posZ;					// �v���C�I���e�B�ݒ�ɗp����

public:
	// �R���X�g���N�^
	Sprite();
	Sprite(int polyW, int polyH);
	// �R�s�[�R���X�g���N�^
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

	// �؂���ݒ�
	void SetUV(float left, float top, float width, float height)
	{
		uv_left = left; uv_top = top; uv_w = width; uv_h = height;
	}
	void SetUV(UV_Pos uv_pos_)
	{
		uv_left = uv_pos_.left; uv_top = uv_pos_.top; uv_w = uv_pos_.w; uv_h = uv_pos_.h;
	}
	UV_Pos GetUV() { return UV_Pos{ uv_left, uv_top, uv_w, uv_h }; }

	// �A���t�@�l�ݒ�
	void SetAlpha(float a) { alpha = a; }
	float GetAlpha() const { return alpha; }

	// �v���C�I���e�B�ݒ�
	// 0.f~1.f�Őݒ肷��
	// 0.f: �őO�ʁA1.f: �Ō��
	void SetPriority(float z) { posZ = z; }
	float GetPriority() const { return posZ; }

	// �A�N�e�B�r�e�B�ݒ�
	void SetActivity(bool activity) { is_active = activity; }
	bool IsActive() const { return is_active; }
	
	int poly_w, poly_h;			// �T�C�Y

};

#endif // !GAME_1_GAME_LIB_SPRITE_H_
