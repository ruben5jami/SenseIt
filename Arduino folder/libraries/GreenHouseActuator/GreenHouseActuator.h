#ifndef GREEN_HOUSE_ACTUATOR
#define GREEN_HOUSE_ACTUATOR

#include <Arduino.h>
#include <Actuator.h>
#include <CommonValues.h>
#include<Actions.h>

/*
all the actuators in our greenhouse act the same.
*/
class GreenHouseActuator :public Actuator{
	public:	
		GreenHouseActuator(int);
		Actions actuate(boolean);
};



#endif