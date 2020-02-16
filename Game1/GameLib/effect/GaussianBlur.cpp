#include "GaussianBlur.h"
#include <atlstr.h>
#include <graphics/DirectX.h>

GaussianBlur5x5Kernel::GaussianBlur5x5Kernel(const char* fileName)
{
    using Hirai::DirectX;
    LPDIRECT3DDEVICE9 device = DirectX::GetD3DDevice();
    LPD3DXBUFFER err;
    if (FAILED(D3DXCreateEffectFromFile(
        device, CString(fileName), nullptr, nullptr,
        D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION,
        nullptr, &effect, &err
    ))) {
        MessageBox(
            nullptr, (LPCWSTR)err->GetBufferPointer(),
            TEXT("ERROR"), MB_OK
        );
        return;
    }

    // ぼかすモデル用のレンダーターゲットになるテクスチャの作成
    //if (FAILED())
}

GaussianBlur5x5Kernel::~GaussianBlur5x5Kernel()
{
}

bool GaussianBlur5x5Kernel::Initialize()
{
    return false;
}

bool GaussianBlur5x5Kernel::Draw(const GameObject& obj)
{
    return false;
}

void GaussianBlur5x5Kernel::Finalize()
{
}
