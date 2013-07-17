/* 
Processing program to visualize MAG3110 readout
Modified for the MAG3110 by
Lara Booth for NanoSatisfi
--------------------------------------
Based on the work by Windell H. Oskay:
--------------------------------------
"Read about this sketch here: http://www.evilmadscientist.com/article.php/fields

Written for Processing 1.0 by Windell H. Oskay, 2010.

You may want to edit the correction factors (magnetic field zero point),
the serial port name, and what data is printed.

Present data printed: X, Y, Z field (mG, approx), Sum field magnitude (mg), Max X, Min X, Max Y, Min Y, Max Z, Min Z.

Type 0, 1, 2, or 3 for xyz, x, y, or z view.

If you rotate the sensor through all possible positions, spanning the maximum orientation along +x, -x, +y, -y, +z, and -z,
then when you end the program by clicking in the window, the data printed will include new offset values-- assuming that your
data should be centered at the origin."
*/




import processing.opengl.*;
import processing.serial.*;


//Correction factors, in milligauss.  Can be signed.  The program will print out new possible values of these when you stop.
float xOffs =  -276.0;
float yOffs = 73.0;
float zOffs = 1251.0;



int bgcolor;			     // Background color

Serial myPort;                       // The serial port
int[] serialInArray = new int[6];    // Where we'll put what we receive
int serialCount = 0;                 // A count of how many bytes we receive
int xpos, ypos, zpos, tpos;		            
boolean firstContact = false;        // Whether we've heard from the microcontroller
boolean DrawAxes;


boolean  BoundariesInitialized;
boolean SerialOnline;
int xMax,xMin,yMax,yMin,zMax,zMin;

int i;

boolean newPoint;
float newX, newY, newZ, newT;

int FGcolorVal;    // Global
int Dummy;    // Global

 
color FgColorFunc(int Dummy) { 
  
int rComp = 150;
int gComp = 150;
int bComp = 150;

FGcolorVal += 3;

if (FGcolorVal >= 768)
  FGcolorVal = 0;

if (FGcolorVal < 256)
{
rComp = 255 - FGcolorVal;
gComp = FGcolorVal;
}
else if (FGcolorVal < 512)
{
gComp = 255 - (FGcolorVal - 256);
bComp = (FGcolorVal - 256);
}
else if (FGcolorVal < 768)
{
bComp = 255 - (FGcolorVal - 512);
rComp = (FGcolorVal - 512);
}  
color colortemp = color(rComp, gComp, bComp);

return colortemp;
}



void setup() {


  //size(800, 600, P3D); 
  size(800, 600, OPENGL); 
 
  lights();

  noStroke(); 


  // Set the starting position of the ball (middle of the stage)
  xpos = width/2;
  ypos = height/2;

  // Print a list of the serial ports, for debugging purposes:
  
int PortCount = Serial.list().length;
  int PortNumber = -1;
  
// Serial port search string:  

  String str1 = "COM5";       // Can change to be the name of the port you want, e.g., COM5.
  // The default value is "/dev/cu.usbserial-"; which works on Macs.
  String str2; 

  int j = 0;
  while (j < PortCount){
    str2 = Serial.list()[j].substring(0,4);
    if(str1.equals(str2) == true)
      PortNumber = j;
    j++; 
  }
  
  
SerialOnline = false;

  if (PortNumber >= 0)
  { 
    String portName = Serial.list()[PortNumber];
    myPort = new Serial(this, portName, 57600);
    myPort.buffer(1);
    myPort.clear(); 
    println("Serial port "+Serial.list()[PortNumber]+" found and activated.");
    SerialOnline = true;
  } 
  else
  {
    println("\nI found "+PortCount+" serial ports, which are:");
    println(PortNumber);
    println(Serial.list());
    println("But I don't know which one to use. :(\n"); 
    println("Now entering offline simulation mode.\n");
    
  }   
  
  
  
  println("Press keys '0' through '3' for angle XYZ, X, Y or Z.\n");
  
  
  
  /*
// Fixed serial config:
  println(Serial.list());
  String portName = Serial.list()[3];
  myPort = new Serial(this, portName, 57600);
*/

  background(255);

  noStroke();

  newPoint = false;
  DrawAxes = true;

  BoundariesInitialized = false;
 FGcolorVal = 0;

}

