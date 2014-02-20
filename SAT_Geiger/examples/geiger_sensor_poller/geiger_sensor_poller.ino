/*

example sketch to poll cooking hacks sensor 
every 1s

@author: Jeroen Cappaert
(c) NanoSatisfi 2012

modified by jfomhover on Jan. 2014 for ArduSatSDK

*/

#include <Wire.h>
#include <I2C_Conv.h>
//#include <I2C_add.h>
#include <EEPROM.h>
#include <OnboardCommLayer.h>

#include "SAT_Geiger.h"

#define CYCLE_TIME 2000


SAT_Geiger geiger;

long cpm;
float rad;


//=========
// SETUPpaas
//=========

void setup(){
  Serial.begin(115200);
  OBCL.begin(); //join I2C as node

  Serial.println("=== ArduSat sensor poller - RESET ===");
}

//======
// LOOP 
//======

void loop(){  
  //flush I2C buffer
  OBCL.flushWrite();
  OBCL.flushRead();
  
  //---------//
  // TUBE 1  //
  //---------//
  
  // get count per minute value
  
  cpm = geiger.getCPM(1);
  Serial.print("Tube 1 | cpm = "); Serial.print(cpm);
  
  delay(100);
  
  //** then get radiation value
  rad = geiger.getUSPH(1);
  Serial.print(" | uSv/h = "); Serial.println(rad, 4);
  
  
  
  
  //---------//
  // TUBE 2  //
  //---------//
 
  delay(100);
  
  
  //** first get CPM value
  cpm = geiger.getCPM(2);
  Serial.print("Tube 2 | cpm = "); Serial.print(cpm);
  
  delay(100);
  
  rad = geiger.getUSPH(2);
  Serial.print(" | uSv/h = "); Serial.println(rad, 4);
  Serial.println(" ");
  
  
  
delay(CYCLE_TIME);
}
