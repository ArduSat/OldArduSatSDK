/* 
    read_xyz.ino - Example prints raw x,y,z values on current magnetic field 
    Copyright (C) 2012  Lara Booth for NanoSatisfi

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Wire.h>
#include <EEPROM.h>
#include <OnboardCommLayer.h>
#include <SAT_Mag.h>


SAT_Mag mag;


void setup()
{
  OBCL.begin();
  Serial.begin(9600);  // start serial for output (fast)
  mag.configMag();          // turn the MAG3110 on
}


void loop()
{
  //print the raw values of x,y,z
  Serial.print("x: ");
  Serial.println( mag.readx());
  Serial.print("y: ");
  Serial.println( mag.ready());
  Serial.print("z: ");
  Serial.println( mag.readz());
  
  delay(1000);//wait a second
}


