/** @brief  Library to query the on board temperature sensors.

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

    @author Jorge Ortiz and NanoSatisfi, Inc.
    @date May 2013
*/


#ifndef TEMP_H
#define TEMP_H

#include <inttypes.h>
#include <Arduino.h>

class SAT_Temp{
  private:
    // uint8_t _local_address; // jfomhover on 07/08/2013 : not used
    uint8_t _temp_i2c_addr;
    byte _buff[2];

  public:
    /**Tempurature sensor constructor
    @param tempid - The id of the tempurature sensor.
      There are four of these sensors.
    */
    SAT_Temp(uint8_t tempid);

    /**Initializes the tempurature sensor for this node.
    @param nodeid - The id of the user's arduino on the ArduSat. This allows 
    the supervisor to know which arduino node to send the data back to. 
    The id is assigned by NanoSatisfi.
    */
    void init(); // jfomhover on 07/08/2013 : uint8_t nodeid not used anywhere
    /**Returns the tempurature in Celsius from the sensor. */
    float getTemp();    //returns temperature in celcius
    int16_t getRawTemp(); // returns the raw value from the sensor (12bits)
};

#endif /* TEMP_H */
