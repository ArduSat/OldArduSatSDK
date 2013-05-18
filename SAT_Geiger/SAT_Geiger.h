/** @brief  sensor library for I2C access to ArduSat geiger counter

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
	
    @author Jeroen Cappaert and NanoSatisfi, Inc.
    @date May 2013
*/

#ifndef NANOSATISFI_GEIGER_H_
#define NANOSATISFI_GEIGER_H_

 
// status regs on geiger counter
#define CPM1          0x01 	// count per minute tube 1
#define CPM2          0x03	// count per minute tube 2
#define USPH1         0x02	// uS/h radiation value tube 1
#define USPH2         0x04	// uS/h radiatoin value tube 2

// delay after transmission before requesting data
#define T_DELAY       100

// buffer sizees
#define BUF_SIZE      4


class SAT_Geiger{
 public:
 	/**Constructor */
    SAT_Geiger();
	
	/**Initialize the geiger counter
			@param node_id - The id of the user's arduino on the ArduSat. This allows 
			the supervisor to know which arduino node to send the data back to. 
			The id is assigned by NanoSatisfi.
    */
    void init(uint8_t node_id);
	
	/**Get's the raw count per minute.
			@param tube_handle - The number of the geiger counter to poll. This either '1' or '2'.
    */
    long getCPM(byte tube_handle);
	
	/**Get's the value from the geiger counter in microSeiverts/h
			@param tube_handle - The number of the geiger counter to poll. This either '1' or '2'.
    */
    float getUSPH(byte tube_handle);
  
 private:  
    uint8_t _local_address;
    OnboardCommLayer* _ocl;
    byte buffer[BUF_SIZE];
    void send(byte* data, unsigned int len);
    void receive(byte* data, unsigned int len);

};




#endif
