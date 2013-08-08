/*
	file : I2C_CommManager.cpp

    @author : Jean-François Omhover

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
 */

#include <Arduino.h>

#include "I2CComm.h"

#ifdef I2CCOMM_NODE
#include <EEPROM.h>
#endif

#include <Wire.h>	// TODO : still not including from the IDE, seems related to the way the IDE is compiling

/* (RECALL)
 * private:
 * uint8_t localAddress_;
 * boolean hasBegun_;
 * uint32_t startingTime;
 *
 * #define I2C_COMM_BEGINASMASTER	0	// use that if you want to begin() as the master of the I2C bus
 * #define I2C_COMM_BLOCKTIMEOUT	-1	// use that to block read functions until the full expected data is received
 * #define I2C_COMM_INSTANTTIMEOUT	0	// by default, don't wait, data is available or die !
 *
 * #define I2C_COMM_OK	0				// everything's perfect !
 * #define I2C_COMM_ERROR	-1			// generic error (allo Houston ?)
 * #define I2C_COMM_ERRORTIMEOUT	 -2	// timeout reached before receiving values
 * #define I2C_COMM_ERRORNOWRITE	 -3	// writing doesn't seem to work (Wire.write() return values mismatch)
 * #define I2C_COMM_ERRORTOOMUCHDATA -4 // data given to the transmission exceeds the internal buffer
 * #define I2C_COMM_ERRORNACKADDR	 -5 // typical return value when there's no OnboardCommLayer listening / connected
 * #define I2C_COMM_ERRORNACKDATA	 -6 // ???
 */


I2C_CommManager::I2C_CommManager() {
	hasBegun_ = false;
}

boolean I2C_CommManager::begin(uint8_t localAddr) {
	if (hasBegun_)
		return(true); // if is already begun, don't do it again (aren't you supposed not to ?)

#ifdef I2CCOMM_NODE
	localAddress_ = EEPROM.read(0x00);
#else
	localAddress_ = localAddr;
#endif

	if (localAddr == I2C_COMM_BEGINASMASTER)
		Wire.begin();
	else
		Wire.begin(localAddress_);

	hasBegun_ = true;
	return(hasBegun_);
};

// sends the whole buffer of values to destinationAddr,
// and if the sending fails (you never know), try again until if works or timeout reached
int8_t I2C_CommManager::sendBuffer(uint8_t destinationAddr,
				  uint8_t * buffer,
				  int size,
				  int timeout) {
	Wire.beginTransmission(destinationAddr);
#if ARDUINO >= 100
    int t_size = Wire.write(buffer, size);
#else
    int t_size = Wire.send(buffer, size);
#endif

    // TODO : i don't know what to do here... use the timeout or whatever ?
/*
    if (t_size < size) {

    }
*/
	byte t_ret = Wire.endTransmission();
	return(endTransmissionErrorCode(t_ret, t_size));
}

// sends a byte, and expect one from destinationAddr
int8_t I2C_CommManager::exchangeByte(uint8_t destinationAddr, uint8_t content, uint8_t * receivedBytePtr,  int timeout) {
	int8_t ret = transmitByte(destinationAddr, content);
	if (ret < 0)
		return(ret);
	ret = requestByte(destinationAddr, (uint8_t *)receivedBytePtr, timeout);
	return(ret);
}

// sends a byte to the destination address
int8_t I2C_CommManager::transmitByte(uint8_t destinationAddr,  uint8_t content) {
	Wire.beginTransmission(destinationAddr);
#if ARDUINO >= 100
	if (Wire.write(content) < 1)
		return(I2C_COMM_ERRORNOWRITE);
#else
	if (Wire.send(content) < 1)
		return(I2C_COMM_ERRORNOWRITE);
#endif
	byte t_ret = Wire.endTransmission();
	return(endTransmissionErrorCode(t_ret, 1));
}

// transmits bytes independently, byte by byte, via Wire.write()
int8_t I2C_CommManager::transmitByteArray(uint8_t destinationAddr,
				  	  	  	  	  	  	  uint8_t * array,
				  	  	  	  	  	  	  int size,
				  	  	  	  	  	  	  int timeout) {
	Wire.beginTransmission(destinationAddr);

	int sentCount = 0;

	for (int i=0; i<size; i++) {
#if ARDUINO >= 100
		sentCount += Wire.write(array[i]);
#else
		sentCount += Wire.send(array[i]);
#endif
	}
    // TODO : i don't know what to do here... use the timeout or whatever ?
/*	if (sentCount < size) {

	}
*/

	byte t_ret = Wire.endTransmission();
	return(endTransmissionErrorCode(t_ret, sentCount));
}

