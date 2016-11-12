#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduino.h>
#include <Actions.h>

/*
this class represents an Actuator in general , and specific actuators will inherit from it. 
*/
class Actuator {
	public:
		Actuator(int _pin):pin(_pin){
			pinMode(_pin,OUTPUT);
		}
		int getPin() { return pin; }
		virtual Actions actuate(boolean) = 0;    //different actuators will act differently
	protected:
		int pin;
};



#endif