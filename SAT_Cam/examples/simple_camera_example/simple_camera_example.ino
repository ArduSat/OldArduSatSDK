#include <Wire.h>
#include <EEPROM.h>
#include <OnboardCommLayer.h>
//#include "I2C_add.h"
//#include "nanosat_message.h"

OnboardCommLayer ocl;
uint8_t nodeAddress;
bool    halted;

void setup()
{
    Serial.begin(115200);
    //    Wire.begin();
    halted      = false;
//	ocl         = OnboardCommLayer();
//    nodeAddress = EEPROM.read(0x00);
    OBCL.begin();
    Serial.println("Initialized Serial and I2C bus");
}

void loop()
{
    if(!halted) {
        nanosat_message_t msg;
        msg.node_addr = nodeAddress;
        msg.prefix    = NODE_COMM_MESSAGE_PREFIX;
        msg.type      = CAM;
        ocl.sendMessage(msg);
        delay(1000);
        ocl.sendExit();
        delay(1000);
        halted = true;
    }
}
