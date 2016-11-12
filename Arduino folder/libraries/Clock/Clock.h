#ifndef CLOCK
#define CLOCK

#include <Arduino.h>
#include <Wire.h>
#include <DateTime.h>

#define DS3231_I2C_ADDRESS 104
/*
this is the class that deels with the clock module DS1307
connections:
SCL - pin A5
SDA - pin A4
To set the clock, run the sketch and use the serial monitor.
Enter T0000174140516; the code will read this and set the clock. See the code for full details.
*/
class Clock {
//private:
public:

	byte seconds;
	byte minutes;
	byte hours;
	byte day;
	byte date;
	byte month;
	byte year;
	char weekDay[4];
	byte tMSB;
	byte tLSB;
	float temp3231;

public:
	Clock();
	byte decToBcd(byte val);
	void watchConsole();
	void set3231Date();
	void get3231Date();
	float get3231Temp();
	void createDateTime(DateTime&);
};

#endif
