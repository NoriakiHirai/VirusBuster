#include "Title.h"
#include <graphics/Plane.h>
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include <common/utility.h>
#include <hid/Input.h>
#include <common/Singleton.h>
#include <audio/dx/Audio.h>
#include "../Game1.h"
#include "Game.h"
#include "CommonData.h"

static DirectAudio& dAudio = singleton<DirectAudio>::GetInstance();

Title::Title()
{
    title = new Plane("texture/title.tga", 256, 256);
    ((UIRenderer*)title->GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 0.f / 256.f, 256.f / 256.f, 40.f / 256.f);
}

Title::~Title()
{
    title->Destroy(title);
}

void Title::Initialize()
{
    title->local_position_ = D3DXVECTOR3{
        DISPLAY_WIDTH_HALF,
        DISPLAY_HEIGHT * 4 / 10,
        0.f
    };
    title->local_scale_ = D3DXVECTOR3{ 2.f, 1.f, 1.f };
    
    dAudio.Stop("Decision");
    dAudio.PlayLoop("BGM");
}

void Title::Update()
{
    using Hirai::Input;
    if (Input::GetMouseLeftButtonTrigger())
    {
        dAudio.Stop("BGM");
        dAudio.PlayOneShot("Decision");
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
}
