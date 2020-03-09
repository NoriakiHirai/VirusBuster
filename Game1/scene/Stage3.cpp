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
#include "../object/Behaviour.h"
#include "Stage4.h"

// for debug
#include "Title.h"

static const float HUMAN_CREATE_INTERVAL_MAX = 5.f;
const float Stage3::HOUSE_POS_X = 736.f;
const float Stage3::HOUSE_POS_Y = 64.f;

Stage3::Stage3()
{
    Human* human;
    // 10人分作成
    for (auto i = 0; i < 2; ++i) {
        human = new Human("texture/GameParts.tga", 256, 256);
        Behaviour1* bh1 = new Behaviour1(
            D3DXVECTOR3{ Human::HUMAN_SPEED, 0.f, 0.f },
            D3DXVECTOR3{ 64.f, 568.f, 0.f }
        );
        human->AddComponent("Behaviour", bh1);
        human->SetActive(false);
        people.push_back(human);

        human = new Human("texture/GameParts.tga", 256, 256);
        Behaviour2* bh2 = new Behaviour2(
            D3DXVECTOR3{ 0.f, -Human::HUMAN_SPEED, 0.f },
            D3DXVECTOR3{ 32.f, 536.f, 0.f }
        );
        human->AddComponent("Behaviour", bh2);
        human->SetActive(false);
        people.push_back(human);
    }

    startArea = new BoxCollider();
    startArea->center_ = Vector2{ 50.f, 550.f };
    startArea->box_width_ = Vector2{ 50.f, 50.f };

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
    house->local_position_ = D3DXVECTOR3{ HOUSE_POS_X, HOUSE_POS_Y, 0.f };
    house->local_scale_ = D3DXVECTOR3{ 0.5f, 0.5f, 1.f };

    // ステージプレートの設定
    Sprite* sprite = ((UIRenderer*)stageName->GetComponent("UIRenderer"))
        ->GetSprite();
    sprite->SetAlpha(1.f);
    sprite->SetUV(0.f / 128.f, 349.f / 1024.f, 128.f / 128.f, 62.f / 1024.f);

    field->Initialize();

    stageName->SetActive(true);
    stageName->state = new Plane::FadeOut;
    waitTime = 0.5f;
    humanCreateInterval = HUMAN_CREATE_INTERVAL_MAX;

    GameObject::InitObjectAll();

    // 先頭の2人以外はBehaviourの更新を止めておく
    std::vector<Human*>::iterator itr = people.begin();
    Behaviour1* bh1;
    Behaviour2* bh2;
    field->SetInviolableArea((*itr)->GetInviolableArea());
    bh1 = dynamic_cast<Behaviour1*>((*itr)->GetComponent("Behaviour"));
    bh1->SetVelocity({ Human::HUMAN_SPEED, 0.f, 0.f });
    ++itr;
    field->SetInviolableArea((*itr)->GetInviolableArea());
    bh2 = dynamic_cast<Behaviour2*>((*itr)->GetComponent("Behaviour"));
    bh2->SetVelocity({ 0.f, -Human::HUMAN_SPEED, 0.f });
    ++itr;
    for (; itr != people.end(); ++itr) {
        bh1 = dynamic_cast<Behaviour1*>((*itr)->GetComponent("Behaviour"));
        bh1->SetVelocity({ Human::HUMAN_SPEED, 0.f, 0.f });
        bh1->Stop();
        ++itr;
        bh2 = dynamic_cast<Behaviour2*>((*itr)->GetComponent("Behaviour"));
        bh2->SetVelocity({ 0.f, -Human::HUMAN_SPEED, 0.f });
        bh2->Stop();
    }

    Timer::Initialize();

    phase = 1;
    isClear = false;
}

void Stage3::Update()
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

void Stage3::Draw()
{
    GameObject::DrawObjectAll();
}

void Stage3::Finalize()
{
}

void Stage3::GameMain()
{
    // 一定時間が経過したら新しい人を増やす
    humanCreateInterval -= Timer::DeltaTime();
    if (humanCreateInterval <= 0.f) {
        for (auto human : people) {
            if (!human->IsActive()) {
                human->SetActive(true);
                humanCreateInterval = HUMAN_CREATE_INTERVAL_MAX;
                break;
            }
        }
    }

    // ウイルスの非発生領域を更新する
    Field::InitAllInviolableArea();
    field->SetInviolableArea(*startArea);
    for (auto human : people) {
        if (human->IsActive()) {
            field->SetInviolableArea(human->GetInviolableArea());
        }
    }

    // ウイルスの増殖など
    // 人が出現したウイルスでいきなり消されないように発生範囲外を設定する
    BoxCollider col;
    //for (auto human : people) {
    //    if (human->IsActive()) {
    //        field->SetInviolableArea(human->GetInviolableArea());
    //    }
    //}
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

void Stage3::SetResultMsg(bool isWin)
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

void Stage3::SetPhase(int ph)
{
    phase = ph;
}

void Stage3::IsClear()
{
    bool allFinished = true;
    for (auto human : people) {
        if (!GoalCheck(human)) {
            allFinished = false;
        }
    }
    if (allFinished) {
        //SetResultMsg(true);
        //SetPhase(3);
        std::vector<Human*>::iterator itr = people.begin();
        Behaviour1* bh1;
        Behaviour2* bh2;
        (*itr)->Initialize();
        field->SetInviolableArea((*itr)->GetInviolableArea());
        bh1 = dynamic_cast<Behaviour1*>((*itr)->GetComponent("Behaviour"));
        bh1->SetVelocity({ Human::HUMAN_SPEED, 0.f, 0.f });
        ++itr;
        (*itr)->Initialize();
        field->SetInviolableArea((*itr)->GetInviolableArea());
        bh2 = dynamic_cast<Behaviour2*>((*itr)->GetComponent("Behaviour"));
        bh2->SetVelocity({ 0.f, -Human::HUMAN_SPEED, 0.f });
        //for (auto human : people) {
        //    human->Initialize();
        //}
    }
}

bool Stage3::GoalCheck(Human* human)
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

void Stage3::GameResult()
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
            Scene::SetScene(new Title);
            //Scene::SetScene(new Stage4);
        }
        else {
            SetPhase(1);
            Initialize();
            field->Finalize();
        }
    }
}

void Stage3::DispStageNum()
{
    waitTime -= Timer::DeltaTime();
    if (waitTime <= 0.f) {
        if (stageName->UpdateFade()) {
            SetPhase(2);
            stageName->SetActive(false);
        }
    }
}

bool Stage3::CollisionCheckWithHuman(const BoxCollider& virusCol)
{
    BoxCollider* hCol;
    for (auto human : people) {
        if (!human->IsActive()) continue;
         hCol = (BoxCollider*)human->GetComponent("BoxCollider");
         if (hCol->Check(virusCol)) {
             //human->SetActive(false);
             return true;
         }
    }
    return false;
}
