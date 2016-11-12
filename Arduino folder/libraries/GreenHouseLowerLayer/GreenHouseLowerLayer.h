#ifndef GREEN_HOUSE_LOWER_LAYER
#define GREEN_HOUSE_LOWER_LAYER

#include <LowerLayer.h>
#include <Radio.h>
#include <CommonValues.h>
#include <Message.h>

class GreenHouseLowerLayer : public LowerLayer<Message>{

	public:
		/*LowerLayer methods*/
		~GreenHouseLowerLayer(){}
		 void initLayer(int);
		 //analyze the current state
		 void analyze();
		 //decode a given message by our protocol (know what the message contains and act by it)
		 void decodeMessage(Message&);
		 //preparing the message is all about updating the senser and receivers address to the message 
		 void prepareMessage(Message&, int);
	
		 void initDataArrays();
		 //avarage the data that has ben collected in a measuring cycle
		 float doAverage(LinkedList<float>&);
		 void prepareDataMessage(Message&, float, char);
		 boolean sendMessage(Message&);
		 void receiveMessages(LinkedList<Message>&);
	private:
		int address;
		unsigned long currentMillis;
		unsigned long previousMillis;
		
		//lower layer data
		LinkedList<float> temperatureData;
		LinkedList<float> soilHumidityData;
		LinkedList<float> airHumidityData;
		LinkedList<float> lightData;
		//lower layer consumption data
		float currentConsumptionData;
		float waterConsumptionData1;
		float waterConsumptionData2;

		
};

#endif
