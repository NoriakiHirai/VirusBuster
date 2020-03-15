#include "Stage5.h"
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
#include "Clear.h"

static const float HUMAN_CREATE_INTERVAL_MAX = 5.f;
const float Stage5::HOUSE_POS_X = 736.f;
const float Stage5::HOUSE_POS_Y = 64.f;

Stage5::Stage5()
{
    human = new Human("texture/GameParts.tga", 256, 256);
    Behaviour3* bh3 = new Behaviour3(
        D3DXVECTOR3{ Human::HUMAN_SPEED, 0.f, 0.f },
        D3DXVECTOR3{ 64.f, 568.f, 0.f },
        D3DXVECTOR3{ HOUSE_POS_X, HOUSE_POS_Y + 32.f, 0.f } // ゴールの下端とプレイヤーの下端を合わせる
    );
    float humanSpeed = 0.25f;
    bh3->SetSpeed(humanSpeed);
    human->AddComponent("Behaviour", bh3);
    
    human2 = new Human("texture/GameParts.tga", 256, 256);
    Behaviour4* bh4 = new Behaviour4(
        D3DXVECTOR3{ Human::HUMAN_SPEED, 0.f, 0.f },
        D3DXVECTOR3{ 32.f, 536.f, 0.f },
        D3DXVECTOR3{ HOUSE_POS_X, HOUSE_POS_Y + 32.f, 0.f } // ゴールの下端とプレイヤーの下端を合わせる
    );
    bh4->SetSpeed(humanSpeed);
    human2->AddComponent("Behaviour", bh4);

    startArea = new BoxCollider();
    startArea->center_ = Vector2{ 50.f, 550.f };
    startArea->box_width_ = Vector2{ 50.f, 50.f };

    house = new Plane("texture/GameParts.tga", 256, 256);
    ((UIRenderer*)house->GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 0.f / 256.f, 128.f / 256.f, 128.f / 256.f);
    house->SetLayer(Layer::kStage);

    field = new Field("data/VirusConfig.csv");
    field->SetVirusRivivalInterval(15000);

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

Stage5::~Stage5()
{
    human->Destroy(human);
    human2->Destroy(human2);
    house->Destroy(house);
    capsule->Destroy(capsule);
    resultMsg->Destroy(resultMsg);
    stageName->Destroy(stageName);
    SAFE_DELETE(field);
}

void Stage5::Initialize()
{
    house->local_position_ = D3DXVECTOR3{ HOUSE_POS_X, HOUSE_POS_Y, 0.f };
    house->local_scale_ = D3DXVECTOR3{ 0.5f, 0.5f, 1.f };

    // ステージプレートの設定
    Sprite* sprite = ((UIRenderer*)stageName->GetComponent("UIRenderer"))
        ->GetSprite();
    sprite->SetAlpha(1.f);
    sprite->SetUV(0.f / 128.f, 473.f / 1024.f, 128.f / 128.f, 62.f / 1024.f);

    field->Initialize();

    stageName->SetActive(true);
    stageName->state = new Plane::FadeOut;
    waitTime = 0.5f;
    humanCreateInterval = HUMAN_CREATE_INTERVAL_MAX;

    GameObject::InitObjectAll();

    Timer::Initialize();

    phase = 1;
    isClear = false;
}

void Stage5::Update()
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

void Stage5::Draw()
{
    GameObject::DrawObjectAll();
}

void Stage5::Finalize()
{
}

void Stage5::GameMain()
{
    // 一定時間が経過したら新しい人を増やす
    humanCreateInterval -= Timer::DeltaTime();

    // ウイルスの非発生領域を更新する
    Field::InitAllInviolableArea();
    field->SetInviolableArea(*startArea);
    field->SetInviolableArea(human->GetInviolableArea());
    field->SetInviolableArea(human2->GetInviolableArea());

    // ウイルスの増殖など
    // 人が出現したウイルスでいきなり消されないように発生範囲外を設定する
    field->Update();

    GameObject::UpdateObjectAll();

    // 衝突検知
    std::vector<Virus*>::iterator itr;
    BoxCollider* vCol;
    BoxCollider* cCol = (BoxCollider*)capsule->GetComponent("BoxCollider");
    BoxCollider* hCol1 = (BoxCollider*)human->GetComponent("BoxCollider");
    BoxCollider* hCol2 = (BoxCollider*)human2->GetComponent("BoxCollider");
    
    for (itr = field->viruses.begin();
        itr != field->viruses.end();
        )
    {
        if (!(*itr)->IsActive()) {
            ++itr;
            continue;
        }

        vCol = (BoxCollider*)(*itr)->GetComponent("BoxCollider");
        if (CollisionCheckWithHuman(*vCol, *hCol1)) {
            SetResultMsg(false);
            SetPhase(3);
            break;
        }
        if (CollisionCheckWithHuman(*vCol, *hCol2)) {
            SetResultMsg(false);
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

void Stage5::SetResultMsg(bool isWin)
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

void Stage5::SetPhase(int ph)
{
    phase = ph;
}

void Stage5::IsClear()
{
    if (GoalCheck(human) && GoalCheck(human2)) {
        SetResultMsg(true);
        SetPhase(3);
    }
}

bool Stage5::GoalCheck(Human* human)
{
    HumanBehaviour* hb;
    if (human->local_position_.x >= house->local_position_.x - 6.f &&
        human->local_position_.y <= house->local_position_.y + 34.f)
    {
        hb = dynamic_cast<HumanBehaviour*>(human->GetComponent("Behaviour"));
        hb->SetVelocity({ 0.f, 0.f, 0.f });
        return true;
    }
    else {
        return false;
    }
}

void Stage5::GameResult()
{
    // クリックすると次のステージへ
    using Hirai::Input;
    if (Input::GetMouseLeftButtonTrigger())
    {
        resultMsg->SetActive(false);
        if (isClear) {
            // ウイルスの削除は、SetSceneからのFinalize()呼び出しの中で実行される
            Scene::SetScene(new Clear);
        }
        else {
            SetPhase(1);
            Initialize();
            field->Finalize();
        }
    }
}

void Stage5::DispStageNum()
{
    waitTime -= Timer::DeltaTime();
    if (waitTime <= 0.f) {
        if (stageName->UpdateFade()) {
            SetPhase(2);
            stageName->SetActive(false);
        }
    }
}

bool Stage5::CollisionCheckWithHuman(const BoxCollider& virusCol, BoxCollider& humCol)
{
    if (humCol.Check(virusCol)) {
        humCol.gameObject().SetActive(false);
        return true;
    }
    return false;
}
