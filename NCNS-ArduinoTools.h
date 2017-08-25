#pragma once

#include <Arduino.h>

// ----------------------------------------------------------
// Do blink a led without delay
// ----------------------------------------------------------
void ledBlink(unsigned int _led, unsigned int _delayMs) {
  float alternativeSignal = (millis() % _delayMs) / (float)_delayMs;
  if (alternativeSignal > 0.5) {
    digitalWrite(_led, HIGH);
  } else {
    digitalWrite(_led, LOW);
  }
}

// ----------------------------------------------------------
// For backward compatibility
// ----------------------------------------------------------
void errorBlink(unsigned int _led, unsigned int _delayMs) {
	ledBlink( _led, _delayMs);
}
