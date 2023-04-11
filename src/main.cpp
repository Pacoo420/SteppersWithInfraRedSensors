#include <Arduino.h>
#include <Stepper.h>

//X axis sensor 
#define IR1_PIN 32
#define IR2_PIN 35
#define IR3_PIN 34

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
#define zIN1 21
#define zIN2 26
#define zIN3 22
#define zIN4 23

const int stepPerRevolution = 2000;

unsigned int timeLoop;
unsigned int timeStart;
bool homing=false;

bool spinMotorX = false;
bool spinMotorY = false;
bool spinMotorZ = false;

double distance;

int value;
int homingState = 0;
int distance_ir = 0;

int currentMessage;
int lastMessage;

Stepper motorX(stepPerRevolution, xIN1, xIN3, xIN2, xIN4);
Stepper motorY(stepPerRevolution, yIN1, yIN3, yIN2, yIN4);
Stepper motorZ(stepPerRevolution, zIN1, zIN3, zIN2, zIN4);

void setup(){
  //IR pinmode setup
  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);
  pinMode(IR3_PIN, INPUT);

  //motor speed setup
  motorX.setSpeed(5); //was 25
  motorY.setSpeed(5);
  motorZ.setSpeed(5);

  //debug monitor setup
  Serial.begin(9600);
}

int IRSensor(int IR_PIN){
  int sensorValue = analogRead(IR_PIN);
  distance = 12.08 * pow(sensorValue / 1024.0, -1.204);               
  return distance;                     
}

void moveMotorX(){
  while (spinMotorX == true){
    int sensorReading = analogRead(A0);
    value = IRSensor(IR1_PIN);
    Serial.println(value);
    motorX.step(stepPerRevolution / 100);

    if(value<=10){
      homingState = 1;
      spinMotorX = false;
    }
  }
}

void moveMotorY(){
  while (spinMotorY == true){
    value = IRSensor(IR2_PIN);
    Serial.println(value);
    motorY.step(stepPerRevolution / 100);

    if(value<=10){
      homingState = 2;
      spinMotorY = false;
    }
  }
}

void moveMotorZ(){
  while (spinMotorZ == true){
    value = IRSensor(IR3_PIN);
    Serial.println(value);
    motorZ.step(stepPerRevolution / 100);

    if(value<=10){
      spinMotorZ = false;
      homing = false;
    }
  }
}

void readMessage(){
  currentMessage = Serial.read();

  if(currentMessage == 'Y'){
    homing = true;
  }
}

void motorControl(){
  while(homing == true){
    switch(homingState){
      case 0:
        spinMotorX = true;
        moveMotorX();
        break;
      case 1:
        spinMotorY = true;
        moveMotorY();
        break;
      case 2:
        spinMotorZ = true;
        moveMotorZ();
        break; 
    }
  }
}

void loop(){
  readMessage();
  motorControl();
}