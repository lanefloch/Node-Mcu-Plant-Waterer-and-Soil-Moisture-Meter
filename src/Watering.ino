//////////////////////////////////////////////////////////////////////////////////
////////////////////////////Pump Control and watering.////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifdef serialdebug
unsigned int pumpElapsedTime;
unsigned int lastPumpElapsedTime;
bool serialPumpOn = 1;
#endif

void pump(bool water) {

    Blynk.run();
    
    int waterSeconds = (waterMl * 0.29);

    currentMillis = millis();

    if(water == 1) {

        #ifdef serialdebug //Print "Turning on Pump"

            if(serialPumpOn == 1){

                Serial.println("Turning on pump");
                serialPumpOn = 0;

            }

        #endif

        digitalWrite(pumpPin, HIGH);

        #ifdef serialdebug //print amount of ML pumped out of ML to pump

            pumpElapsedTime = ((currentMillis - lastSensorMillis) * 0.001) / 0.29;

            if(lastPumpElapsedTime != pumpElapsedTime){

                Serial.print("Pumped "); 
                Serial.print(pumpElapsedTime);
                lastPumpElapsedTime = pumpElapsedTime;
                Serial.print(" ML Out Of ");
                Serial.print((waterSeconds / 0.29), 0);
                Serial.println(" ML");
            }

        #endif

        if((currentMillis - lastSensorMillis) > (waterSeconds*1000)) {

            #ifdef serialdebug
                Serial.println("Turning off pump");
            #endif

            digitalWrite(pumpPin, LOW);
            b = 1;
        }
    }

    else {

        digitalWrite(pumpPin, LOW);

        #ifdef serialdebug
            Serial.println("No need to water");
        #endif
        b = 1;

        }
}