/*
	file : I2C_CommManager.h
	content : 	class that handles a robust communication via the arduino Wire / I2C library
				error codes are made accessible, and let you handle them properly
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

#ifndef I2CComm_h_ 
#define I2CComm_h_ 

// uncomment the #define below to activate the reading of localAddress_ in EEPROM / 0x00
//#define I2CCOMM_NODE

class I2C_CommManager {
  private:
    uint8_t localAddress_;	// the local address of the I2C node
    boolean hasBegun_;		// true if the comm has already been Wire.begin() somehow
    uint32_t startingTime;

#define I2C_COMM_BEGINASMASTER	0	// use that if you want to begin() as the master of the I2C bus

  public:
    I2C_CommManager();


    boolean begin(uint8_t localAddr = I2C_COMM_BEGINASMASTER);		// by default, begin as master of the I2C bus

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
    int8_t transmitByte(uint8_t destinationAddr,
    				 uint8_t content);

    // transmits bytes independently, byte by byte, via Wire.write()
    int8_t transmitByteArray(uint8_t destinationAddr,
    				  uint8_t * array,
    				  int size,
    				  int timeout = I2C_COMM_INSTANTTIMEOUT);   // NOT USED for the moment : timeout value (instantaneous by default)

    // calls on the sourceAddr and request a byte
    int8_t requestByte(uint8_t sourceAddr,
    				uint8_t * receivedBytePtr,		// a pointer to the byte that will be filled by the received value
    				int timeout = I2C_COMM_INSTANTTIMEOUT);		// timeout value (instantaneous by default)

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

extern I2C_CommManager I2CComm;

#endif /* I2CComm_h_ */
