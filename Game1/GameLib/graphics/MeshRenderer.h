#pragma once
#include "../common/Component.h"

class MeshObject;

class MeshRenderer : public Component
{
public:
    MeshRenderer() {}
    ~MeshRenderer() {}

    static void Prepare();
    void Draw(const MeshObject&);
};