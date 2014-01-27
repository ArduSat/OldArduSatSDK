/****************************************************************************
* ITG3200.h - ITG-3200/I2C library v0.6 for Arduino                         *
* Copyright 2010-2012 Filipe Vieira & various contributors                  *
* http://code.google.com/p/itg-3200driver                                   *
* This file is part of ITG-3200 Arduino library.                            *
*                                                                           *
* This library is free software: you can redistribute it and/or modify      *
* it under the terms of the GNU Lesser General Public License as published  *
* by the Free Software Foundation, either version 3 of the License, or      *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU Lesser General Public License for more details.                       *
*                                                                           *
* You should have received a copy of the GNU Lesser General Public License  *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
****************************************************************************/

/* (hardly) modified by jfomhover on Aug 2013 for the purpose of the ArduSatSDK */

#include <I2C_add.h>
#include <Arduino.h>
#include "SAT_Gyro.h"
#include <Wire.h>
#include <OnboardCommLayer.h>	// for OBCL

SAT_Gyro::SAT_Gyro() {
  setOffsets(0,0,0);
  setScaleFactor(1.0, 1.0, 1.0, false);  // true to change readGyro output to radians
  //Wire.begin();       //Normally this code is called from setup() at user code
                        //but some people reported that joining I2C bus earlier
                        //apparently solved problems with master/slave conditions.
                        //Uncomment if needed.
}

void SAT_Gyro::init(unsigned int  address) {
	  OBCL.begin();

	  // Uncomment or change your default ITG3200 initialization
  // fast sample rate - divisor = 0 filter = 0 clocksrc = 0, 1, 2, or 3  (raw values)
  init(address, NOSRDIVIDER, RANGE2000, BW256_SR8, PLL_XGYRO_REF, true, true);

  // slow sample rate - divisor = 0  filter = 1,2,3,4,5, or 6  clocksrc = 0, 1, 2, or 3  (raw values)
  //init(NOSRDIVIDER, RANGE2000, BW010_SR1, INTERNALOSC, true, true);

  // fast sample rate 32Khz external clock - divisor = 0  filter = 0  clocksrc = 4  (raw values)
  //init(NOSRDIVIDER, RANGE2000, BW256_SR8, PLL_EXTERNAL32, true, true);

  // slow sample rate 32Khz external clock - divisor = 0  filter = 1,2,3,4,5, or 6  clocksrc = 4  (raw values)
  //init(NOSRDIVIDER, RANGE2000, BW010_SR1, PLL_EXTERNAL32, true, true);
}

void SAT_Gyro::init(unsigned int address, byte _SRateDiv, byte _Range, byte _filterBW, byte _ClockSrc, bool _ITGReady, bool _INTRawDataReady) {
  _dev_address = address;
  setSampleRateDiv(_SRateDiv);
  setFSRange(_Range);
  setFilterBW(_filterBW);
  setClockSource(_ClockSrc);
  setITGReady(_ITGReady);
  setRawDataReady(_INTRawDataReady);
  delay(GYROSTART_UP_DELAY);  // startup
}

byte SAT_Gyro::getDevAddr() {
  /*readmem(WHO_AM_I, 1, &_buff[0]);
  return _buff[0];  */
  return _dev_address;
}

void SAT_Gyro::setDevAddr(unsigned int  _addr) {
  writemem(WHO_AM_I, _addr);
  _dev_address = _addr;
}

byte SAT_Gyro::getSampleRateDiv() {
  readmem(SMPLRT_DIV, 1, &_buff[0]);
  return _buff[0];
}

void SAT_Gyro::setSampleRateDiv(byte _SampleRate) {
  writemem(SMPLRT_DIV, _SampleRate);
}

byte SAT_Gyro::getFSRange() {
  readmem(DLPF_FS, 1, &_buff[0]);
  return ((_buff[0] & DLPFFS_FS_SEL) >> 3);
}

void SAT_Gyro::setFSRange(byte _Range) {
  readmem(DLPF_FS, 1, &_buff[0]);
  writemem(DLPF_FS, ((_buff[0] & ~DLPFFS_FS_SEL) | (_Range << 3)) );
}

byte SAT_Gyro::getFilterBW() {
  readmem(DLPF_FS, 1, &_buff[0]);
  return (_buff[0] & DLPFFS_DLPF_CFG);
}

void SAT_Gyro::setFilterBW(byte _BW) {
  readmem(DLPF_FS, 1, &_buff[0]);
  writemem(DLPF_FS, ((_buff[0] & ~DLPFFS_DLPF_CFG) | _BW));
}

