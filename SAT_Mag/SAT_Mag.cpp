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
#include <OnboardCommLayer.h>	// for OBCL

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

// jfomhover on 07/08/2013 : function not used ? furthermore, the "node_id" should be given by a "central power" (OBCL)
/* void SAT_Mag::init(uint8_t node_id){
    _local_address = node_id;
    this->configMag();
} */

// Configure magnetometer
// call in setup()
void SAT_Mag::configMag() {
	byte commands[2];
	int8_t t_ret = 0;

	OBCL.begin();

	commands[0] = 0x11;  // cntrl register2
	commands[1] = 0x80;  // send 0x80, enable auto resets
	t_ret = OBCL.transmitByteArray(I2C_ADD_MAG, commands, 2, I2C_COMM_INSTANTTIMEOUT);

	delay(15);

	commands[0] = 0x10;  // cntrl register1
	commands[1] = 0x01;  // send 0x01, active mode
	t_ret = OBCL.transmitByteArray(I2C_ADD_MAG, commands, 2, I2C_COMM_INSTANTTIMEOUT);

	// jfomhover on 07/08/2013 : what should we do here if there's an error ?
	// in particular, I2C_COMM_ERRORNACKADDR that means the sensor's not connected
}

// read X value
int SAT_Mag::readx()
{
	int16_t t_val = 0;
	int8_t t_ret = OBCL.request16bitsFromMSBLSB(I2C_ADD_MAG, 0x01, 0x02, (uint16_t*)&t_val);
	// TODO : what should we do here if there's an error ?
	return t_val;
}

//read Y value
int SAT_Mag::ready()
{
	int16_t t_val = 0;
	int8_t t_ret = OBCL.request16bitsFromMSBLSB(I2C_ADD_MAG, 0x03, 0x04, (uint16_t*)&t_val);
	// TODO : what should we do here if there's an error ?
	return t_val;
}

// read Z value
int SAT_Mag::readz()
{
	int16_t t_val = 0;
	int8_t t_ret = OBCL.request16bitsFromMSBLSB(I2C_ADD_MAG, 0x05, 0x06, (uint16_t*)&t_val);
	// TODO : what should we do here if there's an error ?
	return t_val;
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
