#ifndef S_TEMPERTURE_HUMIDITY_H
#define S_TEMPERTURE_HUMIDITY_H
#include <Arduino.h>
#include <Sensor.h>
#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>


#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Uncomment to enable printing out nice debug messages.
//#define DHT_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef DHT_DEBUG
#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...) {}
#define DEBUG_PRINTLN(...) {}
#endif
class STemptureHumidity : public Sensor {
public:
	STemptureHumidity(int, int);
	Message readSensorData(boolean);
	boolean read(boolean force = false);
	uint32_t expectPulse(boolean);
private:
	byte dat[5];
	float data;
	uint8_t pin;
#ifdef __AVR
	// Use direct GPIO access on an 8-bit AVR so keep track of the port and bitmask
	// for the digital pin connected to the DHT.  Other platforms will use digitalRead.
	uint8_t _bit, _port;
#endif
	uint32_t _lastreadtime, _maxcycles;
	boolean _lastresult;
};


class InterruptLock {
public:
	InterruptLock() {
		noInterrupts();
	}
	~InterruptLock() {
		interrupts();
	}

};

#endif