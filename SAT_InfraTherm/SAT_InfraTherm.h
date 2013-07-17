/* 
    SAT_MLX90614.h - Library for Arduino returns data on temperature from
    MLX90614 sensor
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

          Tested with MLX90614 from Sparkfun, 4.7k Ohm pull-up resistors, .1uF capacitor, and Arduino Uno
////////////////////////////////////////////////////////////////////////////////
*/

#ifndef SAT_MLX90614_H
#define SAT_MLX90614_H

#define NS_MLX90614 0x5A //I2C address

#include <Arduino.h>
#include <Wire.h>

class SAT_MLX90614
{
  public:
  //constructor
  SAT_MLX90614();

  //public methods
  float getTemp();

  private:

  //class-only methods
  void rawTemperature(unsigned char r);

  //global variable
  double factor;
  double tempData;
};

#endif /* SAT_MLX90614_H */
