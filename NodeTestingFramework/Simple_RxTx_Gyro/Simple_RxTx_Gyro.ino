#include <Serial.h>
#include <Wire.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <SAT_Gyro.h>

int led = 13;
SAT_Gyro gyro;


void setup(){
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  Wire.begin();
}

int cnt=0;
void loop(){
  Serial.println("hello");
  
  //float pitch = gyro.getPitch();
  //Serial.println(pitch);
  delay(1000);
  cnt+=1;
}

void blink(){
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(150);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(150);
}