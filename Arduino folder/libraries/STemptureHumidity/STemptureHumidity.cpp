#include <STemptureHumidity.h>

#define MIN_INTERVAL 2000

boolean STemptureHumidity::read(boolean force) {
	// Check if sensor was read less than two seconds ago and return early
	// to use last reading.
	uint32_t currenttime = millis();
	if (!force && ((currenttime - _lastreadtime) < 2000)) {
		return _lastresult; // return last correct measurement
	}
	_lastreadtime = currenttime;

	// Reset 40 bits of received data to zero.
	dat[0] = dat[1] = dat[2] = dat[3] = dat[4] = 0;

	// Send start signal.  See DHT datasheet for full signal diagram:
	//   http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf

	// Go into high impedence state to let pull-up raise data line level and
	// start the reading process.
	digitalWrite(pin, HIGH);
	delay(250);

	// First set data line low for 20 milliseconds.
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(20);

	uint32_t cycles[80];
	{
		// Turn off interrupts temporarily because the next sections are timing critical
		// and we don't want any interruptions.
		InterruptLock lock;

		// End the start signal by setting data line high for 40 microseconds.
		digitalWrite(pin, HIGH);
		delayMicroseconds(40);

		// Now start reading the data line to get the value from the DHT sensor.
		pinMode(pin, INPUT_PULLUP);
		delayMicroseconds(10);  // Delay a bit to let sensor pull data line low.

								// First expect a low signal for ~80 microseconds followed by a high signal
								// for ~80 microseconds again.
		if (expectPulse(LOW) == 0) {
			DEBUG_PRINTLN(F("Timeout waiting for start signal low pulse."));
			_lastresult = false;
			return _lastresult;
		}
		if (expectPulse(HIGH) == 0) {
			DEBUG_PRINTLN(F("Timeout waiting for start signal high pulse."));
			_lastresult = false;
			return _lastresult;
		}

		// Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
		// microsecond low pulse followed by a variable length high pulse.  If the
		// high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
		// then it's a 1.  We measure the cycle count of the initial 50us low pulse
		// and use that to compare to the cycle count of the high pulse to determine
		// if the bit is a 0 (high state cycle count < low state cycle count), or a
		// 1 (high state cycle count > low state cycle count). Note that for speed all
		// the pulses are read into a array and then examined in a later step.
		for (int i = 0; i<80; i += 2) {
			cycles[i] = expectPulse(LOW);
			cycles[i + 1] = expectPulse(HIGH);
		}
	} // Timing critical code is now complete.

	  // Inspect pulses and determine which ones are 0 (high state cycle count < low
	  // state cycle count), or 1 (high state cycle count > low state cycle count).
	for (int i = 0; i<40; ++i) {
		uint32_t lowCycles = cycles[2 * i];
		uint32_t highCycles = cycles[2 * i + 1];
		if ((lowCycles == 0) || (highCycles == 0)) {
			DEBUG_PRINTLN(F("Timeout waiting for pulse."));
			_lastresult = false;
			return _lastresult;
		}
		dat[i / 8] <<= 1;
		// Now compare the low and high cycle times to see if the bit is a 0 or 1.
		if (highCycles > lowCycles) {
			// High cycles are greater than 50us low cycle count, must be a 1.
			dat[i / 8] |= 1;
		}
		// Else high cycles are less than (or equal to, a weird case) the 50us low
		// cycle count so this must be a zero.  Nothing needs to be changed in the
		// stored data.
	}

	DEBUG_PRINTLN(F("Received:"));
	DEBUG_PRINT(dat[0], HEX); DEBUG_PRINT(F(", "));
	DEBUG_PRINT(dat[1], HEX); DEBUG_PRINT(F(", "));
	DEBUG_PRINT(dat[2], HEX); DEBUG_PRINT(F(", "));
	DEBUG_PRINT(dat[3], HEX); DEBUG_PRINT(F(", "));
	DEBUG_PRINT(dat[4], HEX); DEBUG_PRINT(F(" =? "));
	DEBUG_PRINTLN((dat[0] + dat[1] + dat[2] + dat[3]) & 0xFF, HEX);

	// Check we read 40 bits and that the checksum matches.
	if (dat[4] == ((dat[0] + dat[1] + dat[2] + dat[3]) & 0xFF)) {
		_lastresult = true;
		return _lastresult;
	}
	else {
		DEBUG_PRINTLN(F("Checksum failure!"));
		_lastresult = false;
		return _lastresult;
	}
}

// Expect the signal line to be at the specified level for a period of time and
// return a count of loop cycles spent at that level (this cycle count can be
// used to compare the relative time of two pulses).  If more than a millisecond
// ellapses without the level changing then the call fails with a 0 response.
// This is adapted from Arduino's pulseInLong function (which is only available
// in the very latest IDE versions):
//   https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/wiring_pulse.c
uint32_t STemptureHumidity::expectPulse(boolean level) {
	uint32_t count = 0;
	// On AVR platforms use direct GPIO port access as it's much faster and better
	// for catching pulses that are 10's of microseconds in length:
#ifdef __AVR
	uint8_t portState = level ? _bit : 0;
	while ((*portInputRegister(_port) & _bit) == portState) {
		if (count++ >= _maxcycles) {
			return 0; // Exceeded timeout, fail.
		}
	}
	// Otherwise fall back to using digitalRead (this seems to be necessary on ESP8266
	// right now, perhaps bugs in direct port access functions?).
#else
	while (digitalRead(pin) == level) {
		if (count++ >= _maxcycles) {
			return 0; // Exceeded timeout, fail.
		}
	}
#endif

	return count;
}

STemptureHumidity::STemptureHumidity(int id, int pin) : Sensor(id){
	this->pin = pin;
#ifdef __AVR
	_bit = digitalPinToBitMask(pin);
	_port = digitalPinToPort(pin);
#endif
	_maxcycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for
												   // reading pulses from DHT sensor.
												   // Note that count is now ignored as the DHT reading algorithm adjusts itself

												   // set up the pins!
	pinMode(pin, INPUT_PULLUP);
	// Using this value makes sure that millis() - lastreadtime will be
	// >= MIN_INTERVAL right away. Note that this assignment wraps around,
	// but so will the subtraction.
	_lastreadtime = -MIN_INTERVAL;
	DEBUG_PRINT("Max clock cycles: "); DEBUG_PRINTLN(_maxcycles, DEC);
}												   // basd on the speed of the processor.


Message STemptureHumidity::readSensorData(boolean isHumidity) {
	float result = NAN;
	Message message;
	if (isHumidity){
		message.sensorType = 'H';
		if (read()) {
			result = dat[0];
			Serial.print("humidity:");
			Serial.println(result, DEC);
		}
	}
	else {
		message.sensorType = 'T';
		if (read()) {
			result = dat[2];
			Serial.print("temperture:");
			Serial.println(result, DEC);
		}
	}
	message.data = result;		//enter the data
	
	return message;
}







