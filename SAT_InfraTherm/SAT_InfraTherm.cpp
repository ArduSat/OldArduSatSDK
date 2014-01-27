/* 
    SAT_InfraTherm.cpp - Library for Arduino returns data on temperature 
    from InfraTherm sensor
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
          Designed for use with Melexis (C) InfraTherm Infrared Thermometer

          Library gathers temperature data over I2C Bus/SM Bus

          Tested with InfraTherm from Sparkfun, 4.7k Ohm pull-up resistors,
          .1uF capacitor, and Arduino Uno
////////////////////////////////////////////////////////////////////////////////
*/

#include "SAT_InfraTherm.h"
#include <OnboardCommLayer.h>	// for OBCL

//constructor
SAT_InfraTherm::SAT_InfraTherm(){
  //sets common values
  factor = 0.02; // InfraTherm has a resolution of .02
  tempData = 0x0000; //zero out the data
}

//modifies tempData into temperature in Kelvin
void SAT_InfraTherm::rawTemperature(unsigned char r)
{
  int8_t data_t[3];
//  int data_low, data_high, pec;

  OBCL.transmitByte(NS_InfraTherm,r,0);	//repeated start

  uint8_t recdSize = 0;
  int8_t t_ret = OBCL.requestByteArray(NS_InfraTherm, (byte*)data_t, 3, &recdSize);

// TODO : what to do here if errors ?
  if (t_ret < 0)
	  return;

  // jfomhover 08/09/2013 : below, I didn't want to ruin the clarity of the formula, but these variables can be skipped
  int8_t data_low =  data_t[0]; //read first byte
  int8_t data_high = data_t[1]; //read second byte
  int8_t pec = 		 data_t[2]; //read checksum

  // This masks off the error bit of the high byte,
  // then moves it left 8 bits and adds the low byte.
  // Taken from bildr forum on InfraTherm
  rawData = (((data_high & 0x007F) << 8) + data_low);
  tempData =(double)(rawData);
  //multiply by resolution and account for error to convert to Kelvin
  tempData = (tempData * factor)-0.01;
}

//returns temperature in degrees Celsius
float SAT_InfraTherm::getTemp()
{
  rawTemperature(0x07); //modify tempData
  return (float) tempData - 273.15; //convert from Kelvin to Celsius
}

int16_t SAT_InfraTherm::getRawTemp()
{
  rawTemperature(0x07); //modify tempData+rawData
  return(rawData);
}
