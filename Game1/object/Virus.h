#pragma once
#include <vector>
#include <graphics/Plane.h>
#include <utility/InnerTimer.h>
#include <dynamics/Collider.h>

class Virus :
    public Plane, public InnerTimer
{
private:

public:
    Virus(const char* textureName, int tex_w, int tex_h);
    virtual ~Virus();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;
};
