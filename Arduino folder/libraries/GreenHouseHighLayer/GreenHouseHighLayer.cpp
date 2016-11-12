#include "GreenHouseHighLayer.h"

void GreenHouseHighLayer::initLayer(int address) {

	int i;

	this->address = address;

	//initialize greenhouse data
	for (i = 0; i < CommonValues::amountOfGreenHouses; i++ ) {
		greenHouseData[i].setId(CommonValues::middleLayerAddress + i);
		greenHouseThresholds[i].setId(CommonValues::middleLayerAddress + i);
	}

	//initialize communication

	ICommunicationable* radioPtr = new Radio(CommonValues::higherLayerRadioPin1,CommonValues::higherLayerRadioPin2);
	radioPtr->initCommunication(address,CommonValues::middleLayerAddress);
	this->addCommunication(radioPtr);

	ICommunicationable* wifiPtr = new Wifi();
	wifiPtr->initCommunication(CommonValues::ssid, CommonValues::password);
	this->addCommunication(wifiPtr);

}

void GreenHouseHighLayer::analyze() {
}

int GreenHouseHighLayer::findGreenHouseDataIndex(int id) {
	int i, result;
	for (i = 0; i < CommonValues::amountOfGreenHouses; i++ ) {
		if (id == greenHouseData[i].getId())
			result = i;
	}
	return result;
}

int GreenHouseHighLayer::findGreenHouseThresholdsIndex(int id) {
	int i, result;
	for (i = 0; i < CommonValues::amountOfGreenHouses; i++ ) {
		if (id == this->greenHouseThresholds[i].getId())
			result = i;
	}
	return result;
}

void GreenHouseHighLayer::decodeMessage(Message & message) {

	if (CommonValues::emptyMessage == message.sensorType || CommonValues::highLayerAddress != message.dest) {
		return;
	}

	Serial.println("decoding message");

	int defaultAction = 0, greenhouseId;
	String soilHumidityKey = "soilHumidity";
	soilHumidityKey += message.source;
	if (message.messageType == CommonValues::dataType && message.sensorType == CommonValues::soilHumidityType) {
		greenhouseId = message.additionalData;
	}
	else {
		greenhouseId = message.source;
	}

	int i = this->findGreenHouseDataIndex(greenhouseId);
	float data;
	if (CommonValues::amountOfGreenHouses == i) {
		return;
	}

	switch(message.messageType) {
	case CommonValues::emergencyType:
		break;
	case CommonValues::dataType:
		//if message has action
		if (message.action != NONE) {
			StaticJsonBuffer<500> jsonBuffer;
			JsonObject& root = jsonBuffer.createObject();
			root["greenhouse"] = greenhouseId;
			JsonObject& dateTime = root.createNestedObject("dateTime");
			dateTime["year"] = message.dateTime.year;
			dateTime["month"] = message.dateTime.month;
			dateTime["date"] = message.dateTime.date;
			dateTime["hour"] = message.dateTime.hours;
			dateTime["minutes"] = message.dateTime.minutes;
			dateTime["seconds"] = message.dateTime.seconds;

			defaultAction = 1;
			Actions action = message.action;
			Serial.println("is action");
			Serial.println(action);
			switch(action) {
				case PUMP1:
					root["action"] = "pump1";
					break;
				case PUMP2:
					root["action"] = "pump2";
					break;
				case FAN:
					root["action"] = "fan";
					break;
				case LIGHT:
					root["action"] = "light";
					break;
				case HEATER:
					root["action"] = "heater";
					break;
				case STEAMER:
					root["action"] = "steamer";
					break;
				default:
					Serial.println("default");
					defaultAction = 0;
			}

			if (defaultAction != 0) {
				this->communicationList.get(1)->sendMessage(root,"/api/action");
			}

		} //end if message has action



		DateTime dateTime = message.dateTime;

		//update value by sensor type
		switch(message.sensorType) {
			case CommonValues::temperatureType:
				data = message.data;
				greenHouseData[i].updateValue("temperature",data,dateTime);
				break;
			case CommonValues::humidityType:
				data = message.data;
				greenHouseData[i].updateValue("airHumidity",data,dateTime);
				break;
			case CommonValues::lightType:
				data = message.data;
				greenHouseData[i].updateValue("luminance",data,dateTime);
				break;
			case CommonValues::soilHumidityType:
				data = message.data;
				greenHouseData[i].updateValue(soilHumidityKey,data,dateTime);
				break;
			case CommonValues::currentType:
				data = message.data;
				greenHouseData[i].updateValue("current",data,dateTime);
				break;
			case CommonValues::waterType:
				data = message.data;
				greenHouseData[i].updateValue("water",data,dateTime);
				break;
			default:
				break;
		}
		greenHouseData[i].setLastChecked(millis());
		break;
	}

}

void GreenHouseHighLayer::prepareMessage(Message & message, int address) {
	message.source = CommonValues::highLayerAddress;
	message.dest = address;
}

void GreenHouseHighLayer::recieveRFMessages(LinkedList<Message>& messages) {
	this->communicationList.get(0)->receiveMessages(messages);
};

void GreenHouseHighLayer::sendRFMessage(Message message) {
	//if message was not send
	if (!(this->communicationList.get(0)->sendMessage(message))) {
		this->unsentImportantMessages.add(message);
	}
}

