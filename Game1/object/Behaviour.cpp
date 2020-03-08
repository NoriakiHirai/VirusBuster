#include "Behaviour.h"
#include <common/GameObject.h>
#include "../scene/Game.h"
#include "Human.h"

Behaviour1::Behaviour1(D3DXVECTOR3 vel, D3DXVECTOR3 pos)
    : HumanBehaviour(vel, pos)
{
}

void Behaviour1::Start()
{
    gameObject().local_position_ = initPosition;
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

HumanBehaviour::HumanBehaviour(D3DXVECTOR3 vel, D3DXVECTOR3 pos)
    : velocity(vel), initPosition(pos)
{
}

void HumanBehaviour::SetVelocity(D3DXVECTOR3 vel)
{
    velocity = vel;
}
