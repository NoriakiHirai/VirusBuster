#pragma once
#include <d3dx9.h>
#include "Effect.h"

class Light;

class DepthShadow : public Effect
{
private:
    static const int SHADOW_MAP_SIZE;

    LPD3DXEFFECT effect;

    // シェーダ内変数へのハンドラ
    D3DXHANDLE technique;
    D3DXHANDLE wvpMatrix;   // ワールドxビューx射影変換行列
    D3DXHANDLE wlpMatrix;   // カメラから見た射影変換行列
    D3DXHANDLE wlpbMatrix;  // カメラから見たテクスチャ座標への射影変換行列
    D3DXHANDLE meshColor;
    D3DXHANDLE lightDir;
    D3DXHANDLE shadowMap;   // シャドウマップテクスチャ

    LPDIRECT3DVERTEXDECLARATION9 vertexDecl;

    // シャドウマップ
    LPDIRECT3DTEXTURE9 shadowTex;
    LPDIRECT3DSURFACE9 shadowSurf;
    LPDIRECT3DSURFACE9 zBuffer;

    // ライトから見た射影変換行列
    D3DXMATRIX* lightVPMatrix;

public:
    DepthShadow(const char* effectFile);
    ~DepthShadow();

    bool Initialize() override;
    bool Draw(const GameObject& obj) override;
    void Finalize() override;

    void SetLightVPMatrix(D3DXMATRIX* mat) {
        lightVPMatrix = mat;
    }
};

