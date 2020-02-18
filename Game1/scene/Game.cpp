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

Game::Game()
{
    human = new Human("texture/GameParts.tga", 256, 256);

    house = new Plane("texture/GameParts.tga", 256, 256);
    ((UIRenderer*)house->GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 0.f / 256.f, 128.f / 256.f, 128.f / 256.f);

    field = new Field("data/VirusConfig.csv");

    capsule = new Capsule("texture/GameParts.tga", 256, 256);
}

Game::~Game()
{
    SAFE_DELETE(human);
    SAFE_DELETE(house);
    //SAFE_DELETE(virus);
    SAFE_DELETE(capsule);
    SAFE_DELETE(field);
}

void Game::Initialize()
{
    house->local_position_ = D3DXVECTOR3{ 736.f, 536.f, 0.f };
    house->local_scale_ = D3DXVECTOR3{ 0.5f, 0.5f, 1.f };
    
    field->Initialize();

    GameObject::InitObjectAll();
    Timer::Initialize();
}

void Game::Update()
{
    Timer::Update();

    // ウイルスの増殖など
    field->Update();
    GameObject::UpdateObjectAll();

    // for debug
    if (!human) return;

    // 衝突検知
    std::vector<Virus*>::iterator itr;
    BoxCollider* vCol;
    BoxCollider* hCol = (BoxCollider*)human->GetComponent("BoxCollider");
    BoxCollider* cCol = (BoxCollider*)capsule->GetComponent("BoxCollider");
    BoxCollider* gCol = (BoxCollider*)house->GetComponent("BoxCollider");

    for (itr = field->viruses.begin();
        itr != field->viruses.end();
        )
    {
        vCol = (BoxCollider*)(*itr)->GetComponent("BoxCollider");
        if (hCol->Check(*vCol)) {
            human->Destroy(human);
            // TODO:ミス時の処理に遷移
            break;
        }

        if (cCol->Check(*vCol)) {
            (*itr)->Destroy(*itr);
            itr = field->viruses.erase(itr);
        }
        else {
            ++itr;
        }
    }
}

void Game::Draw()
{
    GameObject::DrawObjectAll();
}

void Game::Finalize()
{
}
