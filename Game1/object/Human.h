#pragma once
#include <graphics/Plane.h>

class HumanBehaviour;

class Human : public Plane
{
private:
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 initPosision;

public:
	static float HUMAN_SPEED;

	Human(const char* textureName, int tex_w, int tex_h);
	Human(const char* textureName, int tex_w, int tex_h,
		float initPosX, float initPosY);
	virtual ~Human();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;

	const D3DXVECTOR3 NextPosition();
};