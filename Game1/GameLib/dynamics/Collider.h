#ifndef GAME_LIB_GAME_LIB_COLLIDER_H_
#define GAME_LIB_GAME_LIB_COLLIDER_H_

#include <d3dx9.h>
#include <common/Component.h>
#include <common/GameObject.h>
#include "Math.h"

using Hirai::Vector2;
using Hirai::Vector3;

class MeshObject;

class Collider :
	public Component
{
public:
	Collider() {}
	virtual ~Collider() {}
};

class BoxCollider
	: public Collider
{
public:
	bool Check(const BoxCollider& col) const;
	void Update(Vector2 center) { center_ = center; }

	Vector2 center_;
	Vector2 box_width_;
};

class SphereCollider : public Collider
{
public:
	D3DXVECTOR3 center;
	float radius;
	// 確認用のメッシュ
	LPD3DXMESH sphereMesh;
	D3DMATERIAL9* sphereMeshMaterials;
	// 判定領域の表示フラグ
	bool on_disp;

	SphereCollider();
	~SphereCollider();

	// メッシュの外接円の中心と半径を計算し、メンバにセットする
	bool Initialize(MeshObject* mesh);
	bool Check(const GameObject& obj);
	void DrawRange();
};

#endif // !GAME_LIB_GAME_LIB_COLLIDER_H_
