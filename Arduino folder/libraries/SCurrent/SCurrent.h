#ifndef S_CURRENT
#define S_CURRENT
#include <Arduino.h>
#include <Sensor.h>
#include <RF24.h>
#include <SPI.h>
#include <CommonValues.h>

class SCurrent: public Sensor {
public:
	SCurrent(int, int);
	Message readSensorData(boolean isHumidity = false);
private:
	int pin;
	int sensorValue;
};


#endif