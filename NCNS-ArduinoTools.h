#ifdef NCNSArduinoTools
#define NCNSArduinoTools

#include <Arduino.h>
#include <EEPROM.h>
#include <Ethernet2.h>

// ----------------------------------------------------------
// Do blink a led without delay
// First you have to setup your pin with this line below
// pinMode(LED_BUILTIN, OUTPUT);
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
  // Seb : commit Seems to not exist into EEPROM lib
  //EEPROM.commit();
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

/*************************************************
   Returns a human readable IP from a string
 **************************************************/
String humanReadableIp(IPAddress ip) {
  return String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]);
}

/*************************************************
Returns the Red component of a 32-bit color
**************************************************/
uint8_t Red(uint32_t color)
{
    return (color >> 16) & 0xFF;
}

/*************************************************
Returns the Green component of a 32-bit color
**************************************************/
uint8_t Green(uint32_t color)
{
    return (color >> 8) & 0xFF;
}

/*************************************************
Returns the Blue component of a 32-bit color
**************************************************/
uint8_t Blue(uint32_t color)
{
    return color & 0xFF;
}

#endif
