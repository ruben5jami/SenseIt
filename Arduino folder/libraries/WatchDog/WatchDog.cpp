#include <WatchDog.h>

unsigned long WatchDog::lastUpdate = 0;
unsigned long WatchDog::timeout    = 0;

//reset the program to address 0 in memory
void (*resetFunc)(void) = 0;

void timeInterupt(void) {
	//check if 
	if((millis()- WatchDog::lastUpdate) > WatchDog::timeout) {
		//enable interrupts so serial can work
		sei();
 
		//detach Timer1 interrupt so that if processing goes long, WDT isn't re-triggered
		Timer1.detachInterrupt();
 
		//flush, as Serial is buffered; and on hitting reset that buffer is cleared
		Serial.println(F("timeInterupt triggered"));
		Serial.flush();
 
		//call to bootloader / code at address 0
		resetFunc();
	}
}

void WatchDog::enable(int seconds) {
	timeout = (unsigned long)seconds*CommonValues::second;
	lastUpdate = millis();
	Timer1.initialize(1000000); //1 second pulses
	Timer1.attachInterrupt(timeInterupt); //code to execute
}

void WatchDog::reset() {
	Serial.println(F("reset watch dog"));
	lastUpdate = millis();
}