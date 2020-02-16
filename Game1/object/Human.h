#pragma once
#include <graphics/Plane.h>

class Human : public Plane
{
private:
	D3DXVECTOR3 velocity;

public:
	Human(const char* textureName, int tex_w, int tex_h);
	virtual ~Human();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;
};