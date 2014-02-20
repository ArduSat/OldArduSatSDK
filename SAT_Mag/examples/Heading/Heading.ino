#include <Wire.h>
#include <EEPROM.h>
#include <OnboardCommLayer.h>
#include <SAT_Mag.h>

SAT_Mag mag;


void setup()
{
  OBCL.begin();
  Serial.begin(9600);  // start serial for output (fast)
  mag.configMag();         // turn the MAG3110 on
}


void loop()
{
  //offset  x, y, z values
  float x, y, z;
  
  //raw x, y, z values and heading
  int x1, y1, z1, h;
  
  //save the offset values 
  x = mag.x_value();
  y = mag.y_value();
  z = mag.z_value();
  
  //save the raw values 
  x1 = mag.readx();
  y1 = mag.ready();
  z1 = mag.readz();
  
  //save the heading 
  h = mag.getHeading(x,y,z);
  
  //print the Raw values
  Serial.print("Raw    x: ");
  Serial.print(x1);
  Serial.print(" y: ");
  Serial.print(y1);
  Serial.print(" z: ");
  Serial.println(z1);
  
  //print the Offset values
  Serial.print("Offset x: ");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.print(y);
  Serial.print(" z: ");
  Serial.println(z);
  
  //print the Heading
  Serial.print("Heading : ");
  Serial.println(h);
  
  delay(1000); //wait a second
}


