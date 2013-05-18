#include <string.h>
#include <Wire.h>
#include <Serial.h>

int counter = 0;
int led = 13;

void setup(){
  Wire.begin(0x01);
  Serial.begin(57600);
  pinMode(led, OUTPUT);
  Wire.onReceive(onReceiveEvent);
}

void loop(){
  char* buffer;
  int len=0;
  Wire.beginTransmission(0x00);
  if(counter%2==0){
    buffer = "p";
  } else {
    buffer = (char*)malloc(2);
    sprintf(buffer, "%c%d", '#', counter%100);
  }
  len = strlen(buffer);
  Wire.write((byte*)buffer, len);
  Wire.endTransmission();
  delay(5000);
  counter +=1;
  
  blink();
}

void onReceiveEvent(int howMany){
  while(Wire.available()>0){
    Serial.print(Wire.read());
  }
  Serial.println();
  blink();
}

void blink(){
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(200);
}
