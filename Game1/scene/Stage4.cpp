#include "Stage4.h"
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
#include "Stage5.h"

// for debug
#include "Title.h"

static const float HUMAN_CREATE_INTERVAL_MAX = 5.f;
const float Stage4::HOUSE_POS_X = 736.f;
const float Stage4::HOUSE_POS_Y = 64.f;

Stage4::Stage4()
{
    human = new Human("texture/GameParts.tga", 256, 256);
    Behaviour3* bh = new Behaviour3(
        D3DXVECTOR3{ Human::HUMAN_SPEED, 0.f, 0.f },
        D3DXVECTOR3{ 50.f, 550.f, 0.f },
        D3DXVECTOR3{ HOUSE_POS_X, HOUSE_POS_Y + 32.f, 0.f } // ゴールの下端とプレイヤーの下端を合わせる
    );
    bh->SetSpeed(5.25f);
    human->AddComponent("Behaviour", bh);

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

Stage4::~Stage4()
{
    human->Destroy(human);
    house->Destroy(house);
    capsule->Destroy(capsule);
    resultMsg->Destroy(resultMsg);
    stageName->Destroy(stageName);
    SAFE_DELETE(field);
}

void Stage4::Initialize()
{
    house->local_position_ = D3DXVECTOR3{ HOUSE_POS_X, HOUSE_POS_Y, 0.f };
    house->local_scale_ = D3DXVECTOR3{ 0.5f, 0.5f, 1.f };

    // ステージプレートの設定
    Sprite* sprite = ((UIRenderer*)stageName->GetComponent("UIRenderer"))
        ->GetSprite();
    sprite->SetAlpha(1.f);
    sprite->SetUV(0.f / 128.f, 411.f / 1024.f, 128.f / 128.f, 62.f / 1024.f);

    field->Initialize();

    stageName->SetActive(true);
    stageName->state = new Plane::FadeOut;
    waitTime = 0.5f;
    humanCreateInterval = HUMAN_CREATE_INTERVAL_MAX;

    GameObject::InitObjectAll();

    HumanBehaviour* bh1 = dynamic_cast<HumanBehaviour*>(human->GetComponent("Behaviour"));
    bh1->SetVelocity({ Human::HUMAN_SPEED, 0.f, 0.f });
    
    Timer::Initialize();

    phase = 1;
    isClear = false;
}

void Stage4::Update()
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

void Stage4::Draw()
{
    GameObject::DrawObjectAll();
}

void Stage4::Finalize()
{
}

void Stage4::GameMain()
{
    // 一定時間が経過したら新しい人を増やす
    humanCreateInterval -= Timer::DeltaTime();

    // ウイルスの非発生領域を更新する
    Field::InitAllInviolableArea();
    field->SetInviolableArea(*startArea);
    field->SetInviolableArea(human->GetInviolableArea());

    // ウイルスの増殖など
    // 人が出現したウイルスでいきなり消されないように発生範囲外を設定する
    BoxCollider col;
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
        if (CollisionCheckWithHuman(*vCol)) {
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

void Stage4::SetResultMsg(bool isWin)
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

void Stage4::SetPhase(int ph)
{
    phase = ph;
}

void Stage4::IsClear()
{
    if (GoalCheck(human)) {
        SetResultMsg(true);
        SetPhase(3);
    }
}

bool Stage4::GoalCheck(Human* human)
{
    HumanBehaviour* hb;
    if (human->local_position_.x >= house->local_position_.x &&
        human->local_position_.y <= house->local_position_.y + 32.f)
    {
        hb = dynamic_cast<HumanBehaviour*>(human->GetComponent("Behaviour"));
        hb->SetVelocity({ 0.f, 0.f, 0.f });
        return true;
    }
    else {
        return false;
    }
}

void Stage4::GameResult()
{
    // クリックすると次のステージへ
    using Hirai::Input;
    DIMOUSESTATE2& dims = Input::GetMouseInput();
    if (dims.rgbButtons[0] && 0x80)
    {
        resultMsg->SetActive(false);
        if (isClear) {
            // ウイルスの削除は、SetSceneからのFinalize()呼び出しの中で実行される
            //Scene::SetScene(new Title);
            Scene::SetScene(new Stage5);
        }
        else {
            SetPhase(1);
            Initialize();
            field->Finalize();
        }
    }
}

void Stage4::DispStageNum()
{
    waitTime -= Timer::DeltaTime();
    if (waitTime <= 0.f) {
        if (stageName->UpdateFade()) {
            SetPhase(2);
            stageName->SetActive(false);
        }
    }
}

bool Stage4::CollisionCheckWithHuman(const BoxCollider& virusCol)
{
    BoxCollider* hCol;
    hCol = (BoxCollider*)human->GetComponent("BoxCollider");
    if (hCol->Check(virusCol)) {
        //human->SetActive(false);
        //return true;
    }
    return false;
}
