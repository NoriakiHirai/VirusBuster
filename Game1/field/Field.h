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
    static std::vector<BoxCollider> inviolableAreas;

public:
    Field(const char* configFile);
    ~Field();

    void Initialize();
    void Update();
    void Finalize();
    
    void SetInviolableArea(const BoxCollider& range, const BoxCollider& range2);
    void SetInviolableArea(const BoxCollider& area);

    static const std::vector<BoxCollider>& GetAllInviolableArea();
    static void InitAllInviolableArea();

private:
    void DeleteVirus();
    void MultiplyViruses();
    void MultiplyViruses(POINT center);
};

extern void ReadConfigFile(const char* configFile, std::vector<Config>* vec);
