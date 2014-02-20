
/**************************************************************************/
/*! 
    @file     tsl2561.c
    @author   K. Townsend (microBuilder.eu / adafruit.com)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2010, microBuilder SARL, Adafruit Industries
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#include <avr/pgmspace.h>
#include <stdlib.h>
#include <Wire.h>
#include "SAT_Lum.h"
#include <I2C_add.h>
#include <OnboardCommLayer.h>	// for OBCL

SAT_Lum::SAT_Lum(uint8_t id) {
//  _addr = id; // jfomhover on 07/08/2013 : commented cause not used
  _addr = I2C_ADD_LUX1;
  switch(id){
    case 1:
      _addr=I2C_ADD_LUX1;
      break;
    case 2:
      _addr=I2C_ADD_LUX2;
      break;
  }

  _initialized = false;
  _integration = SAT_Lum_INTEGRATIONTIME_13MS;
  _gain = SAT_Lum_GAIN_16X;
}

boolean SAT_Lum::begin() { // jfomhover on 07/08/2013 : argument (uint8_t nodeid) was not used in here..
/*  Wire.begin(); */ // jfomhover on 07/08/2013 : shouldn't be made, cause already begun in OnboardCommLayer.cpp
	OBCL.begin();

	uint8_t retByte = 0;

	int8_t t_ret = OBCL.exchangeByte(_addr, SAT_Lum_REGISTER_ID, &retByte, I2C_COMM_INSTANTTIMEOUT);

	if ((retByte ^ 0x0A) != 0) {
		// sensor not found
		_initialized = false;
		return(false);
	}
	_initialized = true;

	// Set default integration time and gain
	setTiming(_integration);
	setGain(_gain);
	// Note: by default, the device is in power down mode on bootup
	disable();

	return true;
}

void SAT_Lum::enable(void)
{
  if (!_initialized) begin();

  // Enable the device by setting the control bit to 0x03
  int8_t t_ret = write8(SAT_Lum_COMMAND_BIT | SAT_Lum_REGISTER_CONTROL, SAT_Lum_CONTROL_POWERON);
  // jfomhover on 07/08/2013 : what should we do here if there's an error ?
  // in particular, if t_ret == I2C_COMM_ERRORNACKADDR that means the sensor's not connected
}

void SAT_Lum::disable(void)
{
  if (!_initialized) begin();

  // Disable the device by setting the control bit to 0x03
  int8_t t_ret = write8(SAT_Lum_COMMAND_BIT | SAT_Lum_REGISTER_CONTROL, SAT_Lum_CONTROL_POWEROFF);
  // jfomhover on 07/08/2013 : what should we do here if there's an error ?
  // in particular, if t_ret == I2C_COMM_ERRORNACKADDR that means the sensor's not connected
}

void SAT_Lum::setGain(tsl2561Gain_t gain) {
  if (!_initialized) begin();

  enable();
  _gain = gain;
  int8_t t_ret = write8(SAT_Lum_COMMAND_BIT | SAT_Lum_REGISTER_TIMING, _integration | _gain);
  // jfomhover on 07/08/2013 : what should we do here if there's an error ?
  // in particular, if t_ret == I2C_COMM_ERRORNACKADDR that means the sensor's not connected
  disable();
}

void SAT_Lum::setTiming(tsl2561IntegrationTime_t integration)
{
  if (!_initialized) begin();

  enable();
  _integration = integration;
  int8_t t_ret = write8(SAT_Lum_COMMAND_BIT | SAT_Lum_REGISTER_TIMING, _integration | _gain);
  // jfomhover on 07/08/2013 : what should we do here if there's an error ?
  // in particular, if t_ret == I2C_COMM_ERRORNACKADDR that means the sensor's not connected
  disable();
}

