#pragma once
#include <memory>
#include "../common/Component.h"

class Camera : public Component
{
public:
    virtual void SetTarget(float x, float y, float z) = 0;
    virtual void SetViewMatrix() = 0;
    virtual void SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar) = 0;
    virtual D3DXMATRIXA16* ViewMatrix() = 0;
    virtual D3DXMATRIXA16* ProjectionMatrix() = 0;

    // �t�@�N�g��
    // �錾�N���X�Ƃ̈ˑ����������邽��
    // �h���N���X�̃t�@�C���Œ�`���邱��
    static std::shared_ptr<Camera> Create();
};