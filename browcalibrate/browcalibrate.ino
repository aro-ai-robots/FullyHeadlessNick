//Summer 2017 internship by Sierra Dacey and Molly Nehring
//The orangutans are powered with a battery (separate from raspberry pi)
//Power them with the red and black wires that have silver tape on them.

#include <OrangutanMotors.h>
#include <OrangutanLEDs.h>

OrangutanMotors motors;
OrangutanLEDs leds;

// Variables for they eyebrow's different positions
int browMotor;
int topSpot;
int nowSpot;

int motorSmooth = 5;

void setup() {
  // put your setup code here, to run once:
//  chgBrowSpeed(200);
//  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  topSpot = 900;
  
  chgBrowSpeed(220);
  delay(200);
  chgBrowSpeed(0);
  nowSpot = analogRead(1);

  for(int i = 0; i < (nowSpot / 100); i++){
      leds.red(HIGH);
      delay(100);
      leds.red(LOW);
      delay(100);
  }
  delay(500);

//  if (nowSpot < topSpot){
//    chgBrowSpeed(200);
//    delay(500);
//    leds.red(HIGH);
//    delay(50);
//    leds.red(LOW);
//    delay(50);
//    nowSpot = analogRead(1);
//  } else {
//      for (int i = 1; i < 3; i ++){
//      leds.red(HIGH);
//      delay(500);
//      leds.red(LOW);
//      delay(500);
//    }
//    delay(1000);
//  }
}



void chgBrowSpeed(int newSp){
    if(newSp == browMotor)return;
    double diff = newSp - browMotor;
    double delta = diff/11;
    
    for(double s=browMotor; abs(s-newSp)<=abs(delta); s+=delta){
      motors.setM1Speed(s);
      delay(motorSmooth);
     }
    browMotor = newSp;
    motors.setM1Speed(newSp);
}
