#pragma once
#include <vector>
#include <scene/Scene.h>

class Plane;
class Human;

class Title : public Scene
{
private:
    Plane* title;
    Human* human;
    Plane* house;
    std::vector<Plane*> viruses;

public:
    Title();
    ~Title();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
};

