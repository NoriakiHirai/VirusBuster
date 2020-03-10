#include "Field.h"
#include <algorithm>
#include <Windows.h>
#include <d3dx9.h>
#include <utility/CSVReader.h>
#include <common/Timer.h>
#include <dynamics/Collider.h>
#include <common/utility.h>
#include "../object/Virus.h"
#include "../Game1.h"

const int ORDER_MAX = 15;
const float SQUARE_X = DISPLAY_WIDTH / 15.f;
const float SQUARE_Y = DISPLAY_HEIGHT / 15.f;
const float VIRUS_OFFSET = 26.5f;

std::vector<BoxCollider> Field::inviolableAreas = {};

bool operator<(const Config& left, const Config& right)
{
    return left.order < right.order;
}

bool operator>(const Config& left, const Config& right)
{
    return left.order > right.order;
}

Field::Field(const char* configFile)
    : elapsedTime(0), currentOrder(1), virusRivivalInterval(15000)
{
    ReadConfigFile(configFile, &virusConfig);
    // �������Ƀ\�[�g
    std::sort(virusConfig.begin(), virusConfig.end());

    int divide = 4;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    occurancePoints.push_back(POINT{ DISPLAY_WIDTH * t1 / divide , DISPLAY_HEIGHT * t1 / divide });
    //occurancePoints.push_back(POINT{ DISPLAY_WIDTH * t2 / divide , DISPLAY_HEIGHT * t1 / divide });
    occurancePoints.push_back(POINT{ DISPLAY_WIDTH * t3 / divide , DISPLAY_HEIGHT * t1 / divide });

    //occurancePoints.push_back(POINT{ DISPLAY_WIDTH * t1 / divide , DISPLAY_HEIGHT * t2 / divide });
    //occurancePoints.push_back(POINT{ DISPLAY_WIDTH * t2 / divide , DISPLAY_HEIGHT * t2 / divide });
    //occurancePoints.push_back(POINT{ DISPLAY_WIDTH * t3 / divide , DISPLAY_HEIGHT * t2 / divide });
    occurancePoints.push_back(POINT{ DISPLAY_WIDTH_HALF, DISPLAY_HEIGHT_HALF});

    occurancePoints.push_back(POINT{ DISPLAY_WIDTH * t1 / divide , DISPLAY_HEIGHT * t3 / divide });
    //occurancePoints.push_back(POINT{ DISPLAY_WIDTH * t2 / divide , DISPLAY_HEIGHT * t3 / divide });
    occurancePoints.push_back(POINT{ DISPLAY_WIDTH * t3 / divide , DISPLAY_HEIGHT * t3 / divide });
}

Field::~Field()
{
    for (std::vector<Virus*>::iterator itr = viruses.begin();
        itr != viruses.end();
        ) {
        (*itr)->Destroy(*itr);
        itr = viruses.erase(itr);
    }
}

void Field::Initialize()
{
    start = end = currentOrder = 1;
    startTime = timeGetTime();
}

void Field::Update()
{
    // ��莞�Ԍo�߂��ƂɃE�C���X�𑝐B������
    if (elapsedTime >= 1000) {
        for (auto center : occurancePoints) {
            MultiplyViruses(center);
        }
        currentOrder += 1;
        elapsedTime = 0;
        startTime = timeGetTime();
    }
    else {
        elapsedTime = timeGetTime() - startTime;
    }
}

void Field::DeleteVirus()
{
    for (std::vector<Virus*>::iterator itr = viruses.begin();
        itr != viruses.end();
    ) {
        (*itr)->Destroy(*itr);
        itr = viruses.erase(itr);
    }
}

void Field::Finalize()
{
    inviolableAreas.erase(inviolableAreas.begin(), inviolableAreas.end());
    DeleteVirus();
}

void Field::SetInviolableArea(const BoxCollider& range, const BoxCollider& range2)
{
    inviolableAreas.push_back(range);
    inviolableAreas.push_back(range2);
}

void Field::SetInviolableArea(const BoxCollider& area)
{
    inviolableAreas.push_back(area);
}

void Field::SetVirusRivivalInterval(const int interval)
{
    virusRivivalInterval = interval;
}

const std::vector<BoxCollider>& Field::GetAllInviolableArea()
{
    return inviolableAreas;
}

void Field::InitAllInviolableArea()
{
    inviolableAreas.erase(inviolableAreas.begin(), inviolableAreas.end());
}

