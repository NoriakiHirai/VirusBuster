#include "Timer.h"
#include <Windows.h>

unsigned int	Timer::prev_time_ = 0;
unsigned int	Timer::start_time_ = 0;
float			Timer::delta_time_ = 0.f;

void Timer::Initialize()
{
	start_time_ = timeGetTime();	// Œ»İ‚Ì‚ğƒ~ƒŠ•b‚Å•Ô‹p‚·‚é
	prev_time_ = start_time_;
	delta_time_ = 1 / 60.f;
}

void Timer::Update()
{
	unsigned int now = timeGetTime();
	delta_time_ = (now - prev_time_) / 1000.f;
	prev_time_ = now;
}