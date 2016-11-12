#ifndef S_LIGHT_H
#define S_LIGHT_H
#include <Arduino.h>
#include <Sensor.h>
#include <RF24.h>
#include <SPI.h>

class SLight : public Sensor {
	public:
		SLight(int, int);
		Message readSensorData(boolean isHumidity = false);
	private:
		int pin;
		int sensorValue;
};


#endif