bool SAT_Gyro::isINTActiveOnLow() {
  readmem(INT_CFG, 1, &_buff[0]);
  return ((_buff[0] & INTCFG_ACTL) >> 7);
}

void SAT_Gyro::setINTLogiclvl(bool _State) {
  readmem(INT_CFG, 1, &_buff[0]);
  writemem(INT_CFG, ((_buff[0] & ~INTCFG_ACTL) | (_State << 7)));
}

bool SAT_Gyro::isINTOpenDrain() {
  readmem(INT_CFG, 1, &_buff[0]);
  return ((_buff[0] & INTCFG_OPEN) >> 6);
}

void SAT_Gyro::setINTDriveType(bool _State) {
  readmem(INT_CFG, 1, &_buff[0]);
  writemem(INT_CFG, ((_buff[0] & ~INTCFG_OPEN) | _State << 6));
}

bool SAT_Gyro::isLatchUntilCleared() {
  readmem(INT_CFG, 1, &_buff[0]);
  return ((_buff[0] & INTCFG_LATCH_INT_EN) >> 5);
}

void SAT_Gyro::setLatchMode(bool _State) {
  readmem(INT_CFG, 1, &_buff[0]);
  writemem(INT_CFG, ((_buff[0] & ~INTCFG_LATCH_INT_EN) | _State << 5));
}

bool SAT_Gyro::isAnyRegClrMode() {
  readmem(INT_CFG, 1, &_buff[0]);
  return ((_buff[0] & INTCFG_INT_ANYRD_2CLEAR) >> 4);
}

void SAT_Gyro::setLatchClearMode(bool _State) {
  readmem(INT_CFG, 1, &_buff[0]);
  writemem(INT_CFG, ((_buff[0] & ~INTCFG_INT_ANYRD_2CLEAR) | _State << 4));
}

bool SAT_Gyro::isITGReadyOn() {
  readmem(INT_CFG, 1, &_buff[0]);
  return ((_buff[0] & INTCFG_ITG_RDY_EN) >> 2);
}

void SAT_Gyro::setITGReady(bool _State) {
  readmem(INT_CFG, 1, &_buff[0]);
  writemem(INT_CFG, ((_buff[0] & ~INTCFG_ITG_RDY_EN) | _State << 2));
}

bool SAT_Gyro::isRawDataReadyOn() {
  readmem(INT_CFG, 1, &_buff[0]);
  return (_buff[0] & INTCFG_RAW_RDY_EN);
}

void SAT_Gyro::setRawDataReady(bool _State) {
  readmem(INT_CFG, 1, &_buff[0]);
  writemem(INT_CFG, ((_buff[0] & ~INTCFG_RAW_RDY_EN) | _State));
}

bool SAT_Gyro::isITGReady() {
  readmem(INT_STATUS, 1, &_buff[0]);
  return ((_buff[0] & INTSTATUS_ITG_RDY) >> 2);
}

bool SAT_Gyro::isRawDataReady() {
  readmem(INT_STATUS, 1, &_buff[0]);
  return (_buff[0] & INTSTATUS_RAW_DATA_RDY);
}

void SAT_Gyro::readTemp(float *_Temp) {
  readmem(TEMP_OUT,2,_buff);
  *_Temp = 35 + ((_buff[0] << 8 | _buff[1]) + 13200) / 280.0;    // F=C*9/5+32
}

void SAT_Gyro::readGyroRaw( int *_GyroX, int *_GyroY, int *_GyroZ){
  readmem(GYRO_XOUT, 6, _buff);
  *_GyroX = _buff[0] << 8 | _buff[1];
  *_GyroY = _buff[2] << 8 | _buff[3];
  *_GyroZ = _buff[4] << 8 | _buff[5];
}

void SAT_Gyro::readGyroRaw( int *_GyroXYZ){
  readGyroRaw(_GyroXYZ, _GyroXYZ+1, _GyroXYZ+2);
}

void SAT_Gyro::setScaleFactor(float _Xcoeff, float _Ycoeff, float _Zcoeff, bool _Radians) {
  scalefactor[0] = 14.375 * _Xcoeff;
  scalefactor[1] = 14.375 * _Ycoeff;
  scalefactor[2] = 14.375 * _Zcoeff;

  if (_Radians){
    scalefactor[0] /= 0.0174532925;//0.0174532925 = PI/180
    scalefactor[1] /= 0.0174532925;
    scalefactor[2] /= 0.0174532925;
  }
}

void SAT_Gyro::setOffsets(int _Xoffset, int _Yoffset, int _Zoffset) {
  offsets[0] = _Xoffset;
  offsets[1] = _Yoffset;
  offsets[2] = _Zoffset;
}

