

#include <GreenHouseActuator.h>


GreenHouseActuator::GreenHouseActuator(int pin):Actuator(pin){
	//initialize with the actuator beeing off
	if (CommonValues::pumpPin == pin)// the second relay is working the opposite way. HIGH is off and LOW is on
		digitalWrite(this->pin, HIGH);
	else
		digitalWrite(this->pin, LOW);
}


Actions GreenHouseActuator::actuate(boolean on) {
	Actions action;

	if(on) {

		// the second relay is working the opposite way. HIGH is off and LOW is on
		if (CommonValues::pumpPin == pin) {
			digitalWrite(this->pin, LOW);
			delay(CommonValues::pumpTime);
		}
		else {
			digitalWrite(this->pin, HIGH); //turn on pin on relay
			switch (pin) {
			case CommonValues::fanPin:
				action = FAN;
				delay(CommonValues::fanTime);
				break;
			case CommonValues::ventPin:
				action = VENT;
				delay(CommonValues::ventTime);
				break;
			case CommonValues::steamPin:
				action = STEAMER;
				delay(CommonValues::steamTime);
				break;
			case CommonValues::heatPin:
				action = HEATER;
				delay(CommonValues::heatTime);
				break;
			case CommonValues::lampPin:
				action = LIGHT;
				//TODO light hours
								
				break;
			default:
				break;
			}
		}
	}

	if (CommonValues::pumpPin == pin)// the second relay is working the opposite way. HIGH is off and LOW is on
		digitalWrite(this->pin, HIGH);
	else if(CommonValues::lampPin != pin) // light does not turn off with intervals
		digitalWrite(this->pin, LOW);
	Serial.print(F("action was pefformed!!!!!!!!!!pin :"));
	Serial.println(this->pin);

	return action;
}

