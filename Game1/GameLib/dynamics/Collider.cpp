#include "Collider.h"
#include <math.h>
#include <d3d9.h>
#include <graphics/MeshObject.h>
#include <graphics/DXUtil.h>
#include <graphics/DirectX.h>
#include <common/utility.h>
#include <common/Component.h>

bool BoxCollider::Check(const BoxCollider& col) const
{
	float org_left = center_.x - box_width_.x;
	float org_right = center_.x + box_width_.x;
	float org_top = center_.y - box_width_.y;
	float org_bottom = center_.y + box_width_.y;

	float target_left = col.center_.x - col.box_width_.x;
	float target_right = col.center_.x + col.box_width_.x;
	float target_top = col.center_.y - col.box_width_.y;
	float target_bottom = col.center_.y + col.box_width_.y;

	if (org_left > target_right) return false;
	if (org_right < target_left) return false;
	if (org_bottom < target_top) return false;
 	if (org_top > target_bottom) return false;

	return true;
 }

SphereCollider::SphereCollider()
	: center({ 0, 0, 0 }), radius(0.f), sphereMesh(nullptr),
	sphereMeshMaterials(nullptr)
{

}

SphereCollider::~SphereCollider()
{
	SAFE_RELEASE(sphereMesh);
}

bool SphereCollider::Initialize(MeshObject* mb)
{
	LPDIRECT3DVERTEXBUFFER9 vb = nullptr;
	void* vertices = nullptr;

	// メッシュの頂点バッファをロック
	if (FAILED(mb->mesh->GetVertexBuffer(&vb))) {
		return false;
	}
	if (FAILED(vb->Lock(0, 0, &vertices, 0))) {
		SAFE_RELEASE(vb);
		return false;
	}

	// メッシュの外接円の中心と半径を計算する
	HRESULT hr;
	hr = D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)vertices, mb->mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mb->mesh->GetFVF()), &center,
		&radius);
	vb->Unlock();
	SAFE_RELEASE(vb);
	
	if (FAILED(hr)) {
		return false;
	}

#ifdef _DEBUG
	// 確認用のスフィアをレンダリングするための準備
	// 得られた中心と半径をもとにメッシュとしてのスフィアを作成する
	using Hirai::DirectX;
	LPDIRECT3DDEVICE9 device = DirectX::GetD3DDevice();
	if (FAILED(D3DXCreateSphere(device, radius, 24, 24, &sphereMesh, nullptr)))
		return false;

	// スフィアメッシュのマテリアルの設定
	// 白色、半透明、光沢強
	sphereMeshMaterials = new D3DMATERIAL9;
	sphereMeshMaterials->Diffuse.r = 1.0f;
	sphereMeshMaterials->Diffuse.g = 1.0f;
	sphereMeshMaterials->Diffuse.b = 1.0f;
	sphereMeshMaterials->Diffuse.a = 0.3f;
	sphereMeshMaterials->Ambient = sphereMeshMaterials->Diffuse;
	sphereMeshMaterials->Specular.r = 1.0f;
	sphereMeshMaterials->Specular.g = 1.0f;
	sphereMeshMaterials->Specular.b = 1.0f;
	sphereMeshMaterials->Emissive.r = 0.1f;
	sphereMeshMaterials->Emissive.g = 0.1f;
	sphereMeshMaterials->Emissive.b = 0.1f;
	sphereMeshMaterials->Power = 120.0f;
#endif // DEBUG

	return true;
}

bool SphereCollider::Check(const GameObject& obj)
{
	D3DXVECTOR3 v = obj.local_position_ - (gameObject().local_position_);
	float lengh = D3DXVec3Length(&v);
	Component* c = obj.GetComponent("SphereCollider");
	if (c) {
		SphereCollider* col = dynamic_cast<SphereCollider*>(c);
		if (lengh <= radius + col->radius) {
			// 衝突している
			return true;
		}
	}
	return false;
}

void SphereCollider::DrawRange()
{
	if (!on_disp) return;
	using Hirai::DirectX;
	LPDIRECT3DDEVICE9 device = DirectX::GetD3DDevice();
	device->SetTexture(0, nullptr);
	device->SetMaterial(sphereMeshMaterials);
	sphereMesh->DrawSubset(0);
}
