#include "Arduino.h"
#include <AFMotor.h>

AF_DCMotor motor1(1, MOTOR34_1KHZ);
AF_DCMotor motor2(2, MOTOR34_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
  Serial.begin(9600);           // Initialize the serial communication at 9600 baud rate
  while (!Serial)
  {
    ; // Wait for the serial port to connect
  }
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);
}

int mapSpeed(int speed)
{

  int mapped = (speed / 100.0F) * 255;
  if (mapped > 255)
  {
    mapped = 255;
  }
  return mapped;
}

void loop()
{
  if (Serial.available())
  {                                                    // Check if data is available to read
    String inputString = Serial.readStringUntil('\n'); // Read the string until a newline character ('\n') is received

    // Print the received string

    char target = inputString.charAt(0);
    long speed = inputString.substring(1, inputString.length()).toInt();

    switch (target)
    {
      // All fans
    case '0':
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
      motor1.setSpeed(speed);
      motor1.run(FORWARD);

      break;
    case '2':
      motor2.setSpeed(speed);
      motor2.run(FORWARD);

      break;
    case '3':
      motor3.setSpeed(speed);
      motor3.run(FORWARD);

      break;
    case '4':
      motor4.setSpeed(speed);
      motor4.run(FORWARD);

      break;
    default:
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

  // Other code and operations can be added here
}
