/**************************************************************************
 *                                                                         *
 * ADXL345 Driver for Arduino                                              *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU License.                                  *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU License V2 for more details.                                        *
 *                                                                         *
 * Adapted from http://code.google.com/p/adxl345driver for Arduino 1.0 by  *
 * by Jens C Brynildsen <http://www.flashgamer.com>                        *
 * Modified by J.F. Omhover Aug. 2013 for ArduSatSDK                       *
 ***************************************************************************/

//#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
//#else
 //#include "WProgram.h"
//#endif

#include "SAT_Accel.h"	// jfomhover 08/09/2013 : modified for ArduSat
#include <OnboardCommLayer.h>	// for OBCL

#define DEVICE (0x53)    // ADXL345 device address
#define TO_READ (6)      // num of bytes we are going to read each time (two bytes for each axis)

SAT_Accel::SAT_Accel() {
  status = ADXL345_OK;
  error_code = ADXL345_NO_ERROR;

  gains[0] = 0.00376390;
  gains[1] = 0.00376009;
  gains[2] = 0.00349265;
}

void SAT_Accel::powerOn() {
//  Wire.begin();        // jfomhover 08/09/2013 : modified for ArduSat
	OBCL.begin();
//Turning on the ADXL345
	writeTo(ADXL345_POWER_CTL, 0);
	writeTo(ADXL345_POWER_CTL, 16);
	writeTo(ADXL345_POWER_CTL, 8);
}

// Reads the acceleration into three variable x, y and z
void SAT_Accel::readAccel(int16_t *xyz){	// jfomhover 08/09/2013 : specified type
  readAccel(xyz, xyz + 1, xyz + 2);
}
void SAT_Accel::readAccel(int16_t *x, int16_t *y, int16_t *z) {	// jfomhover 08/09/2013 : specified type
  readFrom(ADXL345_DATAX0, TO_READ, _buff); //read the acceleration data from the ADXL345

  // each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
  // thus we are converting both bytes in to one int
  *x = (((int)_buff[1]) << 8) | _buff[0];
  *y = (((int)_buff[3]) << 8) | _buff[2];
  *z = (((int)_buff[5]) << 8) | _buff[4];
}

void SAT_Accel::get_Gxyz(double *xyz){
  int i;
  int16_t xyz_int[3];
  readAccel(xyz_int);
  for(i=0; i<3; i++){
    xyz[i] = xyz_int[i] * gains[i];
  }
}

// Writes val to address register on device
void SAT_Accel::writeTo(byte address, byte val) {
	_buff[0] = address;
	_buff[1] = val;
	OBCL.transmitByteArray(DEVICE,_buff,2,I2C_COMM_INSTANTTIMEOUT);	// jfomhover 08/09/2013 : modified for ArduSat
//	delayMicroseconds(2);
}

// Reads num bytes starting from address register on device in to _buff array
void SAT_Accel::readFrom(byte address, int num, byte _buff[]) {
	OBCL.transmitByte(DEVICE,address);
	delayMicroseconds(2);
	uint8_t recSize = 0;	// where the length of received data is stored
	int8_t t_ret = OBCL.requestByteArray((uint8_t)DEVICE,(byte*)_buff,(uint8_t)num,&recSize,100);

	if (t_ret == I2C_COMM_OK)
		return;
	if (t_ret < 0) {
	    status = ADXL345_ERROR;
		error_code = ADXL345_READ_ERROR;
		// TODO : maybe distinct different error_code depending on t_ret type ? (switch ?)
	}
}

// Gets the range setting and return it into rangeSetting
// it can be 2, 4, 8 or 16
void SAT_Accel::getRangeSetting(byte* rangeSetting) {
  byte _b;
  readFrom(ADXL345_DATA_FORMAT, 1, &_b);
  *rangeSetting = _b & B00000011;
}

// Sets the range setting, possible values are: 2, 4, 8, 16
void SAT_Accel::setRangeSetting(int val) {
  byte _s;
  byte _b;

  switch (val) {
  case 2:
    _s = B00000000;
    break;
  case 4:
    _s = B00000001;
    break;
  case 8:
    _s = B00000010;
    break;
  case 16:
    _s = B00000011;
    break;
  default:
    _s = B00000000;
  }
  readFrom(ADXL345_DATA_FORMAT, 1, &_b);
  _s |= (_b & B11101100);
  writeTo(ADXL345_DATA_FORMAT, _s);
}
// gets the state of the SELF_TEST bit
bool SAT_Accel::getSelfTestBit() {
  return getRegisterBit(ADXL345_DATA_FORMAT, 7);
}

