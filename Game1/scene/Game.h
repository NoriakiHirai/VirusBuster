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
    //Capsule* capsule;
    Plane* capsule;
    Field* field;
    Plane* resultMsg;

    int phase;
    int winner;

public:
    Game();
    ~Game();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;

private:
    void GameMain();
    void SetWinner(int);
    void SetPhase(int);
    void IsClear();
    void GameResult();
};