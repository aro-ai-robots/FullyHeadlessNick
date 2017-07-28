//Summer 2017 intern ship by Sierra Dacey and Molly Nehring
//Orangutans are powered separate from the pi with a battery
//Use the power/ground wires with silver tape
//Orangutans must be powered to be programmed
//**THE MOUTH OPEN IS NOT CURRENTLY WORKING (MOTOR IS STUCK)**

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
int mouthOpen;// motor M2
int expressionMotor;// motor M1
int motorSmooth = 5;//milliseconds delay between succesive 
                    //motor speeds on the way to final speed
//Standard Motor Speeds
int forwardCruz = 80;
int backCruz = -80;

//Variables used to manage I2C communication;
int lspd = 0, ldir = 0, rspd = 0, rdir = 0;
byte I2Cbuffer[8]; //Used to hold received information
volatile boolean recFlag = false; //True is new command

int openChange = 225;
int closeChange = -225;
int happyChange = 225;
int sadChange = -225;
float openInterval;
float expressionInterval;
int openTop;
int openBottom;
int expressionTop;
int expressionBottom;
int openInput = 5;
int expressionInput = 5;


void setup() {
  // put your setup code here, to run once:
  //myservo.attach(9);
  pinMode(7, INPUT);
  Wire.begin(9);                // join i2c bus with address #9
  Wire.onReceive(receiveEvent); // register event
//  Wire.onRequest(requestEvent); // register event
  calibrateMouth();
}

int encodeOpen;
int encodeExpression;
int negativeChange = 225;
int positiveChange = -225;
double openSetAs;
double expressionSetAs;

void loop() {
  if(recFlag){
    recFlag=false;
    int openInput = I2Cbuffer[2];
    int expressionInput = I2Cbuffer[4];
        
    //lspd = I2Cbuffer[2]; ldir = I2Cbuffer[3];
    //rspd = I2Cbuffer[4]; rdir = I2Cbuffer[5]; 
    delay(500);
//    int openTop = 600;
//    int openBottom = 465;
    openInterval = (openTop - openBottom)/10.0;  //different intervals the mouth can open at
    openSetAs = (openInput * openInterval) + openBottom;

//    int expressionTop = 780;
//    int expressionBottom = 220;
    expressionInterval = (expressionTop - expressionBottom)/10.0; //different intervals the mouth can smile/frown at
    expressionSetAs = (expressionInput * expressionInterval) + expressionBottom;
    
  }

  encodeOpen = analogRead(1);
  if ( abs(encodeOpen - openSetAs)< 7){  
    chgOpenSpeed(0);
//    leds.red(HIGH);
//    delay(50);
//    leds.red(LOW);
//    delay(50);
  
  } else if (encodeOpen > openSetAs) { //make mouth open more
    chgOpenSpeed(positiveChange);
  } else if (encodeOpen < openSetAs){ //make mouth close more
    chgOpenSpeed(negativeChange);
  }

  encodeExpression = analogRead(2);
  if ( abs(encodeExpression - expressionSetAs)<7){  
      chgExpressionSpeed(0);
    leds.red(HIGH);
    delay(50);
    leds.red(LOW);
    delay(50);
    
//   if ( abs(encodeOpen - openSetAs)<25){  
//     // chgOpenSpeed(0);
//      leds.red(HIGH);
//      delay(50);
//      leds.red(LOW);
//      delay(50);
//    }
  } else if (encodeExpression > expressionSetAs) { //make mouth smile more
    chgExpressionSpeed(positiveChange);
  } else if (encodeExpression < expressionSetAs){ //make mouth frown more
    chgExpressionSpeed(negativeChange);
  }
  
 //Check Sensors//*/

 }
 
 void chgOpenSpeed(int newSp){
    if(newSp == mouthOpen)return;
    double diff = newSp - mouthOpen;
    double delta = diff/11;
    
    for(double s = mouthOpen; abs(s-newSp)<=abs(delta); s+=delta){
      motors.setM2Speed(s);
      delay(motorSmooth);
    }
    mouthOpen = newSp;
    motors.setM2Speed(newSp);
 }

 void calibrateMouth(){
  //set values for open closed

  chgOpenSpeed(closeChange);
  delay(500);
  openBottom = (analogRead(1) + 5);
  
  chgOpenSpeed(openChange);
  delay(500);
  openTop = (analogRead(1) - 5);

  openInterval = (openTop - openBottom)/10.0;
  openSetAs = (openInput * openInterval) + openBottom;

  chgExpressionSpeed(happyChange);
  delay(1000);
  expressionTop = (analogRead(5) - 5);

  chgExpressionSpeed(sadChange);
  delay(1000);
  expressionBottom = (analogRead(2) + 5);

  expressionInterval = (expressionTop - expressionBottom)/10.0;
  expressionSetAs = (expressionInput * expressionInterval) + expressionBottom;
  
 }

 void chgExpressionSpeed(int newSp){
    if(newSp == expressionMotor)return;
    double diff = newSp - expressionMotor;
    double delta = diff/11;
    
    for(double s=expressionMotor; abs(s-newSp)<=abs(delta); s+=delta){
      motors.setM1Speed(s);
      delay(motorSmooth);
     }
    expressionMotor = newSp;
    motors.setM1Speed(newSp);
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
 

