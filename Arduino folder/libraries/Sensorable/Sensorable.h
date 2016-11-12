#ifndef SENSORABLE
#define SENSORABLE

#include <Arduino.h>
#include <Sensor.h>
#include <LinkedList.h>
#include <CommonValues.h>

template <class T>
//in green house project,'T' stands for Message.

class Sensorable {
	public:
		virtual ~Sensorable() {}
		
		void addSensor(Sensor* sensor) {
			sensorsArray.add(sensor);
		}
		
		void removeSensor(int pin) { 
			for (int i = 0; i < sensorsArray.size(); i++) {
				if (sensorsArray.get(i)->getId() == pin) {
					sensorsArray.remove(i);
					break;
				}
			}
		}
		
		void readSensorsData(LinkedList<T>&	 sensorsData) {
			Serial.println(F("Sensorable, readSensorsData()"));
			for (int i = 0; i < sensorsArray.size(); i++) {
				T newMessage;
				//if the sensor is the tempHumidity sensor, read it twice : because it 
				//holds data for both humidity and temperature.
				if (sensorsArray.get(i)->getId() == CommonValues::humidityTemperatureSensorId) {
					newMessage = sensorsArray.get(i)->readSensorData(true);
					sensorsData.add(newMessage);
				}
				newMessage = sensorsArray.get(i)->readSensorData(false);
				sensorsData.add(newMessage);
			}
		}
		
		virtual void onSensorFail() {
			Serial.println(F("Sensorable, onSensorFail()"));
		}
		
 	protected:
		LinkedList<Sensor*> sensorsArray;
};

#endif
