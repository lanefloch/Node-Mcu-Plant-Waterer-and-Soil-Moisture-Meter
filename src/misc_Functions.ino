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

void blinkLED(byte blinkAmount){

    for(byte i = 0; i < blinkAmount; i=i){

        if(millis() - lastBlink > 800) {
            Blynk.run();
            i++;
            lastBlink = millis();
            if(state == LOW) state = HIGH;
            else if(state == HIGH) state = LOW;
        }

        #ifdef serialdebug
            Serial.println("blinking lowBattLed");
        #endif

        digitalWrite(lowBattLed, state);
    }
    digitalWrite(lowBattLed, LOW);
}