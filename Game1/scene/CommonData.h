#pragma once

class CommonData {
private:
    static int currentStage;

public:
    static void SetCurrentStage(int);
    static const int GetCurrentStage();
};