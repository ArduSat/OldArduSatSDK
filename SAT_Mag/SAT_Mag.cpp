/* 
    SAT_Mag.cpp - Library for Arduino returns x,y,z data on current magnetic field
    Copyright (C) 2012  Jeroen Cappaert & Lara Booth for NanoSatisfi

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

Designed for use with Freescale (C) MAG3110 Magnetometer

Tested with MAG3110 Breakout from Sparkfun and Arduino Uno

Based on Sparkfun's example for the MAG3110 breakout board:
http://dlnmh9ip6v2uc.cloudfront.net/datasheets/BreakoutBoards/Mag3110_v10.pde

////////////////////////////////////////////////////////////////////////////////
*/

#include "SAT_Mag.h"
#include <I2C_add.h>
#include <Wire.h>

//Constructor 
SAT_Mag::SAT_Mag()
{
    /* 
       The magnetometer has to be calibrated before use
       use Arduino program "Offsets.ino" to find a value for:
       min_x, max_x, min_y, max_y, min_z, max_z
    */

    mag_x_scale= 1.0/(-902 +203); //offset scale factor: 1.0/(max_x - min_x)
    mag_y_scale= 1.0/(365 + 337);    //offset scale factor: 1.0/(max_y - min_y)
    mag_z_scale= 1.0/(2872 - 2722);  //offset scale factor: 1.0/(max_z - min_z)
}

void SAT_Mag::init(uint8_t node_id){
    _local_address = node_id;
    this->configMag();
}

// Configure magnetometer
// call in setup()
void SAT_Mag::configMag() {
  Wire.beginTransmission(I2C_ADD_MAG);  // transmit to device 0x0E
  Wire.write(0x11);                     // cntrl register2
  Wire.write(0x80);                     // send 0x80, enable auto resets
  Wire.endTransmission();               // stop transmitting

  delay(15);

  Wire.beginTransmission(I2C_ADD_MAG);  // transmit to device 0x0E
  Wire.write(0x10);                     // cntrl register1
  Wire.write(1);                        // send 0x01, active mode
  Wire.endTransmission();               // stop transmitting
}

// read X value
int SAT_Mag::readx()
{
  int xout = read16Data(0x01, 0x02); //returns MSB and LSB from 0x01 and 0x02
  return xout;
}

//read Y value
int SAT_Mag::ready()
{
  int yout = read16Data(0x03, 0x04); //returns MSB and LSB from 0x03 and 0x04
  return yout;
}

// read Z value
int SAT_Mag::readz()
{
  int zout = read16Data(0x05, 0x06); //returns MSB and LSB from 0x05 and 0x06
  return zout;
}

//reads two bytes of data
int SAT_Mag::read16Data(char msg_reg, char lsb_reg)
{
  int xl, xh;  //define the MSB and LSB

  Wire.beginTransmission(I2C_ADD_MAG); // transmit to device 0x0E
  Wire.write(msg_reg);              // x MSB reg
  Wire.endTransmission();       // stop transmitting

  delayMicroseconds(2); //needs at least 1.3us free time between start and stop

  Wire.requestFrom(I2C_ADD_MAG, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  {
    xh = Wire.read(); // read the byte
  }

  delayMicroseconds(2); //needs at least 1.3us free time between start and stop

  Wire.beginTransmission(I2C_ADD_MAG); // transmit to device 0x0E
  Wire.write(lsb_reg);              // x LSB reg
  Wire.endTransmission();       // stop transmitting

  delayMicroseconds(2); //needs at least 1.3us free time between start and stop

  Wire.requestFrom(I2C_ADD_MAG, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  {
    xl = Wire.read(); // read the byte
  }

  int xout = (xl|(xh << 8)); //concatenate the MSB and LSB
  return xout;
}

/*
   the following methods: x_value(), y_value(), and z_value() 
   require offset values to be entered before use
   see constructor
*/

//return scaled x-value
float SAT_Mag::x_value()
{
  return (readx()*mag_x_scale);
}

//return scaled y-value
float SAT_Mag::y_value()
{
  return (ready()*mag_y_scale);
}

//return scaled z-value
float SAT_Mag::z_value()
{
  return (readz()*mag_z_scale);
}

/*
  getHeading(...) returns heading
  heading: difference in degrees between current angle and true North
  ------------------------------------------------------------------
  use calibrated x_value(), y_value(), z_value() as parameters x,y,z
  ------------------------------------------------------------------
  method from Sparkfun Forum on MAG3110 magnetometer
*/

int SAT_Mag::getHeading(float x, float y, float z) {
  float heading=0;

  //defines value of heading for various x,y,z values
  if ((x == 0)&&(y < 0))
    heading= PI/2.0; 

  if ((x == 0)&&(y > 0))
    heading=3.0*PI/2.0;

  if (x < 0)  
    heading = PI - atan(y/x);

  if ((x > 0)&&(y < 0))
    heading = -atan(y/x);

  if ((x > 0)&&(y > 0))
    heading = 2.0*PI - atan(y/x);
  //convert heading from radians to degrees and return
  return  int(degrees(heading));
}
