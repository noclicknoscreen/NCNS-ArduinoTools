#include "NCNS-Servo.h"

void ServoWrapper::setup(int _pin, int _centerRef){
  myservo.attach(_pin);  // attaches the servo on pin 9 to the servo object
  centerRef = 1500 + _centerRef;
}

void ServoWrapper::maintainCenter(){
#ifdef SERVO_DEBUG
  Serial.print("Center value : ");
  Serial.println(centerRef);
#endif
  writeServo(centerRef);
}


void ServoWrapper::continousRotate(float _speed){

  int value;

  _speed *= 0.5;
  _speed = constrain(_speed, -1, 1);

  value = centerRef + (_speed * 1000);
  value = constrain(value, 1000, 2000);

#ifdef SERVO_DEBUG
  Serial.print("Sens : ");
  if(value > centerRef){
    Serial.print("+");
  }else{
    Serial.print("-");
  }
  Serial.print(" Speed value : ");
  Serial.println(value);
#endif
  writeServo(value);

}

void ServoWrapper::writeServo(int _value){
  myservo.writeMicroseconds(_value);
}
