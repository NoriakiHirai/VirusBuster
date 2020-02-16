#pragma once

class GameObject;

class Effect
{
public:
    Effect() {}
    virtual ~Effect() {}

    //virtual bool ConfirmDevice() = 0;
    virtual bool Initialize() = 0;
    virtual bool Draw(const GameObject& obj) = 0;
    virtual void Finalize() = 0;
};
