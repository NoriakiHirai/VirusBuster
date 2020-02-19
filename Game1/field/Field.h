#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <dynamics/Collider.h>

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
    int elapsedTime, startTime;
    int currentOrder;
    int start, end;
    int frameCount;
    BoxCollider outOfMultiply;

public:
    Field(const char* configFile);
    ~Field();

    void Initialize();
    void Update();
    
    void SetOutOfMultiply(const BoxCollider&);

private:
    void MultiplyViruses();
};

extern void ReadConfigFile(const char* configFile, std::vector<Config>* vec);
