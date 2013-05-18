/*
@brief ArduSat I2C address list
Defines all the addresses used for I2C devices on
the ArduSat Payload

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
@author Jeroen Cappaert
@date May 2013
*/

#ifndef I2C_add_H_
#define I2C_add_H_

// sensor addresses
#define I2C_ADD_MAG 0X0E // magnetometer
#define I2C_ADD_LUX1 0X29 // TSL2561 #1 (bottomplate camera)
#define I2C_ADD_LUX2 0x39 // TSL2561 #2 (bottomplate slit)
#define I2C_ADD_TMP1 0X48 // temp sensor TMP102 (payload #1)
#define I2C_ADD_TMP2 0X49 // temp sensor TMP102 (payload #2)
#define I2C_ADD_TMP3 0X4A // temp sensor TMP102 (bottomplate #1)
#define I2C_ADD_TMP4 0X4B // temp sensor TMP102 (bottomplate #2)
#define I2C_ADD_ACC 0X53 // accelerometer ADXL345
#define I2C_ADD_GYR 0X69 // gyros ITG3200
#define I2C_ADD_MLX 0X51 // IR thermometer (bottomplate)

// experiment arduino addresses
#define I2C_ADD_ARD1 0X2B // experiment arduino 1
#define I2C_ADD_ARD2 0X2C // experiment arduino 2
#define I2C_ADD_ARD3 0X2D // experiment arduino 3
#define I2C_ADD_ARD4 0X2E // experiment arduino 4
#define I2C_ADD_ARD5 0X2F // experiment arduino 5
#define I2C_ADD_ARD6 0X30 // experiment arduino 6
#define I2C_ADD_ARD7 0X31 // experiment arduino 7
#define I2C_ADD_ARD8 0X32 // experiment arduino 8
#define I2C_ADD_ARD9 0X33 // experiment arduino 9
#define I2C_ADD_ARD10 0X34 // experiment arduino 10
#define I2C_ADD_GPS 0X3C // GPS arduino (node 11)
#define I2C_ADD_ARD12 0X3D // experiment arduino 12
#define I2C_ADD_ARD13 0X3E // experiment arduino 13
#define I2C_ADD_ARD14 0X3F // experiment arduino 14
#define I2C_ADD_ARD15 0X40 // experiment arduino 15
#define I2C_ADD_ARD16 0X41 // experiment arduino 16

// peripheral arduino adresses
#define I2C_ADD_ASSV 0X04 // master arduino
#define I2C_ADD_SPECT 0X22 // spectruino arduino
#define I2C_ADD_GEIGER 0X23 // uSD card arduino

#endif