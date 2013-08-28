#include <Wire.h>
#include "I2C_add.h"
#include "OnboardCommLayer.h"
#include "nanosat_message.h"

OnboardCommLayer ocl;
uint8_t nodeAddress;

void setup()
{
    Serial.being(115200);
    Wire.begin();
    ocl         = OnboardCommLayer();
    nodeAddress = EEPROM.read(0x00);
    Serial.println("Initialized Serial and I2C buses");
}

void loop()
{
    nanosat_message_t msg;
    msg.node_addr = nodeAddress;
    msg.prefix    = NODE_COMM_MESSAGE_PREFIX;
    msg.len       = length;
    msg.type      = CAM;
    commLayer_.sendMessage(msg);
    delay(1000);
    ocl.sendExit();
    delay(1000);
}
