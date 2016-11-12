
#include <Actuatorable.h>



void Actuatorable::addActuator(Actuator* actuator) {
	actuatorsList.add(actuator);
}

//remove an actuator from actuators list , with a given pin number.
void Actuatorable::removeActuator(int pin) {
	int size = actuatorsList.size();
	for (int i = 0; i < size; i++) {
		if (actuatorsList.get(i)->getPin() == pin) {
			actuatorsList.remove(i);
			break;
		}
	}
}

Actuator* Actuatorable::findActuatorById(int pin) {
	int size = actuatorsList.size();
	for (int i = 0; i < size; ++i) {
		if (actuatorsList.get(i)->getPin() == pin) {
			return actuatorsList.get(i);
		}
	}
}

LinkedList<Actuator*>& Actuatorable::getActuatorsList() {
	return this->actuatorsList;
}


