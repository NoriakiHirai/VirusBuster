#include "Stage2.h"
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
#include "../object/Behaviour.h"
#include "Stage3.h"
#include "CommonData.h"

const float Stage2::HOUSE_POS_X = 736.f;
const float Stage2::HOUSE_POS_Y = 64.f;

Stage2::Stage2()
{
    human = new Human("texture/GameParts.tga", 256, 256);
    Behaviour1* bh1 = new Behaviour1(
        D3DXVECTOR3{ Human::HUMAN_SPEED, 0.f, 0.f },
        D3DXVECTOR3{ 64.f, 568.f, 0.f }
    );
    human->AddComponent("Behaviour", bh1);

    human2 = new Human("texture/GameParts.tga", 256, 256);
    Behaviour2* bh2 = new Behaviour2(
        D3DXVECTOR3{ 0.f, -Human::HUMAN_SPEED, 0.f },
        D3DXVECTOR3{ 32.f, 536.f, 0.f }
    );
    human2->AddComponent("Behaviour", bh2);

    house = new Plane("texture/GameParts.tga", 256, 256);
    ((UIRenderer*)house->GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 0.f / 256.f, 128.f / 256.f, 128.f / 256.f);
    house->SetLayer(Layer::kStage);
    house->local_scale_ = D3DXVECTOR3{ 0.5f, 0.5f, 1.f };

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

Stage2::~Stage2()
{
    human->Destroy(human);
    human2->Destroy(human2);
    house->Destroy(house);
    capsule->Destroy(capsule);
    resultMsg->Destroy(resultMsg);
    stageName->Destroy(stageName);
    SAFE_DELETE(field);
}

void Stage2::Initialize()
{
    house->local_position_ = D3DXVECTOR3{ HOUSE_POS_X, HOUSE_POS_Y, 0.f };
    house->local_scale_ = D3DXVECTOR3{ 0.5f, 0.5f, 1.f };

    HumanBehaviour* hb = dynamic_cast<HumanBehaviour*>(human->GetComponent("Behaviour"));
    hb->SetVelocity({ Human::HUMAN_SPEED, 0.f, 0.f });
    hb = dynamic_cast<HumanBehaviour*>(human2->GetComponent("Behaviour"));
    hb->SetVelocity({ 0.f, -Human::HUMAN_SPEED, 0.f });

    // ステージプレートの設定
    Sprite* sprite = ((UIRenderer*)stageName->GetComponent("UIRenderer"))
        ->GetSprite();
    sprite->SetAlpha(1.f);
    sprite->SetUV(0.f / 128.f, 288.f / 1024.f, 128.f / 128.f, 64.f / 1024.f);

    field->Initialize();

    stageName->SetActive(true);
    stageName->state = new Plane::FadeOut;
    waitTime = 0.5f;

    GameObject::InitObjectAll();
    Timer::Initialize();

    phase = 1;
    isClear = false;
}

void Stage2::Update()
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

void Stage2::Draw()
{
    GameObject::DrawObjectAll();
}

void Stage2::Finalize()
{
}

void Stage2::GameMain()
{
    if (!human && !human2) return;
    // ウイルスの増殖など
    // 人が出現したウイルスでいきなり消されないように発生範囲外を設定する
    BoxCollider* hCol = (BoxCollider*)human->GetComponent("BoxCollider");
    BoxCollider col = human->GetInviolableArea();

    BoxCollider* hCol2 = (BoxCollider*)human2->GetComponent("BoxCollider");
    BoxCollider col2 = human2->GetInviolableArea();

    // todo:要修正
    //field->SetInviolableArea(col, col2);
    field->Update();

    GameObject::UpdateObjectAll();

    // 衝突検知
    std::vector<Virus*>::iterator itr;
    BoxCollider* vCol;
    BoxCollider* cCol = (BoxCollider*)capsule->GetComponent("BoxCollider");

    //for (itr = field->viruses.begin();
    //    itr != field->viruses.end();
    //    )
    //{
    //    if (!(*itr)->IsActive()) {
    //        ++itr;
    //        continue;
    //    }

    //    vCol = (BoxCollider*)(*itr)->GetComponent("BoxCollider");
    //    if (hCol->Check(*vCol)) {
    //        human->SetActive(false);
    //        SetResultMsg(false);
    //        SetPhase(3);
    //        break;
    //    }

    //    if (hCol2->Check(*vCol)) {
    //        human2->SetActive(false);
    //        SetResultMsg(false);
    //        SetPhase(3);
    //        break;
    //    }

    //    if (cCol->Check(*vCol)) {
    //        (*itr)->SetActive(false);
    //        (*itr)->TimerReset();
    //    }
    //    ++itr;
    //}
}

void Stage2::SetResultMsg(bool isWin)
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

void Stage2::SetPhase(int ph)
{
    phase = ph;
}

void Stage2::IsClear()
{
    if (human->local_position_.x >= house->local_position_.x &&
        human->local_position_.y <= house->local_position_.y + 32.f &&
        human2->local_position_.x >= house->local_position_.x &&
        human2->local_position_.y <= house->local_position_.y + 32.f
    )
    {
        SetResultMsg(true);
        SetPhase(3);
    }
}

void Stage2::GameResult()
{
    // クリックすると次のステージへ
    using Hirai::Input;
    DIMOUSESTATE2& dims = Input::GetMouseInput();
    if (dims.rgbButtons[0] && 0x80)
    {
        resultMsg->SetActive(false);
        if (isClear) {
            // ステージ2ならタイトルに戻る
            // ウイルスの削除は、SetSceneからのFinalize()呼び出しの中で実行される
            //Scene::SetScene(new Title);
            Scene::SetScene(new Stage3);
            CommonData::SetCurrentStage(3);
        }
        else {
            SetPhase(1);
            Initialize();
            field->Finalize();
        }
    }
}

void Stage2::DispStageNum()
{
    waitTime -= Timer::DeltaTime();
    if (waitTime <= 0.f) {
        if (stageName->UpdateFade()) {
            SetPhase(2);
            stageName->SetActive(false);
        }
    }
}
