#pragma once
#include <common/Component.h>
#include <d3dx9.h>
#include <graphics\DirectX.h>

class Light :
    public Component
{
private:
    D3DLIGHTTYPE type;
    D3DXVECTOR3 position;
    D3DXVECTOR3 lookAt;
    D3DXVECTOR3 up;

    D3DXVECTOR3 direction;
    Hirai::RGB diffuse;
    Hirai::RGB specular;
    float range;

public:
    Light();
    virtual ~Light() {}

    void SetLookAt(float x, float y, float z) {
        lookAt.x = x;
        lookAt.y = y;
        lookAt.z = z;
    }

    D3DXVECTOR3 LookAt() const { return lookAt; }
    D3DXVECTOR3 Up() const { return up; }
};

