#pragma once
#include <vector>
#include <scene/Scene.h>

class Human;
class Plane;
class Capsule;
class Field;
class BoxCollider;

class Stage5 : public Scene
{
public:
    Stage5();
    ~Stage5();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;

private:
    static const float HOUSE_POS_X;
    static const float HOUSE_POS_Y;

    //std::vector<Human*> people;
    Human* human;
    Human* human2;
    Plane* house;
    //Capsule* capsule;
    Plane* capsule;
    Field* field;
    Plane* resultMsg;
    Plane* stageName;
    BoxCollider* startArea;

    int phase;
    bool isClear;
    //int stage;
    float waitTime;
    float humanCreateInterval;

private:
    void GameMain();
    void SetResultMsg(bool iswin);
    void SetPhase(int);
    void IsClear();
    bool GoalCheck(Human*);
    void GameResult();
    void DispStageNum();
    bool CollisionCheckWithHuman(const BoxCollider& virusCol, BoxCollider& humCol);
};
