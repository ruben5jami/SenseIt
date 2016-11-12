#ifndef GREEN_HOUSE_HIGH_LAYER
#define GREEN_HOUSE_HIGH_LAYER

#include <Arduino.h>
#include <HighLayer.h>
#include <Message.h>
#include <ArduinoJson.h>
#include <LinkedList.h>
#include <GreenHouseHighLayerData.h>
#include <GreenHouseHighLayerThresholds.h>
#include <CommonValues.h>
#include <ICommunicationable.h>
#include <Wifi.h>
#include <Radio.h>
#include <float.h>
#include <DateTime.h>
#include <Actions.h>


class GreenHouseHighLayer : public HighLayer<Message> {

private:
		GreenHouseHighLayerData greenHouseData[CommonValues::amountOfGreenHouses];
		GreenHouseHighLayerThresholds greenHouseThresholds[CommonValues::amountOfGreenHouses];
		int address;
public:
		/*Layer methods*/
		~GreenHouseHighLayer() {}
		void initLayer(int);
		void analyze();
		void decodeMessage(Message&);
		void prepareMessage(Message&, int);
		void recieveRFMessages(LinkedList<Message>&);
		void sendRFMessage(Message);
		int findGreenHouseDataIndex(int id);
		int findGreenHouseThresholdsIndex(int id);
		void sendDataToServer(JsonObject& json);
		void getNewSettings();
		void checkMiddleLayer();
		void checkActionPerformed();
		String dateTimeToString(DateTime);

};

#endif
