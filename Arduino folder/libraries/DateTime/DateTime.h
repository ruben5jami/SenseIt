#ifndef DATETIME
#define DATETIME
#include <Arduino.h>

//struct that represents the time and date
struct DateTime
{
	byte seconds;
	byte minutes;
	byte hours;
	byte date;
	byte month;
	byte year;
};

#endif