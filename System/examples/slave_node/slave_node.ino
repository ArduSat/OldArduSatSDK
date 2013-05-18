
#include <Wire.h>
#include <System.h>
#include <nanosat_message.h>

#define MAX_READINGS 10

int led = 13;
boolean setting = true;

int addr = 1;
System nanoSystem(addr);

void toggle(){
  //toggle LED
  if(setting)
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
  setting = !setting;
}

void setup(){
  Wire.begin(addr);
  Wire.onRequest(requestHdlr);
}

void loop(){
 //experiment generates data and calls appStore.sys_appdata_write() 
 uint8_t rexit = (uint8_t)rand()%10+10;
 nanoSystem.exit(rexit);
 delay(200);
}

void requestHdlr(){
  toggle();
  nanoSystem.onRequestEvent();
}


