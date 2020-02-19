#include "Behavior.h"
#include <common/GameObject.h>

Behavior1::Behavior1(D3DXVECTOR3 vel, D3DXVECTOR3 pos)
    : HumanBehaviour(vel, pos)
{
}

void Behavior1::Start()
{
    gameObject().local_position_ = initPosition;
    SetStarted(true);
}

void Behavior1::Update()
{
    if (!IsStarted()) { Start(); }
    gameObject().local_position_ += velocity;
}

Behavior2::Behavior2(D3DXVECTOR3 vel, D3DXVECTOR3 pos)
    : HumanBehaviour(vel, pos)
{
}

void Behavior2::Start()
{
    gameObject().local_position_ = initPosition;
    SetStarted(true);
}

void Behavior2::Update()
{
    if (!IsStarted()) { Start(); }
    gameObject().local_position_ += velocity;
}

HumanBehaviour::HumanBehaviour(D3DXVECTOR3 vel, D3DXVECTOR3 pos)
    : velocity(vel), initPosition(pos)
{
}
