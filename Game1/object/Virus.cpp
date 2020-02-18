#include "Virus.h"
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>

Virus::Virus(const char* textureName, int tex_w, int tex_h)
    : Plane(textureName, tex_w, tex_h)
{
    ((UIRenderer*)GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(192.f / 256.f, 0.f / 256.f, 16.f / 256.f, 16.f / 256.f);
}

Virus::~Virus()
{
}

void Virus::Initialize()
{
}

void Virus::Update()
{
}

void Virus::Draw()
{
    if (!IsActive()) return;
    UIRenderer* renderer = (UIRenderer*)GetComponent("UIRenderer");
    renderer->Draw(*this);
}

void Virus::Finalize()
{
}
