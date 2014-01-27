/*
    file : I2CCM_LumExample.ino
    content : for ArduSat, queries the tsl2561 sensor to check if it's present, with a full error handling
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
#include <Wire.h>
#include <EEPROM.h>
#include <OnboardCommLayer.h>

void printReturnMessage(int ret);

void setup() {  
  Serial.begin(9600);
  OBCL.begin();
}

void loop() {
  int addr = I2C_ADD_LUX1;
  int content = 0x0A; // SAT_Lum_REGISTER_ID
  uint8_t retByte = 0;
  int timeout = 500; // 500 ms

  int8_t t_ret = OBCL.exchangeByte(addr, content, &retByte, timeout);
  printReturnMessage(t_ret);
  
  if ((retByte ^ 0x0A) == 0) {
    Serial.println("Found SAT_Lum");
  } else {
    Serial.println("NOT Found SAT_Lum");
  }
    delay(1000);
}

void printReturnMessage(int t_ret) {
  if (t_ret < 0) {
    switch(t_ret) {
      case I2C_COMM_ERRORTIMEOUT:
        Serial.println("! Error: Timeout reached");
        break;
      case I2C_COMM_ERRORNOWRITE:
        Serial.println("! Error: Data written");
        break;
      case I2C_COMM_ERRORTOOMUCHDATA:
        Serial.println("! Error: Data sent exceeds internal buffer");
        break;
      case I2C_COMM_ERRORNACKADDR:
        Serial.println("! Error: NACK addr received");  // typical return value when there's no OnboardCommLayer listening / connected
        break;
      case I2C_COMM_ERRORNACKDATA:
        Serial.println("! Error: NACK data received");
        break;
      case I2C_COMM_ERROR:
      default:
        Serial.println("! Error: type unknown");
        break;
    }
  } else {
    Serial.print("exchange OK, ");
    Serial.print(t_ret);
    Serial.println(" bytes sent.");
  }
}
