#pragma once
#include <common/Component.h>

class HumanBehaviour : public Component
{
private:

protected:
    D3DXVECTOR3 velocity;
    D3DXVECTOR3 initPosition;
    float speed;

public:
    HumanBehaviour() {}
    HumanBehaviour(D3DXVECTOR3 vel, D3DXVECTOR3 pos);
    virtual ~HumanBehaviour() {}

    void SetVelocity(D3DXVECTOR3);
    void SetSpeed(float);
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

class Behaviour3 :
    public HumanBehaviour
{
private:
    D3DXVECTOR3 goalPosition;
    float slope;    // ïœâªÇÃäÑçá

protected:

public:
    Behaviour3(D3DXVECTOR3 vel, D3DXVECTOR3 initPos, D3DXVECTOR3 goalPos);
    ~Behaviour3() {}

    virtual void Start() override;
    virtual void Update() override;

    virtual void Stop();
};

class Behaviour4 :
    public HumanBehaviour
{
private:
    D3DXVECTOR3 goalPosition;
    float slope;    // ïœâªÇÃäÑçá

protected:

public:
    Behaviour4(D3DXVECTOR3 vel, D3DXVECTOR3 initPos, D3DXVECTOR3 goalPos);
    ~Behaviour4() {}

    virtual void Start() override;
    virtual void Update() override;

    virtual void Stop();
};

class Behaviour5 :
    public HumanBehaviour
{
private:
    D3DXVECTOR3 goalPosition;
    float slope;    // ïœâªÇÃäÑçá

protected:

public:
    Behaviour5(D3DXVECTOR3 vel, D3DXVECTOR3 initPos);
    ~Behaviour5() {}

    virtual void Start() override;
    virtual void Update() override;
};
