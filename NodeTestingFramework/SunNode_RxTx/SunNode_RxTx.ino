#include <Serial.h>
#include <Wire.h>

#include <I2C_add.h>
#include <nanosat_message.h>
#include <SAT_Sun.h>
#include <OnboardCommLayer.h>


int led = 13;
SAT_Sun sun=NULL;
float sunvalue;
uint8_t nodeid= 1;

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

typedef struct {
  test_message_hdr hdr;
  uint8_t* payload;    //payload
} test_message_t;

////////////////////////////////////

void txSetup(){
  Serial.println("Starting Sun_RxTx");
  
  Wire.begin();
  test_message_t init_msg;
  memset(&init_msg, 0, sizeof(test_message_t));
    
  
  init_msg.hdr.tid = TEST_MESSAGE_ID;
  init_msg.hdr.type = SET_SLAVE_ADDR;
  init_msg.hdr.plen = sizeof(uint8_t);
  init_msg.payload = (uint8_t*)malloc(init_msg.hdr.plen);
  memset(init_msg.payload, 0, sizeof(int));
  uint8_t slave_addr = I2C_ADD_SUPERVISOR;
  memcpy(init_msg.payload, &slave_addr, init_msg.hdr.plen);
  
  //set the request_size and the slave_addr
  Wire.beginTransmission(0xFF);
  Wire.write((uint8_t*)&init_msg, TEST_MESSAGE_HDR_SZ);
  Wire.write(init_msg.payload, init_msg.hdr.plen);
  Wire.endTransmission();
    
}

void setup(){
  Serial.begin(115200);
  pinMode(led, OUTPUT); 
  //txSetup();
  sun=SAT_Sun(1);
}

int cnt =1;
void loop(){
  blink();
  delay(2000);
  
  if(cnt<10){
    sunvalue = sun.getValue(nodeid);
    cnt+=1;
  }
}

void blink(){
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(150);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(150);
}
