#pragma once
#include <scene/Scene.h>

class Plane;
class Human;

class Clear : public Scene
{
private:
    Plane* clear;
    Human* human;
    Human* human2;

public:
    Clear();
    ~Clear();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
};