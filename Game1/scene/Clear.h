#pragma once
#include <scene/Scene.h>

class Plane;

class Clear : public Scene
{
private:
    Plane* clear;

public:
    Clear();
    ~Clear();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
};