
#include <Wire.h>
#include <nanosat_message.h>
#include <string.h>
#include <SAT_ExperimentData.h>

message_t recv_buf;
int bufsize = sizeof(message_t);
int led = 13;
boolean setting = true;


void toggle(){
  //toggle LED
  if(setting)
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
  setting = !setting;
}

void setup(){
  Wire.begin();
  Serial.begin(9600);  // start serial for output
  pinMode(led, OUTPUT);   
}

void loop(){
  toggle();
  Wire.requestFrom(1, bufsize, false);    // request a message from address 1

  int bytes_read = 0;
  memset(&recv_buf, 0, bufsize);
  byte* buf = (byte*)&recv_buf;
  uint8_t idx = 0;
  boolean received = false;
  while(Wire.available()>0 && bytes_read <= bufsize)    // slave may send less than requested
  { 
    buf[idx]=Wire.read();
    idx+=1;
    bytes_read+=1;
    received = true;
  }
  if(received){
    int addr = (int)recv_buf.node_addr;
    if(addr<16){
      nano_datamsg_t* nano_msg =  (nano_datamsg_t*)&recv_buf.data;
      exp_data_t* exp_data_msg = (exp_data_t*)&nano_msg->buf;
      Serial.print("[type=");
      Serial.print((int)recv_buf.t);
      Serial.print(",addr=");
      Serial.print(addr);
      Serial.print(",len=");
      Serial.print(nano_msg->len);
      Serial.print(",exp_id=");
      Serial.print(exp_data_msg->exp_id);
      Serial.println("]   ");
    }
  }

  delay(500);
}
