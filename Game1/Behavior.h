#pragma once
#include <common/Component.h>

class Behavior1 :
    public Component
{
public:
    virtual void Start() override;
    virtual void Update() override;
};

class Behavior2 :
    public Component
{
public:
    virtual void Start() override;
    virtual void Update() override;
};

