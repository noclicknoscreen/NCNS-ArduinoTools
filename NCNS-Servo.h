#pragma once

#include <Arduino.h>
#include "Servo.h"

class ServoWrapper{

public:

  void setup(int _pin, int _centerRef);
  void maintainCenter();
  void continousRotate(float _speed);

private:

  int centerRef;
  Servo myservo;  // create servo object to control a servo
  void writeServo(int _value);
};
