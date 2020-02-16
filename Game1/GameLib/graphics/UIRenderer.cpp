#include "UIRenderer.h"
#include <d3d9.h>
#include "Sprite.h"
#include "DirectX.h"
#include "DXUtil.h"
#include <common/GameObject.h>
#include <camera/Camera.h>
#include "../../Game1.h"
#include <common/utility.h>

// staticメンバ変数宣言
IDirect3DVertexBuffer9* UIRenderer::buffer_ = nullptr;
IDirect3DVertexDeclaration9* UIRenderer::declaration_ = nullptr;
ID3DXEffect* UIRenderer::effect_ = nullptr;
int UIRenderer::scw_ = 800;
int UIRenderer::sch_ = 600;
D3DXVECTOR3 UIRenderer::pos_correction_ = D3DXVECTOR3{ 0.f, 0.f, 0.f };

UIRenderer::~UIRenderer()
{
	SAFE_DELETE(sprite_);
}

void UIRenderer::Initialize()
{
	using Hirai::DirectX;
	LPDIRECT3DDEVICE9 d3d_device = DirectX::GetD3DDevice();

	// 共通の頂点バッファ作成
	if (!buffer_)
	{
		float common_vertex[] =
		{
			// テクスチャの位置と各頂点の対応
			-0.5f, 0.5f, 0.0f,		1.0f, 1.0f,  // 0
			0.5f, 0.5f, 0.0f,		0.0f, 1.0f,  // 1
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f,  // 2
			0.5f, -0.5f, 0.0f,		0.0f, 0.0f,  // 3
		};

		d3d_device->CreateVertexBuffer(
			sizeof(common_vertex), 0, 0, D3DPOOL_MANAGED,
			&buffer_, nullptr
		);

		float* p = 0;
		if (buffer_)
		{
			buffer_->Lock(0, 0, (void**)& p, 0);
			memcpy(p, common_vertex, sizeof(common_vertex));
			buffer_->Unlock();
		}
	}

	// シェーダーオブジェクトの作成
	ID3DXBuffer* error = nullptr;
	if (FAILED(D3DXCreateEffectFromFile(
		d3d_device, TEXT("shader/SimpleSpriteRender.hlsl"), nullptr, 0, 0, 0, &effect_, &error
	)))
	{
		MessageBox(0, TEXT("ファイル読み込みエラーが発生しました。"), NULL, MB_OK);
		return;
	}

	// 頂点デクラレーション作成
	if (!declaration_)
	{
		D3DVERTEXELEMENT9 elems[] =
		{
			{0,0,D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,0},
			{0, sizeof(float) * 3, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
			D3DDECL_END()
		};
		d3d_device->CreateVertexDeclaration(elems, &declaration_);
	}
}

void UIRenderer::SetSprite(Sprite* sprite)
{
	if (sprite_)
	{
		// すでに設定されていた場合は上書き
		delete sprite_;
	}
	sprite_ = sprite;
}

void UIRenderer::Draw(const GameObject& obj)
{
	if (!(obj.CanRendering())) return;

	if (!buffer_ || !effect_ || !declaration_)
		return;	// 描画不可

	using Hirai::DirectX;
	LPDIRECT3DDEVICE9 d3d_device = DirectX::GetD3DDevice();
	// 頂点バッファ・頂点宣言設定
	d3d_device->SetStreamSource(0, buffer_, 0, sizeof(float) * 5);
	d3d_device->SetVertexDeclaration(declaration_);

	// 2D描画用射影変換行列
	D3DXMATRIX proj;
	D3DXMatrixIdentity(&proj);
	proj._41 = -1.0f;
	proj._42 = 1.f;

	UINT pass_number = 0;
	effect_->SetTechnique("SpriteDrawing");
	effect_->Begin(&pass_number, 0);
	effect_->BeginPass(0);

	proj._11 = 2.0f / UIRenderer::scw_;
	proj._22 = -2.0f / UIRenderer::sch_;

	D3DXMATRIX world, scale, rot;
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&world, (float)sprite_->poly_w, (float)sprite_->poly_h, 1.0f);
	D3DXMatrixScaling(&scale, -1.f * obj.local_scale_.x, obj.local_scale_.y, 1.0f);
	D3DXMatrixRotationZ(&rot, obj.local_rotation_.z);

	// ピボット分オフセット
	Vector2 pivot = sprite_->GetPivot();
	world._41 = -pivot.x;
	world._42 = -pivot.y;

	// ワールド座標変換 -> 回転 -> 拡大・縮小
	world = world * rot * scale;
	world._41 += obj.local_position_.x + pivot.x;
	world._42 += obj.local_position_.y + pivot.y;

	effect_->SetMatrix("world", &world);
	effect_->SetMatrix("proj", &proj);
	effect_->SetTexture("tex", &(sprite_->GetTexture()));

	UV_Pos uv = sprite_->GetUV();
	effect_->SetFloat("uv_left", uv.left);
	effect_->SetFloat("uv_top", uv.top);
	effect_->SetFloat("uv_width", uv.w);
	effect_->SetFloat("uv_height", uv.h);

	effect_->SetFloat("alpha", sprite_->GetAlpha());
	effect_->CommitChanges();
	d3d_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	effect_->EndPass();
	effect_->End();
}

void UIRenderer::Finalize()
{
	using namespace Hirai;
	SAFE_RELEASE(buffer_);
	SAFE_RELEASE(effect_);
	SAFE_RELEASE(declaration_);
}
