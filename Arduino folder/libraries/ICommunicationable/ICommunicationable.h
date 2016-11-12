#ifndef ICOMMUNICATIONABLE
#define ICOMMUNICATIONABLE

#include <Arduino.h>
#include <Message.h>
#include <ArduinoJson.h>
#include <LinkedList.h>
/*
base class that represent a behaviour of comunicationable module. (a module that has comunicate)
*/
class ICommunicationable {
	public:
		virtual ~ICommunicationable() {}
		virtual void initCommunication(int, int) {}
		virtual void initCommunication(const char*, const char*) {}
		virtual boolean sendMessage(Message) {};
		virtual String sendMessage(JsonObject&,String) {return "";}
		virtual void receiveMessages(LinkedList<Message>& m) {}
		virtual String receiveMessage(String) {return "";};
};

#endif
