#include "MeshObject.h"
#include <atlstr.h>
#include "DXUtil.h"
#include <common/utility.h>
#include <graphics/DirectX.h>
#include <graphics/MeshRenderer.h>

MeshObject::MeshObject(const char* fileName)
	: mesh(nullptr), meshMaterials(nullptr), meshTextures(nullptr),
	numOfMaterials(0)
{
	// Xファイルからメッシュをロードする
	using Hirai::DirectX;
	LPDIRECT3DDEVICE9 device = DirectX::GetD3DDevice();
	LPD3DXBUFFER d3Buffer = nullptr;
	CString cstrFileName(fileName);
	if (FAILED(D3DXLoadMeshFromX(cstrFileName, D3DXMESH_SYSTEMMEM,
		device, nullptr, &d3Buffer, nullptr, &numOfMaterials,&mesh)))
	{
		MessageBoxA(nullptr, "Xファイルの読み込みに失敗しました", nullptr, MB_OK);
		return;
	}

	D3DXMATERIAL* materials = (D3DXMATERIAL*)d3Buffer->GetBufferPointer();
	meshMaterials = new D3DMATERIAL9[numOfMaterials];
	meshTextures = new LPDIRECT3DTEXTURE9[numOfMaterials];

	// マテリアル・テクスチャの読み込み
	for (DWORD i = 0; i < numOfMaterials; ++i) {
		meshMaterials[i] = materials[i].MatD3D;
		// 環境光の設定
		meshMaterials[i].Ambient = meshMaterials[i].Diffuse;
		meshTextures[i] = nullptr;
		if (materials[i].pTextureFilename != nullptr &&
			lstrlenA(materials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFileA(
				device, materials[i].pTextureFilename,
				&meshTextures[i])
			)) {
				MessageBoxA(nullptr, "テクスチャの読み込みに失敗しました。", nullptr, MB_OK);
			}
		}
	}
	d3Buffer->Release();
}

MeshObject::~MeshObject()
{
	for (DWORD i = 0; i < numOfMaterials; ++i) {
		SAFE_RELEASE(meshTextures[i]);
	}
	SAFE_RELEASE(mesh);
}

void MeshObject::Draw()
{
	MeshRenderer* mr = dynamic_cast<MeshRenderer*>(GetComponent("MeshRenderer"));
	mr->Draw(*this);
}
