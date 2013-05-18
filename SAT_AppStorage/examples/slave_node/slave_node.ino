
#include <Wire.h>
#include <nanosat_message.h>
#include <I2C_add.h>
#include <OnboardCommLayer.h>
#include <SAT_AppStorage.h>
#include <SAT_ExperimentData.h>

#define MAX_READINGS 10

int led = 13;
boolean setting = true;

int addr = 1;
SAT_AppStorage appStorage= SAT_AppStorage(addr);

byte readings[MAX_READINGS];
uint8_t cnt = 0;

void setup(){
  Wire.begin(addr);
  Wire.onRequest(requestHdlr);
}

void loop(){
 //experiment generates data and calls appStore.sys_appdata_write() 
 uint8_t random_temp = (uint8_t)rand()%10+10;
 while(cnt<MAX_READINGS){
   readings[cnt]=random_temp;
   cnt+=1;
   delay(20);
   random_temp = (uint8_t)rand()%10+10;
 }
 if(cnt==MAX_READINGS){
   appStorage.store_exp_data(&readings[0], MAX_READINGS);
   cnt=0;
 }
 delay(200);
}

void requestHdlr(){
  //toggle LED
  if(setting)
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
  setting = !setting;
}
