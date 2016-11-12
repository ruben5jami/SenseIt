

#include <nRF24L01.h>
#include <RF24.h>

//KY015 DHT11 Temperature and humidity sensor 
int DHpin = 2;
byte dat [5];
int LEDin = 4;

struct Message{
  short int source; //2 byte
  short int dest;   //
  
  };
RF24 radio(7, 8);
int limit = 40;
//const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
const byte rxAddr[6] = "00001";
const byte rxAddr2[6] = "00002";
byte read_data () {
  byte data;
  for (int i = 0; i < 8; i ++) {
    if (digitalRead (DHpin) == LOW) {
      while (digitalRead (DHpin) == LOW); // wait for 50us
      delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
      if (digitalRead (DHpin) == HIGH)
        data |= (1 << (7-i)); // high front and low in the post
      while (digitalRead (DHpin) == HIGH); // data '1 ', wait for the next one receiver
     }
  }
return data;
}

void start_test () {
  digitalWrite (DHpin, LOW);      // bus down, send start signal
  delay (30);                     // delay greater than 18ms, so DHT11 start signal can be detected
 
  digitalWrite (DHpin, HIGH);
  delayMicroseconds (40); // Wait for DHT11 response


  pinMode (DHpin, INPUT);

  while (digitalRead (DHpin) == HIGH);

  delayMicroseconds (80); // DHT11 response, pulled the bus 80us
  if (digitalRead (DHpin) == LOW);
  delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data
      

  for (int i = 0; i < 4; i ++) // receive temperature and humidity data, the parity bit is not considered
    dat[i] = read_data ();
 
  pinMode (DHpin, OUTPUT);

  digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
}

void setup()
{
   pinMode(LEDin, OUTPUT);
    pinMode (DHpin, OUTPUT);
    while (!Serial);
    Serial.begin(9600);
    radio.begin();
    radio.setRetries(15, 15);
    radio.openWritingPipe(rxAddr);
     radio.openReadingPipe(1, rxAddr2);
       radio.printDetails();

    radio.startListening();

}

void loop()
{
  start_test ();
  Serial.print ("Current humdity = ");
  Serial.print (dat [0], DEC);                    // display the humidity-bit integer;
  if(dat[0]>30) digitalWrite(LEDin, HIGH);
  else digitalWrite(LEDin, LOW);
  Serial.print ('.');
  Serial.print (dat [1], DEC);                    // display the humidity decimal places;
  Serial.print ("% ");
  Serial.print ("              Temperature = ");
  Serial.print (dat [2], DEC);                    // display the temperature of integer bits;
  Serial.print ('.');
  Serial.print (dat [3], DEC);                    // display the temperature of decimal places;
  Serial.println ('C');
  delay (500);

  
  char text[32]="";
  itoa(dat[0],text,10);
  Serial.print("itoa check :");
  Serial.println(text);
    
    Serial.println("here1");
   radio.stopListening();
  char text2[32]="hi";
bool ok =  radio.write(&text, sizeof(text));
  if(ok){
    Serial.println("send seccess");
         
  }
else{
      Serial.println("send failed");

  }
   radio.startListening();

  if (radio.available())
  {
      radio.read(&text, sizeof(text));
      Serial.print("recived message:");
      Serial.println(text);

      limit = atoi(text);
  }

  delay(1000);
}
