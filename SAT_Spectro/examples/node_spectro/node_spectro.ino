#include <string.h>
#include <Serial.h>
#include <Wire.h>
#include "nanosat_message.h"
#include "I2C_add.h"
#include "OnboardCommLayer.h"
#include "Spectro.h"

int counter = 0;
int led = 13;
const int SIZE=64;
byte _buffer[SIZE];
int receiving = 0;
int esetting = 0;

Spectro spectro=NULL;

void setup(){
  memset(&_buffer[0],0,sizeof(byte)*SIZE);  
  //register the receive handler
  spectro.onReceiveChunk(onReceiveSpectro, &_buffer[0], SIZE);
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  spectro = Spectro(1);
  
}

void loop(){
  if(receiving==0){
    //the setting ranges from 1-99
    //esetting = (esetting + 1)%99;
    /*Serial.print("node_spectro::set_exposure(");
    Serial.print(esetting);
    Serial.println(")");*/
    
    //set the exposure time
    //spectro.set_exposure(esetting);
    //delay(100);
    //initiate the transfer of data
    //Serial.println("node_spectro::take_sample()");
    spectro.take_sample();
  }
  delay(2000);
}

/*
  This function is called multiple times by the spectruino.
  @pram buffer a local buffer populated by the spectruino library
  @param len the number of bytes written to the local buffer
  @param done 0 is not done receiving data for this request, 1 otherwise.
*/
void onReceiveSpectro(size_t len, boolean isDone){
  /*receiving=1;
  //write buffer to long-term storage
  if(done==1){
    receiving=0;
  }*/
  receiving=1;
  //Serial.println("onReceiveSpectro() triggered");
  //Serial.print("\tbytesRead="); Serial.println((int)len);
  //Serial.println((boolean)isDone);
  blink();
  receiving=0;
}

void blink(){
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(200);
}
