#pragma once
#include <scene/Scene.h>

class Human;
class Plane;
class Capsule;
class Field;

class Stage2 : public Scene 
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
    //int stage;
    float waitTime;

public:
    static const float HOUSE_POS_X;
    static const float HOUSE_POS_Y;

    Stage2();
    ~Stage2();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;

private:
    void GameMain();
    void SetResultMsg(bool iswin);
    void SetPhase(int);
    void IsClear();
    void GameResult();
    void DispStageNum();
};