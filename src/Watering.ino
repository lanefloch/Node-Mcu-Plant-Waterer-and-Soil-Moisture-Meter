//////////////////////////////////////////////////////////////////////////////////
////////////////////////////Pump Control and watering.////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void pump(bool water) {

    Blynk.run();
    int waterSeconds = (waterMl * 0.29);

    if(water == 1) {

        #ifdef serialdebug
            Serial.println("Turning on pump");
        #endif

        digitalWrite(pumpPin, HIGH);

        currentMillis = millis();

        #ifdef serialdebug
            Serial.print("Pumped ");
            Serial.print(((currentMillis - lastSensorMillis) * 0.001) / 0.29, 0);
            Serial.print(" ML Out Of ");
            Serial.print((waterSeconds / 0.29), 0);
            Serial.println(" ML");
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