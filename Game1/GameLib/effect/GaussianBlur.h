#pragma once
#include <effect/Effect.h>
#include <d3dx9.h>

class GaussianBlur5x5Kernel :
    public Effect
{
public:
    LPD3DXEFFECT effect;
    LPDIRECT3DTEXTURE9 pass1Texture;
    LPDIRECT3DTEXTURE9 pass2Texture;
    LPDIRECT3DTEXTURE9 pass3Texture;
    LPDIRECT3DSURFACE9 textureBuffer1;
    LPDIRECT3DSURFACE9 textureBuffer2;
    LPDIRECT3DSURFACE9 backBuffer;
    LPDIRECT3DSURFACE9 zBuffer;

public:
    GaussianBlur5x5Kernel(const char* fileName);
    ~GaussianBlur5x5Kernel();

    bool Initialize() override;
    bool Draw(const GameObject& obj) override;
    void Finalize() override;
};

