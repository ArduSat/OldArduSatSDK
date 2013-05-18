
#include <Wire.h>
#include <nanosat_message.h>
message_t recv_buf;
int led = 13;
boolean setting = true;

void setup(){
  Wire.begin();
  Serial.begin(9600);  // start serial for output
  pinMode(led, OUTPUT);  
  /*Serial.print("sizeof(message_t)=");
  Serial.print((int)sizeof(message_t));*/
}

void loop(){
  //toggle LED
  if(setting)
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
  setting = !setting;
  
  Wire.requestFrom(1, sizeof(message_t), false);    // request a message from address 1

  int bytes_read = 0;
  byte* buf = (byte*)&recv_buf;
  uint8_t idx = 0;

  while(Wire.available() && bytes_read <= sizeof(message_t))    // slave may send less than requested
  {
    /*char c = Wire.read(); // receive a byte as character*/
    buf[bytes_read]=(byte)Wire.read();
    bytes_read+=1;
    idx+=1;
  }
  
  Serial.print("[type=");
  Serial.print((int)recv_buf.t);
  Serial.print(",addr=");
  Serial.print((int)recv_buf.addr);
  Serial.println("]   ");
  
  delay(500);
}
