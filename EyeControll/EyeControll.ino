#include <Wire.h>
#include <OrangutanLEDs.h>
//#include <OrangutanAnalog.h>
#include <OrangutanMotors.h>
#include <Servo.h>

//OrangutanAnalog analog;
OrangutanLEDs leds;
OrangutanMotors motors;
//Turning Servo
Servo myservo;

//Variables used in controlling motor speeds
//30 lowest speed from dead stop
int eyeOpen = 0;// motor M2
int sideMotor = 0;// motor M1
int motorSmooth = 5;//milliseconds delay between succesive 
                    //motor speeds on the way to final speed
//Standard Motor Speeds
int forwardCruz = 80;
int backCruz = -80;

//Variables used to manage I2C communication;
int lspd = 0, ldir = 0, rspd = 0, rdir = 0;
byte I2Cbuffer[8]; //Used to hold received information
volatile boolean recFlag = false; //True is new command

int openTop;
int openBottom;


void setup() {
  // put your setup code here, to run once:
  //myservo.attach(9);
  pinMode(7, INPUT);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
//  Wire.onRequest(requestEvent); // register event
}

int encodeOpen;
int encodeSide;
int negativeChange = 225;
int positiveChange = -225;
double openSetAs;
double sideSetAs;
int openInput = 5;
int sideInput = 5;

void loop() {
  if(recFlag){
    recFlag=false;
    openInput = I2Cbuffer[2];
    sideInput = I2Cbuffer[4];
    
    //lspd = I2Cbuffer[2]; ldir = I2Cbuffer[3];
    //rspd = I2Cbuffer[4]; rdir = I2Cbuffer[5]; 
    //delay(500);
    
    
    //int openTop = 700;
    //int openBottom = 420;
    calibrateEyes();
    float openInterval = (openTop - openBottom)/10.0;
    openSetAs = (openInput * openInterval) + openBottom;

    int sideTop = 600;
    int sideBottom = 430;
    float sideInterval = (sideTop - sideBottom)/10.0;
    sideSetAs = (sideInput * sideInterval) + sideBottom;
    
  }
/*
  for(int i = 0; i< openInput; i++){
      leds.red(HIGH);
      delay(200);
      leds.red(LOW);
      delay(200);
  }
  delay(1000);*/

  encodeOpen = analogRead(1);
  for(int i = 0; i< encodeOpen/70; i++){
      leds.red(HIGH);
      delay(200);
      leds.red(LOW);
      delay(200);
  }
  delay(1000);
  
  if ( abs(encodeOpen - openSetAs)<10){  
    chgOpenSpeed(0);
    leds.red(HIGH);
    delay(50);
    leds.red(LOW);
    delay(50);
  
  } else if (encodeOpen > openSetAs) { //make eyes open more
    chgOpenSpeed(positiveChange);
  } else if (encodeOpen < openSetAs){
    chgOpenSpeed(negativeChange);
  }

  encodeSide = analogRead(2);
  if ( abs(encodeSide - sideSetAs)<10){  
      chgSideSpeed(0);
//    leds.red(HIGH);
//    delay(50);
//    leds.red(LOW);
//    delay(50);
    if ( abs(encodeOpen - openSetAs)<7){  
     // chgOpenSpeed(0);
      leds.red(HIGH);
      delay(50);
      leds.red(LOW);
      delay(500);
      
    }
  } else if (encodeSide > sideSetAs) { //make eyes open more
    chgSideSpeed(positiveChange);
  } else if (encodeSide < sideSetAs){
    chgSideSpeed(negativeChange);
  }
  
 //Check Sensors//*/

 }

 void calibrateEyes(){
  //set values for open closed

  chgOpenSpeed(positiveChange);
  delay(500);
  openTop = analogRead(1);
  
  chgOpenSpeed(negativeChange);
  delay(500);
  openBottom = analogRead(1);
  
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
 

