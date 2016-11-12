#include <nRF24L01.h>
#include <RF24.h>
#include <message.h>
#include <SPI.h>
#include <SSoil.h>
#include <ExponentialBackoff.h>


#define MIDDLE_LAYER_ADDRESS 101  
#define MY_ADDRESS 1 

 /*sensor value description:
  * 0 - 300   dry soil
  * 300 - 700 humid soil
  * 700 - 950 in water
  */

//globals
//-------
RF24 radio(7, 8);
Sensor* sensor;
int soil_humidity_threshold_minimum = 40;
int soil_humidity_threshold_maximmum = 40;
int soilPin = A0;
//available addresses
const byte rxAddr[6] = "00001"; 
const byte wxAddr[6] = "00002";             


void initConsole() {
  Serial.println("initConsole()");
  while (!Serial);
  Serial.begin(9600);
}

void initRadio() {
    Serial.println("initRadio()");
    radio.begin();
    radio.setRetries(15, 15);
    radio.openWritingPipe(wxAddr);
    radio.openReadingPipe(1,rxAddr);
    radio.stopListening();
}

void setup() {
  Serial.println("setup()");
  initConsole();
  initRadio();
  sensor= new SSoil(soilPin);              //create new soil humidity sensor instanse   
}

void sendMessage(Message message){
        bool ok = false;
        int iteration = 0;
        int delayMili = 0;
        ExponentialBackoff exponentialBackoff;
        radio.stopListening();
        while(!ok && delayMili != -1){  //if message fails 
            ok =  radio.write(&message, sizeof(message));
            if(ok)
               Serial.println("send success");      
            else{
              Serial.println("send failed backing off");
              delayMili = exponentialBackoff.getDelayTime(++iteration);
              if(delayMili >= 0)
                delay(delayMili);
              else;   
              //send failed (max retries)  TODO  
            }
       }
        radio.startListening();
    }
  
Message prepareMessage(Message message){
  message.source = MY_ADDRESS;
  message.dest = MIDDLE_LAYER_ADDRESS;
  return message;
}

bool receiveMessage(Message& message){
  Serial.println("receiveMessage()");
  if (radio.available()){
    radio.read(&message, sizeof(message));
    Serial.print("recived message:");
//    Serial.println(message.data);
    return true;
  }
  else {
    Serial.println("nothing to read");
    return false;
  }
}

void loop() {
  Serial.println("loop()");
  Message readSensor =  sensor->readSensorData();          //read sensor data

  if (readSensor.data[0] <= 300)  
    Serial.println(" sopping");  
  if ((readSensor.data[0] > 300) and (readSensor.data[0] <= 700))  
    Serial.println(" Moist, not water");   
  if (readSensor.data[0] > 700)  
    Serial.println(" Dry, you need to water"); 
    
//  Message messageToSend = prepareMessage(readSensor); //add sender id and receiver id to message
 
  /*sendMessage(messageToSend);                          //send message  
  Message messageToRead;
  if(receiveMessage(messageToRead)){                   //receive message
    Serial.print("main loop, i got: ");
    Serial.println(messageToRead.minimum_threshold);
    Serial.print("and: ");
    Serial.println(messageToRead.maximum_threshold);
  }*/
  delay(1000);
}
