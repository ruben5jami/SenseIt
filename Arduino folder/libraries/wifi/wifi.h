#ifndef WIFI_H
#define WIFI_H
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Message.h>
#include <ArduinoJson.h>
#include <ICommunicationable.h>
#include <CommonValues.h>


class Wifi : public ICommunicationable{
private:
	WiFiClient client;
public:
	 void initCommunication(const char* , const char*);
	 String sendMessage(JsonObject&,String);
	 String receiveMessage(String);

};


#endif
