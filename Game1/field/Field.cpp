#include "Field.h"
#include <algorithm>
#include <d3dx9.h>
#include <utility/CSVReader.h>
#include <common/Timer.h>
#include "../object/Virus.h"

const int ORDER_MAX = 17;

bool operator<(const Config& left, const Config& right)
{
    return left.order < right.order;
}

bool operator>(const Config& left, const Config& right)
{
    return left.order > right.order;
}

Field::Field(const char* configFile)
    : elapsedTime(0), currentOrder(1)
{
    ReadConfigFile(configFile, &virusConfig);
    // 発生順にソート
    std::sort(virusConfig.begin(), virusConfig.end());
}

Field::~Field()
{
}

void Field::Initialize()
{
    start = end = currentOrder = 1;
    startTime = timeGetTime();
    Virus* virus = new Virus("texture/GameParts.tga", 256, 256);
    float x = 50.f * (float)(virusConfig[0].indexX);
    float y = 50.f * (float)(virusConfig[0].indexY) - 150.f;
    virus->local_position_ = D3DXVECTOR3{
        x, y, 0.f
    };
    virus->local_scale_ = D3DXVECTOR3{ 0.1f, 0.1f, 1.f };
    viruses.push_back(virus);


    //size_t lengthX = virusConfig[0].size();
    //size_t lengthY = virusConfig.size();
    //Virus* virus;
    //float x, y;
    //for (size_t i = 0; i < lengthY; ++i) {
    //    for (size_t j = 0; j < lengthX; ++j) {
    //        if (virusConfig[i][j] == 1) {
    //            virus = new Virus("texture/GameParts.tga", 256, 256);
    //            x = 50.f * (float)j;
    //            y = 50.f * (float)i;
    //            virus->local_position_ = D3DXVECTOR3{
    //                x, y, 0.f
    //            };
    //            virus->local_scale_ = D3DXVECTOR3{ 0.1f, 0.1f, 1.f };
    //            Viruses.push_back(virus);
    //        }
    //    }
    //}
}

void Field::Update()
{
    // 2秒経過ごとにウイルスを増殖させる
    if (elapsedTime >= 1000) {
        MultiplyViruses();
        elapsedTime = 0;
        startTime = timeGetTime();
    }
    else {
        elapsedTime = timeGetTime() - startTime;
    }
}

void Field::MultiplyViruses()
{
    if (currentOrder > ORDER_MAX) return;

    currentOrder += 2;
    start = end + 1;
    end = currentOrder * currentOrder;
    int length = virusConfig.size() - 1;
    if (end > length) {
        end = length;
    }

    Virus* virus = nullptr;
    for (auto i = start; virusConfig[i - 1].order <= end; ++i) {
        virus = new Virus("texture/GameParts.tga", 256, 256);
        float x = 50.f * (float)(virusConfig[i - 1].indexX);
        float y = 50.f * (float)(virusConfig[i - 1].indexY) - 150.f;
        virus->local_position_ = D3DXVECTOR3{
            x, y, 0.f
        };
        virus->local_scale_ = D3DXVECTOR3{ 0.1f, 0.1f, 1.f };
        viruses.push_back(virus);
    }
}

void ReadConfigFile(const char* configFile, std::vector<Config>* vec)
{
    std::ifstream ifs(configFile);
    if (!ifs.is_open()) {
        return;
    }

    string line;
    int row = 0;
    while (std::getline(ifs, line))
    {
        vector<string> v = split(line, ',');
        for (size_t i = 0; i < v.size(); i++)
        {
            Config config;
            config.indexX = i;
            config.indexY = row;
            config.order = std::stoi(v.at(i));
            vec->push_back(config);
        }
        row++;
    }

}
