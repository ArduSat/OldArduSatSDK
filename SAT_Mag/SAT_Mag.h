/** @brief  Library for Arduino returns x,y,z data on current magnetic field

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

    Designed for use with Freescale (C) MAG3110 Magnetometer

    Tested with MAG3110 Breakout from Sparkfun and Arduino Uno

    Based on Sparkfun's example for the MAG3110 breakout board:
    http://dlnmh9ip6v2uc.cloudfront.net/datasheets/BreakoutBoards/Mag3110_v10.pde

    @author Jeroen Cappaert & Lara Booth for NanoSatisfi
    @author Jorge Ortiz
    @date May 2013
*/

#ifndef SAT_MAG_H
#define SAT_MAG_H

#include <Arduino.h>

class SAT_Mag
{
  public:
    /**Constructor that has a single ID parameter. */
    SAT_Mag();

    /**Constructor that has a single ID parameter.
    @param node_id - The id of the user's arduino on the ArduSat. This allows 
    the supervisor to know which arduino node to send the data back to. 
    The id is assigned by NanoSatisfi.
    */
    // void init(uint8_t node_id); // jfomhover on 07/08/2013 : function not used ? furthermore, the "node_id" should be given by a "central power" (I2CComm)


    /**Initializes the magnetometer. */
    void configMag();

    /**Reads the x vector. */
    int readx();

    /**Reads the y vector. */
    int ready();

    /**Reads the z vector. */
    int readz();

    /**Reads the x offset value. */
    float x_value();

    /**Reads the y offset value. */
    float y_value();

    /**Reads the z offset value. */
    float z_value();

    /**Returns the heading.
    @param x the x vector. Typically, you would pass the value you recieved from readx();
    @param y the y vector. Typically, you would pass the value you recieved from ready();
    @param z the z vector. Typically, you would pass the value you recieved from readz();
    */
    int getHeading(float x, float y, float z);

  private:
    //uint8_t _local_address; // jfomhover on 07/08/2013 : handled by I2CComm
    // byte _buff[2]; // jfomhover on 07/08/2013 : unused in the class ??

    // class-only methods:
    // int read16Data(char msg_reg, char lsb_reg); // jfomhover on 07/08/2013 : handled by I2CComm

    //global variables
    float mag_x_scale;
    float mag_y_scale;
    float mag_z_scale;
};


#endif /* SAT_MAG_H */
