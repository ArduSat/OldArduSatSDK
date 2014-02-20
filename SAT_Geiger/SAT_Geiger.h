/** @brief  sensor library for I2C access to ArduSat geiger counter

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

    @author Jeroen Cappaert and NanoSatisfi, Inc.
    @date May 2013
    modified by jfomhover on Aug. 2013 for ArduSatSDK
*/

#ifndef NANOSATISFI_GEIGER_H_
#define NANOSATISFI_GEIGER_H_


// status regs on geiger counter
#define CPM1          0x01 // count per minute tube 1
#define CPM2          0x03 // count per minute tube 2
#define USPH1         0x02 // uS/h radiation value tube 1
#define USPH2         0x04 // uS/h radiatoin value tube 2

// delay after transmission before requesting data
#define T_DELAY       100

// buffer sizees
#define BUF_SIZE      4

class SAT_Geiger{
  public:
    /**Constructor */
    SAT_Geiger();
    /*
     * Initialize the geiger counter
     * @param node_id The id of the user's arduino on the ArduSat. This allows
     * the supervisor to know which arduino node to send the data back to.
     * The id is assigned by NanoSatisfi.
     */
    //void init(uint8_t node_id); // jfomhover 08/09/2013 : function not implemented ?

    /*
     * Get's the raw count per minute.
     * @param tube_handle The number of the geiger counter to poll.
     *    This either '1' or '2'.
     */
    long getCPM(byte tube_handle);

    /*
     * Get's the value from the geiger counter in microSeiverts/h
     * @param tube_handle The number of the geiger counter to poll.
     * This either '1' or '2'.
     */
    float getUSPH(byte tube_handle);

  private:
    //uint8_t _local_address; // jfomhover 08/09/2013 : not used
    byte buffer[BUF_SIZE];
    //void send(byte* data, unsigned int len);  // jfomhover 08/09/2013 : replaced by I2CComm functions
    //void receive(byte* data, unsigned int len);  // jfomhover 08/09/2013 : replaced by I2CComm functions
};

#endif /* NANOSATISFI_GEIGER_H_*/
