//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////Sensor Functions/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

///////////////////////////
///Read Moisture Sensor///
//////////////////////////
#ifdef shortenedDebugTimes
#define sensorStabilizeSeconds 10 //time to wait for moisture sensor readings to stabilize
#else
#define sensorStabilizeSeconds 100 //time to wait for moisture sensor readings to stabilize
#endif

int sensorVal[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int moistureLevel = 0;

byte readSensor() {
    
    Blynk.run();

    currentMillis = millis();

    //turn on sensor//
    sensorOn(HIGH);

    #ifdef serialdebug
        Serial.print("Sensor Time Elapsed - ");
        Serial.print((currentMillis - connectMillis)*0.001, 0);
        Serial.print(" Out Of ");
        Serial.print(sensorStabilizeSeconds);
        Serial.println(" Seconds");
    #endif

    //wait for sensorStabilizeSeconds to be greater than current seconds//
    if(currentMillis - connectMillis > (sensorStabilizeSeconds*1000)) {
        
        //take 15 readings//
        for (byte i = 0; i < 15; i++){

            sensorVal[i] = (analogRead(moistureSensor));
            #ifdef serialdebug
                Serial.print("Reading # ");
                Serial.print(i+1);
                Serial.print(" = ");
                Serial.println(sensorVal[i]);
            #endif
        }
    
        //take an average of the 15 readings//
        int sensorValue = average(sensorVal,15);

        #ifdef serialdebug
            Serial.print("Average = ");
            Serial.println(sensorValue);
        #endif

        //map and constrain sensorValue to a moistureLevel between 0 - 100//
        moistureLevel = map(sensorValue, 290, 500, 100, 0);
        moistureLevel = max(moistureLevel, 0);
        moistureLevel = min(moistureLevel, 100);

        #ifdef serialdebug
            Serial.print("moisturelevel = ");
            Serial.println(moistureLevel);
        #endif

        //turn off sensor//
        sensorOn(LOW);

        return(moistureLevel);

        a = 1; //set a = 1; to exit while loop in main.ino

    }
}

////////////////////////////
///Moisture Sensor ON/OFF///
////////////////////////////
void sensorOn(bool on){
    if(on == HIGH) {

        #ifdef serialdebug
            Serial.println("Sensor On");
        #endif

        digitalWrite(activateMoistureSensor, HIGH);
    }
    else if(on == LOW) {

        #ifdef serialdebug
            Serial.println("turning off sensor");
        #endif

        digitalWrite(activateMoistureSensor, LOW);
    }
}

////////////////////
///Battery Sensor///
////////////////////
#ifdef shortenedDebugTimes
#define secondsToWaitAfterPumping 10 //time to wait for battery voltage to stabilize
#else
#define secondsToWaitAfterPumping 60 //time to wait for battery voltage to stabilize
#endif

bool readBatteryLevel() {

    Blynk.run();
    currentMillis = millis();

    #ifdef serialdebug
        Serial.print("Batt stabilization Time Elapsed - ");
        Serial.print((currentMillis - lastPumpMillis)*0.001, 0);
        Serial.print(" Out Of ");
        Serial.print(secondsToWaitAfterPumping);
        Serial.println(" Seconds");
    #endif

    if(currentMillis - lastPumpMillis > secondsToWaitAfterPumping*1000){

        int sensorVal = 0;
        for (byte i = 0; i < 25; i++) {

        sensorVal = (sensorVal + digitalRead(lowBattSensor));
    }

        if(sensorVal > 0) {

            #ifdef serialdebug
                Serial.println("BATT LOW");
            #endif
            c = 1;
            return(1);
        }

        else {

            #ifdef serialdebug
                Serial.println("BATT HIGH");
            #endif
            c = 1;
            return(0);
        }
    }
}

//////////////////////
///Average Function///
//////////////////////
int average (int * array, int len) {

    long sum = 0L ;  // sum will be larger than an item, long for safety.
    for (int i = 0 ; i < len ; i++)

        sum += array [i] ;

    return  ((int) sum) / len ;  // average will be fractional, so float may be appropriate.
}