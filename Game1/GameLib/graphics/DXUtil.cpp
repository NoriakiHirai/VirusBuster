#include "DXUtil.h"
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

namespace Hirai {
    VOID InitMaterial(D3DMATERIAL9& material, FLOAT r, FLOAT g, FLOAT b, FLOAT a)
    {
        ZeroMemory(&material, sizeof(D3DMATERIAL9));
        material.Diffuse.r = material.Ambient.r = r;
        material.Diffuse.g = material.Ambient.g = g;
        material.Diffuse.b = material.Ambient.b = b;
        material.Diffuse.a = material.Ambient.a = a;
    }
}
