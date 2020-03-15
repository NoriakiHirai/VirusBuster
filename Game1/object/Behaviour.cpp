#include "Behaviour.h"
#include <common/GameObject.h>
#include "../scene/Game.h"
#include "Human.h"

HumanBehaviour::HumanBehaviour(D3DXVECTOR3 vel, D3DXVECTOR3 pos)
    : velocity(vel), initPosition(pos)
{
}

void HumanBehaviour::SetVelocity(D3DXVECTOR3 vel)
{
    velocity = vel;
}

void HumanBehaviour::SetSpeed(float sp)
{
    speed = sp;
}

Behaviour1::Behaviour1(D3DXVECTOR3 vel, D3DXVECTOR3 pos)
    : HumanBehaviour(vel, pos)
{
}

void Behaviour1::Start()
{
    gameObject().local_position_ = initPosition;
    //velocity = D3DXVECTOR3{ speed, 0.f, 0.f };
    gameObject().SetActive(true);
    SetStarted(true);
}

void Behaviour1::Update()
{
    if (!IsStarted()) { Start(); }
    D3DXVECTOR3 pos = gameObject().local_position_;
    // •ûŒü“]Š·
    if (pos.x >= Game::HOUSE_POS_X && pos.y == initPosition.y) {
        velocity = D3DXVECTOR3{ 0.f, -Human::HUMAN_SPEED, 0.f };
    }
    gameObject().local_position_ += velocity;
}

void Behaviour1::Stop()
{
    gameObject().SetActive(false);
    SetStarted(false);
}

Behaviour2::Behaviour2(D3DXVECTOR3 vel, D3DXVECTOR3 pos)
    : HumanBehaviour(vel, pos)
{
}

void Behaviour2::Start()
{
    gameObject().local_position_ = initPosition;
    gameObject().SetActive(true);
    SetStarted(true);
}

void Behaviour2::Update()
{
    if (!IsStarted()) { Start(); }
    D3DXVECTOR3 pos = gameObject().local_position_;
    // •ûŒü“]Š·
    if (pos.x == initPosition.x && pos.y <= 96.f) {
        velocity = D3DXVECTOR3{ Human::HUMAN_SPEED, 0.f, 0.f };
    }
    gameObject().local_position_ += velocity;
}

void Behaviour2::Stop()
{
    gameObject().SetActive(false);
    SetStarted(false);
}

Behaviour3::Behaviour3(D3DXVECTOR3 vel, D3DXVECTOR3 initPos, D3DXVECTOR3 goalPos)
    : HumanBehaviour(vel, initPos)
{
    goalPosition = goalPos;
    float diffX = goalPos.x - initPos.x;
    float diffY = goalPos.y - initPos.y;
    if (diffX == 0.f) {
        slope = 0.f;
    }
    else {
        slope = diffY / diffX;
    }
}

void Behaviour3::Start()
{
    gameObject().local_position_ = initPosition;
    gameObject().SetActive(true);
    SetStarted(true);
}

void Behaviour3::Update()
{
    if (!IsStarted()) { Start(); }
    // sinƒJ[ƒu‚ð•`‚¢‚ÄƒS[ƒ‹‚ÉŒü‚©‚Á‚Ä‚¢‚­
    D3DXVECTOR3 currentPos = gameObject().local_position_;
    
    // ˆê’è‹——£‚Ü‚Å‹ß‚Ã‚¢‚½‚çˆê’¼ü‚ÉƒS[ƒ‹‚ÉŒü‚©‚¤
    D3DXVECTOR3 toGoal = goalPosition - currentPos;
    float len = D3DXVec3Length(&(toGoal));
    if (len <= 100.f) {
        D3DXVECTOR3 vec;
        D3DXVec3Normalize(&vec, &toGoal);
        D3DXVECTOR3 velocity = vec * speed;
        gameObject().local_position_ += velocity;
    }
    else {
        float nextX = currentPos.x + speed;
        float nextY = (sinf(nextX / 5.f) * 100.f) + (slope * nextX) + initPosition.y;
        D3DXVECTOR3 nextPos{ nextX, nextY, 0.f };
        gameObject().local_position_ = nextPos;
    }
}

void Behaviour3::Stop()
{
    gameObject().SetActive(false);
    SetStarted(false);
}

Behaviour4::Behaviour4(D3DXVECTOR3 vel, D3DXVECTOR3 initPos, D3DXVECTOR3 goalPos)
    : HumanBehaviour(vel, initPos)
{
    goalPosition = goalPos;
    float diffX = goalPos.x - initPos.x;
    float diffY = goalPos.y - initPos.y;
    if (diffX == 0.f) {
        slope = 0.f;
    }
    else {
        slope = diffY / diffX;
    }
}

void Behaviour4::Start()
{
    gameObject().local_position_ = initPosition;
    gameObject().SetActive(true);
    SetStarted(true);
}

void Behaviour4::Update()
{
    if (!IsStarted()) { Start(); }
    // sinƒJ[ƒu‚ð•`‚¢‚ÄƒS[ƒ‹‚ÉŒü‚©‚Á‚Ä‚¢‚­
    D3DXVECTOR3 currentPos = gameObject().local_position_;

    // ˆê’è‹——£‚Ü‚Å‹ß‚Ã‚¢‚½‚çˆê’¼ü‚ÉƒS[ƒ‹‚ÉŒü‚©‚¤
    D3DXVECTOR3 toGoal = goalPosition - currentPos;
    float len = D3DXVec3Length(&(toGoal));
    if (len <= 100.f) {
        D3DXVECTOR3 vec;
        D3DXVec3Normalize(&vec, &toGoal);
        D3DXVECTOR3 velocity = vec * speed;
        gameObject().local_position_ += velocity;
    }
    else {
        float nextX = currentPos.x + speed;
        float nextY = (cosf(nextX / 5.f) * 100.f) + (slope * nextX) + initPosition.y;
        D3DXVECTOR3 nextPos{ nextX, nextY, 0.f };
        gameObject().local_position_ = nextPos;
    }
}

void Behaviour4::Stop()
{
    gameObject().SetActive(false);
    SetStarted(false);
}

Behaviour5::Behaviour5(D3DXVECTOR3 vel, D3DXVECTOR3 initPos)
    : HumanBehaviour(vel, initPos)
{
}

void Behaviour5::Start()
{
    gameObject().local_position_ = initPosition;
    gameObject().SetActive(true);
    SetStarted(true);
}

void Behaviour5::Update()
{
    if (!IsStarted()) { Start(); }
    if (gameObject().local_position_.y >= 568.f) {
        velocity.y = -5.f;
    }
    else {
        velocity.y += 0.2f;
    }
    gameObject().local_position_ += velocity;
}