// Sets the SELF-TEST bit
// if set to 1 it applies a self-test force to the sensor causing a shift in the output data
// if set to 0 it disables the self-test force
void SAT_Accel::setSelfTestBit(bool selfTestBit) {
  setRegisterBit(ADXL345_DATA_FORMAT, 7, selfTestBit);
}

// Gets the state of the SPI bit
bool SAT_Accel::getSpiBit() {
  return getRegisterBit(ADXL345_DATA_FORMAT, 6);
}

// Sets the SPI bit
// if set to 1 it sets the device to 3-wire mode
// if set to 0 it sets the device to 4-wire SPI mode
void SAT_Accel::setSpiBit(bool spiBit) {
  setRegisterBit(ADXL345_DATA_FORMAT, 6, spiBit);
}

// Gets the state of the INT_INVERT bit
bool SAT_Accel::getInterruptLevelBit() {
  return getRegisterBit(ADXL345_DATA_FORMAT, 5);
}

// Sets the INT_INVERT bit
// if set to 0 sets the interrupts to active high
// if set to 1 sets the interrupts to active low
void SAT_Accel::setInterruptLevelBit(bool interruptLevelBit) {
  setRegisterBit(ADXL345_DATA_FORMAT, 5, interruptLevelBit);
}

// Gets the state of the FULL_RES bit
bool SAT_Accel::getFullResBit() {
  return getRegisterBit(ADXL345_DATA_FORMAT, 3);
}

// Sets the FULL_RES bit
// if set to 1, the device is in full resolution mode, where the output resolution increases with the
//   g range set by the range bits to maintain a 4mg/LSB scal factor
// if set to 0, the device is in 10-bit mode, and the range buts determine the maximum g range
//   and scale factor
void SAT_Accel::setFullResBit(bool fullResBit) {
  setRegisterBit(ADXL345_DATA_FORMAT, 3, fullResBit);
}

// Gets the state of the justify bit
bool SAT_Accel::getJustifyBit() {
  return getRegisterBit(ADXL345_DATA_FORMAT, 2);
}

// Sets the JUSTIFY bit
// if sets to 1 selects the left justified mode
// if sets to 0 selects right justified mode with sign extension
void SAT_Accel::setJustifyBit(bool justifyBit) {
  setRegisterBit(ADXL345_DATA_FORMAT, 2, justifyBit);
}

// Sets the THRESH_TAP byte value
// it should be between 0 and 255
// the scale factor is 62.5 mg/LSB
// A value of 0 may result in undesirable behavior
void SAT_Accel::setTapThreshold(int tapThreshold) {
  tapThreshold = min(max(tapThreshold,0),255);
  byte _b = byte (tapThreshold);
  writeTo(ADXL345_THRESH_TAP, _b);
}

// Gets the THRESH_TAP byte value
// return value is comprised between 0 and 255
// the scale factor is 62.5 mg/LSB
int SAT_Accel::getTapThreshold() {
  byte _b;
  readFrom(ADXL345_THRESH_TAP, 1, &_b);
  return int (_b);
}

// set/get the gain for each axis in Gs / count
void SAT_Accel::setAxisGains(double *_gains){
  int i;
  for(i = 0; i < 3; i++){
    gains[i] = _gains[i];
  }
}
void SAT_Accel::getAxisGains(double *_gains){
  int i;
  for(i = 0; i < 3; i++){
    _gains[i] = gains[i];
  }
}


// Sets the OFSX, OFSY and OFSZ bytes
// OFSX, OFSY and OFSZ are user offset adjustments in twos complement format with
// a scale factor of 15,6mg/LSB
// OFSX, OFSY and OFSZ should be comprised between
void SAT_Accel::setAxisOffset(int x, int y, int z) {
  writeTo(ADXL345_OFSX, byte (x));
  writeTo(ADXL345_OFSY, byte (y));
  writeTo(ADXL345_OFSZ, byte (z));
}

// Gets the OFSX, OFSY and OFSZ bytes
void SAT_Accel::getAxisOffset(int* x, int* y, int*z) {
  byte _b;
  readFrom(ADXL345_OFSX, 1, &_b);
  *x = int (_b);
  readFrom(ADXL345_OFSY, 1, &_b);
  *y = int (_b);
  readFrom(ADXL345_OFSZ, 1, &_b);
  *z = int (_b);
}

