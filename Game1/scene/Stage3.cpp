#include "Stage3.h"
#include <common/utility.h>
#include <common/Timer.h>
#include <graphics/Plane.h>
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include <dynamics/Collider.h>
#include <hid/Input.h>
#include "../object/Human.h"
#include "../field/Field.h"
#include "../object/Capsule.h"
#include "../object/Virus.h"
#include "../Game1.h"
#include "../object/Behavior.h"


Stage3::Stage3()
{
    Human* human;
    // 8êlï™çÏê¨
    for (auto i = 0; i < 4; ++i) {
        human = new Human("texture/GameParts.tga", 256, 256);
        Behavior1* bh1 = new Behavior1(
            D3DXVECTOR3{ Human::HUMAN_SPEED, 0.f, 0.f },
            D3DXVECTOR3{ 64.f, 568.f, 0.f }
        );
        human->AddComponent("Behaviour", bh1);
        human->SetActive(false);
        people.push_back(human);

        human = new Human("texture/GameParts.tga", 256, 256);
        Behavior2* bh2 = new Behavior2(
            D3DXVECTOR3{ 0.f, -Human::HUMAN_SPEED, 0.f },
            D3DXVECTOR3{ 32.f, 536.f, 0.f }
        );
        human->AddComponent("Behaviour", bh2);
        people.push_back(human);
    }

    house = new Plane("texture/GameParts.tga", 256, 256);
    ((UIRenderer*)house->GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 0.f / 256.f, 128.f / 256.f, 128.f / 256.f);
    house->SetLayer(Layer::kStage);

    field = new Field("data/VirusConfig.csv");

    capsule = new Capsule("texture/GameParts.tga", 256, 256);
    resultMsg = new Plane("texture/sign.tga", 128, 512);
    resultMsg->SetActive(false);
    resultMsg->local_position_ = D3DXVECTOR3{ DISPLAY_WIDTH_HALF, DISPLAY_HEIGHT_HALF, 0.f };
    resultMsg->local_scale_ = D3DXVECTOR3{ 1.f, 0.25f, 1.f };
    resultMsg->SetLayer(Layer::kMsg);

    stageName = new Plane("texture/sign.tga", 128, 512);
    stageName->state = new Plane::Display;
    stageName->SetActive(true);
    stageName->local_position_ = D3DXVECTOR3{ DISPLAY_WIDTH_HALF, DISPLAY_HEIGHT * 2 / 5, 0.f };
    stageName->local_scale_ = D3DXVECTOR3{ 1.f, 0.25f, 1.f };
    stageName->SetLayer(Layer::kMsg);
}

Stage3::~Stage3()
{
    for (auto human : people) {
        human->Destroy(human);
    }
    house->Destroy(house);
    capsule->Destroy(capsule);
    resultMsg->Destroy(resultMsg);
    stageName->Destroy(stageName);
    SAFE_DELETE(field);
}

void Stage3::Initialize()
{
}

void Stage3::Update()
{
}

void Stage3::Draw()
{
}

void Stage3::Finalize()
{
}
