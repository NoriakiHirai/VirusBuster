#pragma once
#include <d3dx9.h>
#include "Effect.h"

class Light;

class DepthShadow : public Effect
{
private:
    static const int SHADOW_MAP_SIZE;

    LPD3DXEFFECT effect;

    // �V�F�[�_���ϐ��ւ̃n���h��
    D3DXHANDLE technique;
    D3DXHANDLE wvpMatrix;   // ���[���hx�r���[x�ˉe�ϊ��s��
    D3DXHANDLE wlpMatrix;   // �J�������猩���ˉe�ϊ��s��
    D3DXHANDLE wlpbMatrix;  // �J�������猩���e�N�X�`�����W�ւ̎ˉe�ϊ��s��
    D3DXHANDLE meshColor;
    D3DXHANDLE lightDir;
    D3DXHANDLE shadowMap;   // �V���h�E�}�b�v�e�N�X�`��

    LPDIRECT3DVERTEXDECLARATION9 vertexDecl;

    // �V���h�E�}�b�v
    LPDIRECT3DTEXTURE9 shadowTex;
    LPDIRECT3DSURFACE9 shadowSurf;
    LPDIRECT3DSURFACE9 zBuffer;

    // ���C�g���猩���ˉe�ϊ��s��
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