// Sets the DUR byte
// The DUR byte contains an unsigned time value representing the maximum time
// that an event must be above THRESH_TAP threshold to qualify as a tap event
// The scale factor is 625Âµs/LSB
// A value of 0 disables the tap/double tap funcitons. Max value is 255.
void SAT_Accel::setTapDuration(int tapDuration) {
  tapDuration = min(max(tapDuration,0),255);
  byte _b = byte (tapDuration);
  writeTo(ADXL345_DUR, _b);
}

// Gets the DUR byte
int SAT_Accel::getTapDuration() {
  byte _b;
  readFrom(ADXL345_DUR, 1, &_b);
  return int (_b);
}

// Sets the latency (latent register) which contains an unsigned time value
// representing the wait time from the detection of a tap event to the start
// of the time window, during which a possible second tap can be detected.
// The scale factor is 1.25ms/LSB. A value of 0 disables the double tap function.
// It accepts a maximum value of 255.
void SAT_Accel::setDoubleTapLatency(int doubleTapLatency) {
  byte _b = byte (doubleTapLatency);
  writeTo(ADXL345_LATENT, _b);
}

// Gets the Latent value
int SAT_Accel::getDoubleTapLatency() {
  byte _b;
  readFrom(ADXL345_LATENT, 1, &_b);
  return int (_b);
}

// Sets the Window register, which contains an unsigned time value representing
// the amount of time after the expiration of the latency time (Latent register)
// during which a second valud tap can begin. The scale factor is 1.25ms/LSB. A
// value of 0 disables the double tap function. The maximum value is 255.
void SAT_Accel::setDoubleTapWindow(int doubleTapWindow) {
  doubleTapWindow = min(max(doubleTapWindow,0),255);
  byte _b = byte (doubleTapWindow);
  writeTo(ADXL345_WINDOW, _b);
}

// Gets the Window register
int SAT_Accel::getDoubleTapWindow() {
  byte _b;
  readFrom(ADXL345_WINDOW, 1, &_b);
  return int (_b);
}

// Sets the THRESH_ACT byte which holds the threshold value for detecting activity.
// The data format is unsigned, so the magnitude of the activity event is compared
// with the value is compared with the value in the THRESH_ACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the
// activity interrupt is enabled. The maximum value is 255.
void SAT_Accel::setActivityThreshold(int activityThreshold) {
  activityThreshold = min(max(activityThreshold,0),255);
  byte _b = byte (activityThreshold);
  writeTo(ADXL345_THRESH_ACT, _b);
}

// Gets the THRESH_ACT byte
int SAT_Accel::getActivityThreshold() {
  byte _b;
  readFrom(ADXL345_THRESH_ACT, 1, &_b);
  return int (_b);
}

// Sets the THRESH_INACT byte which holds the threshold value for detecting inactivity.
// The data format is unsigned, so the magnitude of the inactivity event is compared
// with the value is compared with the value in the THRESH_INACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the
// inactivity interrupt is enabled. The maximum value is 255.
void SAT_Accel::setInactivityThreshold(int inactivityThreshold) {
  inactivityThreshold = min(max(inactivityThreshold,0),255);
  byte _b = byte (inactivityThreshold);
  writeTo(ADXL345_THRESH_INACT, _b);
}

// Gets the THRESH_INACT byte
int SAT_Accel::getInactivityThreshold() {
  byte _b;
  readFrom(ADXL345_THRESH_INACT, 1, &_b);
  return int (_b);
}

// Sets the TIME_INACT register, which contains an unsigned time value representing the
// amount of time that acceleration must be less thant the value in the THRESH_INACT
// register for inactivity to be declared. The scale factor is 1sec/LSB. The value must
// be between 0 and 255.
void SAT_Accel::setTimeInactivity(int timeInactivity) {
  timeInactivity = min(max(timeInactivity,0),255);
  byte _b = byte (timeInactivity);
  writeTo(ADXL345_TIME_INACT, _b);
}

// Gets the TIME_INACT register
int SAT_Accel::getTimeInactivity() {
  byte _b;
  readFrom(ADXL345_TIME_INACT, 1, &_b);
  return int (_b);
}

// Sets the THRESH_FF register which holds the threshold value, in an unsigned format, for
// free-fall detection. The root-sum-square (RSS) value of all axes is calculated and
// compared whith the value in THRESH_FF to determine if a free-fall event occured. The
// scale factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the free-fall
// interrupt is enabled. The maximum value is 255.
void SAT_Accel::setFreeFallThreshold(int freeFallThreshold) {
  freeFallThreshold = min(max(freeFallThreshold,0),255);
  byte _b = byte (freeFallThreshold);
  writeTo(ADXL345_THRESH_FF, _b);
}

