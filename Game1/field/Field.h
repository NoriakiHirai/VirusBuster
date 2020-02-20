#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <dynamics/Collider.h>
#include <dynamics/Math.h>

class Virus;

struct Config
{
    int indexX, indexY;
    int order;
};

//struct MetaConfig
//{
//    int 
//};

class Field
{
public:
    std::vector<Virus*> viruses;

private:
    std::vector<Config> virusConfig;
    std::vector<POINT> occurancePoints;
    int elapsedTime, startTime;
    int currentOrder;
    int start, end;
    int frameCount;
    BoxCollider outOfMultiply;
    BoxCollider outOfMultiply2;

public:
    Field(const char* configFile);
    ~Field();

    void Initialize();
    void Update();
    void DeleteVirus();
    
    void SetOutOfMultiply(const BoxCollider& range, const BoxCollider& range2);

private:
    void MultiplyViruses();
    void MultiplyViruses(POINT center);
};

extern void ReadConfigFile(const char* configFile, std::vector<Config>* vec);
