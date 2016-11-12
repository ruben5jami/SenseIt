#include <GreenHouseLowerLayer.h>
#include <SCurrent.h>
#include <SWater.h>

//globals
//-------
GreenHouseLowerLayer lowerLayer;

//sensors
Sensor * currentSensor;
Sensor * waterSensor1;
Sensor * waterSensor2;


//pins
int currentPin = CommonValues::currentConsumptionPin;
int waterPin1 = CommonValues::waterConsumptionPin1;
int waterPin2 = CommonValues::waterConsumptionPin2;

//inits the console for debugging
void initConsole() {
  Serial.println("initConsole()");
  while (!Serial);
  Serial.begin(9600);
}

void createAndAddSensors() {
  Serial.println("createAndAddSensors()");
  currentSensor = new SCurrent(CommonValues::currentSensorId, currentPin);            
  waterSensor1 = new SWater(CommonValues::waterFlowSensorId, waterPin1);              
  waterSensor2 = new SWater(CommonValues::waterFlowSensorId, waterPin2);            

  lowerLayer.addSensor(currentSensor);
  lowerLayer.addSensor(waterSensor1);
  lowerLayer.addSensor(waterSensor2);
}

void setup() {
  initConsole();
  Serial.println("setup()");
  lowerLayer.initLayer(CommonValues::lowerLayerConsumptionAdress);
  createAndAddSensors();
}

void loop() {
  Serial.println("loop()");
  
  //analyze the data from all the sensors
  lowerLayer.analyze();

 //handle with received messages
  LinkedList<Message> messages;
  lowerLayer.receiveMessages(messages);

  //decode all messages
  int mSize = messages.size();
  Message* messagesArray = new Message[mSize];
  for (int i = 0; i<mSize; i++) {
    messagesArray[i] = messages.get(i);
    lowerLayer.decodeMessage(messagesArray[i]);
  }
  delete messagesArray;
  
 // reset the watch dog every time finish loop.
  lowerLayer.getWatchDog().reset();

  delay(lowerLayer.getLoopTime());
}
