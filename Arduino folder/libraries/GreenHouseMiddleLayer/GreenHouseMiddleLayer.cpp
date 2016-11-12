#include <GreenHouseMiddleLayer.h>

Actions GreenHouseMiddleLayer:: handleThresholds(float value, int min, int max, int minPin, int maxPin) {
	Actions action = NONE;
	//check max threshold
	if (value >= max)
		action = actuate(maxPin,true);
	//check min threshold
	else if (value <= min) {
		//if its the light threshold, we dont want to actuate in intervals,we want on,or off.
		//TODO check time of light
		action = actuate(minPin, true);
	}
	return action; //no action performed
}

boolean GreenHouseMiddleLayer::updateDataAndCheckIfFull(LinkedList<Message>& list,Message& lastMessage,int fullSize){
	if (list.size() == 0) {					//for the first time
		list.add(lastMessage);
	}
	else {
		// update data
		int i;
		for (i = 0; i < list.size(); ++i) {
			if (list.get(i).source == lastMessage.source) {	//if arduino id exsist, overwrite the data
				list.remove(i);
				list.add(lastMessage);
				break;
			}
		}
		if(list.size() == i) // if we didnt find id that maches last message id
			list.add(lastMessage);			//add the message (it has a new id)
		//check if full
		if (fullSize == list.size()) {
			return true;
		}
		return false;
	}
		

	if (list.size() == CommonValues::lowerLayerRegisteredNum)
		return true;
	else
		return false;
}

void GreenHouseMiddleLayer::initLayer(int address) {
	this->address = address;
	
	//register lower layers
	addLowerId(CommonValues::lowerLayerAddress1);
	addLowerId(CommonValues::lowerLayerAddress2);
	addLowerId(CommonValues::lowerLayerConsumptionAdress);	//TODO maybe remove the consumption layer from the low layers array
	plantsLowerLayers = lowersIds.size() - 1; // minus the consumption
	//init radio
	ICommunicationable* radio = new Radio(CommonValues::radioPin1, CommonValues::radioPin2);
	radio->initCommunication(this->address, CommonValues::lowerLayerAddress1);
	communicationList.add(radio);
	setLoopTime(CommonValues::defaultLoopTime);

	//more inits here
}

boolean GreenHouseMiddleLayer::sendMessage(Message& message) {
	Serial.print("sending : ");
	Serial.println(message.data);
	return communicationList.get(CommonValues::radioIndex)->sendMessage(message);
};

void GreenHouseMiddleLayer::receiveMessages(LinkedList<Message>& messages) {
	 communicationList.get(CommonValues::radioIndex)->receiveMessages(messages);
};

void GreenHouseMiddleLayer::prepareMessage(Message& message, int add) {
	message.source = this->address;
	message.dest = add;	
}

float GreenHouseMiddleLayer::doAverage(LinkedList<Message>& messages) {
	float average;
	Serial.print(F("messages.size()"));
	Serial.println(messages.size());
	for (int i = 0 ; i<messages.size();++i) {
		average+=messages.get(i).data;
	}
	average = average/messages.size();
	Serial.print(F("returning av :"));
	Serial.println(average);
	return average;
}

void GreenHouseMiddleLayer::analyze() {	
	float temperatureAverage = 0;
	float airHumidityAverage = 0;
	float lightAverage = 0;
	Message newMessage;
	DateTime dateTime;
	clock.createDateTime(dateTime);
	newMessage.dateTime = dateTime;	 //add time to message
	prepareMessage(newMessage, CommonValues::highLayerAddress);
	//we have some data to analyze
	if (isTemperatureReadyToAnalyze) {
		Serial.println(F("in isTemperatureReadyToAnalyze"));
		//calculate average
		temperatureAverage = doAverage(temperatureData);
		//check thresholds
		newMessage.action = handleThresholds(temperatureAverage, CommonValues::temperatureThresholdMin,
			CommonValues::temperatureThresholdMax, CommonValues::heatPin,CommonValues::fanPin);
			//todo assign action
		newMessage.data = temperatureAverage;
		newMessage.messageType = CommonValues::dataType;
		newMessage.sensorType = CommonValues::temperatureType;
		if (!(sendMessage(newMessage))) {

			//TODO handle if  message fails
		}
		//clear the array after done
		isTemperatureReadyToAnalyze = false;
		temperatureData.clear();
	}
	//we have some data to analyze
	if (isHumidityReadyToAnalyze) {
		Serial.println(F("in isHumidityReadyToAnalyze"));
		//calculate average
		airHumidityAverage = doAverage(humidityData);
		//check thresholds
		handleThresholds(airHumidityAverage, CommonValues::airHumidityThresholdMin,
			CommonValues::airHumidityThresholdMax, CommonValues::steamPin, CommonValues::ventPin);
		newMessage.action = STEAMER;
		newMessage.data = airHumidityAverage;
		newMessage.messageType = CommonValues::dataType;
		newMessage.sensorType = CommonValues::humidityType;
		if (!(sendMessage(newMessage))) {
		//	Serial.println(F("message failed, adding to unsent"));
		//	unsentImportantMessages.add(newMessage);
		}
		//clear the array after done
		isHumidityReadyToAnalyze = false;
		humidityData.clear();
		Serial.print(F("after isHumidityReadyToAnalyze clear size is :"));
		Serial.println(humidityData.size());
	}
	if (isLightReadyToAnalyze) {
		Serial.println(F("in isLightReadyToAnalyze"));
		//calculate average
		lightAverage = doAverage(lightData);
		//TODO decide what to do with light thresholds
		newMessage.action = handleThresholds(lightAverage, CommonValues::lightThresholdMin,
			CommonValues::lightThresholdMax, CommonValues::lampPin, CommonValues::lampPin);
		newMessage.data = lightAverage;
		newMessage.messageType = CommonValues::dataType;
		newMessage.sensorType = CommonValues::lightType;
		if (!(sendMessage(newMessage))) {
			//TODO
		}
		//clear the array after done
		isLightReadyToAnalyze = false;
		lightData.clear();
	}	
	//TODO reduce duplicate code all above
	//no need to do average on consumption data
	
}

