#include "Human.h"
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include <dynamics/Collider.h>
#include <common/utility.h>
#include "Behavior.h"

float Human::HUMAN_SPEED = 0.5f;

Human::Human(const char* textureName, int tex_w, int tex_h)
    : Plane(textureName, tex_w, tex_h)
{
    ((UIRenderer*)GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 128.f / 256.f, 64.f / 256.f, 64.f / 256.f);
    BoxCollider* bc = new BoxCollider;
    AddComponent("BoxCollider", bc);
    SetLayer(Layer::kPlayer);
}

Human::Human(const char* textureName, int tex_w, int tex_h,
    float initPosX, float initPosY)
    : Plane(textureName, tex_w, tex_h), 
    initPosision(D3DXVECTOR3{initPosX, initPosY, 0.f})
{

    ((UIRenderer*)GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 128.f / 256.f, 64.f / 256.f, 64.f / 256.f);
    BoxCollider* bc = new BoxCollider;
    AddComponent("BoxCollider", bc);
}


Human::~Human()
{
}

void Human::Initialize()
{
    Component* behaviour = GetComponent("Behaviour");
    behaviour->Start();
    local_scale_ = D3DXVECTOR3{ 0.2f, 0.2f, 1.f };

    BoxCollider* bc = (BoxCollider*)GetComponent("BoxCollider");
    bc->center_ = Vector2{
        local_position_.x,
        local_position_.y,
    };
    bc->box_width_ = Vector2{ 20.f, 20.f };   // 若干小さめに設定
}

void Human::Update()
{
    if (!IsActive()) return;

    Component* behaviour = GetComponent("Behaviour");
    behaviour->Update();
    //local_position_ += velocity;
    // colliderの座標を更新
    BoxCollider* bc = (BoxCollider*)GetComponent("BoxCollider");
    bc->Update(Vector2{ local_position_.x, local_position_.y });
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

const D3DXVECTOR3 Human::NextPosition()
{
    return (local_position_ + velocity);
}

