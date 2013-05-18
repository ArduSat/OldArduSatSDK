#include <Wire.h>
#include <I2C_add.h>
#include <nanosat_message.h>

byte buffer[32];

boolean send_response = false;
void setup(){
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  Wire.begin();
  Wire.begin(I2C_ADD_SUPERVISOR);
  Wire.onReceive(__onReceive);
  Serial.println("hello");
  Serial.println(I2C_ADD_SUPERVISOR);
}

void loop(){
  char* path = "/path/to/picture";
  if(send_response){
    delay(800);
    Serial.println("Sending response");
    Wire.beginTransmission(1);
    Wire.write((uint8_t*)path,sizeof(char)*strlen(path));
    Wire.endTransmission();
    delay(1500);
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
    send_response = false;
  }
}

void print_message(){
  nanosat_message_t* m = (nanosat_message_t*)&buffer[0];
  Serial.print("message.prefix\t"); Serial.println(m->prefix);
  //Serial.print("message.type\t");Serial.println(m->type);
  byte* d = (byte*)&m->data;
  Serial.print("message.data\t");Serial.println(d[0]);
}

void __onReceive(int bytes){
  Serial.print("received:");Serial.println(bytes);
  memset(&buffer[0], 0, 32);
  int i=0;
  while(Wire.available()){
    buffer[i]=Wire.read();
    i+=1;
  }
  print_message();
  send_response = true;
}
