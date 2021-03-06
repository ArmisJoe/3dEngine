#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();

	void Pause();
	void UnPause();

	Uint32 Read();
	Uint32 ReadSc();

	void SetTimeMultiplier(float tm);

private:
	bool	running;
	bool	paused;
	Uint32	started_at;
	Uint32	stopped_at;
	Uint32	paused_at;
	Uint32	unpaused_at;
	Uint32	pausetime = 0;

	float time_multiplier = 1.0f;

};

#endif //__TIMER_H__