#pragma once
#include <scene/Scene.h>

class Human;
class Plane;
class Capsule;
class Field;

class Game : public Scene
{
private:
    Human* human;
    Plane* house;
    Capsule* capsule;
    Field* field;

public:
    Game();
    ~Game();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
};