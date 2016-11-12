#ifndef S_SOIL
#define S_SOIL
#include <Arduino.h>
#include <Sensor.h>
#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>


class SSoil : public Sensor {
public:
	SSoil(int, int);
	Message readSensorData(boolean isHumidity = false);
private:
	int pin;
	int sensorValue;
};


#endif