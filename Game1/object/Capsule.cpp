#include "Capsule.h"
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include <hid/Input.h>
#include <dynamics/Collider.h>
#include "../Game1.h"

Capsule::Capsule(const char* textureName, int tex_w, int tex_h)
    : Plane(textureName, tex_w, tex_h)
{
    ((UIRenderer*)GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(128.f / 256.f, 0.f / 256.f, 64.f / 256.f, 64.f / 256.f);

    BoxCollider* bc = new BoxCollider;
    AddComponent("BoxCollider", bc);
}

Capsule::~Capsule()
{
}

void Capsule::Initialize()
{
    local_position_ = D3DXVECTOR3{ 400.f, 400.f, 0.f };
    local_scale_ = D3DXVECTOR3{ 0.2f, 0.2f, 1.f };

    BoxCollider* bc = (BoxCollider*)GetComponent("BoxCollider");
    bc->center_ = Vector2{
        local_position_.x,
        local_position_.y,
    };
    bc->box_width_ = Vector2{ 20.f, 20.f };   // 若干小さめに設定
}

void Capsule::Update()
{
    using Hirai::Input;
    DIMOUSESTATE2& dims = Input::GetMouseInput();
    // 名前が長いので
    float posX = local_position_.x;
    float posY = local_position_.y;
    // マウスの移動量を取る
    posX += dims.lX;
    posY += dims.lY;

    if (posX < 0) {
        posX = 0;
    }
    else if (posX > DISPLAY_WIDTH) {
        posX = DISPLAY_WIDTH;
    }
    if (posY < 0) {
        posY = 0;
    }
    else if (posY > DISPLAY_HEIGHT) {
        posY = DISPLAY_HEIGHT;
    }
    local_position_.x = posX;
    local_position_.y = posY;

    // colliderの座標を更新
    BoxCollider* bc = (BoxCollider*)GetComponent("BoxCollider");
    bc->Update(Vector2{ local_position_.x, local_position_.y });
}

void Capsule::Draw()
{
    if (!IsActive()) return;
    UIRenderer* renderer = (UIRenderer*)GetComponent("UIRenderer");
    renderer->Draw(*this);
}

void Capsule::Finalize()
{
}
