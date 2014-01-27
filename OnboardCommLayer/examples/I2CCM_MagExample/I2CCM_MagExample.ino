/*
    file : I2CCM_MagExample.ino
    content : for ArduSat, it does the config of the SAT_Mag (like SAT_Mag.configMag()), with a full error handling
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
//#include <SAT_Mag.h> // no need here

void printReturnMessage(int ret);

void setup() {  
  Serial.begin(9600);
  OBCL.begin();
}

void loop() {
  int addr = I2C_ADD_MAG;
  int timeout = 500; // 500 ms
  byte commands[2];
  int8_t t_ret = 0;
  	
// configMag
  commands[0] = 0x11;  // cntrl register2
  commands[1] = 0x80;  // send 0x80, enable auto resets
  t_ret = OBCL.transmitByteArray(addr, commands, 2, timeout);
  printReturnMessage(t_ret);

  commands[0] = 0x10;  // cntrl register1
  commands[1] = 0x01;  // send 0x01, active mode
  t_ret = OBCL.transmitByteArray(addr, commands, 2, timeout);
  printReturnMessage(t_ret);

  while(1) {
    Serial.println("requesting X value...");
    int16_t mag_x = 0;
    t_ret = OBCL.request16bitsFromMSBLSB(addr, 0x01, 0x02, (uint16_t*)&mag_x);
    printReturnMessage(t_ret);
    Serial.print("magnetometer X = ");
    Serial.print(mag_x);
    Serial.println();

    Serial.println("requesting Y value...");
    int16_t mag_y = 0;
    t_ret = OBCL.request16bitsFromMSBLSB(addr, 0x03, 0x04, (uint16_t*)&mag_y);
    printReturnMessage(t_ret);
    Serial.print("magnetometer Y = ");
    Serial.print(mag_y);
    Serial.println();

    Serial.println("requesting Z value...");
    int16_t mag_z = 0;
    t_ret = OBCL.request16bitsFromMSBLSB(addr, 0x05, 0x06, (uint16_t*)&mag_z);
    printReturnMessage(t_ret);
    Serial.print("magnetometer Z = ");
    Serial.print(mag_z);
    Serial.println();
    delay(1000);
  }
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

