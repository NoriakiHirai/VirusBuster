#pragma once
#include <graphics/Plane.h>
#include <utility/InnerTimer.h>
#include <dynamics/Collider.h>

class Virus :
    public Plane, public InnerTimer
{
private:
    BoxCollider outOfMultiply;
    BoxCollider outOfMultiply2;

public:
    Virus(const char* textureName, int tex_w, int tex_h);
    virtual ~Virus();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;

    void SetOutOfMultiply(const BoxCollider& range, const BoxCollider& range2);
};
