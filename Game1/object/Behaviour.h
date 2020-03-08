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

class Behaviour1 :
    public HumanBehaviour
{
protected:

public:
    Behaviour1(D3DXVECTOR3 vel, D3DXVECTOR3 pos);
    ~Behaviour1() {}
    
    virtual void Start() override;
    virtual void Update() override;

    virtual void Stop();
};

class Behaviour2 :
    public HumanBehaviour
{
protected:

public:
    Behaviour2(D3DXVECTOR3 vel, D3DXVECTOR3 pos);
    ~Behaviour2() {}

    virtual void Start() override;
    virtual void Update() override;

    virtual void Stop();
};

