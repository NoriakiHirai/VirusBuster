#include "Game.h"
#include <vector>
#include <common/utility.h>
#include <common/Timer.h>
#include <graphics/Plane.h>
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include <dynamics/Collider.h>
#include "../object/Human.h"
#include "../field/Field.h"
#include "../object/Capsule.h"
#include "../object/Virus.h"
#include "../Game1.h"
#include "../object/Behavior.h"

static const float HOUSE_POS_X = 736.f;
static const float HOUSE_POS_Y = 536.f;

Game::Game()
{
    human = new Human("texture/GameParts.tga", 256, 256);
    Behavior1* bh1 = new Behavior1(
        D3DXVECTOR3{ 0.5f, 0.f, 0.f },
        D3DXVECTOR3{ 32.f, 568.f, 0.f }
    );
    human->AddComponent("Behaviour", bh1);

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
}

Game::~Game()
{
    SAFE_DELETE(human);
    SAFE_DELETE(human2);
    SAFE_DELETE(house);
    //SAFE_DELETE(virus);
    SAFE_DELETE(capsule);
    SAFE_DELETE(field);
    SAFE_DELETE(resultMsg);
}

void Game::Initialize()
{
    house->local_position_ = D3DXVECTOR3{ HOUSE_POS_X, HOUSE_POS_Y, 0.f };
    house->local_scale_ = D3DXVECTOR3{ 0.5f, 0.5f, 1.f };
    
    field->Initialize();

    GameObject::InitObjectAll();
    Timer::Initialize();

    phase = 1;
    winner = 0;
}

void Game::Update()
{
    Timer::Update();
    IsClear();

    switch (phase)
    {
    case 1:
        GameMain();
        break;
    case 2:
        GameResult();
        break;
    default:
        break;
    }
}

void Game::Draw()
{
    GameObject::DrawObjectAll();
}

void Game::Finalize()
{
    SAFE_DELETE(human);
    SAFE_DELETE(house);
    SAFE_DELETE(capsule);
    SAFE_DELETE(field);
}

void Game::GameMain()
{
    // for debug
    if (!human) return;
    // ウイルスの増殖など
    // 人が出現したウイルスでいきなり消されないように発生範囲外を設定する
    BoxCollider* hCol = (BoxCollider*)human->GetComponent("BoxCollider");
    D3DXVECTOR3 pos = human->NextPosition();
    hCol->center_.x = pos.x;
    hCol->center_.y = pos.y;
    field->SetOutOfMultiply(*hCol);
    field->Update();

    GameObject::UpdateObjectAll();

    // 衝突検知
    std::vector<Virus*>::iterator itr;
    BoxCollider* vCol;
    BoxCollider* cCol = (BoxCollider*)capsule->GetComponent("BoxCollider");
    BoxCollider* gCol = (BoxCollider*)house->GetComponent("BoxCollider");

    for (itr = field->viruses.begin();
        itr != field->viruses.end();
        )
    {
        if (!(*itr)->IsActive()) {
            ++itr;
            continue;
        }

        vCol = (BoxCollider*)(*itr)->GetComponent("BoxCollider");
        if (hCol->Check(*vCol)) {
            human->SetActive(false);
            SetWinner(2);
            SetPhase(2);
            // TODO:ミス時の処理に遷移
            break;
        }

        if (cCol->Check(*vCol)) {
            (*itr)->SetActive(false);
            (*itr)->TimerReset();
            //(*itr)->Destroy(*itr);
            //itr = field->viruses.erase(itr);
        }
        else {
            ++itr;
        }
    }
}

void Game::SetWinner(int num)
{
    winner = num;
    resultMsg->SetActive(true);
    if (num == 1) {
        ((UIRenderer*)resultMsg->GetComponent("UIRenderer"))
            ->GetSprite()
            ->SetUV(0.f / 128.f, 64.f / 512.f, 128.f / 128.f, 64.f / 512.f);
    }
    else if (num == 2) {
        ((UIRenderer*)resultMsg->GetComponent("UIRenderer"))
            ->GetSprite()
            ->SetUV(0.f / 128.f, 128.f / 512.f, 128.f / 128.f, 64.f / 512.f);
    }
}

void Game::SetPhase(int ph)
{
    phase = ph;
}

void Game::IsClear()
{
    if (human->local_position_.x >= HOUSE_POS_X)
    {
        SetWinner(1);
        SetPhase(2);
    }
}

void Game::GameResult()
{
}