void SAT_Gyro::zeroCalibrate(unsigned int totSamples, unsigned int sampleDelayMS) {
  float tmpOffsets[] = {0,0,0};
  int xyz[3];

  for (int i = 0;i < totSamples;i++){
    delay(sampleDelayMS);
    readGyroRaw(xyz);
    tmpOffsets[0] += xyz[0];
    tmpOffsets[1] += xyz[1];
    tmpOffsets[2] += xyz[2];
  }
  setOffsets(-tmpOffsets[0] / totSamples + 0.5, -tmpOffsets[1] / totSamples + 0.5, -tmpOffsets[2] / totSamples + 0.5);
}

void SAT_Gyro::readGyroRawCal(int *_GyroX, int *_GyroY, int *_GyroZ) {
  readGyroRaw(_GyroX, _GyroY, _GyroZ);
  *_GyroX += offsets[0];
  *_GyroY += offsets[1];
  *_GyroZ += offsets[2];
}

void SAT_Gyro::readGyroRawCal(int *_GyroXYZ) {
  readGyroRawCal(_GyroXYZ, _GyroXYZ+1, _GyroXYZ+2);
}

void SAT_Gyro::readGyro(float *_GyroX, float *_GyroY, float *_GyroZ){
  int x, y, z;
  readGyroRawCal(&x, &y, &z); // x,y,z will contain calibrated integer values from the sensor
  *_GyroX =  x / scalefactor[0];
  *_GyroY =  y / scalefactor[1];
  *_GyroZ =  z / scalefactor[2];
}

void SAT_Gyro::readGyro(float *_GyroXYZ){
  readGyro(_GyroXYZ, _GyroXYZ+1, _GyroXYZ+2);
}

void SAT_Gyro::reset() {
  writemem(PWR_MGM, PWRMGM_HRESET);
  delay(GYROSTART_UP_DELAY); //gyro startup
}

bool SAT_Gyro::isLowPower() {
  readmem(PWR_MGM, 1, &_buff[0]);
  return (_buff[0] & PWRMGM_SLEEP) >> 6;
}

void SAT_Gyro::setPowerMode(bool _State) {
  readmem(PWR_MGM, 1, &_buff[0]);
  writemem(PWR_MGM, ((_buff[0] & ~PWRMGM_SLEEP) | _State << 6));
}

bool SAT_Gyro::isXgyroStandby() {
  readmem(PWR_MGM, 1, &_buff[0]);
  return (_buff[0] & PWRMGM_STBY_XG) >> 5;
}

bool SAT_Gyro::isYgyroStandby() {
  readmem(PWR_MGM, 1, &_buff[0]);
  return (_buff[0] & PWRMGM_STBY_YG) >> 4;
}

bool SAT_Gyro::isZgyroStandby() {
  readmem(PWR_MGM, 1, &_buff[0]);
  return (_buff[0] & PWRMGM_STBY_ZG) >> 3;
}

void SAT_Gyro::setXgyroStandby(bool _Status) {
  readmem(PWR_MGM, 1, &_buff[0]);
  writemem(PWR_MGM, ((_buff[0] & PWRMGM_STBY_XG) | _Status << 5));
}

void SAT_Gyro::setYgyroStandby(bool _Status) {
  readmem(PWR_MGM, 1, &_buff[0]);
  writemem(PWR_MGM, ((_buff[0] & PWRMGM_STBY_YG) | _Status << 4));
}

void SAT_Gyro::setZgyroStandby(bool _Status) {
  readmem(PWR_MGM, 1, &_buff[0]);
  writemem(PWR_MGM, ((_buff[0] & PWRMGM_STBY_ZG) | _Status << 3));
}

byte SAT_Gyro::getClockSource() {
  readmem(PWR_MGM, 1, &_buff[0]);
  return (_buff[0] & PWRMGM_CLK_SEL);
}

void SAT_Gyro::setClockSource(byte _CLKsource) {
  readmem(PWR_MGM, 1, &_buff[0]);
  writemem(PWR_MGM, ((_buff[0] & ~PWRMGM_CLK_SEL) | _CLKsource));
}

void SAT_Gyro::writemem(uint8_t _addr, uint8_t _val) {
	byte content[2];

	content[0] = _addr;
	content[1] = _val;

	OBCL.transmitByteArray(_dev_address, content, 2);
	// TODO : what to do if this fails ?
}

void SAT_Gyro::readmem(uint8_t _addr, uint8_t _nbytes, uint8_t __buff[]) {
	OBCL.transmitByte(_dev_address, _addr);

	uint8_t recLen = 0;
	OBCL.requestByteArray(_dev_address, _buff, _nbytes, &recLen);
	// TODO : what to do if this fails ?
}


