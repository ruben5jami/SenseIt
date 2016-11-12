#ifndef COMMON_VALUES
#define COMMON_VALUES

#include <Arduino.h>
/*
all frequently used variables in the project in one place.
*/
class CommonValues {
	public:
	
		//addresses
		static const int lowerLayerMinAddress		 = 1;
		static const int lowerLayerMaxAddress		 = 99;
		static const int middleLayerMinAddress		 = 100;
		static const int middleLayerMaxAddress		 = 199;
		static const int highLayerMinAddress		 = 200;
		static const int highLayerMaxAddress		 = 299;
		static const int lowerLayerAddress1			 = 1;
		static const int lowerLayerAddress2			 = 2;
		static const int lowerLayerConsumptionAdress = 3;
		static const int middleLayerAddress 		 = 101;
		static const int fakeMiddleLayerAddress		 = 102;
		static const int highLayerAddress 			 = 201;

		//sensors id
		static const int humidityTemperatureSensorId = 1;
		static const int lightSensorId 			  	 = 2;
		static const int soil1SensorId 				 = 3;
		static const int soil2SensorId 				 = 4;
		static const int currentSensorId			 = 5;
		static const int waterFlowSensorId			 = 6;

		//input pins
		static const int radioPin1 			   = 7;
		static const int radioPin2 			   = 8;
		static const int tempHumidityPin 	   = 2;
		static const int soilPin 			   = A0;
		static const int lightPin 			   = A1;
		static const int currentConsumptionPin = A5;
		static const int waterConsumptionPin1  = 2;
		static const int waterConsumptionPin2  = 3;
		static const int higherLayerRadioPin1  = 4;
		static const int higherLayerRadioPin2  = 15;

		//actuators pins
		static const int pumpPin  = 6;
		static const int fanPin   = 3;
		static const int ventPin  = 4;
		static const int steamPin = 10;
		static const int lampPin  = 9;
		static const int heatPin  = 5;

		//Conventions
		static const char emptyMessage		 = 'z';
		static const char policyChange 		 = 'p';
		static const char loopTimeChange 	 = 'a';
		static const char myAddressChange	 = 'b';
		static const char yourAddressChange  = 'y';
		static const char arduinoMalfunction = 'e';
		static const char dataType			 = 'd';
		static const char emergencyType		 = 'E';
		static const char ACTION_TYPE		 = 'A';

		//sensor types
		static const char temperatureType  = 'T';
		static const char humidityType 	   = 'H';
		static const char soilHumidityType = 'S';
		static const char lightType		   = 'L';
		static const char currentType	   = 'C';
		static const char waterType		   = 'W';
		
		//indexes
		static const int radioIndex = 0;
		static const int wifiIndex  = 1;

		static const int exponentialBackoffMaxRetries = 7;
		static const int sendMaxRetries				  = 10;
		static const int defaultLoopTime			  = 2000;
		static const int producersSize				  = 2;
		static const int lowerLayerRegisteredNum	  = 2;
		static const int EMERGENCY_TEMPERATURE		  = 40;

		//time consts
		static const unsigned long day	  = 86400000;  //24 hour
		static const unsigned long hour	  = 3600000;
		static const unsigned long minute = 60000;
		static const unsigned long second = 1000;
		
		//actuators intervals
		static const unsigned long fanTime   = 10 * second;
		static const unsigned long ventTime  = 8 * second;
		static const unsigned long heatTime  = 7 * second;
		static const unsigned long steamTime = 10 * second;
		static const unsigned long pumpTime  = 5 * second;
		static const unsigned long lampTime  = 3 * second;

		static const char* ssid;
		static const char* password;
		static const char* host;
		static const char* key;

		static const unsigned long lastTimeSentToServer = 0;
		static const long sendToServerInterval = 5000;
		static const unsigned long lastTimeCheckedForNewSettings = 0;
		static const long checkedForNewSettingsInterval = 10000;
		static const int amountOfGreenHouses = 1;
		static const int MiddleLayerLostConnectionTime = 50000;

		//thresholds - not consts! implemented at CommonValues.cpp
		static float soilHumidityThresholdMin;
		static float soilHumidityThresholdMax;

		static float airHumidityThresholdMin;
		static float airHumidityThresholdMax;

		static float temperatureThresholdMin;
		static float temperatureThresholdMax;

		static float lightThresholdMin;
		static float lightThresholdMax;

		static long whenTosendConsumption;
		static int minutesInInterval;
		static int watchDogSecondsUntilReset;

		

};

#endif
