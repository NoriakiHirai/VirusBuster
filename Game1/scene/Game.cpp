#include "Game.h"
#include <common/utility.h>
#include <common/Timer.h>
#include <graphics/Plane.h>
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include "../object/Human.h"
#include "../field/Field.h"

Game::Game()
{
    human = new Human("texture/GameParts.tga", 256, 256);

    house = new Plane("texture/GameParts.tga", 256, 256);
    ((UIRenderer*)house->GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 0.f / 256.f, 128.f / 256.f, 128.f / 256.f);

    field = new Field("data/VirusConfig.csv");

    capsule = new Plane("texture/GameParts.tga", 256, 256);
    ((UIRenderer*)capsule->GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(128.f / 256.f, 0.f / 256.f, 64.f / 256.f, 64.f / 256.f);
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
    // for debug

    capsule->local_position_ = D3DXVECTOR3{ 400.f, 400.f, 0.f };
    capsule->local_scale_ = D3DXVECTOR3{ 0.2f, 0.2f, 1.f };

    GameObject::InitObjectAll();
    Timer::Initialize();
}

void Game::Update()
{
    Timer::Update();

    // ƒEƒCƒ‹ƒX‚Ì‘B‚È‚Ç
    field->Update();
    GameObject::UpdateObjectAll();
}

void Game::Draw()
{
    GameObject::DrawObjectAll();
}

void Game::Finalize()
{
}
