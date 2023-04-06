#include <Arduino.h>
#include <Stepper.h>

//X axis sensor 
#define IR_PIN 32

//Motor for X axis
#define xIN1 15
#define xIN2 2
#define xIN3 4
#define xIN4 16

//Motor for Y axis
#define yIN1 17
#define yIN2 5
#define yIN3 18
#define yIN4 19

//Motor for Z axis
#define zIN1 22
#define zIN2 1
#define zIN3 3
#define zIN4 21

const int stepPerRevolution = 100;

unsigned int timeLoop;
unsigned int timeStart;
bool spinMotorX = true;
double distance;

int value;
int homingState = 0;
int distance_ir = 0;

Stepper motorX(stepPerRevolution, xIN1, xIN3, xIN2, xIN4);
Stepper motorY(stepPerRevolution, yIN1, yIN3, yIN2, yIN4);
Stepper motorZ(stepPerRevolution, zIN1, zIN3, zIN2, zIN4);

void setup(){
  //IR pinmode setup
  pinMode(IR_PIN, INPUT);

  //motor speed setup
  motorX.setSpeed(25);
  motorY.setSpeed(25);
  motorZ.setSpeed(25);

  //debug monitor setup
  Serial.begin(9600);
}

int IRSensor(){
  int sensorValue = analogRead(IR_PIN);
  distance = 12.08 * pow(sensorValue / 1024.0, -1.204);               
  timeStart = millis();
  return distance;                     
}

void moveMotorX(){
  while (spinMotorX == true){
    value = IRSensor();
    Serial.println(value);
    motorX.step(stepPerRevolution);

    if(value<=10){
      homingState = 1;
      spinMotorX = false;
    }
  }
 }
void loop(){
  switch(homingState){
    case 0:
      moveMotorX();
      break;
    case 1:
      Serial.println("koniec kurvy");
      break;
  }

  //motorZ.step(stepPerRevolution);
  //motorX.step(stepPerRevolution);
 // motorY.step(stepPerRevolution);
 // delay(1000);

}

