#pragma once
#include <graphics/Plane.h>

class Virus :
    public Plane
{
public:
    Virus(const char* textureName, int tex_w, int tex_h);
    virtual ~Virus();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;
};