int CameraType = 0;  // 0: XYZ, 1: X, 2: Y, 3: Z

void draw() { 
  
   if(keyPressed) {
    if(key >= '0' && key <= '3') {
      DrawAxes = true;
      
      if(key == '0') {
        CameraType = 0;
        }
              if(key == '1') {
        CameraType = 1;
        }
              if(key == '2') {
        CameraType = 2;
        }
              if(key == '3') {
        CameraType = 3;
        }
    }
  }
  
  
  lights();
  ambientLight(5,5,5); 

  if (CameraType == 0){
    // XYZ camera:
    camera(300,-300,300, // eyeX, eyeY, eyeZ
    0.0, 0.0, 0.0, // centerX, centerY, centerZ
    0, 0, -1); // upX, upY, upZ
  }

  if (CameraType == 1){
    // Along X:
    camera(500,0,0, // eyeX, eyeY, eyeZ
    0.0, 0.0, 0.0, // centerX, centerY, centerZ
    0, 0, -1); // upX, upY, upZ
  }

  if (CameraType == 3){
    // Along Z:
    camera(0,0,500, // eyeX, eyeY, eyeZ
    0.0, 0.0, 0.0, // centerX, centerY, centerZ
    0, 1, 0); // upX, upY, upZ
  }

  if (CameraType == 2){
    // Along Y:
    camera(0,-500,000, // eyeX, eyeY, eyeZ
    0.0, 0.0, 0.0, // centerX, centerY, centerZ
    0, 0, -1); // upX, upY, upZ
  }

  if (DrawAxes == true)
  {

    
    DrawAxes = false;
  background(255);
  
    fill(255, 0, 0);

    i = 0;  
    while (i < 50){
      pushMatrix();
      translate(6*i,0,0); 
      sphere(3);
      popMatrix();
      i++;
    }

    pushMatrix();
    translate(100,0,0); 
    sphere(5);
    popMatrix();
    pushMatrix();
    translate(200,0,0); 
    sphere(5);
    popMatrix(); 


    fill(0, 255, 0);

    i = 0;  
    while (i < 50){
      pushMatrix();
      translate(0,-6*i,0); 
      sphere(3);
      popMatrix();
      i++;
    } 

    pushMatrix();
    translate(0,-100,0); 
    sphere(5);
    popMatrix();
    pushMatrix();
    translate(0,-200,0); 
    sphere(5);
    popMatrix(); 


    fill(0, 0, 255);

    i = 0;  
    while (i < 50){
      pushMatrix();
      translate(0,0,6*i); 
      sphere(3);
      popMatrix();
      i++;
    }    

    pushMatrix();
    translate(0,0,100); 
    sphere(5);
    popMatrix();
    pushMatrix();
    translate(0,0,200); 
    sphere(5);
    popMatrix(); 



  }


  if ( newPoint== true)
  {  
  //  fill (250);
     
 fill(FgColorFunc(1));

    newPoint = false;
    pushMatrix();
 
    translate(xpos/2, -1 * ypos/2, zpos/2); 

    sphere(3);
    popMatrix(); 




  }
  else if ( SerialOnline == false)
  {
    // Simulation Mode
  
      fill (250);

    pushMatrix();
 
      rotateX(random(2*PI));
     rotateY(random(2*PI));
     rotateX(random(2*PI));     
     translate(200,0,0); 
      
//    translate(xpos/2, -1 * ypos/2, zpos/2); 

    sphere(3);
    popMatrix(); 



  
  
  }




}