void GreenHouseHighLayer::sendDataToServer(JsonObject& json) {
	JsonArray& jGreenHouses = json.createNestedArray("greenhouses");
	int i,j;
	for (i = 0; i < CommonValues::amountOfGreenHouses; i++ ) {
		JsonObject& object = jGreenHouses.createNestedObject();
		object["id"] = this->greenHouseData[i].getId();

		JsonArray& dataArray = object.createNestedArray("data");

		for (j = 0; j < this->greenHouseData[i].getValuesSize(); j++ ) {
			DataValue data = this->greenHouseData[i].getValue(j);
			JsonObject& jsonData = dataArray.createNestedObject();
			jsonData["key"] = data.name;
			jsonData["value"] = data.value;

			JsonObject& dateTime = jsonData.createNestedObject("dateTime");
			dateTime["year"] = data.dateTime.year;
			dateTime["month"] = data.dateTime.month;
			dateTime["date"] = data.dateTime.date;
			dateTime["hour"] = data.dateTime.hours;
			dateTime["minutes"] = data.dateTime.minutes;
			dateTime["seconds"] = data.dateTime.seconds;

		}

		if (j > 0) {
			Serial.println("sending data to server");
			this->communicationList.get(1)->sendMessage(json,"/api/data");
		}

	}
}

//get new settings from server and send it to middle layer
void GreenHouseHighLayer::getNewSettings() {
	int i,j;
	Message message;
	String name;
	float value;

	//for every greenhouse check if it settings has changed
	for (i = 0; i < CommonValues::amountOfGreenHouses; i++ ) {
		int greenhouse = this->greenHouseData[i].getId();
		int index;
		String response;
		String url = "/api/options/";
		url += greenhouse;

		//get the current thresholds from the server
		response = this->communicationList.get(1)->receiveMessage(url);

		//parse respone to json
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(response);

		//if not a valid json
		if (!root.success()) {
			Serial.println("parseObject() failed");
			return;
		}

		JsonArray& options = root["options"];

		//iterate over the thresholds we got
		for(JsonArray::iterator it=options.begin(); it!=options.end(); ++it) {

			JsonObject& itValue = *it;
			String updated_at = itValue["updated_at"];

			//get the current threshold object stored
			ThresholdsValue tValue = this->greenHouseThresholds[index].getValueByName(itValue["key"]);

			//
			if (tValue.last_updated != updated_at) {
				String key = itValue["key"];
				float minValue = itValue["minValue"];
				float maxValue = itValue["maxValue"];

				Serial.println("updating threshold");
				this->greenHouseThresholds[index].updateValue(key,minValue,maxValue,updated_at);

				message.data = minValue;
				message.additionalData = maxValue;
				message.messageType = CommonValues::policyChange;
				if (key == "airHumidity") {
					message.sensorType = CommonValues::humidityType;
				}
				if (key == "soilHumidity") {
					message.sensorType = CommonValues::soilHumidityType;
				}
				if (key == "temperature") {
					message.sensorType = CommonValues::temperatureType;
				}
				if (key == "luminance") {
					message.sensorType = CommonValues::lightType;
				}

				this->prepareMessage(message,greenhouse);
				this->sendRFMessage(message);
			}
		}
	}
}

void GreenHouseHighLayer::checkMiddleLayer() {
	int i;
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	for (i = 0; i < CommonValues::amountOfGreenHouses; i++ ) {
		root["greenhouse"] = findGreenHouseDataIndex(i);
		//if the greenhouse has not contacted in the defined time
		if (millis() - greenHouseData[i].getLastChecked() >= CommonValues::MiddleLayerLostConnectionTime ) {
			//set it to not working state
			greenHouseData[i].setWorking(false);
			Serial.println("updating status");
			this->communicationList.get(1)->sendMessage(root, "/api/status");
		}
		else {
			if (!greenHouseData[i].getWorking()) {
				//set it back to working state
				greenHouseData[i].setWorking(true);
				Serial.println("updating status");
				this->communicationList.get(1)->sendMessage(root, "/api/status");
			}
		}
	}
}


void GreenHouseHighLayer::checkActionPerformed() {
	int i,greenhouse,counter;
	Message message;
	String url,response;
	DynamicJsonBuffer jsonBuffer;

	for (i = 0; i < CommonValues::amountOfGreenHouses; i++ ) {

		greenhouse  = greenHouseData[i].getId();
		counter = 0;
		url = "/api/action/perform/";
		url += greenhouse;

		//get the actions to perform
		response = this->communicationList.get(1)->receiveMessage(url);
		JsonArray& root = jsonBuffer.parseArray(response);

		for(JsonArray::iterator it=root.begin(); it!=root.end(); ++it) {
			JsonObject& itValue = *it;
			String action = itValue["action"];
			String on = itValue["on"];

			//if the action is to turn on
			//in case of light we can also switch off
			if (on == "1" || (on == "2" && action == "light")) {

				//set the message type
				message.messageType = CommonValues::ACTION_TYPE;
				//set the action type
				if (action == "pump1") {
					message.action = PUMP1;
				}
				else if (action == "pump2") {
					message.action = PUMP2;
				}
				else if (action == "fan") {
					message.action = FAN;
				}
				else if (action == "light") {
					message.action = LIGHT;
				}
				else if (action == "heater") {
					message.action = HEATER;
				}
				else if (action == "steamer") {
					message.action = STEAMER;
				}
				else if (action == "vent") {
					message.action = VENT;
				}

				//set on or off
				if (on == "2" && action == "light") {
					message.data = 0;
				}
				else {
					message.data = 1;
				}

				//send rf message to the middle layer
				this->prepareMessage(message,greenhouse);
				this->sendRFMessage(message);
				counter++;
			}
		}


		//reset perform actions
		if (counter > 0) {
			url = "/api/action/perform/reset/";
			url += greenhouse;
			StaticJsonBuffer<10> jsonBuffer;
			JsonObject& root = jsonBuffer.createObject();
			this->communicationList.get(1)->sendMessage(root,url);
		}

	}
}
