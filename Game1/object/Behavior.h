#pragma once
#include <common/Component.h>

class HumanBehaviour : public Component
{
private:

protected:
    D3DXVECTOR3 velocity;
    D3DXVECTOR3 initPosition;


public:
    HumanBehaviour() {}
    HumanBehaviour(D3DXVECTOR3 vel, D3DXVECTOR3 pos);
    virtual ~HumanBehaviour() {}

    void SetVelocity(D3DXVECTOR3);
};

class Behavior1 :
    public HumanBehaviour
{
protected:

public:
    Behavior1(D3DXVECTOR3 vel, D3DXVECTOR3 pos);
    ~Behavior1() {}
    
    virtual void Start() override;
    virtual void Update() override;
};

class Behavior2 :
    public HumanBehaviour
{
protected:

public:
    Behavior2(D3DXVECTOR3 vel, D3DXVECTOR3 pos);
    ~Behavior2() {}

    virtual void Start() override;
    virtual void Update() override;
};

