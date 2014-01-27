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


#ifndef ONBOARD_COMM_LAYER_H
#define ONBOARD_COMM_LAYER_H

#include <Arduino.h>
#include "I2C_add.h"
#include "nanosat_message.h"

// uncomment the #define below to activate the reading of localAddress_ in EEPROM addr 0x00
#define I2CCOMM_REALNODE

#define I2C_COMM_BEGINASMASTER	0	// use that if you want to begin() as the master of the I2C bus

class OnboardCommLayer{
  private:
    uint8_t assvAddress_;
    uint8_t localAddress_;	// the local address of the I2C node
    boolean hasBegun_;		// true if the comm has already been Wire.begin() somehow
    uint32_t startingTime;
  public:
    uint8_t sendMessage(nanosat_message_t msg);
    uint8_t sendExit();

    OnboardCommLayer();
    boolean begin(uint8_t localAddr = I2C_COMM_BEGINASMASTER);		// by default, begin as master of the I2C bus
    uint8_t getAddress();
    void flushWrite();		// flush the write buffer of the Wire
    void flushRead();		// flush the read buffer of the Wire (needed in geiger_sensor_poller.ino, dunno why)

#define I2C_COMM_BLOCKTIMEOUT	-1	// use that to block read functions until the full expected data is received
#define I2C_COMM_INSTANTTIMEOUT	0	// by default, don't wait, data is available or die !

// possible return values
#define I2C_COMM_OK					0	// everything's perfect !
#define I2C_COMM_ERROR				-1	// generic error (allo Houston ?)
#define I2C_COMM_ERRORTIMEOUT		-2	// timeout reached before receiving values
#define I2C_COMM_ERRORNOWRITE		-3	// writing doesn't seem to work (Wire.write() return values mismatch)
#define I2C_COMM_ERRORTOOMUCHDATA	-4	// data given to the transmission exceeds the internal buffer
#define I2C_COMM_ERRORNACKADDR		-5	// typical return value when there's no OnboardCommLayer listening / connected
#define I2C_COMM_ERRORNACKDATA		-6	// ???
#define I2C_COMM_NOTENOUGHDATA		-7	// received less data than expected

	// sends the whole buffer of values to destinationAddr,
    // and if the sending fails (you never know), try again until if works or timeout reached
    int8_t sendBuffer(uint8_t destinationAddr,
    				  uint8_t * buffer,
    				  int size,
    				  int timeout = I2C_COMM_INSTANTTIMEOUT);   // NOT USED for the moment : timeout value (instantaneous by default)

    // sends a byte, and expect one from destinationAddr
    int8_t exchangeByte(uint8_t destinationAddr,
    					uint8_t content,			// the value sent to the destination address
    					uint8_t * receivedBytePtr,	// a pointer to the byte that will be filled by the received value
    					int timeout = I2C_COMM_INSTANTTIMEOUT);	// timeout value (instantaneous by default)

    // sends a byte to the destination address
    // left for compatibility reasons
    int8_t transmitByte(uint8_t destinationAddr,
    				 uint8_t content);

    // sends a byte to the destination address,
    // has a "restart" boolean for endTransmission() to send a restart message after transmission
    int8_t transmitByte(uint8_t destinationAddr,
    				 uint8_t content,
    				 boolean restart);

    // transmits bytes independently, byte by byte, via Wire.write()
    int8_t transmitByteArray(uint8_t destinationAddr,
    				  uint8_t * array,
    				  int size,
    				  int timeout = I2C_COMM_INSTANTTIMEOUT);   // NOT USED for the moment : timeout value (instantaneous by default)

    // calls on the sourceAddr and request a byte
    int8_t requestByte(uint8_t sourceAddr,
    				uint8_t * receivedBytePtr,		// a pointer to the byte that will be filled by the received value
    				int timeout = I2C_COMM_INSTANTTIMEOUT);		// timeout value (instantaneous by default)

    // calls on the sourceAddr and request a byte array
    int8_t requestByteArray(uint8_t sourceAddr,
							byte * recBuffer,			// byte buffer for the received values
							uint8_t expectedLen,			// length of the expected data array
							uint8_t * receivedLen,			// where to put the length actually received
							int timeout = I2C_COMM_INSTANTTIMEOUT);				// timeout value (instantaneous by default)

    // calls on the sourceAddr and request 2 bytes
    int8_t request16bits(uint8_t sourceAddr,
    				uint16_t * receivedIntPtr,		// a pointer to the int that will be filled by the received value
    				boolean order,					// true for MSB-LSB, false for LSB-MSB
    				int timeout = I2C_COMM_INSTANTTIMEOUT);		// timeout value (instantaneous by default)

    // calls on the sourceAddr and request a compile a MSB and LSB kind of int
    int8_t request16bitsFromMSBLSB(uint8_t sourceAddr,
    				uint8_t msb_reg,
    				uint8_t lsb_reg,
    				uint16_t * receivedIntPtr,		// a pointer to the byte that will be filled by the received value
    				int timeout = I2C_COMM_INSTANTTIMEOUT);		// timeout value (instantaneous by default)

    // calls on the sourceAddr and request a 16 bits int from a REG
    int8_t request16bitsFromREG(uint8_t sourceAddr,
    				uint8_t reg,
    				uint16_t * receivedIntPtr,		// a pointer to the byte that will be filled by the received value
    				boolean order,					// true for MSB-LSB, false for LSB-MSB
    				int timeout = I2C_COMM_INSTANTTIMEOUT);		// timeout value (instantaneous by default)

private :
    // if retValue (value returned by Wire.endTransmission()) shows an error, returns the proper I2C_COMM_ERROR* code
    // if (retValue == 0) it means ok, so it returns sizeSent
    int8_t endTransmissionErrorCode(byte retValue, int sizeSent);
};

extern OnboardCommLayer OBCL;

#endif /* ONBOARD_COMM_LAYER_H */
