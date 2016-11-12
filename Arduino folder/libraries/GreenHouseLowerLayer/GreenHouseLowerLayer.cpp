#include <GreenHouseLowerLayer.h>

boolean GreenHouseLowerLayer::sendMessage(Message& message) {
	Serial.println("GreenHouseLowerLayer::sendMessage");
	return (communicationList.get(CommonValues::radioIndex)->sendMessage(message));
}

void GreenHouseLowerLayer::receiveMessages(LinkedList<Message>& messages) {
	communicationList.get(CommonValues::radioIndex)->receiveMessages(messages);
}

void GreenHouseLowerLayer::initLayer(int address) {
	this->address = address;
	previousMillis = 0;
	ICommunicationable* radio = new Radio(CommonValues::radioPin1, CommonValues::radioPin2);
	radio->initCommunication(this->address, CommonValues::middleLayerAddress);
	communicationList.add(radio);
	setLoopTime(CommonValues::defaultLoopTime);
	//more inits here , think maybe to move the inits to relevant constractors
}
	
void GreenHouseLowerLayer::analyze(){
	Serial.println("analyze");
	LinkedList<Message> sensorsData;
	readSensorsData(sensorsData);
	if (this->address == CommonValues::lowerLayerConsumptionAdress) { //am i the consumtion layer?
		currentMillis = millis();
		/*
		this for loop sums the data from the water and current consumption
		and saves the sum in a private member.
		*/
		for (int i = 0; i < sensorsData.size(); i++) {
			switch (sensorsData.get(i).sensorType) {
			case CommonValues::currentType:
				currentConsumptionData += sensorsData.get(i).data;
				break;
			case CommonValues::waterType:
				if (sensorsData.get(i).additionalData == CommonValues::waterConsumptionPin1) {
					waterConsumptionData1 += sensorsData.get(i).data;
				}
				else if (sensorsData.get(i).additionalData == CommonValues::waterConsumptionPin2) {
					waterConsumptionData2 += sensorsData.get(i).data;
				}
				break;
			}
		}//end of for() loop

		//check if need to send to higher
		if ((unsigned long)(currentMillis - previousMillis) >= CommonValues::hour) {
			Message currentMessage;
			prepareDataMessage(currentMessage, currentConsumptionData, CommonValues::currentType);
			if (!(sendMessage(currentMessage))) {
				//TODO if no sent
			}
			Message waterMessage1;
			//TODO ROTEM why to add CommonValues::waterType ? we do it at the Sensor class
			prepareDataMessage(waterMessage1, waterConsumptionData1, CommonValues::waterType);
			if (!(sendMessage(waterMessage1))) {
				//TODO if no sent
			}
			Message waterMessage2;
			//TODO ROTEM why to add CommonValues::waterType ? we do it at the Sensor class
			prepareDataMessage(waterMessage2, waterConsumptionData2, CommonValues::waterType);

			if (!(sendMessage(waterMessage2))) {
				//TODO if no sent
			}
			previousMillis = currentMillis;
		}
	}//end off consumtion layer
	else {	//i am not the Consumption layer, im a regular plant lower layer
		for (int i = 0; i < sensorsData.size(); ++i) {
			Serial.print("sensor type:");
			Serial.println(sensorsData.get(i).sensorType);
			switch (sensorsData.get(i).sensorType) {
				case CommonValues::temperatureType:
					//this is emergency state - send message to middle layer immidietly
					if (sensorsData.get(i).data >= CommonValues::EMERGENCY_TEMPERATURE) {
						Message newMessage;
						prepareDataMessage(newMessage, sensorsData.get(i).data, CommonValues::emergencyType);
						if (!(sendMessage(newMessage))) {
						}
					}
					//add data to data array for average
					temperatureData.add(sensorsData.get(i).data);
					if (temperatureData.size() == CommonValues::producersSize) {
						float average = doAverage(temperatureData);
						Message newMessage;
						prepareDataMessage(newMessage, average, CommonValues::temperatureType);
						temperatureData.clear();
						if (!(sendMessage(newMessage))) {
						}
					}
				break;
				case CommonValues::humidityType:
					//add data to data array for average
					airHumidityData.add(sensorsData.get(i).data);
					if (airHumidityData.size() == CommonValues::producersSize) {
						float average = doAverage(airHumidityData);
						Message newMessage;
						prepareDataMessage(newMessage, average, CommonValues::humidityType);
						if (!(sendMessage(newMessage))) {
						}
						//clear array after doing average
						airHumidityData.clear();
					}
					break;
				case CommonValues::soilHumidityType:
					//add data to data array for average
					soilHumidityData.add(sensorsData.get(i).data);
					//check if we can do avarage
					if (soilHumidityData.size() == CommonValues::producersSize) {
						float average = doAverage(soilHumidityData);
						Message newMessage;
						//check if need to actuate TODO move to function
						if (sensorsData.get(i).data <= CommonValues::soilHumidityThresholdMin) {
							for (int i = 0; i < getActuatorsList().size(); ++i) {
								if (getActuatorsList().get(i)->getPin() == CommonValues::pumpPin) {
									getActuatorsList().get(i)->actuate(true);
									// put in message which pump was activated. 
									//since both lower layers use the same code.
									if (this->address == CommonValues::lowerLayerAddress1) {
										newMessage.action = PUMP1;
									}
									else if (this->address == CommonValues::lowerLayerAddress2) {
										newMessage.action = PUMP2;
									}
									break;
								}//end of if(CommonValues::pumpPin)
							}//end of for loop
						}//end of soilHumidityThresholdMin check
						else 
							newMessage.action = NONE;
						prepareDataMessage(newMessage, average, CommonValues::soilHumidityType);

						if (!(sendMessage(newMessage))) {
						}
						//clear array after doing average
						soilHumidityData.clear();
					}
					break;
				case CommonValues::lightType:
					//add data to data array for average
					lightData.add(sensorsData.get(i).data);
					if (lightData.size() == CommonValues::producersSize) {
						float average = doAverage(lightData);
						Message newMessage;
						prepareDataMessage(newMessage, average, CommonValues::lightType);
						if (!(sendMessage(newMessage))) {
						}
						//clear array after doing average
						lightData.clear();
					}
					break;
				default: 
					Serial.println("default");
				break;
			}
		}//and of for loop
		
	}//end of regular plant lower layer treatment
}
//TODO rotem what is this? dont know if necessary..
void GreenHouseLowerLayer::prepareDataMessage(Message& message, float data, char type) {
	message.data = data;
	if (type == CommonValues::emergencyType) {
		message.messageType = CommonValues::emergencyType;		
	}
	else {
		message.messageType = CommonValues::dataType;
		message.sensorType = type;	
	}
	prepareMessage(message, CommonValues::middleLayerAddress);
}

float GreenHouseLowerLayer::doAverage(LinkedList<float>& data) {
	float average;
	for (int i = 0 ; i<data.size();i++) {
		average+=data.get(i);
	}
	average = average/data.size();
	return average;
}	

void GreenHouseLowerLayer::decodeMessage(Message& message) {
	if (CommonValues::emptyMessage == message.sensorType) {
		//do nothing the message is empty
		return;
	}
	if (this->address != message.dest) {
		//not for me, resend the message to it's original destination
		if (!(sendMessage(message))) {}
		return;
	}
	//means we got new policy from upper layer
	if (message.messageType == CommonValues::policyChange) {
		if (message.sensorType == CommonValues::soilHumidityType) {
			CommonValues::soilHumidityThresholdMin = message.data;
			CommonValues::soilHumidityThresholdMax = message.additionalData;
		}
	}
}

void GreenHouseLowerLayer::prepareMessage(Message& message, int address) {
	message.source = this->address;
	message.dest = address;
}




