/*
 * test arduino sketch for TMP102 libary
*/

#include <Wire.h>
#include <EEPROM.h>
#include <OnboardCommLayer.h>
#include <SAT_Temp.h>

#define TEMP_PAYLOAD 0
#define TEMP_BOTTOMPLATE 2

SAT_Temp tmp_payload(TEMP_PAYLOAD);

/**
 * @brief Initialize Serial bus speed and output initialization text.
 */
void setup(){
    Serial.begin(9600);
    OBCL.begin();
    Serial.println("Setup done");
}

/**
 * @brief Grab temperature readings from payload area and bottom plate then
 *  output them to serial bus. If one wants to write to disk for publishing back
 *  down to earth, look at both ExperimentData and AppStorage libraries.
 */
void loop(){
    float temp_payload;
    temp_payload = tmp_payload.getTemp();

    Serial.print(" Temp PL: ");
    Serial.println(temp_payload);
    delay(3000);
}
