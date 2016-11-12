#ifndef MESSAGE_H
#define MESSAGE_H
#include <DateTime.h>
#include <Actions.h>

struct Message {
	char sensorType;
	char messageType;
	short int source;
	short int dest;
	DateTime dateTime;
	float data;
	float additionalData;
	Actions action;
	boolean flag;
};


#endif
