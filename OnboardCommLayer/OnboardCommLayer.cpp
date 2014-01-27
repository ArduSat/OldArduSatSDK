/*
*   @file nanosat_message.h
*   @version 1.0
*   @name NanoSatisfi Inc.
*
*   @section LICENSE
*
*   see LICENSE and NOTICE files.
*
*   @section DESCRIPTION
*   Message passing schema between OnboardCommLayer (I2C wrapper) and Payload
*   supervisor. This interface exposes system calls to user space and sensor
*   access. The main interaction players are Payload Supervisor(assv) and
*   experiment nodes.
*/

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "OnboardCommLayer.h"
#include "nanosat_message.h"
#include "I2C_add.h"
#include <EEPROM.h>
#include <Wire.h>
//#include <I2CComm.h>

static uint8_t transmitBuffer[sizeof(nanosat_message_t)];

OnboardCommLayer::OnboardCommLayer() {
  hasBegun_ = false;
  localAddress_ = I2C_COMM_BEGINASMASTER;
  startingTime = 0;
  assvAddress_ = I2C_ADD_ASSV_1;		// jfomhover on 07/08/2013 : should be the address of the master arduino in the Sat
}

uint8_t OnboardCommLayer::sendMessage(nanosat_message_t msg) {
  memcpy(transmitBuffer, &msg, sizeof(nanosat_message_t)); // jfomhover on 2013/08/12 : is this necessary since we're not multitasking ?
  OBCL.transmitByteArray(assvAddress_,transmitBuffer,sizeof(nanosat_message_t),I2C_COMM_INSTANTTIMEOUT);
  OBCL.flushWrite();
  return(0);	// TODO : what should we do here ?

//  Wire.begin(nodeAddress_); 				// jfomhover on 07/08/2013 : join the I2C bus as a slave with its node address in the Sat
//  Wire.beginTransmission(assvAddress_);		// transmit to the master arduino in the Sat
//  Wire.write(transmitBuffer, sizeof(nanosat_message_t));
//  Wire.endTransmission();
}

uint8_t OnboardCommLayer::sendExit() {
  nanosat_message_t msg = {
    NODE_COMM_MESSAGE_PREFIX,
    EXIT,
    EEPROM.read(0x00)
  };
  sendMessage(msg);
  return(0);	// TODO : what should we do here ?
}

boolean OnboardCommLayer::begin(uint8_t localAddr) {
	if (hasBegun_)
		return(true); // if is already begun, don't do it again (aren't you supposed not to ?)

#ifdef I2CCOMM_REALNODE
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

// returns the local address
uint8_t OnboardCommLayer::getAddress()
{
	return(localAddress_);
}

// flush the Wire (needed in geiger_sensor_poller.ino, dunno why)
void OnboardCommLayer::flushWrite()
{
	Wire.flush();
}

// flush the Wire (needed in geiger_sensor_poller.ino, dunno why)
void OnboardCommLayer::flushRead()
{
	while(Wire.available())
		Wire.read();
}

// sends the whole buffer of values to destinationAddr,
// and if the sending fails (you never know), try again until if works or timeout reached
int8_t OnboardCommLayer::sendBuffer(uint8_t destinationAddr,
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
int8_t OnboardCommLayer::exchangeByte(uint8_t destinationAddr, uint8_t content, uint8_t * receivedBytePtr,  int timeout) {
	int8_t ret = transmitByte(destinationAddr, content);
	if (ret < 0)
		return(ret);
	ret = requestByte(destinationAddr, (uint8_t *)receivedBytePtr, timeout);
	return(ret);
}

// sends a byte to the destination address
int8_t OnboardCommLayer::transmitByte(uint8_t destinationAddr,  uint8_t content) {
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

// sends a byte to the destination address,
// has a "restart" boolean for endTransmission() to send a restart message after transmission
int8_t OnboardCommLayer::transmitByte(uint8_t destinationAddr,
				 uint8_t content,
				 boolean restart) {
	Wire.beginTransmission(destinationAddr);
#if ARDUINO >= 100
	if (Wire.write(content) < 1)
		return(I2C_COMM_ERRORNOWRITE);
#else
	if (Wire.send(content) < 1)
		return(I2C_COMM_ERRORNOWRITE);
#endif
	byte t_ret = Wire.endTransmission(restart);
	return(endTransmissionErrorCode(t_ret, 1));
}

// transmits bytes independently, byte by byte, via Wire.write()
int8_t OnboardCommLayer::transmitByteArray(uint8_t destinationAddr,
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
int8_t OnboardCommLayer::requestByte(uint8_t sourceAddr, uint8_t * receivedBytePtr, int timeout) {
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
			if (currentTime > (unsigned long int)timeout) // jfomhover 08/09/2013 : not so sure about types
				return(I2C_COMM_ERRORTIMEOUT);
		};

	}

	*receivedBytePtr = Wire.read();
	return(1);
}

// calls on the sourceAddr and request a byte array
int8_t OnboardCommLayer::requestByteArray(uint8_t sourceAddr,
				byte * recBuffer,			// byte buffer for the received values
				uint8_t expectedLen,			// length of the expected data array
				uint8_t * receivedLen,			// where to put the length actually received
				int timeout)				// timeout value (instantaneous by default)
{
	Wire.beginTransmission(sourceAddr);
	Wire.requestFrom((byte)sourceAddr, (byte)expectedLen);

	// reading data loop

	uint8_t obtainedData = 0;

	while(Wire.available())
	{
		recBuffer[obtainedData++] = Wire.read();
	}

	int8_t t_ret = Wire.endTransmission();         // end transmission

    receivedLen[0] = obtainedData;

	if (obtainedData < expectedLen)
		return(I2C_COMM_NOTENOUGHDATA);

	return(endTransmissionErrorCode(t_ret, obtainedData));
}

// calls on the sourceAddr and request 2 bytes
int8_t OnboardCommLayer::request16bits(uint8_t sourceAddr,
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
			if (currentTime > (unsigned long int)timeout) // jfomhover 08/09/2013 : not so sure about types
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
int8_t OnboardCommLayer::request16bitsFromMSBLSB(uint8_t sourceAddr,
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
int8_t OnboardCommLayer::request16bitsFromREG(uint8_t sourceAddr,
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
int8_t OnboardCommLayer::endTransmissionErrorCode(byte retValue, int sizeSent) {
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

OnboardCommLayer OBCL;
