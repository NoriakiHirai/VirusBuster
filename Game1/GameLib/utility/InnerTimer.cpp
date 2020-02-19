#include "InnerTimer.h"

void InnerTimer::TimerStart() {
    startTime = timeGetTime();
}

void InnerTimer::TimerReset() {
    startTime = timeGetTime();
}

const int InnerTimer::ElapsedTime() {
    return (timeGetTime() - startTime);
}