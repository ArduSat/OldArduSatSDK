/*
    file : I2CCM_OnboardExample.ino
    content : for ArduSat, shows what the OnboardCommLayer does
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
#include <OnboardCommLayer.h> // no need here

nanosat_message_t msg;
char messageStr[] = "allo Houston ?";

void setup() {  
  Serial.begin(9600);
  OBCL.begin();
}

void loop() {
  int addr = I2C_ADD_ASSV_1;
  uint8_t retByte = 0;
  int timeout = 500; // 500 ms

  msg.node_addr = EEPROM.read(0x00);
  msg.prefix    = NODE_COMM_MESSAGE_PREFIX;
  msg.len       = strlen(messageStr);
  msg.type      = APPEND;
  memcpy(msg.buf, (uint8_t*)messageStr, strlen(messageStr));
  
  int8_t t_ret = OBCL.sendBuffer(addr, (byte*)&msg, sizeof(nanosat_message_t));
  printReturnMessage(t_ret);

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
