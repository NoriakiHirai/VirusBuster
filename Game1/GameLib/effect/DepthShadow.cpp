#include "DepthShadow.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <atlstr.h>
#include <graphics/DirectX.h>
#include <graphics/DXUtil.h>
#include <common/utility.h>
#include <common/GameObject.h>

const int DepthShadow::SHADOW_MAP_SIZE = 256;

DepthShadow::DepthShadow(const char* effectFile)
{
    using Hirai::DirectX;
    LPDIRECT3DDEVICE9 device = DirectX::GetD3DDevice();
    LPD3DXBUFFER err;
    if (FAILED(D3DXCreateEffectFromFile(
        device, CString(effectFile), nullptr, nullptr,
        D3DXSHADER_DEBUG, nullptr, &effect, &err
    ))) {
        MessageBox(
            nullptr, (LPCWSTR)err->GetBufferPointer(),
            TEXT("ERROR"), MB_OK
        );
        return;
    }

    technique = effect->GetTechniqueByName("DepthShadowShader");
    wvpMatrix = effect->GetParameterByName(nullptr, "mWVP");
    wlpMatrix = effect->GetParameterByName(nullptr, "mWLP");
    wlpbMatrix = effect->GetParameterByName(nullptr, "mWLPB");
    meshColor = effect->GetParameterByName(nullptr, "vCol");
    lightDir = effect->GetParameterByName(nullptr, "vLightDir");
    shadowMap = effect->GetParameterByName(nullptr, "ShadowMap");

    // ���_�錾�I�u�W�F�N�g�쐬
    D3DVERTEXELEMENT9 elem[] =
    {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
        {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0}, // uv���W
    };
    if (FAILED(device->CreateVertexDeclaration(elem, &vertexDecl)))
    {
        MessageBox(
            nullptr, CString("���_�錾�̐ݒ�Ɏ��s���܂����B"),
            TEXT("ERROR"), MB_OK
        );
    }
}

DepthShadow::~DepthShadow()
{
    SAFE_RELEASE(effect);
    SAFE_RELEASE(vertexDecl);
}

bool DepthShadow::Initialize()
{
    using Hirai::DirectX;
    LPDIRECT3DDEVICE9 device = DirectX::GetD3DDevice();
    // �V���h�E�}�b�v�̍쐬
    if (FAILED(device->CreateTexture(
        SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1,
        D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8,
        D3DPOOL_DEFAULT, &shadowTex, nullptr
    ))) {
        return false;
    }
    if (FAILED(shadowTex->GetSurfaceLevel(0, &shadowSurf)))
        return false;
    if (FAILED(device->CreateDepthStencilSurface(
        SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, D3DFMT_D16,
        D3DMULTISAMPLE_NONE, 0, TRUE, &zBuffer, nullptr
    ))) {
        return false;
    }

    // �}�e���A���̐ݒ�
    D3DMATERIAL9 material;
    Hirai::InitMaterial(material, 1.f, 0.f, 0.f);
    device->SetMaterial(&material);

// �Z�k�`�̒�`
#define RS device->SetRenderState
#define TSS device->SetTextureStageState
#define SS device->SetSamplerState

    // �����_�����O�X�e�[�g�̐ݒ�
    RS(D3DRS_DITHERENABLE, FALSE);
    RS(D3DRS_SPECULARENABLE, FALSE);
    RS(D3DRS_ZENABLE, TRUE);
    RS(D3DRS_AMBIENT, 0x000F0F0F);  // ����

    TSS(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    TSS(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    TSS(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    TSS(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    TSS(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    TSS(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    SS(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    SS(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    
    effect->OnResetDevice();

    return true;
}

bool DepthShadow::Draw(const GameObject& obj)
{
    D3DXMATRIX view, projection;
    LPDIRECT3DSURFACE9 orgBackBuffer, orgZBuffer;
    D3DVIEWPORT9 orgViewPort;

    // �����_�����O�^�[�Q�b�g�̑ޔ�

    return true;
}

void DepthShadow::Finalize()
{
    SAFE_RELEASE(zBuffer);
    SAFE_RELEASE(shadowSurf);
    SAFE_RELEASE(shadowTex);

    if (effect) effect->OnLostDevice();
}
