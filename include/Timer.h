#ifndef TIMER_H
#define TIMER_H
#include <SDL2/SDL.h>

class Timer
{
    private:
	    Uint32 startTicks;
		Uint32 pausedTicks;

		bool paused;
		bool started;

		Uint32 getTicks();

    public:
		Timer();

		Uint32 time;

		void start();
		void stop();
		void pause();
		void unpause();
		bool countdown(const Uint32 &timer);

		bool isStarted();
		bool isPaused();
};

#endif