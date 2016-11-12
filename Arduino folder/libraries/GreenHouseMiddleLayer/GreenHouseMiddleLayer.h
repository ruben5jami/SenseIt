#ifndef GREEN_HOUSE_MIDDLE_LAYER
#define GREEN_HOUSE_MIDDLE_LAYER

#include <MiddleLayer.h>
#include <Sensor.h>
#include <Radio.h>
#include <CommonValues.h>
#include <Clock.h>
#include<Actions.h>

/*
class GreenHouseMiddleLayer implements the behavior of a middle layer in the generic term.
*/
class GreenHouseMiddleLayer :  public MiddleLayer<Message> {

	public:

		void initDataArrays();
		//avarage the data that has ben collected in a measuring cycle
		float doAverage(LinkedList<Message>&); 
		//check the time of the given data, and see if maybe its irelevant
		boolean isTimeConsistency(LinkedList<Message>&, int); 
		//indicate if when we got data from  all the sensors
		boolean updateDataAndCheckIfFull(LinkedList<Message>&, Message&,int);
		//actuate or deactuate an actuator 
		Actions actuate(int,boolean);
		//what to do with the current state? act by the thesholds guidelines
		Actions handleThresholds(float, int, int, int, int);
		//adress getter
		int getMyAddress();
		//analyze the current state
		void analyze();
		//MiddleLayer implementation
		void initLayer(int);
		//preparing the message is all about updating the senser and receivers address to the message 
		void prepareMessage(Message&, int);
		//decode a given message by our protocol (know what the message contains and act by it)
		void decodeMessage(Message&);

		boolean sendMessage(Message&);
		void receiveMessages(LinkedList<Message>&);
		unsigned long convertDateTimeToMillis(DateTime);
		
	private:

		int sendDataTime;
		//the time that is reasonable that a layer did not send annt message.
		int sensorTypeNotRespondingTime;
		//whats the time now?
		Clock clock;
		int address;
		int plantsLowerLayers; // the consumption is a lower layer, but in the greenhouse we treat him different.

		//data in each measuring cycle
		LinkedList<Message> temperatureData;
		LinkedList<Message> humidityData;
		LinkedList<Message> lightData;
		LinkedList<Message> currentData;
		LinkedList<Message> waterData;

		//variable to know if specific sensor is ready to analyze
		boolean isTemperatureReadyToAnalyze = false;
		boolean isHumidityReadyToAnalyze = false;
		boolean isLightReadyToAnalyze = false;

};

#endif
