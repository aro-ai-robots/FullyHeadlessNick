//Summer 2017 internship by Sierra Dacey and Molly Nehring
//The orangutans are powered with a battery (separate from raspberry pi)
//Power them with the red and black wires that have silver tape on them.
//Orangutans must be powered to be programmed

#include <Wire.h>
#include <OrangutanLEDs.h>
//#include <OrangutanAnalog.h>
#include <OrangutanMotors.h>
#include <Servo.h>

//OrangutanAnalog analog
OrangutanLEDs leds;
OrangutanMotors motors;
//Turning Servo
Servo myservo;

//Variables used in controlling motor speeds
//30 lowest speed from dead stop
int eyeOpen = 0;// motor M1
int sideMotor = 0;// motor M2
int motorSmooth = 5;//milliseconds delay between succesive 
                    //motor speeds on the way to final speed
//Standard Motor Speeds
int forwardCruz = 80;
int backCruz = -80;

//Variables used to manage I2C communication
byte I2Cbuffer[8]; //Used to hold received information 
volatile boolean recFlag = false; //True is new command

//initialize variables for calculations
int openTop;
int openBottom;
int sideTop;
int sideBottom;
float openInterval;
float sideInterval;
int encodeOpen;
int encodeSide;
int eyeOpenChange = 225;   //top speed can be 250
int eyeCloseChange = -225; 
int rightChange = 225;
int leftChange = -225;
double openSetAs;
double sideSetAs;
int openInput = 5;
int sideInput = 5;


void setup() {
  // put your setup code here, to run once:
  //myservo.attach(9);
  pinMode(7, INPUT);
  Wire.begin(8);                // join i2c bus with address #8,corresponds to the device adress in python
  Wire.onReceive(receiveEvent); // register event
//  Wire.onRequest(requestEvent); // register event
  calibrateEyes();
}



void loop() {
  if(recFlag){
    recFlag=false;

    //buffer corrisponds to the positions in led_out in the python code     
    openInput = I2Cbuffer[2];
    sideInput = I2Cbuffer[4];


    //caluculations to scale the numbers sent to the encoder terms
    openInterval = (openTop - openBottom)/10.0;
    openSetAs = (openInput * openInterval) + openBottom;

    
    sideInterval = (sideTop - sideBottom)/10.0;
    sideSetAs = (sideInput * sideInterval) + sideBottom;
    
  }

  encodeOpen = analogRead(1);
  
  if ( abs(encodeOpen - openSetAs)< 20){  //if the motor is close enough
    chgOpenSpeed(0);

  } else if (encodeOpen > openSetAs) { //make eyes open more
    chgOpenSpeed(eyeCloseChange);
  } else if (encodeOpen < openSetAs){ //make eyes close more
    chgOpenSpeed(eyeOpenChange);
  }

  encodeSide = analogRead(2);
  if ( abs(encodeSide - sideSetAs)<7){
    chgSideSpeed(0);  
    if ( abs(encodeOpen - openSetAs)< 20){ //if both are close enough stabalize
      chgSideSpeed(0);
      leds.red(HIGH);
      delay(50);
      leds.red(LOW);
      delay(50);
    }
    
  } else if (encodeSide < sideSetAs) { //make eyes close 
    chgSideSpeed(eyeCloseChange);
  } else if (encodeSide > sideSetAs){ //make eyes open
    chgSideSpeed(eyeOpenChange);
  }
 
 }

 void calibrateEyes(){
  //set values for open closed

  chgOpenSpeed(eyeCloseChange);     //go all the way down and save value
  delay(500);
  openBottom = (analogRead(1) + 5);
  
  chgOpenSpeed(eyeOpenChange);      //go all the way up and save value
  delay(500);
  openTop = (analogRead(1) - 5);

  openInterval = (openTop - openBottom)/10.0;   //use valuse to calculate intervals
  openSetAs = (openInput * openInterval) + openBottom;

  chgSideSpeed(leftChange);
  delay(1000);
  sideTop = (analogRead(5) - 80);

  chgSideSpeed(rightChange);
  delay(1000);
  sideBottom = (analogRead(2) + 3);

  sideInterval = (sideTop - sideBottom)/10.0;
  sideSetAs = (sideInput * sideInterval) + sideBottom;
  
 }
 
 void chgOpenSpeed(int newSp){
    if(newSp == eyeOpen)return;
    double diff = newSp - eyeOpen;
    double delta = diff/11;
    
    for(double s = eyeOpen; abs(s-newSp)<=abs(delta); s+=delta){
      motors.setM1Speed(s);
      delay(motorSmooth);
    }
    eyeOpen = newSp;
    motors.setM1Speed(newSp);
 }

 void chgSideSpeed(int newSp){
    if(newSp == sideMotor)return;
    double diff = newSp - sideMotor;
    double delta = diff/11;
    
    for(double s=sideMotor; abs(s-newSp)<=abs(delta); s+=delta){
      motors.setM2Speed(s);
      delay(motorSmooth);
     }
    sideMotor = newSp;
    motors.setM2Speed(newSp);
 }//*/
 
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  Wire.readBytes(I2Cbuffer,howMany);
  recFlag = true;
}

void requestEvent() {
  Wire.write("hello "); // respond with message of 6 bytes
  // as expected by master
}
 

