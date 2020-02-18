#include "Virus.h"
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include <dynamics/Collider.h>

Virus::Virus(const char* textureName, int tex_w, int tex_h)
    : Plane(textureName, tex_w, tex_h)
{
    ((UIRenderer*)GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(192.f / 256.f, 0.f / 256.f, 16.f / 256.f, 16.f / 256.f);

    BoxCollider* bc = new BoxCollider;
    bc->center_ = Vector2{
        local_position_.x,
        local_position_.y,
    };
    bc->box_width_ = Vector2{ 6.f, 6.f };   // ŽáŠ±¬‚³‚ß‚ÉÝ’è
    AddComponent("BoxCollider", bc);
}

Virus::~Virus()
{
}

void Virus::Initialize()
{
}

void Virus::Update()
{
    // collider‚ÌÀ•W‚ðXV
    BoxCollider* bc = (BoxCollider*)GetComponent("BoxCollider");
    bc->Update(Vector2{ local_position_.x, local_position_.y });
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
