#pragma once

#include <Arduino.h>
#include <EEPROM.h>

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


/*************************************************
   Writing a float in EEPROM
 **************************************************/
void eeprom_write(int addr, float f) {
  unsigned char *buf = (unsigned char*)(&f);
  for ( int i = 0 ; i < (int)sizeof(f) ; i++ ) {
    EEPROM.write(addr + i, buf[i]);
  }
  EEPROM.commit();
}

/*************************************************
   Reading a float from EEPROM
 **************************************************/
float eeprom_read(int addr) {
  float f;
  unsigned char *buf = (unsigned char*)(&f);
  for ( int i = 0 ; i < (int)sizeof(f) ; i++ ) {
    buf[i] = EEPROM.read(addr + i);
  }
  return f;
}

