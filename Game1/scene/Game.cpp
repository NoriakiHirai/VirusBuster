#include "Game.h"
#include <vector>
#include <common/utility.h>
#include <common/Timer.h>
#include <graphics/Plane.h>
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include <dynamics/Collider.h>
#include <hid/Input.h>
#include <common/Singleton.h>
#include <audio/dx/Audio.h>
#include "../object/Human.h"
#include "../field/Field.h"
#include "../object/Capsule.h"
#include "../object/Virus.h"
#include "../Game1.h"
#include "../object/Behaviour.h"
#include "../scene/Stage2.h"
#include "CommonData.h"

const float Game::HOUSE_POS_X = 736.f;
const float Game::HOUSE_POS_Y = 536.f;

static DirectAudio& dAudio = singleton<DirectAudio>::GetInstance();

Game::Game()
{
    human = new Human("texture/GameParts.tga", 256, 256);
    Behaviour1* bh1 = new Behaviour1(
        D3DXVECTOR3{ Human::HUMAN_SPEED, 0.f, 0.f },
        D3DXVECTOR3{ 64.f, 568.f, 0.f }
    );
    human->AddComponent("Behaviour", bh1);

    house = new Plane("texture/GameParts.tga", 256, 256);
    ((UIRenderer*)house->GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 0.f / 256.f, 128.f / 256.f, 128.f / 256.f);
    house->SetLayer(Layer::kStage);
    house->local_scale_ = D3DXVECTOR3{ 0.5f, 0.5f, 1.f };

    field = new Field("data/VirusConfig.csv");
    field->SetVirusRivivalInterval(2000);

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

Game::~Game()
{
    human->Destroy(human);
    house->Destroy(house);
    capsule->Destroy(capsule);
    resultMsg->Destroy(resultMsg);
    stageName->Destroy(stageName);
    SAFE_DELETE(field);
}

void Game::Initialize()
{
    human->SetActive(true);
    Behaviour1* bh1 = (Behaviour1*)human->GetComponent("Behaviour");
    bh1->SetVelocity(D3DXVECTOR3{ Human::HUMAN_SPEED, 0.f, 0.f });
    house->local_position_ = D3DXVECTOR3{ HOUSE_POS_X, HOUSE_POS_Y, 0.f };

    // ステージプレートの設定
    Sprite *sprite = ((UIRenderer*)stageName->GetComponent("UIRenderer"))
        ->GetSprite();
    sprite->SetAlpha(1.f);
    sprite->SetUV(0.f / 128.f, 224.f / 1024.f, 128.f / 128.f, 64.f / 1024.f);
    
    field->Initialize();

    stageName->SetActive(true);
    stageName->state = new Plane::FadeOut;
    waitTime = 0.5f;

    GameObject::InitObjectAll();
    Timer::Initialize();

    //dAudio.PlayOneShot("LevelDisp");

    phase = 1;
    isClear = false;
}

void Game::Update()
{
    Timer::Update();

    switch (phase)
    {
    case 1:
        DispStageNum();
        break;
    case 2:
        IsClear();
        GameMain();
        break;
    case 3:
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
}

void Game::GameMain()
{
    if (!human) return;
    // ウイルスの増殖など
    // 人が出現したウイルスでいきなり消されないように発生範囲外を設定する
    Field::InitAllInviolableArea();
    field->SetInviolableArea(human->GetInviolableArea());
    field->Update();

    GameObject::UpdateObjectAll();

    // 衝突検知
    std::vector<Virus*>::iterator itr;
    BoxCollider* vCol;
    BoxCollider* cCol = (BoxCollider*)capsule->GetComponent("BoxCollider");
    BoxCollider* hCol = (BoxCollider*)human->GetComponent("BoxCollider");
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
            SetResultMsg(false);
            SetPhase(3);
            dAudio.PlayOneShot("Miss");
            break;
        }

        if (cCol->Check(*vCol)) {
            (*itr)->SetActive(false);
            (*itr)->TimerReset();
        }
        ++itr;
    }
}

void Game::SetResultMsg(bool isWin)
{
    isClear = isWin;
    resultMsg->SetActive(true);
    if (isWin) {
        ((UIRenderer*)resultMsg->GetComponent("UIRenderer"))
            ->GetSprite()
            ->SetUV(0.f / 128.f, 64.f / 1024.f, 128.f / 128.f, 64.f / 1024.f);
    }
    else {
        ((UIRenderer*)resultMsg->GetComponent("UIRenderer"))
            ->GetSprite()
            ->SetUV(0.f / 128.f, 128.f / 1024.f, 128.f / 128.f, 64.f / 1024.f);
    }
}

void Game::SetPhase(int ph)
{
    phase = ph;
    if (phase == 3) {
        dAudio.Stop("BGM");
    }
}

void Game::IsClear()
{
    if (human->local_position_.x >= house->local_position_.x &&
        human->local_position_.y <= house->local_position_.y + 32.f)
    {
        SetResultMsg(true);
        SetPhase(3);
        dAudio.PlayOneShot("StageClear");
    }
}

void Game::GameResult()
{
    using Hirai::Input;
    if (Input::GetMouseLeftButtonTrigger())
    {
        dAudio.PlayOneShot("Decision");
        resultMsg->SetActive(false);
        if (isClear) {

            CommonData::SetCurrentStage(2);
            Scene::SetScene(new Stage2);
        }
        else {
            SetPhase(1);
            Initialize();
            field->Finalize();
        }
    }
}


void Game::DispStageNum()
{
    waitTime -= Timer::DeltaTime();
    if (waitTime <= 0.f) {
        if (stageName->UpdateFade()) {
            //dAudio.Stop("LevelDisp");
            dAudio.PlayLoop("BGM");
            SetPhase(2);
            stageName->SetActive(false);
        }
    }
}
