#include <Serial.h>
#include <Wire.h>

#include <I2C_add.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <SAT_Clock.h>

int led = 13;

void setup(){
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  Clock.init(1);
}

int cnt=0;
void loop(){
  delay(3000);
  Serial.print(cnt);Serial.print("\t");
  float time =Clock.getTime();
  Serial.println(time);
  cnt+=1;
}

void blink(){
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(150);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(150);
}