void GreenHouseMiddleLayer::decodeMessage(Message& msg) {	
	Serial.print(msg.source);

	if (CommonValues::emptyMessage == msg.messageType) {
		//do nothing the message is empty
		return;
	}
	DateTime dateTime;
	clock.createDateTime(dateTime);
	msg.dateTime = dateTime;	 //add time to message
	if (msg.dest != CommonValues::middleLayerAddress) {		 //check if the message is for me
		sendMessage(msg);			// if so, pass it on
		return;
	}
	//the message is from higer layer
	 if (msg.source >= CommonValues::highLayerMinAddress && msg.source < CommonValues::highLayerMaxAddress) {   
		switch (msg.messageType) {
			case CommonValues::policyChange:
				switch (msg.sensorType) {
					case CommonValues::soilHumidityType:
						//if it's soil Humidity policy changes, send it to the lower layers
						for (int i = 0; i<lowersIds.size() ; ++i) {
							if (lowersIds.get(i) != CommonValues::lowerLayerConsumptionAdress) {
								prepareMessage(msg, lowersIds.get(i));
								if (!sendMessage(msg)) {
									//unsentImportantMessages.add(msg);
								}
							}
						}			
					break;
					case CommonValues::temperatureType:
						CommonValues::temperatureThresholdMin = msg.data;
						CommonValues::temperatureThresholdMax = msg.additionalData;
					break;
					case CommonValues::humidityType:
						CommonValues::airHumidityThresholdMin = msg.data;
						CommonValues::airHumidityThresholdMax = msg.additionalData;
					break;
					case CommonValues::lightType:
						CommonValues::lightThresholdMin = msg.data;
						CommonValues::lightThresholdMax = msg.additionalData;
					break;
				}
			case CommonValues::loopTimeChange:
				
			break;
			case CommonValues::myAddressChange:
				
			break;
			case CommonValues::yourAddressChange:
			break;
			case CommonValues::ACTION_TYPE:
				switch (msg.action) {
					case PUMP1:
						prepareMessage(msg, lowersIds.get(0));
						if (!sendMessage(msg)) {
							//unsentImportantMessages.add(msg);
						}
						break;
					case PUMP2:
						prepareMessage(msg, lowersIds.get(1));
						if (!sendMessage(msg)) {
							//unsentImportantMessages.add(msg);
						}
						break;
					case FAN:
						msg.messageType = CommonValues::dataType;
						msg.action = actuate(CommonValues::fanPin,msg.data);
						prepareMessage(msg, CommonValues::highLayerAddress);
						if (!sendMessage(msg)) {
							//unsentImportantMessages.add(msg);
						}
					break;
					case LIGHT:
						msg.messageType = CommonValues::dataType;
						msg.action = actuate(CommonValues::lampPin, msg.data);
						prepareMessage(msg, CommonValues::highLayerAddress);
						if (!sendMessage(msg)) {
							//unsentImportantMessages.add(msg);
						}
						break;
					case HEATER:
						msg.messageType = CommonValues::dataType;
						msg.action = actuate(CommonValues::heatPin,msg.data);
						prepareMessage(msg, CommonValues::highLayerAddress);
						if (!sendMessage(msg)) {
							//unsentImportantMessages.add(msg);
						}
						break;
					case VENT:
						msg.messageType = CommonValues::dataType;
						msg.action = actuate(CommonValues::ventPin,msg.data);
						prepareMessage(msg, CommonValues::highLayerAddress);
						if (!sendMessage(msg)) {
							//unsentImportantMessages.add(msg);
						}
					break;
					case STEAMER:
						msg.messageType = CommonValues::dataType;
						msg.action = actuate(CommonValues::steamPin,msg.data);
						prepareMessage(msg, CommonValues::highLayerAddress);
						if (!sendMessage(msg)) {
							//unsentImportantMessages.add(msg);
						}
					break;
					case NONE:
						//TODO
					break;
					default:
						//TODO
					break;
				}
					
			break;
			case CommonValues::arduinoMalfunction: 

			break;			 
		}
	}
	//if the meesgae came from bottom layer
	//the layer should act/send up the hirarchy if needed
	else if (msg.source >= CommonValues::lowerLayerMinAddress && msg.source < CommonValues::lowerLayerMaxAddress) {
		switch (msg.messageType) {
			//first check if it's an emergency message
			case CommonValues::emergencyType:
				actuate(CommonValues::fanPin, true);
				actuate(CommonValues::ventPin, true);
				prepareMessage(msg, CommonValues::highLayerAddress); // prepare to send to high
				if (!sendMessage(msg)) {
					//unsentImportantMessages.add(msg);
				}
				break;
			case CommonValues::dataType:
				switch (msg.sensorType) {
					case CommonValues::soilHumidityType:
						//if it's soil Humidity data, send it to the high layer
						msg.additionalData = getMyAddress(); // the higher needs to know which pot it is.
						msg.dest = CommonValues::highLayerAddress;
						 //check if the message is important (action performed),and add to unsentImportantMessages if not sent
						 if (NONE != msg.action) {
							 if (!sendMessage(msg)) {
								// unsentImportantMessages.add(msg);
							 }
						 }
						 else
							sendMessage(msg); 
					break;
					case CommonValues::currentType:
						//if it's current consumption data, send it to the high layer
						prepareMessage(msg, CommonValues::highLayerAddress); // prepare to send to high
						sendMessage(msg);
					break;
					case CommonValues::waterType:
						//if it's water consumption data, send it to the high layer
						prepareMessage(msg, CommonValues::highLayerAddress);
						sendMessage(msg);
					break;
					case CommonValues::temperatureType:						
						isTemperatureReadyToAnalyze = ((updateDataAndCheckIfFull(temperatureData,msg,plantsLowerLayers)) && isTimeConsistency(temperatureData, CommonValues::minutesInInterval));
					break;
					case CommonValues::humidityType:
						isHumidityReadyToAnalyze = ((updateDataAndCheckIfFull(humidityData,msg, plantsLowerLayers)) && isTimeConsistency(humidityData, CommonValues::minutesInInterval));
					break;
					case CommonValues::lightType:
						isLightReadyToAnalyze = ((updateDataAndCheckIfFull(lightData,msg, plantsLowerLayers)) && isTimeConsistency(lightData, CommonValues::minutesInInterval));
					break;
				
				}
			break;
		//TODO think maybe handle with more messageTypes	
			//TODO action type
			//TODO what happens if we are not erady to analyze??
		}//end of switch (msg.messageType)
		analyze();
	}//end of if (msg.source >= CommonValues::lowerLayerMinAddress && msg.source < CommonValues::lowerLayerMaxAddress)
}

