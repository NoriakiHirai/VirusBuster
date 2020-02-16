#include "Human.h"
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>

Human::Human(const char* textureName, int tex_w, int tex_h)
    : Plane(textureName, tex_w, tex_h)
{
    ((UIRenderer*)GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 128.f / 256.f, 64.f / 256.f, 64.f / 256.f);
}

Human::~Human()
{
}

void Human::Initialize()
{
    local_position_ = D3DXVECTOR3{ 32.f, 568.f, 0.f };
    local_scale_ = D3DXVECTOR3{ 0.2f, 0.2f, 1.f };
    velocity = D3DXVECTOR3{0.5f, 0.f, 0.f};
}

void Human::Update()
{
    local_position_ += velocity;
}

void Human::Draw()
{
    if (!IsActive()) return;
    UIRenderer* renderer = (UIRenderer*)GetComponent("UIRenderer");
    renderer->Draw(*this);
}

void Human::Finalize()
{
}
