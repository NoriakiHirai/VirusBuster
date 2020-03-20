#include "Clear.h"
#include <graphics/Plane.h>
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include <common/utility.h>
#include <hid/Input.h>
#include <common/Singleton.h>
#include <audio/dx/Audio.h>
#include "../Game1.h"
#include "Title.h"
#include "CommonData.h"
#include "../object/Human.h"
#include "../object/Behaviour.h"

static DirectAudio& dAudio = singleton<DirectAudio>::GetInstance();

Clear::Clear()
{
    clear = new Plane("texture/title.tga", 256, 256);
    ((UIRenderer*)clear->GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 40.f / 256.f, 256.f / 256.f, 100.f / 256.f);
}

Clear::~Clear()
{
    clear->Destroy(clear);
    human->Destroy(human);
    human2->Destroy(human2);
}

void Clear::Initialize()
{
    clear->local_position_ = D3DXVECTOR3{
    DISPLAY_WIDTH_HALF,
    DISPLAY_HEIGHT * 4 / 10,
    0.f
    };
    clear->local_scale_ = D3DXVECTOR3{ 2.f, 1.f, 1.f };

    Behaviour5* bh5;
    human = new Human("texture/GameParts.tga", 256, 256);
    bh5 = new Behaviour5(
        D3DXVECTOR3{ 0.f, -5.f, 0.f },
        D3DXVECTOR3{ 300.f, 568.f, 0.f }
    );
    human->AddComponent("Behaviour", bh5);

    human2 = new Human("texture/GameParts.tga", 256, 256);
    bh5 = new Behaviour5(
        D3DXVECTOR3{ 0.f, -5.f, 0.f },
        D3DXVECTOR3{ 500.f, 568.f, 0.f }
    );
    human2->AddComponent("Behaviour", bh5);
   
    GameObject::InitObjectAll();
    dAudio.PlayOneShot("GameClear");
}

void Clear::Update()
{
    using Hirai::Input;
    if (Input::GetMouseLeftButtonTrigger())
    {
        dAudio.Stop("GameClear");
        dAudio.PlayOneShot("Decision");
        Scene::SetScene(new Title);
        CommonData::SetCurrentStage(1);
    }
    GameObject::UpdateObjectAll();
}

void Clear::Draw()
{
    GameObject::DrawObjectAll();
}

void Clear::Finalize()
{
}
