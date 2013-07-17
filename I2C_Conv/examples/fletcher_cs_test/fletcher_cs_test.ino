/*
Testing fletcher checksum

for Fletcher's algorithm see:
Fletcher, J.G., 1982. An Arithmetic Checksum for Serial Transmissions. 
IEEE transactions on communications, COM-30(1).

- make random datastream
- fletcher encode
- fletcher decode

(c) Jeroen Cappaert for NanoSatisfi 2012
*/

#include <I2C_Conv.h>


byte buffer1[10];
byte buffer2[20];
byte buffer3[30];

int cycleTime = 3000;
long checkSum;

void setup(){
  Serial.begin(19200);
  Serial.println("Serial initialized");
  
}


void loop(){
  
  Serial.println("-- testing buffer1 --");
  testFletcher(buffer1, sizeof(buffer1));
  delay(1000);
  
  Serial.println("-- testing buffer2 --");
  testFletcher(buffer2, sizeof(buffer2));
  delay(1000);
  
  Serial.println("-- testing buffer3 --");
  testFletcher(buffer3, sizeof(buffer3));
  delay(1000);
  
  delay(cycleTime);
}

// test fletcher decoding/encoding
void testFletcher(byte buffer[], int len){
  Serial.println("---------------------------------------");
  Serial.println("making random bytestream");
  fillBuffer(buffer, len);
  printBuffer(buffer, len);
  
  Serial.println(" ");
  Serial.println("Decoding Fletcher checksum. Result: ");
  checkSum = fletcher_decode(buffer,len);
  Serial.println(checkSum);
  
  Serial.println(" ");
  Serial.println("encoding Fletcher checksum on bytestream");
  fletcher_encode(buffer, len);
  printBuffer(buffer, len);
  
  Serial.println(" ");
  Serial.println("Decoding Fletcher checksum. Result: ");
  checkSum = fletcher_decode(buffer,len);
  Serial.println(checkSum);
  
  Serial.println("---------------------------------------");
  Serial.println(" ");
  
}


// fill buffer with random bytes, leave last two bytes zero
void fillBuffer(byte buffer[], int len){
  for (int i=0; i<len-2; i++){
   buffer[i] = (byte)random(); 
  }
  buffer[len-1] = 0;
  buffer[len-2] = 0;
}

// print buffer on Serial 
void printBuffer(byte buffer[], int len){
 for (int i=0; i<len; i++){
  Serial.print(buffer[i],HEX);
  Serial.print(" ");
 }
 Serial.println(" "); 
}