// Gets the THRESH_FF register.
int SAT_Accel::getFreeFallThreshold() {
  byte _b;
  readFrom(ADXL345_THRESH_FF, 1, &_b);
  return int (_b);
}

// Sets the TIME_FF register, which holds an unsigned time value representing the minimum
// time that the RSS value of all axes must be less than THRESH_FF to generate a free-fall
// interrupt. The scale factor is 5ms/LSB. A value of 0 may result in undesirable behavior if
// the free-fall interrupt is enabled. The maximum value is 255.
void SAT_Accel::setFreeFallDuration(int freeFallDuration) {
  freeFallDuration = min(max(freeFallDuration,0),255);
  byte _b = byte (freeFallDuration);
  writeTo(ADXL345_TIME_FF, _b);
}

// Gets the TIME_FF register.
int SAT_Accel::getFreeFallDuration() {
  byte _b;
  readFrom(ADXL345_TIME_FF, 1, &_b);
  return int (_b);
}

bool SAT_Accel::isActivityXEnabled() {
  return getRegisterBit(ADXL345_ACT_INACT_CTL, 6);
}
bool SAT_Accel::isActivityYEnabled() {
  return getRegisterBit(ADXL345_ACT_INACT_CTL, 5);
}
bool SAT_Accel::isActivityZEnabled() {
  return getRegisterBit(ADXL345_ACT_INACT_CTL, 4);
}
bool SAT_Accel::isInactivityXEnabled() {
  return getRegisterBit(ADXL345_ACT_INACT_CTL, 2);
}
bool SAT_Accel::isInactivityYEnabled() {
  return getRegisterBit(ADXL345_ACT_INACT_CTL, 1);
}
bool SAT_Accel::isInactivityZEnabled() {
  return getRegisterBit(ADXL345_ACT_INACT_CTL, 0);
}

void SAT_Accel::setActivityX(bool state) {
  setRegisterBit(ADXL345_ACT_INACT_CTL, 6, state);
}
void SAT_Accel::setActivityY(bool state) {
  setRegisterBit(ADXL345_ACT_INACT_CTL, 5, state);
}
void SAT_Accel::setActivityZ(bool state) {
  setRegisterBit(ADXL345_ACT_INACT_CTL, 4, state);
}
void SAT_Accel::setInactivityX(bool state) {
  setRegisterBit(ADXL345_ACT_INACT_CTL, 2, state);
}
void SAT_Accel::setInactivityY(bool state) {
  setRegisterBit(ADXL345_ACT_INACT_CTL, 1, state);
}
void SAT_Accel::setInactivityZ(bool state) {
  setRegisterBit(ADXL345_ACT_INACT_CTL, 0, state);
}

bool SAT_Accel::isActivityAc() {
  return getRegisterBit(ADXL345_ACT_INACT_CTL, 7);
}
bool SAT_Accel::isInactivityAc(){
  return getRegisterBit(ADXL345_ACT_INACT_CTL, 3);
}

void SAT_Accel::setActivityAc(bool state) {
  setRegisterBit(ADXL345_ACT_INACT_CTL, 7, state);
}
void SAT_Accel::setInactivityAc(bool state) {
  setRegisterBit(ADXL345_ACT_INACT_CTL, 3, state);
}

bool SAT_Accel::getSuppressBit(){
  return getRegisterBit(ADXL345_TAP_AXES, 3);
}
void SAT_Accel::setSuppressBit(bool state) {
  setRegisterBit(ADXL345_TAP_AXES, 3, state);
}

bool SAT_Accel::isTapDetectionOnX(){
  return getRegisterBit(ADXL345_TAP_AXES, 2);
}
void SAT_Accel::setTapDetectionOnX(bool state) {
  setRegisterBit(ADXL345_TAP_AXES, 2, state);
}
bool SAT_Accel::isTapDetectionOnY(){
  return getRegisterBit(ADXL345_TAP_AXES, 1);
}
void SAT_Accel::setTapDetectionOnY(bool state) {
  setRegisterBit(ADXL345_TAP_AXES, 1, state);
}
bool SAT_Accel::isTapDetectionOnZ(){
  return getRegisterBit(ADXL345_TAP_AXES, 0);
}
void SAT_Accel::setTapDetectionOnZ(bool state) {
  setRegisterBit(ADXL345_TAP_AXES, 0, state);
}

bool SAT_Accel::isActivitySourceOnX(){
  return getRegisterBit(ADXL345_ACT_TAP_STATUS, 6);
}
bool SAT_Accel::isActivitySourceOnY(){
  return getRegisterBit(ADXL345_ACT_TAP_STATUS, 5);
}
bool SAT_Accel::isActivitySourceOnZ(){
  return getRegisterBit(ADXL345_ACT_TAP_STATUS, 4);
}

