#include "Title.h"
#include <graphics/Plane.h>
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include <common/utility.h>
#include <hid/Input.h>
#include "../Game1.h"
#include "Game.h"

Title::Title()
{
    title = new Plane("texture/title.tga", 256, 256);
    ((UIRenderer*)title->GetComponent("UIRenderer"))
        ->GetSprite()
        ->SetUV(0.f / 256.f, 0.f / 256.f, 256.f / 256.f, 40.f / 256.f);
}

Title::~Title()
{
    //SAFE_DELETE(title);
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
}

void Title::Update()
{
    using Hirai::Input;
    DIMOUSESTATE2& dims = Input::GetMouseInput();
    if (dims.rgbButtons[0] && 0x80)
    {
        Scene::SetScene(new Game);
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
