#include <SSoil.h>

SSoil::SSoil(int id, int pin) : Sensor(id){
	this->pin = pin;
	sensorValue = 0;
}

Message SSoil::readSensorData(boolean isHumidity){
	Message message;					//create new message
	Serial.println("readSensorData called");
	sensorValue = analogRead(pin	);
	Serial.print("soil humidity:");
	Serial.println(sensorValue);
	message.data= sensorValue;		//enter the data
	message.sensorType = 'S';
	Serial.print("copied from sensor to messege: check: ");
	Serial.println(message.data);	
	return message;
}










