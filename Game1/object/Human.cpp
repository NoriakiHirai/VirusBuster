#include "Human.h"
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include <dynamics/Collider.h>
#include <common/utility.h>
#include "Behaviour.h"

float Human::HUMAN_SPEED = 1.f;

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
    if (behaviour) behaviour->Start();
    local_scale_ = D3DXVECTOR3{ 0.2f, 0.2f, 1.f };

    BoxCollider* bc = (BoxCollider*)GetComponent("BoxCollider");
    bc->center_ = Vector2{
        local_position_.x,
        local_position_.y,
    };
    bc->box_width_ = Vector2{ 20.f, 20.f };   // ŽáŠ±¬‚³‚ß‚ÉÝ’è
}

void Human::Update()
{
    if (!IsActive()) return;

    Component* behaviour = GetComponent("Behaviour");
    if (behaviour) behaviour->Update();
    //local_position_ += velocity;
    // collider‚ÌÀ•W‚ðXV
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

const BoxCollider Human::GetInviolableArea()
{
    BoxCollider* hCol = (BoxCollider*)GetComponent("BoxCollider");
    BoxCollider col;
    D3DXVECTOR3 pos = NextPosition();
    col.center_.x = pos.x;
    col.center_.y = pos.y;
    col.box_width_ = hCol->box_width_;
    col.box_width_.x += 40.f;
    col.box_width_.y += 50.f;
    //col.box_width_.y += 20.f;

    return col;
}

void Human::GoalCheck(const POINT& goalPos)
{

}

void Human::Stop()
{
}

Human* Human::CreateHuman(HumanBehaviour* hb)
{
    Human* human = new Human("texture/GameParts.tga", 256, 256);
    human->AddComponent("Behaviour", hb);
    return human;
}

