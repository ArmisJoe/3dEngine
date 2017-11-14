// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
{
	Start();
	Stop();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	paused = false;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Pause() {
	if (paused == false) {
		paused = true;
		paused_at = SDL_GetTicks();
	}
}
// ---------------------------------------------
void Timer::UnPause() {
	if (paused == true) {
		paused = false;
		unpaused_at = SDL_GetTicks();
		pausetime += (unpaused_at - paused_at);
	}
}


// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		if (paused == true)
			return (paused_at * time_multiplier - started_at);
		else
			return (SDL_GetTicks() * time_multiplier - started_at) - pausetime;
	}
	else
	{
		return stopped_at * time_multiplier - started_at;
	}
}
Uint32 Timer::ReadSc()
{
	if (running == true)
	{
		if (paused == true)
			return ((paused_at * time_multiplier - started_at) / 1000);
		else
			return (((SDL_GetTicks() * time_multiplier - started_at) - pausetime) / 1000);
	}
	else
	{
		return ((stopped_at * time_multiplier - started_at) / 1000);
	}
}

void Timer::SetTimeMultiplier(float tm) {
	if (tm < 1)
		tm = 1;
	time_multiplier = tm;
}


