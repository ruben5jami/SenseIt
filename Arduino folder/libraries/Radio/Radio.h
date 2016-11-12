#ifndef RADIO_H
#define RADIO_H
#include <Arduino.h>
#include <RF24.h>
#include <Message.h>
#include <ExponentialBackoff.h>
#include <CommonValues.h>
#include <ICommunicationable.h>


class Radio : public ICommunicationable{
public:
	Radio(int, int);
	void initCommunication (int, int);
	boolean sendMessage(Message);
	void receiveMessages(LinkedList<Message>&);
	static int sendCounter;//TEST
	static int receiveCounter; //TEST
private:
	RF24* radio;

};

#endif
