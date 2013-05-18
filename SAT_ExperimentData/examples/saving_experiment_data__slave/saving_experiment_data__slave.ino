#include <nanosat_message.h>
#include <Wire.h>
#include <OnboardCommLayer.h>
#include <SAT_ExperimentData.h>
#include <SAT_AppStorage.h>
#include <I2C_add.h>

int addr = 1;
SAT_AppStorage appStorage(addr);

int expid = 0;
int max_experiments = 1;
int max_rows = 1;
int max_cols = 1;

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
  Wire.begin(addr);
  pinMode(led, OUTPUT);
  //Serial.begin(9600);  // start serial for output
}

void loop(){
  toggle();
  expid +=1;
  //experiment generates data and calls appStore.sys_appdata_write() 
  uint8_t random_temp = (uint8_t)rand()%10+10;
  //Serial.print("here");
   
  for(int j=0; j<max_rows; j++){
    for(int k=0; k<max_cols; k++){
      SAT_ExperimentData datapt(expid, j, k, false, 1, 1, INTEGER, &random_temp, sizeof(uint8_t));
      appStorage.store_exp_data(&datapt);
      //printall(expid, j, k, random_temp);
    }
  }
  delay(2000);
}

//serial communication on nano a little problematic
//check received values on master
/*void printall(int experiment, int row, int col, int value){
  Serial.print("[exp=");
  Serial.print(experiment);
  Serial.print(",row=");
  Serial.print(row);
  Serial.print(",col=");
  Serial.print(col);
  Serial.print(",value=");
  Serial.println(value);
}*/