#include "GreenHouseHighLayer.h"
#include "CommonValues.h"

GreenHouseHighLayer layer;


unsigned long lastTimeSentToServer = 0;
unsigned long lastTimeCheckedForNewSettings = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
  layer.initLayer(CommonValues::highLayerAddress);
  layer.setLoopTime(2000);
}

void loop() {
    Serial.println("loop start");
    LinkedList<Message> messages;
    int i;
    StaticJsonBuffer<2000> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    layer.checkActionPerformed();
    unsigned long now = millis();

    layer.recieveRFMessages(messages);

    //try to read RF message from lower level
    int mSize = messages.size();
    Message* messagesArray = new Message[mSize];
    for (int i = 0; i<mSize; i++) {
      messagesArray[i] = messages.get(i);
      layer.decodeMessage(messagesArray[i]);
    }

    delete messagesArray;

    //if it passeed xxx seconds from the last time we sent to the server than send
    if (now - lastTimeSentToServer >= CommonValues::sendToServerInterval ) {
      lastTimeSentToServer = now;
      //layer.sendDataToServer(root);
    }

    //if it passeed xxx seconds from the last time we checked for new settings
    if (now - lastTimeCheckedForNewSettings >= CommonValues::checkedForNewSettingsInterval ) {
      lastTimeCheckedForNewSettings = now;
      layer.getNewSettings();
    }

    layer.sendUnsentImportantMessages();

    delay(layer.getLoopTime());
}
