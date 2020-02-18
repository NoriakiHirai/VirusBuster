#pragma once
#include <vector>
#include <fstream>
#include <sstream>

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

public:
    Field(const char* configFile);
    ~Field();

    void Initialize();
    void Update();

private:
    void MultiplyViruses();
};

extern void ReadConfigFile(const char* configFile, std::vector<Config>* vec);
