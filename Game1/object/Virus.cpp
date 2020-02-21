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
    bc->box_width_ = Vector2{ 6.f, 6.f };   // 若干小さめに設定
    AddComponent("BoxCollider", bc);

    TimerStart();
}

Virus::~Virus()
{
}

void Virus::Initialize()
{
}

void Virus::Update()
{
    // colliderの座標を更新
    BoxCollider* bc = (BoxCollider*)GetComponent("BoxCollider");
    bc->Update(Vector2{ local_position_.x, local_position_.y });

    // 削除されてから、一定時間後に復活する
    if (!IsActive() && ElapsedTime() >= 10000) {
        if (!bc->Check(outOfMultiply) && 
            !bc->Check(outOfMultiply2)) {
            SetActive(true);
        }
    }
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

void Virus::SetOutOfMultiply(const BoxCollider& range, const BoxCollider& range2)
{
    outOfMultiply = range;
    outOfMultiply2 = range2;
}
