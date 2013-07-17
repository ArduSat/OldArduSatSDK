//==================================================
// I2C_Conv.h
//
// break up and reconstitute vars into Bytes
// for sending over I2C
//
// writen by pplatzer & jcappaert 2012
// (c) nanosatisfi 2012
//
// v1.0 August 2012
//==================================================

#include <Arduino.h>
#include "I2C_Conv.h"

//--------------------------------------------------
// Split float into  byte array with 4 elements
//--------------------------------------------------
int Float2Byte(float usrData, byte usrBytes[])
{
  //usrBytes is passed by reference, as all arrays are
  //usrData is a container for the results
  //the next line creates an pointer array (as indicated by byte * dataPtr
  //getting the pointers from the reference of usrData,
  byte * dataPtr = (byte *) &usrData;
  unsigned int i;
  for (i = 0; i < sizeof usrData; i++) {
    usrBytes[i] = dataPtr[i];
  }

  return 0; //No error return    
}


//--------------------------------------------------
// Reconstitute a float from an array of 4 bytes
//--------------------------------------------------
int Byte2Float(float& resultValue, byte usrBytes[])
{
  //resultValue is passed by reference
  //the next line creats pointers to the 4 bytes of resultValue
  //the loop then writes the values from usrBytes
  //into the positions of the pointers
  byte * resultPtr = (byte*) &resultValue;
  unsigned int i;
  for (i = 0; i < sizeof resultValue; i++)
        //*p++ is equivalent to *(p++); 
        //it increments p, and then returns 
        //the value which p pointed to before p was incremented.
        *resultPtr++ = usrBytes[i];
  return 0; //No error return
}

//--------------------------------------------------
// Split integer into byte array with 2 elements
//--------------------------------------------------
int Int2Byte(int usrData, byte usrBytes[])
{
  //usrBytes is passed by reference, as all arrays are
  //usrData is a container for the results
  //the next line creates an pointer array (as indicated by byte * dataPtr
  //getting the pointers from the reference of usrData,
  byte * dataPtr = (byte *) &usrData;
  unsigned int i;
  for (i = 0; i < sizeof usrData; i++) {
    usrBytes[i] = dataPtr[i];
  }
  return 0; //No error return    
}

//--------------------------------------------------
// Reconstitute an int from an array of 2 bytes
//--------------------------------------------------
int Byte2Int(int& resultValue, byte usrBytes[])
{
  //resultValue is passed by reference
  //the next line creats pointers to the 4 bytes of resultValue
  //the loop then writes the values from usrBytes
  //into the positions of the pointers
  byte * resultPtr = (byte*) &resultValue;
  unsigned int i;
  for (i = 0; i < sizeof resultValue; i++)
        //*p++ is equivalent to *(p++); 
        //it increments p, and then returns 
        //the value which p pointed to before p was incremented.
        *resultPtr++ = usrBytes[i];
  return 0; //No error return
}


//--------------------------------------------------
// Split a long into byte array with 4 elements
//--------------------------------------------------
int Long2Byte(long usrData, byte usrBytes[])
{
  //usrBytes is passed by reference, as all arrays are
  //usrData is a container for the results
  //the next line creates an pointer array (as indicated by byte * dataPtr
  //getting the pointers from the reference of usrData,
  byte * dataPtr = (byte *) &usrData;
  unsigned int i;
  for (i = 0; i < sizeof usrData; i++) {
    usrBytes[i] = dataPtr[i];
  }
  return 0; //No error return    
}

//--------------------------------------------------
// Reconstitute a long from an array of 2 bytes
//--------------------------------------------------
int Byte2Long(long& resultValue, byte usrBytes[])
{
  //resultValue is passed by reference
  //the next line creats pointers to the 4 bytes of resultValue
  //the loop then writes the values from usrBytes
  //into the positions of the pointers
  byte * resultPtr = (byte*) &resultValue;
  unsigned int i;
  for (i = 0; i < sizeof resultValue; i++)
        //*p++ is equivalent to *(p++); 
        //it increments p, and then returns 
        //the value which p pointed to before p was incremented.
        *resultPtr++ = usrBytes[i];
  return 0; //No error return
}


//-----------------------------------------------------------
// Encode a Fletcher checksum in the last 2 bytes of buffer
//-----------------------------------------------------------

/*void fletcher_encode(byte buffer[], long count )
{
    int i;
    unsigned char c0 = 0;
    unsigned char c1 = 0;
     *( buffer + count - 1 ) = 0;
     *( buffer + count - 2 ) = 0;
    for( i = 0; i < count; i++)
    {
    c0 = c0 + *( buffer + i );
    c1 = c1 + c0;
    }
    *( buffer + count - 2 ) = c0 - c1;
    *( buffer + count - 1 ) = c1 - 2*c0;
 }
 
 //-------------------------------------------------------------
 // Decode Fletcher Checksum. Returns zero if buffer error-free
 //-------------------------------------------------------------
 
long fletcher_decode(byte buffer[], long count )
{
  long result = 0;
  int i;
  unsigned char c0 = 0;
  unsigned char c1 = 0;
  for( i = 0; i < count; i++)
    {
      c0 = c0 + *( buffer + i );
      c1 = c1 + c0;
    }
  return((long)(c0 + c1)); // returns zero if buffer is error-free
}*/