uint32_t SAT_Lum::calculateLux(uint16_t ch0, uint16_t ch1)
{
  unsigned long chScale;
  unsigned long channel1;
  unsigned long channel0;

  switch (_integration)
  {
    case SAT_Lum_INTEGRATIONTIME_13MS:
      chScale = SAT_Lum_LUX_CHSCALE_TINT0;
      break;
    case SAT_Lum_INTEGRATIONTIME_101MS:
      chScale = SAT_Lum_LUX_CHSCALE_TINT1;
      break;
    default: // No scaling ... integration time = 402ms
      chScale = (1 << SAT_Lum_LUX_CHSCALE);
      break;
  }

  // Scale for gain (1x or 16x)
  if (!_gain) chScale = chScale << 4;

  // scale the channel values
  channel0 = (ch0 * chScale) >> SAT_Lum_LUX_CHSCALE;
  channel1 = (ch1 * chScale) >> SAT_Lum_LUX_CHSCALE;

  // find the ratio of the channel values (Channel1/Channel0)
  unsigned long ratio1 = 0;
  if (channel0 != 0) ratio1 = (channel1 << (SAT_Lum_LUX_RATIOSCALE+1)) / channel0;

  // round the ratio value
  unsigned long ratio = (ratio1 + 1) >> 1;

  unsigned int b, m;

#ifdef SAT_Lum_PACKAGE_CS
  if ((ratio >= 0) && (ratio <= SAT_Lum_LUX_K1C))
    {b=SAT_Lum_LUX_B1C; m=SAT_Lum_LUX_M1C;}
  else if (ratio <= SAT_Lum_LUX_K2C)
    {b=SAT_Lum_LUX_B2C; m=SAT_Lum_LUX_M2C;}
  else if (ratio <= SAT_Lum_LUX_K3C)
    {b=SAT_Lum_LUX_B3C; m=SAT_Lum_LUX_M3C;}
  else if (ratio <= SAT_Lum_LUX_K4C)
    {b=SAT_Lum_LUX_B4C; m=SAT_Lum_LUX_M4C;}
  else if (ratio <= SAT_Lum_LUX_K5C)
    {b=SAT_Lum_LUX_B5C; m=SAT_Lum_LUX_M5C;}
  else if (ratio <= SAT_Lum_LUX_K6C)
    {b=SAT_Lum_LUX_B6C; m=SAT_Lum_LUX_M6C;}
  else if (ratio <= SAT_Lum_LUX_K7C)
    {b=SAT_Lum_LUX_B7C; m=SAT_Lum_LUX_M7C;}
  else if (ratio > SAT_Lum_LUX_K8C)
    {b=SAT_Lum_LUX_B8C; m=SAT_Lum_LUX_M8C;}
#else
  if ((ratio >= 0) && (ratio <= SAT_Lum_LUX_K1T))
    {b=SAT_Lum_LUX_B1T; m=SAT_Lum_LUX_M1T;}
  else if (ratio <= SAT_Lum_LUX_K2T)
    {b=SAT_Lum_LUX_B2T; m=SAT_Lum_LUX_M2T;}
  else if (ratio <= SAT_Lum_LUX_K3T)
    {b=SAT_Lum_LUX_B3T; m=SAT_Lum_LUX_M3T;}
  else if (ratio <= SAT_Lum_LUX_K4T)
    {b=SAT_Lum_LUX_B4T; m=SAT_Lum_LUX_M4T;}
  else if (ratio <= SAT_Lum_LUX_K5T)
    {b=SAT_Lum_LUX_B5T; m=SAT_Lum_LUX_M5T;}
  else if (ratio <= SAT_Lum_LUX_K6T)
    {b=SAT_Lum_LUX_B6T; m=SAT_Lum_LUX_M6T;}
  else if (ratio <= SAT_Lum_LUX_K7T)
    {b=SAT_Lum_LUX_B7T; m=SAT_Lum_LUX_M7T;}
  else if (ratio > SAT_Lum_LUX_K8T)
    {b=SAT_Lum_LUX_B8T; m=SAT_Lum_LUX_M8T;}
#endif

  unsigned long temp;
  temp = ((channel0 * b) - (channel1 * m));

  // do not allow negative lux value
  if (temp < 0) temp = 0;

  // round lsb (2^(LUX_SCALE-1))
  temp += (1 << (SAT_Lum_LUX_LUXSCALE-1));

  // strip off fractional portion
  uint32_t lux = temp >> SAT_Lum_LUX_LUXSCALE;

  // Signal I2C had no errors
  return lux;
}

uint32_t SAT_Lum::getFullLuminosity (void)
{
  if (!_initialized) begin();

  // Enable the device by setting the control bit to 0x03
  enable();

  // Wait x ms for ADC to complete
  switch (_integration)
  {
    case SAT_Lum_INTEGRATIONTIME_13MS:
      delay(14);
      break;
    case SAT_Lum_INTEGRATIONTIME_101MS:
      delay(102);
      break;
    default:
      delay(400);
      break;
  }

  uint32_t x;
  x = read16(SAT_Lum_COMMAND_BIT | SAT_Lum_WORD_BIT | SAT_Lum_REGISTER_CHAN1_LOW);
  x <<= 16;
  x |= read16(SAT_Lum_COMMAND_BIT | SAT_Lum_WORD_BIT | SAT_Lum_REGISTER_CHAN0_LOW);

  disable();

  return x;
}
uint16_t SAT_Lum::getLuminosity (uint8_t channel) {

  uint32_t x = getFullLuminosity();

  if (channel == 0) {
    // Reads two byte value from channel 0 (visible + infrared)
    return (x & 0xFFFF);
  } else if (channel == 1) {
    // Reads two byte value from channel 1 (infrared)
    return (x >> 16);
  } else if (channel == 2) {
    // Reads all and subtracts out just the visible!
    return ( (x & 0xFFFF) - (x >> 16));
  }

  // unknown channel!
  return 0;
}

uint16_t SAT_Lum::read16(uint8_t reg)
{
	uint16_t out = 0;	// output value of the function

	int8_t t_ret = OBCL.request16bitsFromREG(_addr, reg, &out, false, I2C_COMM_INSTANTTIMEOUT);
	return(out);

/* KEPT FOR MEMORY
  uint16_t x; uint16_t t;

  Wire.beginTransmission(_addr);
#if ARDUINO >= 100
  Wire.write(reg);
#else
  Wire.send(reg);
#endif
  Wire.endTransmission();

  Wire.requestFrom(_addr, 2);
#if ARDUINO >= 100
  t = Wire.read();
  x = Wire.read();
#else
  t = Wire.receive();
  x = Wire.receive();
#endif
  x <<= 8;
  x |= t;
  return x;
*/
}

int8_t SAT_Lum::write8 (uint8_t reg, uint8_t value)
{
	_buff[0] = reg;
	_buff[1] = value;
	int8_t t_ret = OBCL.transmitByteArray(_addr, _buff, 2, I2C_COMM_INSTANTTIMEOUT);
	return(t_ret);
}
