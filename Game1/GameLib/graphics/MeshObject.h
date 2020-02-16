#pragma once
#include <d3dx9.h>
#include "../common/GameObject.h"

class MeshObject : public GameObject
{
public:
	LPD3DXMESH mesh;
	D3DMATERIAL9* meshMaterials;
	LPDIRECT3DTEXTURE9* meshTextures;
	DWORD numOfMaterials;

	MeshObject(const char* fileName);
	~MeshObject();

	void Draw() override;
};
