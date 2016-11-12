#ifndef ACTUATORABLE
#define ACTUATORABLE

#include <Arduino.h>
#include <Actuator.h>
#include <LinkedList.h>


/*
base class that represent a behaviour of Actuatorable module. (a module that has actuators)
*/
class Actuatorable {
	public:
		virtual ~Actuatorable() {}

		void addActuator(Actuator* actuator);

		void removeActuator(int);

		Actuator* findActuatorById(int);
	
		LinkedList<Actuator*>& getActuatorsList();
	
		
	protected:
		LinkedList<Actuator*> actuatorsList;

};

#endif
