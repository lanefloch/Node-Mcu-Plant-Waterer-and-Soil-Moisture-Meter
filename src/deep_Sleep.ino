//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////SLEEP - WAKEUP FUNCTIONS/////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


void connect() {

    #ifdef serialdebug
        Serial.println("connecting to blynk");
    #endif

    //initialize blynk//
    Blynk.begin(auth, ssid, pass);
    delay(1000);

    //check if blynk is connected//
    if(Blynk.connected() == 1) {

        #ifdef serialdebug
            Serial.println("Connected to Blynk");
        #endif

        return; //do nothing//

    }

    else {
        #ifdef serialdebug
            Serial.println("Failed to connect, trying again");
        #endif

        //Blink LED//
        digitalWrite(lowBattLed, HIGH);
        delay(1500);
        digitalWrite(lowBattLed, LOW);
        delay(1500);
        digitalWrite(lowBattLed, HIGH);
        delay(1500);
        digitalWrite(lowBattLed, LOW);
        delay(1500);
        digitalWrite(lowBattLed, HIGH);
        delay(1500);
        digitalWrite(lowBattLed, LOW);
        
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
