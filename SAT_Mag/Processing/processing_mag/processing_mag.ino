/* 
MAG3110 Readout Program for Arduino
Lara Booth for NanoSatisfi

For use with "Processing_magField.pde"

Based on the work "hacked together by Windell H. Oskay" 
http://www.evilmadscientist.com/2010/start-seeing-magnetic-fields/
*/

#include "SAT_Mag.h"
#include <Wire.h> //I2C

SAT_Mag mag;

byte ledPin = 13; // LED connected to digital pin 13
byte delaytime = 50; // There will be new values every n ms

int inByte = 0; // incoming serial byte

void setup()
{

Serial.begin(57600); //set the baud to 57600 (fast!)
delay(50); // give the MAG3110 some time to communicate
pinMode(ledPin, OUTPUT); //set the LED pin to output
mag.configMag(); //configure MAG3110
establishContact(); //send the character 'A' until contact is established with Processing
}

void loop()
{

int x,y,z; //declare variables x, y, and z

//when contact is established, scale x,y,z and write to Serial as two bytes
if (Serial.available() > 0) {
  
  digitalWrite(ledPin, HIGH); // set the LED on 

  x = (int) mag.readx();
  y = (int) mag.ready();
  z = (int) mag.readz();

if (x < 0)
{
x = -x;
x += 32767;
}

if (y < 0)
{
y = -y;
y += 32767;
}

if (z < 0)
{
z = -z;
z += 32767;
}

Serial.write(byte(x >> 8));
Serial.write(byte(x & 255));

Serial.write(byte(y >> 8));
Serial.write(byte(y & 255));

Serial.write(byte(z >> 8));
Serial.write(byte(z & 255)); 

delay(10); // LED on at least 10 ms
digitalWrite(ledPin, LOW); // set the LED off

delay(delaytime - 10); // LED off at least 90 ms
}
}

void establishContact() {
  
while (Serial.available() <= 0) {
Serial.write('A'); // send a capital A
delay(300);
}

}
