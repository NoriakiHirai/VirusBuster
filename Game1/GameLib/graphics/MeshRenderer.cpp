#include "MeshRenderer.h"
#include <common/GameObject.h>
#include <graphics/DirectX.h>
#include <graphics/MeshObject.h>
#include <dynamics/Collider.h>

void MeshRenderer::Prepare()
{
    // ���b�V�������_�����O�p�̐ݒ���s��
    using Hirai::DirectX;
    LPDIRECT3DDEVICE9 device = DirectX::GetD3DDevice();
    // �J�����O
    device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    // �O�[���[�V�F�[�f�B���O
    device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    // �\���b�h�i�ʏ�̃|���S�����[�h�j
    device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    // z�o�b�t�@�L��
    device->SetRenderState(D3DRS_ZENABLE, true);
    // ���C�g��L����
    device->SetRenderState(D3DRS_LIGHTING, true);
    // �����i�A���r�G���g���C�g�j��ݒ肷��
    device->SetRenderState(D3DRS_AMBIENT, 0x00111111);
    // �X�y�L�����i���ʔ��ˁj��L���ɂ���
    device->SetRenderState(D3DRS_SPECULARENABLE, true);
    // �A���t�@�u�����f�B���O
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
    device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
}

void MeshRenderer::Draw(const MeshObject& obj)
{
    using Hirai::DirectX;
    D3DXMATRIX world;
    D3DXMatrixTranslation(&world,
        gameObject().local_position_.x,
        gameObject().local_position_.y,
        gameObject().local_position_.z
    );
    LPDIRECT3DDEVICE9 device = DirectX::GetD3DDevice();
    device->SetTransform(D3DTS_WORLD, &world);
    for (DWORD i = 0; i < obj.numOfMaterials; ++i) {
        device->SetMaterial(&obj.meshMaterials[i]);
        device->SetTexture(0, obj.meshTextures[i]);
        obj.mesh->DrawSubset(i);
    }

    Component* c = obj.GetComponent("SphereCollider");
    if (c) {
        SphereCollider* sc = dynamic_cast<SphereCollider*>(c);
        sc->DrawRange();
    }
}
