#ifndef SENSOR_H
#define SENSOR_H
#include <Message.h>
#include <RF24.h>

//abstract class Sensor
class Sensor {
    public:
		Sensor(int id) {
			this->id = id;
		}
		
		int getId() {
			return id;
		}
		
        //read function must be implemented
        //this is called a pure virtual function
        virtual Message readSensorData(boolean) = 0; 
	protected:
		int id;
};
#endif