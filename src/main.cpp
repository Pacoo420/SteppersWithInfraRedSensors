#include <Arduino.h>
#include <Stepper.h>
#include "WiFi.h"

// X axis sensor
#define IR1_PIN 32
#define IR2_PIN 35
#define IR3_PIN 34

// Motor for X axis
#define xIN1 15
#define xIN2 2
#define xIN3 4
#define xIN4 16

// Motor for Y axis
#define yIN1 17
#define yIN2 5
#define yIN3 18
#define yIN4 19

// Motor for Z axis
#define zIN1 21
#define zIN2 26
#define zIN3 22
#define zIN4 23

const int stepPerRevolution = 2000;

unsigned int timeLoop;
unsigned int timeStart;
bool homing = false;

bool spinMotorX = false;
bool spinMotorY = false;
bool spinMotorZ = false;

double distance;

int value;
int homingState = 0;
int distance_ir = 0;

String currentMessage;
String lastmessage = "";

// const char *ssid = "iPhone užívateľa Adrián";
// const char *password = "hello123";

// const char *ssid = "12connect";
// const char *password = "";

const char *ssid = "Zed-dication";
const char *password = "83644740";

const uint16_t port = 2323;
const char *host = "192.168.0.100";

WiFiServer server(port);

Stepper motorX(stepPerRevolution, xIN1, xIN3, xIN2, xIN4);
Stepper motorY(stepPerRevolution, yIN1, yIN3, yIN2, yIN4);
Stepper motorZ(stepPerRevolution, zIN1, zIN3, zIN2, zIN4);

void setup()
{
  // IR pinmode setup
  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);
  pinMode(IR3_PIN, INPUT);

  // motor speed setup
  motorX.setSpeed(5); // was 25
  motorY.setSpeed(5);
  motorZ.setSpeed(5);

  // debug monitor setup
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Trying to connect to the wifi");
  }
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
}

int IRSensor(int IR_PIN)
{
  int sensorValue = analogRead(IR_PIN);
  distance = 12.08 * pow(sensorValue / 1024.0, -1.204);
  return distance;
}

void moveMotorX()
{
  while (spinMotorX == true)
  {
    int sensorReading = analogRead(A0);
    value = IRSensor(IR1_PIN);
    motorX.step(stepPerRevolution / 100);

    if (value <= 10)
    {
      homingState = 1;
      spinMotorX = false;
    }
  }
}

void moveMotorY()
{
  while (spinMotorY == true)
  {
    value = IRSensor(IR2_PIN);
    motorY.step(stepPerRevolution / 100);

    if (value <= 10)
    {
      homingState = 2;
      spinMotorY = false;
    }
  }
}

void moveMotorZ()
{
  while (spinMotorZ == true)
  {
    value = IRSensor(IR3_PIN);
    motorZ.step(stepPerRevolution / 100);

    if (value <= 10)
    {
      spinMotorZ = false;
      homing = false;
    }
  }
}

void motorControl()
{
  while (homing == true)
  {
    switch (homingState)
    {
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

void handleCommunication(WiFiClient *client)
{
  if (!client->connect(host, port))
  {
    Serial.println("Connection to host failed");
    delay(1000);
    return;
  }
  Serial.println("CONNECTED TO SERVER");

  while (true)
  {
    if (client->available())
    {
      //Exctract message from server
      currentMessage = client->readStringUntil(';');
      currentMessage.trim();
      if (currentMessage.substring(0, 1) == "$")
      {
        currentMessage = currentMessage.substring(1, -1);
      }

      //see what server sent
      Serial.println(currentMessage);

      //Respond to server
      if (currentMessage == "HOME")
      {
        homing = true;
        client->print("ACK\n");
        Serial.println("ACK SENT");
      }
      
      else if(currentMessage == "RESET"){
        homing = false;
        client->print("ACK\n");
        Serial.println("ACK SENT");      
      }

      else{
        client->print("NACK\n");
        Serial.println("NACK SENT");
      }

      if (lastmessage != currentMessage)
      {
        motorControl();
      }
      lastmessage = currentMessage;
    }
  }
}

void loop()
{
  WiFiClient client;
  handleCommunication(&client);
}