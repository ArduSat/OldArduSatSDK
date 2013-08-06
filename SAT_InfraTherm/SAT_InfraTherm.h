/* 
    SAT_InfraTherm.h - Library for Arduino returns data on temperature from
    InfraTherm sensor
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

          Tested with InfraTherm from Sparkfun, 4.7k Ohm pull-up resistors, .1uF capacitor, and Arduino Uno
////////////////////////////////////////////////////////////////////////////////
*/

#ifndef SAT_InfraTherm_H
#define SAT_InfraTherm_H

#define NS_InfraTherm 0x5A //I2C address

#include <Arduino.h>
#include <Wire.h>

class SAT_InfraTherm
{
  public:
  //constructor
  SAT_InfraTherm();

  //public methods
  float getTemp();
  int16_t getRawTemp();

  private:

  //class-only methods
  void rawTemperature(unsigned char r);

  //global variable
  double factor;
  double tempData;
  int16_t rawData;
};

#endif /* SAT_InfraTherm_H */