void mousePressed() {
        println(xpos + "\t" + ypos + "\t" + zpos + "\t" + tpos
          + "\t" + xMin  + "\t" + xMax
          + "\t" + yMin  + "\t" + yMax
          + "\t" + zMin + "\t" + zMax
          );

         println("Bx\tBy\tBz\t|B|\tBxMin\tBxMax\tByMin\tByMax\tBzMin\tBzMax");

float xt;
float yt;
float zt;

// Suggest new values for offsets:

     xt = float((xMax + xMin)/2) + xOffs;
     yt =  float((yMax + yMin)/2) + yOffs;
     zt =  float((zMax + zMin)/2)  + zOffs;
 
         println("Correction factors:\nfloat xOffs =  " +   xt +
           ";\nfloat yOffs = " +   yt +
           ";\nfloat zOffs = " +  zt + ";\n");
            
  
  
  exit(); 
}


void serialEvent(Serial myPort) {
  // read a byte from the serial port:
  int inByte = myPort.read();
  // if this is the first byte received, and it's an A,
  // clear the serial buffer and note that you've
  // had first contact from the microcontroller. 
  // Otherwise, add the incoming byte to the array:
  if (firstContact == false) {
    if (inByte == 'A') { 
      myPort.clear();          // clear the serial port buffer
      firstContact = true;     // you've had first contact from the microcontroller
      myPort.write('A');       // ask for more
    } 
  } 
  else {
    // Add the latest byte from the serial port to array:
    serialInArray[serialCount] = inByte;
    serialCount++;

    // If we have 6 bytes:
    if (serialCount > 5 ) {
      
      xpos = (serialInArray[0] << 8) | serialInArray[1];
      ypos = (serialInArray[2] << 8) | serialInArray[3];
      zpos = (serialInArray[4] << 8) | serialInArray[5]; 

      if (xpos > 32767)
      {
        xpos -= 32767;
        xpos = -xpos;
      }
      if (ypos > 32767)
      {
        ypos -= 32767;
        ypos = -ypos;
      } 
      if (zpos > 32767)
      {
        zpos -= 32767;
        zpos = -zpos;
      }
      
     // println("\nProcessing new data set. xpos: " + xpos + ", ypos: " + ypos + ", zpos: " + zpos);

   /*   if ((abs(xpos) < 4000) && (abs(ypos) < 4000) && (abs(zpos) < 4000))  // Omit overflow values
      { */
        newPoint = true;

        // Scale values:  2048 units/Gauss
        
        newX = (float)xpos/2048; 
        newY =  (float)ypos/2048;
        newZ =  (float)zpos/2048;   

        //Scale up to milligauss and subtract sensor offsets:
        
        newX = (1000 * newX)  - xOffs;  
        newY = (1000 * newY)  - yOffs;
        newZ = (1000 * newZ)  - zOffs;

        // Calculate magnitude of B field:
        newT = sqrt(newX * newX + newY * newY + newZ * newZ); 

        xpos = (int) round(newX);
        ypos = (int) round(newY);
        zpos = (int) round(newZ);
        tpos = (int) round(newT);

        if (BoundariesInitialized == false){
          BoundariesInitialized = true;

          xMin = xpos;
          xMax = xpos;
          yMax= ypos;
          yMin= ypos;
          zMax= zpos;
          zMin= zpos; 
        }

        if (xpos < xMin)
          xMin = xpos;
        if (ypos < yMin)
          yMin = ypos;
        if (zpos < zMin)
          zMin = zpos;
        if (xpos > xMax)
          xMax = xpos;
        if (ypos > yMax)
          yMax = ypos;    
        if (zpos > zMax)
          zMax = zpos;     


        // print the values:
        println(xpos + "\t" + ypos + "\t" + zpos + "\t" + tpos
          + "\t" + xMin  + "\t" + xMax
          + "\t" + yMin  + "\t" + yMax
          + "\t" + zMin + "\t" + zMax
          );
 /*
      } else {
        println("Values are 'overflow'");
      } */

      // Send a capital A to request new sensor readings:
      myPort.write('A');
      // Reset serialCount:
      serialCount = 0;
    }
  }
}




