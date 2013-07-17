#include <Wire.h>
#include <I2C_add.h>
#include <Serial.h>

int cnt=0;
byte _buffer[32];
int led = 13;

void setup(){
  Wire.begin(I2C_ADD_SPECT);
  Wire.onReceive(receiveHandler);
  Wire.onRequest(requestHandler);
  Serial.begin(115200);
  Serial.println("Starting node_spectro_fake");
  memset(&_buffer[0],0,32);
}

void loop(){
  /*Serial.print(cnt);
  Serial.println("\tnode_spectro_fake::ping");
  Serial.print("Address::");
  Serial.println(I2C_ADD_SPECT);
  delay(1000);
  cnt+=1;*/
}

void receiveHandler(int bytesReceived){
  Serial.println("\tnode_spectro_fake::receiveHandlerInvoked");
  Serial.print("\t\tData::");  
  while(Wire.available()){
    Serial.print((char)Wire.read());
  }
  Serial.println();
  blink();
  
}

void requestHandler(){
  Serial.print("\tnode_spectro_fake::requestHandler invoked");
  Serial.println(cnt);
  cnt+=1;
  //delay(200);
  memset(&_buffer[0], (byte)'a', 32);
  //delay(200);
  Wire.write(&_buffer[0],32);
  blink();
}

void blink(){
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(150);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(150);
}
