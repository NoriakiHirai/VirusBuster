#pragma once
#include <scene/Scene.h>

class Plane;

class Title : public Scene
{
private:
    Plane* title;

public:
    Title();
    ~Title();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
};

