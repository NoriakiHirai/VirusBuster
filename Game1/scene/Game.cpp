#include "Game.h"
#include <vector>
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
#include "../scene/Title.h"

const float Game::HOUSE_POS_X = 736.f;
const float Game::HOUSE_POS_Y = 536.f;

Game::Game()
{
    human = new Human("texture/GameParts.tga", 256, 256);
    Behavior1* bh1 = new Behavior1(
        D3DXVECTOR3{ Human::HUMAN_SPEED, 0.f, 0.f },
        D3DXVECTOR3{ 64.f, 568.f, 0.f }
    );
    human->AddComponent("Behaviour", bh1);

    human2 = new Human("texture/GameParts.tga", 256, 256);
    Behavior2* bh2 = new Behavior2(
        D3DXVECTOR3{ 0.f, -Human::HUMAN_SPEED, 0.f },
        D3DXVECTOR3{ 32.f, 536.f, 0.f }
    );
    human2->AddComponent("Behaviour", bh2);

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
    //clickMsg = new Plane("texture/sign.tga", 128, 512);

    stage = 1;
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
    human->SetActive(true);
    Behavior1* bh1 = (Behavior1*)human->GetComponent("Behaviour");
    bh1->SetVelocity(D3DXVECTOR3{ Human::HUMAN_SPEED, 0.f, 0.f });
    
    Behavior2* bh2 = nullptr;

    Sprite* sprite = nullptr;
    switch (stage)
    {
    case 1:
        house->local_position_ = D3DXVECTOR3{ HOUSE_POS_X, HOUSE_POS_Y, 0.f };

        human2->SetActive(false);
        sprite = ((UIRenderer*)stageName->GetComponent("UIRenderer"))
            ->GetSprite();
        sprite->SetAlpha(1.f);
        sprite->SetUV(0.f / 128.f, 224.f / 512.f, 128.f / 128.f, 64.f / 512.f);

        break;
    case 2:
        house->local_position_ = D3DXVECTOR3{ HOUSE_POS_X, 64.f, 0.f };
        
        human2->SetActive(true);
        bh2 = (Behavior2*)human2->GetComponent("Behaviour");
        bh2->SetVelocity(D3DXVECTOR3{ 0.f, -Human::HUMAN_SPEED, 0.f });
        
        sprite = ((UIRenderer*)stageName->GetComponent("UIRenderer"))
                ->GetSprite();
        sprite->SetAlpha(1.f);
        sprite->SetUV(0.f / 128.f, 288.f / 512.f, 128.f / 128.f, 64.f / 512.f);

        break;
    default:
        break;
    }
    house->local_scale_ = D3DXVECTOR3{ 0.5f, 0.5f, 1.f };
    
    field->Initialize();

    stageName->SetActive(true);
    stageName->state = new Plane::FadeOut;
    waitTime = 0.5f;

    GameObject::InitObjectAll();
    Timer::Initialize();

    phase = 1;
    isClear = false;
}

void Game::Update()
{
    Timer::Update();
    IsClear();

    switch (phase)
    {
    case 1:
        DispStageNum();
        break;
    case 2:
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
    SAFE_DELETE(human);
    SAFE_DELETE(house);
    SAFE_DELETE(capsule);
    SAFE_DELETE(field);
}

void Game::GameMain()
{
    if (!human) return;
    // ウイルスの増殖など
    // 人が出現したウイルスでいきなり消されないように発生範囲外を設定する
    BoxCollider* hCol = (BoxCollider*)human->GetComponent("BoxCollider");
    BoxCollider col;
    D3DXVECTOR3 pos = human->NextPosition();
    col.center_.x = pos.x;
    col.center_.y = pos.y;
    col.box_width_ = hCol->box_width_;
    col.box_width_.x += 10.f;
    col.box_width_.y += 10.f;
    col.box_width_.y += 10.f;

    BoxCollider* hCol2 = nullptr;
    if (stage == 2) {
        hCol2 = (BoxCollider*)human2->GetComponent("BoxCollider");
        BoxCollider col2;
        D3DXVECTOR3 pos2 = human2->NextPosition();
        col2.center_.x = pos2.x;
        col2.center_.y = pos2.y;
        col2.box_width_ = hCol2->box_width_;
        col2.box_width_.x += 10.f;
        col2.box_width_.y += 10.f;
        col2.box_width_.y += 10.f;

        field->SetOutOfMultiply(col, col2);
    }
    else {
        field->SetOutOfMultiply(col, col);
    }
    field->Update();

    GameObject::UpdateObjectAll();

    // 衝突検知
    std::vector<Virus*>::iterator itr;
    BoxCollider* vCol;
    BoxCollider* cCol = (BoxCollider*)capsule->GetComponent("BoxCollider");

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
            SetClear(false);
            SetPhase(3);
            break;
        }

        if (stage == 2 &&
            hCol2->Check(*vCol)
        ) {
            human2->SetActive(false);
            SetClear(false);
            SetPhase(3);
            break;
        }

        if (cCol->Check(*vCol)) {
            (*itr)->SetActive(false);
            (*itr)->TimerReset();
        }
        ++itr;
    }
}

void Game::SetClear(bool clear)
{
    isClear = clear;
    resultMsg->SetActive(true);
    if (clear) {
        ((UIRenderer*)resultMsg->GetComponent("UIRenderer"))
            ->GetSprite()
            ->SetUV(0.f / 128.f, 64.f / 512.f, 128.f / 128.f, 64.f / 512.f);
    }
    else {
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
    if (human->local_position_.x >= house->local_position_.x &&
        human->local_position_.y <= house->local_position_.y + 32.f)
    {
        if (stage == 2 &&
            human2->local_position_.x >= house->local_position_.x &&
            human2->local_position_.y <= house->local_position_.y + 32.f
        ) {
            SetClear(true);
            SetPhase(3);
        }
        else {
            SetClear(true);
            SetPhase(3);
        }
    }
}

void Game::GameResult()
{
    // クリックすると次のステージへ
    using Hirai::Input;
    DIMOUSESTATE2& dims = Input::GetMouseInput();
    if (dims.rgbButtons[0] && 0x80)
    {
        resultMsg->SetActive(false);
        if (stage == 1) {
            field->DeleteVirus();
            if (isClear) {
                SetStageNum(2);
            }
            SetPhase(1);
            Initialize();
        }
        else if (stage == 2) {
            if (isClear) {
                // ステージ2ならタイトルに戻る
                Scene::SetScene(new Title);
            }
            else {
                SetPhase(1);
                Initialize();
            }
            field->DeleteVirus();
            
        }
    }
}

void Game::SetStageNum(int n)
{
    stage = n;
}

void Game::DispStageNum()
{
    waitTime -= Timer::DeltaTime();
    if (waitTime <= 0.f) {
        if (stageName->UpdateFade()) {
            SetPhase(2);
            stageName->SetActive(false);
        }
    }
}

void Game::PrepareStage(int stageNum)
{
    if (stage == 1) {
        field->DeleteVirus();
        SetStageNum(2);
        SetPhase(1);
        Initialize();
    }
    else if (stage == 2) {
        // ステージ2ならタイトルに戻る
        Scene::SetScene(new Title);
    }
}
