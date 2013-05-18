#include <nanosat_message.h>
#include <Serial.h>
#include <I2C_add.h>
#include <Wire.h>

int cnt=0;
int led = 13;
nanosat_message_t m;

const int BUFFER_SIZE = 32;
byte _buffer[BUFFER_SIZE];
byte _app_buffer[BUFFER_SIZE];

int request_size=BUFFER_SIZE;
uint8_t slave_addr = 0xFF;


////////////////////////////////////
//copy these in Tx test sketches
////////////////////////////////////
#define TEST_MESSAGE_ID  0xAA
#define TEST_MESSAGE_HDR_SZ sizeof(test_message_hdr)

enum test_message_types{
  SET_REQUEST_SIZE,      //sets the request size
  SET_SLAVE_ADDR,        //sets the slave addr of this receiver
};

typedef struct {
  uint8_t tid;
  uint8_t type;        
  uint8_t plen;        //the size of the payload
} test_message_hdr;

typedef struct{
  test_message_hdr hdr;
  uint8_t* payload;    //payload
} test_message_t;

////////////////////////////////////

void setup(){
  pinMode(led, OUTPUT); 
  Wire.begin();
  Wire.begin(slave_addr);
  Wire.onReceive(receiveHandler);
  Wire.onRequest(requestHandler);
  Serial.begin(115200);
  memset(&_buffer[0],0,BUFFER_SIZE);
  memset(&_app_buffer[0],0, BUFFER_SIZE);
  Serial.print("Starting General_Rx(slave_addr=");
  Serial.print((int)slave_addr);
  Serial.print(", request_size=");
  Serial.print((int)request_size);
  Serial.println(")");
  
}

void loop(){
}

void handleTestMessage(){
  test_message_t* tmsg = (test_message_t*)&_buffer[0];
  tmsg->payload = &_app_buffer[0];
  switch(tmsg->hdr.type){
   case SET_REQUEST_SIZE:
     request_size = *((int*)&tmsg->payload[0]);
     break;
   case SET_SLAVE_ADDR:
     slave_addr=tmsg->payload[0];
     Wire.begin(slave_addr);
     break;
   }
   
   memset(&_buffer[0],0,BUFFER_SIZE);
   memset(&_app_buffer[0],0, BUFFER_SIZE);
}

void handleMessage(){
  
  nanosat_message_t* msg = (nanosat_message_t*)&_buffer[0];
  if(msg->prefix==NODE_COMM_MESSAGE_PREFIX){
    switch(msg->type){
      case SUN:
        float v = 25.3;
        //Serial.println("SENDING\0");
        Wire.beginTransmission(1);
        Wire.write((uint8_t*)&v,sizeof(float));
        Wire.endTransmission();
        break;
    }
  } else {
    Serial.println(msg->prefix);
  }
  
  memset(&_buffer[0],0,BUFFER_SIZE);
  memset(&_app_buffer[0],0, BUFFER_SIZE);
}

void receiveHandler(int bytesReceived){
  //Serial.println("General_Rx::receiverHandler");
  int i=0;
  int j=0;
  byte* mem= &_buffer[0];
  boolean isTestMsg=false;
  //int br =0;
  if(bytesReceived<=BUFFER_SIZE){
    while(Wire.available()){
      byte thisByte = Wire.read();
      //Serial.print(thisByte); Serial.print(" ");
      if(!isTestMsg){
        mem[i]=thisByte;
        //br+=1;
      } else {
        mem[j]=thisByte;
        j+=1;
      }
      i+=1;
      if(i==sizeof(test_message_hdr)){
        test_message_t* test_msg = (test_message_t*)&mem[0];
        if(test_msg->hdr.tid==TEST_MESSAGE_ID){
          isTestMsg = true;
          //read the payload into the application buffer
          mem = &_app_buffer[0];
        }
      }
    }
  }
  Serial.println();
  delay(100);
  if(isTestMsg){
    Serial.println("Test Message Rx'd");
    handleTestMessage();
    //blink();
  } else {
    //Serial.println("Other Message Rx'd");
    handleMessage();
    //it's another kind of message
    //blink();blink();blink();
  }
}

void requestHandler(){
  Serial.print("General_Rx.requestHandler::");
  Serial.println(cnt);
  cnt+=1;
  memset(&_buffer[0], (byte)'a', 32);
  Wire.write(&_buffer[0],32);
  blink();
}

void blink(){
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(150);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(150);
}
