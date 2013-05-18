/**
	@brief  I2C_Conv.h
    
	break up and reconstitute Float into Bytes
	for sending over I2C

   Copyright 2013 NanoSatisfi, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
    
	@author Peter Platzer
	@author Jeroen Cappaert
    @date August 2012
  
*/


#ifndef I2C_Conv_h
#define I2C_Conv_h

#include <Arduino.h>

/* conversion for floats (uint32_t) */
int Float2Byte(float usrData, byte usrBytes[]);
int Byte2Float(float &resultValue, byte usrBytes[]);

/* conversion for ints (uint_16_t) */
int Int2Byte(int usrData, byte usrBytes[]);
int Byte2Int(int &resultValue, byte usrBytes[]);

/* conversion for longs (uint32_t) */
int Long2Byte(long usrData, byte usrBytes[]);
int Byte2Long(long &resultValue, byte usrBytes[]);


  
  
#endif

