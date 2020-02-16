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
	// �m�F�p�̃��b�V��
	LPD3DXMESH sphereMesh;
	D3DMATERIAL9* sphereMeshMaterials;
	// ����̈�̕\���t���O
	bool on_disp;

	SphereCollider();
	~SphereCollider();

	// ���b�V���̊O�ډ~�̒��S�Ɣ��a���v�Z���A�����o�ɃZ�b�g����
	bool Initialize(MeshObject* mesh);
	bool Check(const GameObject& obj);
	void DrawRange();
};

#endif // !GAME_LIB_GAME_LIB_COLLIDER_H_
