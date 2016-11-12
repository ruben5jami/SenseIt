#include <LowerLayer.h>
#include <FanActuator.h>
#include <CommonValues.h>


RF24 radio(7, 8);
LowerLayer lowerLayer;
CommonValues commonValues;
FanActuator fan(commonValues.fan1ActuatorId, 2);


void initConsole() {
  while (!Serial);
  Serial.begin(9600);
}

void setup() {
  initConsole();
  lowerLayer.initLayer(commonValues.lowerLayerAddress1);
  lowerLayer.initCommunication(radio, CommonValues::lowerLayerAddress1,CommonValues::middleLayerAddress);
  
}

void loop() {
  Message msg;
  msg.sensorType = 'T';
  msg.data = 34.34;
  lowerLayer.sendMessage(radio, msg);

}
