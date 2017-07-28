//Summer 2017 internship by Sierra Dacey and Molly Nehring
//The orangutans are powered with a battery (separate from raspberry pi)
//Power them with the red and black wires that have silver tape on them.
//Orangutans must be powered to be programmed

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
int browMotor;// motor M2
int neckMotor;// motor M1
int motorSmooth = 5;//milliseconds delay between succesive 
                    //motor speeds on the way to final speed
//Standard Motor Speeds
int forwardCruz = 80;
int backCruz = -80;

//Variables used to manage I2C communication;
int lspd = 0, ldir = 0, rspd = 0, rdir = 0;
byte I2Cbuffer[8]; //Used to hold received information
volatile boolean recFlag = false; //True is new command

int riseChange = 225;
int lowerChange = -225;
int upChange = 225;
int downChange = -225;
float browInterval;
float neckInterval;
int browTop = 900;
int browBottom =400;
int neckTop;
int neckBottom;
int browInput = 5;
int neckInput = 5;

//Calibrating Everything
void setup() {
  // put your setup code here, to run once:
  //myservo.attach(9);
  pinMode(7, INPUT);
  Wire.begin(10);                // join i2c bus with address #10
  Wire.onReceive(receiveEvent); // register event
  calibrateBrow();  //eyebrows click when calibrated
  calibrateNeck();  
}

int encodeBrow;
int encodeNeck;
double browSetAs;
double neckSetAs;


void loop() {
  if(recFlag){
    recFlag=false;
    int browInput = I2Cbuffer[2];
    int neckInput = I2Cbuffer[4];
    delay(500);

    browInterval = (browTop - browBottom)/10.0;  //different intervals of the eybrows' height
    browSetAs = (browInput * browInterval) + browBottom;

    neckInterval = (neckTop - neckBottom)/10.0;   //different intervals of the neck's bend
    neckSetAs = (neckInput * neckInterval) + neckBottom;
    
  }

  encodeBrow = analogRead(2);
  if ( abs(encodeBrow - browSetAs)< 7){  
    chgBrowSpeed(0);
    


  } else if (encodeBrow > browSetAs) { //make eyebrows raise
    chgBrowSpeed(riseChange);
  } else if (encodeBrow < browSetAs){  //make eyebrows lower
    chgBrowSpeed(lowerChange);
  }

  encodeNeck = analogRead(1);
  if ( abs(encodeNeck - neckSetAs)<7){  
      chgNeckSpeed(0);
//    leds.red(HIGH);
//    delay(50);
//    leds.red(LOW);
//    delay(50);
    
   if ( abs(encodeBrow - browSetAs)<7){  
     
//      leds.red(HIGH);
//      delay(50);
//      leds.red(LOW);
//      delay(50);
  
   }
  } else if (encodeNeck > neckSetAs) { //make neck look up
    chgNeckSpeed(upChange);
  } else if (encodeNeck < neckSetAs){ //make neck look down
    chgNeckSpeed(downChange);
  }

}

void calibrateBrow(){
  int topSpot = 900;
 // 
//  chgBrowSpeed(220);
//  delay(200);

while(true){
  int nowSpot = analogRead(2);

  if(nowSpot < topSpot){
    chgBrowSpeed(200);
    delay(200);
    chgBrowSpeed(0);
  } else {
    chgBrowSpeed(0);
   // leds.red(HIGH);
    break;
   }
}
  //
  
 }

void calibrateNeck(){
  chgNeckSpeed(downChange);
  delay(500);
  neckBottom = (analogRead(1) + 5);
  
  chgNeckSpeed(upChange);
  delay(500);
  neckTop = (analogRead(1) - 5);

  neckInterval = (neckTop - neckBottom)/10.0;
  neckSetAs = (neckInput * neckInterval) + neckBottom;
 }


 void chgBrowSpeed(int newSp){
    if(newSp == browMotor)return;
    double diff = newSp - browMotor;
    double delta = diff/11;
    
    for(double s = browMotor; abs(s-newSp)<=abs(delta); s+=delta){
      motors.setM2Speed(s);
      delay(motorSmooth);
    }
    browMotor = newSp;
    motors.setM2Speed(newSp);
 }

 void chgNeckSpeed(int newSp){
    if(newSp == neckMotor)return;
    double diff = newSp - neckMotor;
    double delta = diff/11;
    
    for(double s=neckMotor; abs(s-newSp)<=abs(delta); s+=delta){
      motors.setM1Speed(s);
      delay(motorSmooth);
     }
    neckMotor = newSp;
    motors.setM1Speed(newSp);
 }
 
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
 

