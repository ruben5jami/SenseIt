#ifndef S_WATER_H
#define S_WATER_H
#include <Arduino.h>
#include <Sensor.h>
#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>
#include<CommonValues.h>

class SWater : public Sensor {
public:
	SWater(int, int);
	Message readSensorData(boolean isHumidity = false);
	//void pulseCounter();

private:
	int pin;
	// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
	// litre/minute of flow.
	float calibrationFactor = 4.5;
	float flowRate;
	unsigned int flowMilliLitres;
	unsigned long totalMilliLitres;
	unsigned long oldTime;
};


#endif