#include <Wire.h>
#include <I2C_add.h>

boolean send_response = false;
void setup(){
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  Wire.begin();
  Wire.begin(I2C_ADD_SUPERVISOR);
  Wire.onReceive(__onReceive);
}

void loop(){
  float val = 1.23;
  if(send_response){
    Serial.println("Sending response");
    Wire.beginTransmission(1);
    Wire.write((uint8_t*)&val,sizeof(float));
    Wire.endTransmission();
    delay(1500);
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(150);               // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(150);
    send_response = false;
  }
}

void __onReceive(int bytes){
  Serial.print("received:");Serial.println(bytes);
  while(Wire.available())
    Wire.read();
  //send_response = true;
}
