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
    Human* human2;
    Plane* house;
    //Capsule* capsule;
    Plane* capsule;
    Field* field;
    Plane* resultMsg;
    Plane* stageName;
    Plane* clickMsg;

    int phase;
    bool isClear;
    int stage;
    float waitTime;

public:
    static const float HOUSE_POS_X;
    static const float HOUSE_POS_Y;

    Game();
    ~Game();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;

private:
    void GameMain();
    void SetClear(bool);
    void SetPhase(int);
    void IsClear();
    void GameResult();
    void SetStageNum(int);
    void DispStageNum();
    void PrepareStage(int);
};