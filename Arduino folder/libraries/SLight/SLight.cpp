#include <SLight.h>

SLight::SLight(int id, int pin) : Sensor(id){
	this->pin = pin;
	pinMode(pin, INPUT);
	sensorValue = 0;
}

Message SLight	::readSensorData(boolean isHumidity){
	Message message;					//create new message
	Serial.println("readSensorData called");
	sensorValue = analogRead(pin);
	sensorValue = 100 - (int)(sensorValue / 1023.0 * 100.0); //calculate light in percentage.
	Serial.print("light:");
	Serial.println(sensorValue, DEC);
	message.data= sensorValue;		//enter the data
	message.sensorType = 'L';
	Serial.print("copied from sensor to messege: check: ");
	Serial.println(message.data,DEC);	 
	return message;
}








