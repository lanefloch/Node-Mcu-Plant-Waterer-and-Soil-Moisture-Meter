//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////PLANT WATERING PROJECT//////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////
///SETTINGS///
//////////////

#define serialdebug //Serial Printouts for debugging

#define shortenedDebugTimes //Shorten sleep and stabilize time to allow for easier debugging

#define moisturePercentToWater 15 //At what soil moisture percent should the pump come on?

char auth[] = "zClZn8Pvs0p05s1Ft0E41HCQ-v7pEfS3"; //Blynk Auth Code
char ssid[] = "flochwireless"; //Wifi SSID
char pass[] = "buster95"; //Wifi Password

///////////
///DEBUG///
///////////

#ifdef shortenedDebugTimes
#define sleepTimeSeconds 30 //time in seconds for node to sleep between wakeups in debug mode
#else
#define sleepTimeSeconds 1800 //time in seconds for node to sleep between wakeups
#endif

///////////////////
///LIBRARY STUFF///
///////////////////

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

/////////////////
///BLYNK STUFF///
/////////////////

#define BLYNK_PRINT Serial

//////////
///PINS///
//////////

//inputs//

#define moistureSensor A0
#define waterLevelSensor D1
#define lowBattSensor D5

//outputs//

#define pumpPin D2
#define activateMoistureSensor D6
#define lowBattLed D7

//Virtual Pins//

#define vWateringOnOff V0
#define vWateringAmount V1
#define vLowWaterLevelIndicator V4
#define vLowBatteryIndicator V3
#define vMoistureLevel V2

//////////////////////
///global Variables///
//////////////////////
int soilMoisture = 0;
bool battLev = 0;
bool connected = 0;
byte waterLevel;
byte a = 0; // variable to lock into sensor mode
byte b = 0; // variable to lock into pump mode
byte c = 0; //variable to lock into batt sense mode
unsigned long currentMillis;
unsigned long lastSensorMillis;
unsigned long lastPumpMillis;
unsigned long connectMillis;
int waterMl;
int waterOnOff;
int okToWater;

BLYNK_WRITE (vWateringAmount){
 waterMl = param.asInt();
}

BLYNK_WRITE (vWateringOnOff){
  if(param.asInt() == 1) waterOnOff = 1;
  else waterOnOff = 0;
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(vWateringOnOff, vWateringAmount); //sync button state and amount to water//
}

void setup()
{

  //inputs//
  pinMode(moistureSensor, INPUT);
  pinMode(waterLevelSensor, INPUT_PULLUP);
  pinMode(lowBattSensor, INPUT);

  //outputs//
  pinMode(pumpPin, OUTPUT);
  pinMode(activateMoistureSensor, OUTPUT);
  pinMode(lowBattLed, OUTPUT);
  pinMode(D0, WAKEUP_PULLUP);

  // Debug console
  Serial.begin(115200);

  blinkLED(10);
}

void loop() {
  //connect to blynk//
  if(connected == 0) connect(); connectMillis = millis();

  Blynk.run();

   //Read sensor//
  while(a == 0) {
    soilMoisture = readSensor();
  }

  lastSensorMillis = millis();

  while(true) Serial.println("done"); delay(1500);

  return;

  #ifdef serialdebug
  Serial.print("waterOnOff = ");
  Serial.println(waterOnOff);
  Serial.print("waterMl = ");
  Serial.println(waterMl);
  #endif

  waterLevel = digitalRead(waterLevelSensor);

  if(soilMoisture < moisturePercentToWater && waterOnOff == 1 && waterLevel) okToWater = 1;
  else okToWater = 0;

  #ifdef serialdebug
  Serial.print("okToWater = ");
  Serial.println(okToWater);
  #endif

   //pump water//
   while(b == 0){
     pump(okToWater);
   }

  lastPumpMillis = millis();

  //Read battery level//
  bool battLev;
  while(c == 0)  {
   battLev = readBatteryLevel();
 }

 //illuminate low batt led based on battlev//
 digitalWrite(lowBattLed, (!battLev));

  //write battery level to blynk//
  if(battLev == 0) Blynk.virtualWrite(vLowBatteryIndicator, 0);
  else Blynk.virtualWrite(vLowBatteryIndicator, 255);

  //write water level to blynk//
  if(waterLevel == 1) {
  
    #ifdef serialdebug
      Serial.println("Water level Low");
    #endif

    Blynk.virtualWrite(vLowWaterLevelIndicator, 255);

  }

  else {
  
    #ifdef serialdebug
      Serial.println("Water level Ok");
    #endif

    Blynk.virtualWrite(vLowWaterLevelIndicator, 0);

  }

  //write sensor val Blynk//
  Blynk.virtualWrite(vMoistureLevel, (soilMoisture));
  
  if(battLev > 0) {
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
    delay(10);
    while(true){

    }
  }

  //sleep//
  goToSleep();
 }
