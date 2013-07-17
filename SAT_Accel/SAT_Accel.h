#ifndef SAT_ACCEL_H
#define SAT_ACCEL_H

#define CS          10
#define POWER_CTL   (0x2D)     //Power Control Register
#define DATA_FORMAT (0x31)
#define DATAX0      (0x32)     //X-Axis Data 0
#define DATAX1      (0x33)     //X-Axis Data 1
#define DATAY0      (0x34)     //Y-Axis Data 0
#define DATAY1      (0x35)     //Y-Axis Data 1
#define DATAZ0      (0x36)     //Z-Axis Data 0
#define DATAZ1      (0x37)     //Z-Axis Data 1

#include <Arduino.h>

class SAT_Accel {
public:
  SAT_Accel();
  void populateValues(int &x, int &y, int &z);
private:
  char values[10];
  void writeRegister(char registerAddress, char value);
  void readRegister(char registerAddress, int numBytes, char * values);
};

#endif /* SAT_ACCEL_H */
