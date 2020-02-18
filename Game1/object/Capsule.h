#pragma once
#include <graphics/Plane.h>

class Capsule : public Plane
{
public:
	Capsule(const char* textureName, int tex_w, int tex_h);
	virtual ~Capsule();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;
};

