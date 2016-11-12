#ifndef WATCHDOG
#define WATCHDOG

#include <Arduino.h>
#include <TimerOne.h>
#include <CommonValues.h>


class WatchDog {
	
	public:
		void enable(int seconds);
		void reset();
		static unsigned long lastUpdate;
		static unsigned long timeout;
};

#endif