boolean GreenHouseMiddleLayer::isTimeConsistency(LinkedList<Message>& data, int minutes) {
	unsigned long interval = minutes*CommonValues::minute;
	for (int i = 0; i<data.size(); ++i) {
		for (int j = 1; j<data.size(); j++) {
			if ((GreenHouseMiddleLayer::convertDateTimeToMillis(data.get(i).dateTime) - convertDateTimeToMillis(data.get(j).dateTime)) > interval) {
				return false;
			}
		}
	}
	return true;
}

unsigned long GreenHouseMiddleLayer::convertDateTimeToMillis(DateTime dateTime) {
	unsigned long millis = (dateTime.seconds*1000) + (dateTime.minutes*60000) + (dateTime.hours*3600000) + 
		(dateTime.date*86400000) + (dateTime.month*2629746000) + (dateTime.year*31556952000);
	return millis;
}

Actions GreenHouseMiddleLayer::actuate(int pin_,boolean on) {
	Serial.print(F("actuate pin :"));
	Serial.println(pin_);

	for (int i = 0; i<actuatorsList.size() ; ++i) {
		if (actuatorsList.get(i)->getPin() == pin_) {
			return actuatorsList.get(i)->actuate(on);
		}
	}	
}

int GreenHouseMiddleLayer::getMyAddress(){
	return this->address;
}