// calls on the sourceAddr and request a byte
int8_t I2C_CommManager::requestByte(uint8_t sourceAddr, uint8_t * receivedBytePtr, int timeout) {
	Wire.requestFrom(sourceAddr, (uint8_t)1);    // request 1 byte from device sourceAddr

	startingTime = millis();
	if (timeout == I2C_COMM_BLOCKTIMEOUT) {

		// waits for the expected data until the end of times...
		while(Wire.available() < 1);

	} else {

		// waits for the expected data until the timeout is reached
		unsigned long int currentTime = 0;

		while ((Wire.available() < 1)) {
			currentTime = millis() - startingTime;
			if (currentTime > timeout)
				return(I2C_COMM_ERRORTIMEOUT);
		};

	}

	*receivedBytePtr = Wire.read();
	return(1);
}

// calls on the sourceAddr and request 2 bytes
int8_t I2C_CommManager::request16bits(uint8_t sourceAddr,
				uint16_t * receivedIntPtr,		// a pointer to the int that will be filled by the received value
				boolean order,					// true for MSB-LSB, false for LSB-MSB
				int timeout) {
	Wire.requestFrom(sourceAddr, (uint8_t)2);    // request 2 bytes from device sourceAddr

	startingTime = millis();
	if (timeout == I2C_COMM_BLOCKTIMEOUT) {

		// waits for the expected data until the end of times...
		while(Wire.available() < 2);

	} else {

		// waits for the expected data until the timeout is reached
		unsigned long int currentTime = 0;

		while ((Wire.available() < 2)) {
			currentTime = millis() - startingTime;
			if (currentTime > timeout)
				return(I2C_COMM_ERRORTIMEOUT);
		};

	}

	uint8_t lsb,msb;
	if (order) {
		msb = Wire.read();	// taken from SAT_Lum::read16
		lsb = Wire.read();
	} else {
		lsb = Wire.read();	// taken from SAT_Lum::read16
		msb = Wire.read();
	}
	uint16_t out = ( (uint16_t)lsb | ( (uint16_t)msb << 8 ) ); //concatenate the MSB and LSB and apply mask
	*receivedIntPtr = out;

	return(2);
}

// calls on the sourceAddr and request a MSB and LSB kind of int
int8_t I2C_CommManager::request16bitsFromMSBLSB(uint8_t sourceAddr,
				uint8_t msb_reg,
				uint8_t lsb_reg,
				uint16_t * receivedIntPtr,		// a pointer to the byte that will be filled by the received value
				int timeout) {
	uint8_t lsb,msb;
	int8_t t_ret = 0;

	transmitByte(sourceAddr, msb_reg);
	delayMicroseconds(2); //needs at least 1.3us free time between start and stop
	t_ret = requestByte(sourceAddr, &msb, timeout);
	// TODO : what do we do here if t_ret < 0 ?

	transmitByte(sourceAddr, lsb_reg);
	delayMicroseconds(2); //needs at least 1.3us free time between start and stop
	t_ret = requestByte(sourceAddr, &lsb, timeout);
	// TODO : what do we do here if t_ret < 0 ?

	uint16_t out = ( (uint16_t)lsb | ( (uint16_t)msb << 8 ) ); //concatenate the MSB and LSB and apply mask
	*receivedIntPtr = out;
	return(2);
}

// calls on the sourceAddr and request a 16 bits int from a REG
int8_t I2C_CommManager::request16bitsFromREG(uint8_t sourceAddr,
				uint8_t reg,
				uint16_t * receivedIntPtr,		// a pointer to the byte that will be filled by the received value
				boolean order,					// true for MSB-LSB, false for LSB-MSB
				int timeout) {

	int8_t t_ret = 0;
	transmitByte(sourceAddr, reg);
	delayMicroseconds(2); //needs at least 1.3us free time between start and stop
	t_ret = request16bits(sourceAddr, receivedIntPtr, order, timeout);
	return(t_ret);
}

// provides the error value for the kind of errors happening at Wire.endTransmission()
int8_t I2C_CommManager::endTransmissionErrorCode(byte retValue, int sizeSent) {
	switch(retValue) {
		case 0:	// success
			return((int8_t)sizeSent);
		case 1: // data too long to fit in transmit buffer
			return(I2C_COMM_ERRORTOOMUCHDATA);
		case 2: // received NACK on transmit of address
			return(I2C_COMM_ERRORNACKADDR);
		case 3: // received NACK on transmit of data
			return(I2C_COMM_ERRORNACKDATA);
		case 4: // other error
		default:
			return(I2C_COMM_ERROR);
	}
}

I2C_CommManager I2CComm;
