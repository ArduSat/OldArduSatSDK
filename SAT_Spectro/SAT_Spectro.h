/** @brief  Library to read the spectrometer.
	
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


#ifndef SPECTRO_H
#define SPECTRO_H

#include <inttypes.h>
#include <stdlib.h>

#include <Arduino.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>

#define SEPCTRO_SAMPLE_SIZE 500

class SAT_Spectro
{
  private:
    uint8_t _local_address;         //local address of this node
    OnboardCommLayer* commLayer;
    char m[3];

  public:
    /**Constructor that has a single ID parameter.
    @param id - The id of the user's arduino on the ArduSat. This allows 
    the supervisor to know which arduino node to send the data back to. 
    The id is assigned by NanoSatisfi.
    */
    SAT_Spectro(uint8_t id);

    /**Sets the exposure on the spectrometer.
    @param setting - A value from 1-99.
    */
    void set_exposure(uint8_t setting);

    /**Initiates the taking from a spectrometer sample */
    void take_sample(void);

    /**
     * @param function(size_t, boolean) size_t is returned chunk size,
     *                                  the boolean is a done flag
     * @param appbuf a pointer to the application buffer to be
     *          filled with a chunk of data from the spectruino
     * @param buffer_len the maximum buffer size of the application buffer
     */
    void onReceiveChunk(void (*) (size_t, boolean), byte*, size_t);
};

#endif /* SPECTRO_H */
