#include "Clear.h"
#include <graphics/Plane.h>
#include <graphics/UIRenderer.h>
#include <graphics/Sprite.h>
#include <common/utility.h>
#include <hid/Input.h>
#include "../Game1.h"
#include "Title.h"
#include "CommonData.h"

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
}

void Clear::Initialize()
{
    clear->local_position_ = D3DXVECTOR3{
    DISPLAY_WIDTH_HALF,
    DISPLAY_HEIGHT * 4 / 10,
    0.f
    };
    clear->local_scale_ = D3DXVECTOR3{ 2.f, 1.f, 1.f };
}

void Clear::Update()
{
    using Hirai::Input;
    DIMOUSESTATE2& dims = Input::GetMouseInput();
    if (dims.rgbButtons[0] && 0x80)
    {
        Scene::SetScene(new Title);
        CommonData::SetCurrentStage(1);
    }
}

void Clear::Draw()
{
    GameObject::DrawObjectAll();
}

void Clear::Finalize()
{
}
