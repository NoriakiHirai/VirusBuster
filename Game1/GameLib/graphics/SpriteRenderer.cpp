#include "SpriteRenderer.h"
#include <d3d9.h>
#include "Sprite.h"
#include "DirectX.h"
#include "DXUtil.h"
#include <common/GameObject.h>
#include <camera/Camera.h>
#include "../../Game1.h"
#include <common/utility.h>

// static�����o�ϐ��錾
IDirect3DVertexBuffer9* SpriteRenderer::buffer_ = nullptr;
IDirect3DVertexDeclaration9* SpriteRenderer::declaration_ = nullptr;
ID3DXEffect* SpriteRenderer::effect_ = nullptr;
int SpriteRenderer::scw_ = 800;
int SpriteRenderer::sch_ = 600;
D3DXVECTOR3 SpriteRenderer::pos_correction_ = D3DXVECTOR3{ 0.f, 0.f, 0.f };

SpriteRenderer::~SpriteRenderer()
{
	SAFE_DELETE(sprite_);
}

void SpriteRenderer::Initialize()
{
	using Hirai::DirectX;
	LPDIRECT3DDEVICE9 d3d_device = DirectX::GetD3DDevice();

	// ���ʂ̒��_�o�b�t�@�쐬
	if (!buffer_)
	{
		float common_vertex[] =
		{
				// �e�N�X�`���̈ʒu�Ɗe���_�̑Ή�
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

	// �V�F�[�_�[�I�u�W�F�N�g�̍쐬
	ID3DXBuffer* error = nullptr;
	if (FAILED(D3DXCreateEffectFromFile(
		d3d_device, TEXT("shader/SimpleSpriteRender.hlsl"), nullptr, 0, 0, 0, &effect_, &error
	)))
	{
		MessageBox(0, TEXT("�V�F�[�_�[�t�@�C���ǂݍ��݂ŃG���[���������܂����B"), NULL, MB_OK);
		return;
	}

	// ���_�f�N�����[�V�����쐬
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

void SpriteRenderer::SetSprite(Sprite* sprite)
{
	if (sprite_)
	{
		// ���łɐݒ肳��Ă����ꍇ�͏㏑��
		delete sprite_;
	}
	sprite_ = sprite;
}

void SpriteRenderer::Draw(GameObject* obj)
{
	if (!(obj->CanRendering())) return;

	if (!buffer_ || !effect_ || !declaration_)
		return;	// �`��s��

	using Hirai::DirectX;
	LPDIRECT3DDEVICE9 d3d_device = DirectX::GetD3DDevice();
	// ���_�o�b�t�@�E���_�錾�ݒ�
	d3d_device->SetStreamSource(0, buffer_, 0, sizeof(float) * 5);
	d3d_device->SetVertexDeclaration(declaration_);

	// 2D�`��p�ˉe�ϊ��s��
	D3DXMATRIX proj;
	D3DXMatrixIdentity(&proj);
	
	//D3DXVECTOR3 look_at = Scene::GetScene()->camera_list_[0]->look_at_position_;
	//proj._41 = -look_at.x / (float)kDisplaySizeWidth - 0.5f - (look_at.x / 1000.f);
	//if (look_at.y < Stage::kCameraViewBottomEnd) {
	//	proj._42 = (look_at.y / (float)kDisplaySizeHeight)  
	//		+ (1.5f / Stage::kStageHeightMax * look_at.y) + (0.5f * 0.001 * look_at.y);
	//}
	//else {
	//	proj._42 = (Stage::kCameraViewBottomEnd / (float)kDisplaySizeHeight)
	//		+ 1.5f;
	//}
	
	UINT pass_number = 0;
	effect_->SetTechnique("SpriteDrawing");
	effect_->Begin(&pass_number, 0);
	effect_->BeginPass(0);

	proj._11 = 2.0f / SpriteRenderer::scw_;
	proj._22 = -2.0f / SpriteRenderer::sch_;

	D3DXMATRIX world, scale, rot;
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&world, (float)sprite_->poly_w, (float)sprite_->poly_h, 1.0f);
	D3DXMatrixScaling(&scale, -1.f * obj->local_scale_.x, obj->local_scale_.y, 1.0f);
	D3DXMatrixRotationZ(&rot, 0.f);
	
	// �s�{�b�g���I�t�Z�b�g
	Vector2 pivot = sprite_->GetPivot();
	world._41 = -pivot.x;
	world._42 = -pivot.y;

	// ���[���h���W�ϊ� -> ��] -> �g��E�k��
	world = world * rot * scale;
	world._41 += obj->local_position_.x + pivot.x;
	world._42 += obj->local_position_.y + pivot.y;
	world._43 += obj->local_position_.z;

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

void SpriteRenderer::Finalize()
{
	using namespace Hirai;
	SAFE_RELEASE(buffer_);
	SAFE_RELEASE(effect_);
	SAFE_RELEASE(declaration_);
}
