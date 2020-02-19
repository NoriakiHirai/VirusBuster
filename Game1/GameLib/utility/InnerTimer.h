#pragma once
#include <Windows.h>

class InnerTimer
{
private:
    int startTime;
    int elapsedTime;

public:
    void TimerStart();
    const int ElapsedTime();
    void TimerReset();
};

