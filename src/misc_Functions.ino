//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////SLEEP - WAKEUP FUNCTIONS/////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


void connect() {

    #ifdef serialdebug
        Serial.println("connecting to blynk");
    #endif

    //initialize blynk//
    Blynk.begin(auth, ssid, pass);


    //check if blynk is connected//
    if(Blynk.connected() == 1) {

        #ifdef serialdebug
            Serial.println("Connected to Blynk");
        #endif
        connected = 1;
        return; //do nothing//

    }

    else {
        #ifdef serialdebug
            Serial.println("Failed to connect, trying again");
        #endif

        //Blink LED 5 Times//
        blinkLED(5);
        
        //go back and try again//
        connect();
    }

}

void goToSleep() {

    #ifdef serialdebug
        Serial.println("going to sleep");
    #endif

    unsigned long sleepTime = (sleepTimeSeconds * 10e5);

    Blynk.disconnect();
    ESP.deepSleep(sleepTime);

    delay(100);
}


bool state = 1;
unsigned long lastBlink;

#ifdef serialdebug
    bool serialBlink=1;
#endif

void blinkLED(byte blinkAmount){

    for(byte i = 0; i < (blinkAmount*2); i=i){

        delay(10); //was hanging

        if(millis() - lastBlink > 800) {
            Blynk.run();
            i++;
            lastBlink = millis();
            if(state == LOW) state = HIGH;
            else if(state == HIGH) state = LOW;
        }

        #ifdef serialdebug
            

            if(serialBlink == 1){

                Serial.print("blinking lowBattLed ");
                Serial.print(blinkAmount);
                Serial.println(" times");
                serialBlink = 0;

            }

        #endif

        digitalWrite(lowBattLed, state);
    }
    digitalWrite(lowBattLed, LOW);
}

void dataSend(){ 

    //write sensor val Blynk//
    Blynk.virtualWrite(vMoistureLevel, (soilMoisture));

    //write water level to blynk//
    if(waterLevel == 1) {

        Blynk.virtualWrite(vLowWaterLevelIndicator, 255);

    }

    else {

        Blynk.virtualWrite(vLowWaterLevelIndicator, 0);

    }

    //write battery level to blynk//
    if(battLev == 0) Blynk.virtualWrite(vLowBatteryIndicator, 0);
    else Blynk.virtualWrite(vLowBatteryIndicator, 255);

}

void tweet(){
    Blynk.tweet("I am a bamboo plant, my moisture level is " + String(soilMoisture) + "% ");
    if(okToWater == 1){
        Blynk.tweet("My soil moisture is below the " + String(moisturePercentToWater) + "% threshold I am adding " + String(waterMl) + "ml. of water.");
    }
    d = 1;
}