#ifndef GAME_LIB_GAME_LIB_TIMER_H_
#define GAME_LIB_GAME_LIB_TIMER_H_

class Timer
{
public:
	Timer() {}
	~Timer() {}

	static void Initialize();
	static void Update();

	static float DeltaTime() { return delta_time_; }

private:
	static unsigned int prev_time_;
	static unsigned int start_time_;
	static float delta_time_;	// ’PˆÊ‚Í•b

};

#endif // !GAME_LIB_GAME_LIB_TIMER_H_