void Field::MultiplyViruses()
{
    if (currentOrder > 16) return;
    int virusMax = (currentOrder + 1) * (currentOrder + 1);
    Virus* virus = nullptr;
    BoxCollider* col = nullptr;
    // theta = rad * (180 / PI);
    float theta = (360.f / virusMax) * (D3DX_PI / 180.f);
    for (auto i = 0; i < virusMax; ++i) {
         virus = new Virus("texture/GameParts.tga", 256, 256);
         // x = (���S���W�j + cos(�p�x�j* ���a
         float x = DISPLAY_WIDTH_HALF + cosf(theta * i) * (35.f * currentOrder);
         float y = DISPLAY_HEIGHT_HALF + sinf(theta * i) * (35.f * currentOrder);
         //float x = SQUARE_X * (float)(virusConfig[i - 1].indexX) + VIRUS_OFFSET;
         //float y = SQUARE_Y * (float)(virusConfig[i - 1].indexY) + VIRUS_OFFSET;
         virus->local_position_ = D3DXVECTOR3{
             x, y, 1.f
         };
         virus->local_scale_ = D3DXVECTOR3{ 0.1f, 0.1f, 1.f };
         virus->SetLayer(Layer::kStage);
         col = (BoxCollider*)virus->GetComponent("BoxCollider");
         col->center_.x = virus->local_position_.x;
         col->center_.y = virus->local_position_.y;
         //if (col->Check(outOfMultiply) ||
         //    col->Check(outOfMultiply2)) {
         //    virus->SetActive(false);
         //}
         viruses.push_back(virus);
    }
    currentOrder += 1;

    // CSV�t�@�C������E�C���X�̔������W�b�N��ǂݍ��ޏꍇ
     //if (currentOrder > ORDER_MAX) return;
    //currentOrder += 2;
    //start = end + 1;
    //end = currentOrder * currentOrder;
    //int length = virusConfig.size() - 1;
    //if (end > length) {
    //    end = length;
    //}

    //Virus* virus = nullptr;
    //BoxCollider* col = nullptr;
    //for (auto i = start; virusConfig[i - 1].order <= end; ++i) {
    //    virus = new Virus("texture/GameParts.tga", 256, 256);
    //    float x = SQUARE_X * (float)(virusConfig[i - 1].indexX) + VIRUS_OFFSET;
    //    float y = SQUARE_Y * (float)(virusConfig[i - 1].indexY) + VIRUS_OFFSET;
    //    virus->local_position_ = D3DXVECTOR3{
    //        x, y, 1.f
    //    };
    //    virus->local_scale_ = D3DXVECTOR3{ 0.1f, 0.1f, 1.f };
    //    virus->SetLayer(Layer::kStage);
    //    col = (BoxCollider*)virus->GetComponent("BoxCollider");
    //    col->center_.x = virus->local_position_.x;
    //    col->center_.y = virus->local_position_.y;
    //    if (col->Check(outOfMultiply)) {
    //        virus->SetActive(false);
    //    }
    //    viruses.push_back(virus);
    //}
}

void Field::MultiplyViruses(POINT center)
{
    if (currentOrder > 9) return;
    int virusMax = (currentOrder + 1) * (currentOrder + 1);
    Virus* virus = nullptr;
    BoxCollider* col = nullptr;
    // theta = rad * (180 / PI);
    float theta = (360.f / virusMax) * (D3DX_PI / 180.f);
    for (auto i = 0; i < virusMax; ++i) {
        virus = new Virus("texture/GameParts.tga", 256, 256);
        // x = (���S���W�j + cos(�p�x�j* ���a
        float x = center.x + cosf(theta * i) * (35.f * currentOrder);
        float y = center.y + sinf(theta * i) * (35.f * currentOrder);
        virus->local_position_ = D3DXVECTOR3{
            x, y, 1.f
        };
        virus->local_scale_ = D3DXVECTOR3{ 0.1f, 0.1f, 1.f };
        virus->SetLayer(Layer::kStage);
        col = (BoxCollider*)virus->GetComponent("BoxCollider");
        col->center_.x = virus->local_position_.x;
        col->center_.y = virus->local_position_.y;
        for (auto area : inviolableAreas) {
            if (col->Check(area)) {
                virus->SetActive(false);
                break;
            }
        }
        virus->SetRivivalInterval(virusRivivalInterval);
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
