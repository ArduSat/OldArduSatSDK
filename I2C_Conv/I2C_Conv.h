//==================================================
// I2C_Conv.h
//
// break up and reconstitute Float into Bytes
// for sending over I2C
//
// writen by pplatzer & jcappaert 2012
// (c) nanosatisfi 2012
//
// v1.0 August 2012
//==================================================

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

/* Fletcher's checksum encode/decode */
//void fletcher_encode(byte buffer[], long count );
//long fletcher_decode(byte buffer[], long count );

#endif

