#include "Title.h"
#include <graphics/Plane.h>
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include <common/utility.h>
#include <hid/Input.h>
#include "../Game1.h"
#include "Game.h"
#include "CommonData.h"
//#include "../object/Human.h"
//#include "../object/Behaviour.h"

Title::Title()
{
    title = new Plane("texture/title.tga", 256, 256);
    ((UIRenderer*)title->GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 0.f / 256.f, 256.f / 256.f, 40.f / 256.f);

    //human = new Human("texture/GameParts.tga", 256, 256);
    //Behaviour1* bh1 = new Behaviour1(
    //    D3DXVECTOR3{ Human::HUMAN_SPEED, 0.f, 0.f },
    //    D3DXVECTOR3{ 64.f, 568.f, 0.f }
    //);
    //human->AddComponent("Behaviour", bh1);

    //house = new Plane("texture/GameParts.tga", 256, 256);
    //((UIRenderer*)house->GetComponent("UIRenderer"))
    //    ->GetSprite()
    //    ->SetUV(0.f / 256.f, 0.f / 256.f, 128.f / 256.f, 128.f / 256.f);
    //house->SetLayer(Layer::kStage);
    //house->local_scale_ = D3DXVECTOR3{ 0.5f, 0.5f, 1.f };
}

Title::~Title()
{
    //SAFE_DELETE(title);
    title->Destroy(title);
    //human->Destroy(human);
    //house->Destroy(house);
}

void Title::Initialize()
{
    title->local_position_ = D3DXVECTOR3{
        DISPLAY_WIDTH_HALF,
        DISPLAY_HEIGHT * 4 / 10,
        0.f
    };
    title->local_scale_ = D3DXVECTOR3{ 2.f, 1.f, 1.f };
}

void Title::Update()
{
    using Hirai::Input;
    if (Input::GetMouseLeftButtonTrigger())
    {
        Scene::SetScene(new Game);
        CommonData::SetCurrentStage(1);
    }
}

void Title::Draw()
{
    GameObject::DrawObjectAll();
}

void Title::Finalize()
{
    //GameObject::DeleteObjectAll();
}
