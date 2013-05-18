#include <Serial.h>
#include <Wire.h>

#include <I2C_add.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <SAT_Camera.h>

int led = 13;
boolean inited= false;

nanosat_message_t msg;

void setup(){
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  Wire.begin();
  Camera.on(1);
}

int cnt=0;
void loop(){
  delay(3000);
  Serial.println(cnt);
  Camera.takePictureSimple();
  //blah();
  cnt+=1;
}

void blink(){
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(150);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(150);
}

void blah(){
  msg.prefix = NODE_COMM_MESSAGE_PREFIX;
  msg.type = SENSOR_REG;
  msg.node_addr = 1;
  msg.len = 1;
  byte* _data = (byte*)&msg.data;
  _data[0] = ADD_SUPERVISOR_CAM;        //unique identifier for camera
  
  Wire.beginTransmission(I2C_ADD_SUPERVISOR);
  Wire.write((uint8_t*)&msg, sizeof(nanosat_message_t));
  Wire.endTransmission();
}
