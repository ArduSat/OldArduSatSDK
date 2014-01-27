#include <Wire.h>
#include <EEPROM.h>
#include <OnboardCommLayer.h>
#include <SAT_Lum.h>

// Example for demonstrating the SAT_Lum library - public domain!

// connect SCL to analog 5
// connect SDA to analog 4
// connect VDD to 3.3V DC
// connect GROUND to common ground
// ADDR can be connected to ground, or vdd or left floating to change the i2c address

// The argument corresponds to the address of the luminosity sensor
// use 1 for I2C_ADD_LUX1 (0x29) - TSL2561 #1 bottomplate camera
// use 2 for I2C_ADD_LUX2 (0x39) - TSL2561 #2 bottomplate slit
SAT_Lum tsl(1);

void setup(void) {
  Serial.begin(115200);
  OBCL.begin();

  if (tsl.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No sensor?");
    while (1);
  }

  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(SAT_Lum_GAIN_0X);         // set no gain (for bright situtations)
  tsl.setGain(SAT_Lum_GAIN_16X);      // set 16x gain (for dim situations)

  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  tsl.setTiming(SAT_Lum_INTEGRATIONTIME_13MS);  // shortest integration time (bright light)
  //tsl.setTiming(SAT_Lum_INTEGRATIONTIME_101MS);  // medium integration time (medium light)
  //tsl.setTiming(SAT_Lum_INTEGRATIONTIME_402MS);  // longest integration time (dim light)

  // Now we're ready to get readings!
}

void loop(void) {
  // Simple data read example. Just read the infrared, fullspecrtrum diode 
  // or 'visible' (difference between the two) channels.
  // This can take 13-402 milliseconds! Uncomment whichever of the following you want to read
  uint16_t x = tsl.getLuminosity(SAT_Lum_VISIBLE);
  //uint16_t x = tsl.getLuminosity(SAT_Lum_FULLSPECTRUM);
  //uint16_t x = tsl.getLuminosity(SAT_Lum_INFRARED);

  Serial.println(x, DEC);

  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print("IR: "); Serial.print(ir);   Serial.print("\t\t");
  Serial.print("Full: "); Serial.print(full);   Serial.print("\t");
  Serial.print("Visible: "); Serial.print(full - ir);   Serial.print("\t");

  Serial.print("Lux: "); Serial.println(tsl.calculateLux(full, ir));

  delay(100);
}
