/* 
    read_celsius.ino - Example for MLX90614 library returns temperature in
    Celsius
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
#include <SAT_InfraTherm.h>

SAT_InfraTherm mlx;

void setup(){
  Serial.begin(9600); //begin Serial monitor
  OBCL.begin(); //begin Wire
  Serial.println("Setup..."); //initialization complete
}

//print temperature in Celsius
void loop(){
  float celsius = 0;
  celsius = mlx.getTemp();
  Serial.print("Temperature: ");
  Serial.print(celsius);
  Serial.println(" * C"); //indicate units
  delay(1000); // wait a second before printing again
}
