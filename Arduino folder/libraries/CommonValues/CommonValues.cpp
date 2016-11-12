#include <CommonValues.h>

/*
values that can change at run time.
*/

float CommonValues::soilHumidityThresholdMin = 300;
float CommonValues::soilHumidityThresholdMax = 1024;

float CommonValues::airHumidityThresholdMin = 0;
float CommonValues::airHumidityThresholdMax = 100;

float CommonValues::temperatureThresholdMin = 18;
float CommonValues::temperatureThresholdMax = 35;
float CommonValues::lightThresholdMin = 60; 
float CommonValues::lightThresholdMax = 100;

int CommonValues::minutesInInterval			= 3;
int CommonValues::watchDogSecondsUntilReset = 60;

const char* CommonValues::ssid 	   = "SuperBox";
const char* CommonValues::password = "0547231229";
const char* CommonValues::host     = "multlayermngmnt.com";
