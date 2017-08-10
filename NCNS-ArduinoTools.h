#pragma once

#include <Arduino.h>

// ----------------------------------------------------------
// Do blink a led without delay
// ----------------------------------------------------------
void errorBlink(unsigned int _led, unsigned int _delayMs) {

  float alternativeSignal = (millis() % _delayMs) / (float)_delayMs;

//  Serial.print("Alternative signal : ");
//  Serial.println(alternativeSignal);

  if (alternativeSignal > 0.5) {
    digitalWrite(_led, HIGH);
  } else {
    digitalWrite(_led, LOW);
  }

}