bool SAT_Accel::isTapSourceOnX(){
  return getRegisterBit(ADXL345_ACT_TAP_STATUS, 2);
}
bool SAT_Accel::isTapSourceOnY(){
  return getRegisterBit(ADXL345_ACT_TAP_STATUS, 1);
}
bool SAT_Accel::isTapSourceOnZ(){
  return getRegisterBit(ADXL345_ACT_TAP_STATUS, 0);
}

bool SAT_Accel::isAsleep(){
  return getRegisterBit(ADXL345_ACT_TAP_STATUS, 3);
}

bool SAT_Accel::isLowPower(){
  return getRegisterBit(ADXL345_BW_RATE, 4);
}
void SAT_Accel::setLowPower(bool state) {
  setRegisterBit(ADXL345_BW_RATE, 4, state);
}

double SAT_Accel::getRate(){
  byte _b;
  readFrom(ADXL345_BW_RATE, 1, &_b);
  _b &= B00001111;
  return (pow(2,((int) _b)-6)) * 6.25;
}

void SAT_Accel::setRate(double rate){
  byte _b,_s;
  int v = (int) (rate / 6.25);
  int r = 0;
  while (v >>= 1)
  {
    r++;
  }
  if (r <= 9) {
    readFrom(ADXL345_BW_RATE, 1, &_b);
    _s = (byte) (r + 6) | (_b & B11110000);
    writeTo(ADXL345_BW_RATE, _s);
  }
}

void SAT_Accel::set_bw(byte bw_code){
  if((bw_code < ADXL345_BW_3) || (bw_code > ADXL345_BW_1600)){
    status = false;
    error_code = ADXL345_BAD_ARG;
  }
  else{
    writeTo(ADXL345_BW_RATE, bw_code);
  }
}

byte SAT_Accel::get_bw_code(){
  byte bw_code;
  readFrom(ADXL345_BW_RATE, 1, &bw_code);
  return bw_code;
}

byte SAT_Accel::getInterruptSource() {
  byte _b;
  readFrom(ADXL345_INT_SOURCE, 1, &_b);
  return _b;
}

bool SAT_Accel::getInterruptSource(byte interruptBit) {
  return getRegisterBit(ADXL345_INT_SOURCE,interruptBit);
}

bool SAT_Accel::getInterruptMapping(byte interruptBit) {
  return getRegisterBit(ADXL345_INT_MAP,interruptBit);
}

// Set the mapping of an interrupt to pin1 or pin2
// eg: setInterruptMapping(ADXL345_INT_DOUBLE_TAP_BIT,ADXL345_INT2_PIN);
void SAT_Accel::setInterruptMapping(byte interruptBit, bool interruptPin) {
  setRegisterBit(ADXL345_INT_MAP, interruptBit, interruptPin);
}

bool SAT_Accel::isInterruptEnabled(byte interruptBit) {
  return getRegisterBit(ADXL345_INT_ENABLE,interruptBit);
}

void SAT_Accel::setInterrupt(byte interruptBit, bool state) {
  setRegisterBit(ADXL345_INT_ENABLE, interruptBit, state);
}

void SAT_Accel::setRegisterBit(byte regAdress, int bitPos, bool state) {
  byte _b;
  readFrom(regAdress, 1, &_b);
  if (state) {
    _b |= (1 << bitPos);  // forces nth bit of _b to be 1.  all other bits left alone.
  }
  else {
    _b &= ~(1 << bitPos); // forces nth bit of _b to be 0.  all other bits left alone.
  }
  writeTo(regAdress, _b);
}

bool SAT_Accel::getRegisterBit(byte regAdress, int bitPos) {
  byte _b;
  readFrom(regAdress, 1, &_b);
  return ((_b >> bitPos) & 1);
}

// print all register value to the serial ouptut, which requires it to be setup
// this can be used to manually to check the current configuration of the device
void SAT_Accel::printAllRegister() {
  byte _b;
  Serial.print("0x00: ");
  readFrom(0x00, 1, &_b);
  print_byte(_b);
  Serial.println("");
  int i;
  for (i=29;i<=57;i++){
    Serial.print("0x");
    Serial.print(i, HEX);
    Serial.print(": ");
    readFrom(i, 1, &_b);
    print_byte(_b);
    Serial.println("");
  }
}

void print_byte(byte val){
  int i;
  Serial.print("B");
  for(i=7; i>=0; i--){
    Serial.print(val >> i & 1, BIN);
  }
}
