#include <SCurrent.h>

SCurrent::SCurrent(int id, int pin) : Sensor(id){
	this->pin = pin;
	sensorValue = 0;
}

Message SCurrent::readSensorData(boolean isHumidity){
	//create new message
	Message message;
	message.sensorType = CommonValues::currentType;
	Serial.println(F("readSensorData called"));
	/*
	The value that links the two measurements is 
	sensitivity which – for 5A model – has a typical value of 185mV/A.
	The sensor can measure positive and negative currents (range -5A…5A),
	so if input current is 0, output value is 2.5V.We’re going to read output
	value with one of the analog inputs of Arduino and its analogRead()
	function. That function outputs a value between 0 (0V in input)
	and 1023 (5V in input) that is 0, 0049V for each increment.
	*/
		float average = 0;
		for (int i = 0; i < 1000; i++) {
			//formula needed to convert units from analogRead() to Ampere is:
			average = average + (.0264 * analogRead(pin) - 13.51) / 1000;
			delay(1);
		}
		
		if (average < 0)	//if its negative electric
			average *= -1.0;	//show it as positive
		average -= 0.17;
		Serial.print(F("analog read:"));
		Serial.println(analogRead(A5));
		Serial.print(F("current amper:"));
		Serial.println(average);
		//formula needed to convert amper to kilo watt.
		float kiloWatt = average * 220 / 1000;
		Serial.print(F("current KW:"));
		Serial.println(kiloWatt);
		message.data = kiloWatt;
		
  return message;
}







