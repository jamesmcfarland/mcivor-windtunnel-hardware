#include "Arduino.h"
#include <AFMotor.h>

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

int motor1Speed = 0;
int motor2Speed = 0;
int motor3Speed = 0;
int motor4Speed = 0;

int iter = 0;

int mapSpeed(int speed)
{

  int mapped = (speed / 100.0F) * 255;
  if (mapped > 255)
  {
    mapped = 255;
  }
  return mapped;
}

const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;

void recvWithStartEndMarkers()
{
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false)
  {
    rc = Serial.read();

    if (recvInProgress == true)
    {
      if (rc != endMarker)
      {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars)
        {
          ndx = numChars - 1;
        }
      }
      else
      {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker)
    {
      recvInProgress = true;
    }
  }
}

void processInput()
{
  if (newData == true)
  {
    newData = false;
    char target = receivedChars[0];
    long speed = String(receivedChars).substring(1, String(receivedChars).length()).toInt();
    speed = mapSpeed(speed);
    switch (target)
    {
      // All fans
    case '0':
      motor1Speed = motor2Speed = motor3Speed = motor4Speed = speed;
      motor1.setSpeed(speed);
      motor2.setSpeed(speed);
      motor3.setSpeed(speed);
      motor4.setSpeed(speed);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      break;
    case '1':
      motor1Speed = speed;
      motor1.setSpeed(speed);
      motor1.run(FORWARD);

      break;
    case '2':
      motor2Speed = speed;
      motor2.setSpeed(speed);
      motor2.run(FORWARD);

      break;
    case '3':
      motor3Speed = speed;
      motor3.setSpeed(speed);
      motor3.run(FORWARD);

      break;
    case '4':
      motor4Speed = speed;
      motor4.setSpeed(speed);
      motor4.run(FORWARD);

      break;
    default:
      motor1Speed = motor2Speed = motor3Speed = motor4Speed = 0;
      motor1.setSpeed(0);
      motor2.setSpeed(0);
      motor3.setSpeed(0);
      motor4.setSpeed(0);
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      motor3.run(RELEASE);
      motor4.run(RELEASE);

      break;
    }
  }
}

void setup()
{
  // blink led 10 times
  for (int i = 0; i < 2; i++)
  {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
  Serial.begin(115200);
}

void loop()
{
  if (iter > 1000)
  {
    iter = 0;
  }
  iter++;
  recvWithStartEndMarkers();
  processInput();
  // every 250ms send the motor speeds, space seperated
  Serial.print(motor1Speed);
  Serial.print(" ");
  Serial.print(motor2Speed);
  Serial.print(" ");
  Serial.print(motor3Speed);
  Serial.print(" ");
  Serial.print(motor4Speed);
  Serial.print(" ");
  Serial.print(iter);
  Serial.println();
  delay(250);
}
