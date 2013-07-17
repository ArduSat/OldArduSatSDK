/* 
    SAT_MLX90614.cpp - Library for Arduino returns data on temperature 
    from MLX90614 sensor
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

////////////////////////////////////////////////////////////////////////////////
          Notes:
          Designed for use with Melexis (C) MLX90614 Infrared Thermometer

          Library gathers temperature data over I2C Bus/SM Bus

          Tested with MLX90614 from Sparkfun, 4.7k Ohm pull-up resistors,
          .1uF capacitor, and Arduino Uno
////////////////////////////////////////////////////////////////////////////////
*/

#include "NanoSatisfi_MLX90614.h"

//constructor
SAT_MLX90614::SAT_MLX90614(){
  //sets common values
  factor = 0.02; // MLX90614 has a resolution of .02
  tempData = 0x0000; //zero out the data
}

//modifies tempData into temperature in Kelvin
void SAT_MLX90614::rawTemperature(unsigned char r)
{
  int data_low, data_high, pec;

  Wire.beginTransmission(NS_MLX90614); //begins transmission with device
  Wire.write(r); //sends register address to read
  Wire.endTransmission(0); //repeated start

  Wire.requestFrom(NS_MLX90614, 3); //request three bytes from device

  while(Wire.available() <3); //wait for three bytes to become available
  data_low = Wire.read(); //read first byte
  data_high = Wire.read(); //read second byte
  pec = Wire.read(); //read checksum 

  Wire.endTransmission(); //terminate transmission

  // This masks off the error bit of the high byte,
  // then moves it left 8 bits and adds the low byte.
  // Taken from bildr forum on MLX90614
  tempData =(double)(((data_high & 0x007F) << 8) + data_low);
  //multiply by resolution and account for error to convert to Kelvin
  tempData = (tempData * factor)-0.01;
}

//returns temperature in degrees Celsius
float SAT_MLX90614::getTemp()
{
  rawTemperature(0x07); //modify tempData
  return (float) tempData - 273.15; //convert from Kelvin to Celsius
}
