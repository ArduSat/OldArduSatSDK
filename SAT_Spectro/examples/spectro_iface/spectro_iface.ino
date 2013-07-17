#include <Wire.h>
#include <Serial.h>
int led = 13;
char buffer[2];

void setup(){
  Wire.begin(0x00);
  Wire.onReceive(onReceiveEvent);
  Serial.begin(115200);
  pinMode(led, OUTPUT);  
}

void loop(){
  delay(500);
  if(Serial.available()>0){
    blink();blink();
    //forward it on the wire to the destination (not currently recorded)
    Wire.beginTransmission(0x01);
    while(Serial.available()>0){
      Wire.write(Serial.read());
    }
    Wire.endTransmission();
  }
}

void onReceiveEvent(int howMany){
  int idx = 0;
  memset(buffer, 0, 2);
  while(Wire.available()>0){
    buffer[idx]=(char)Wire.read();
    idx=(idx+1)%2;
  }
  Serial.write((byte*)buffer, 2);
  
  //Serial.println();
  //blink();
}

void blink(){
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(200);
}

