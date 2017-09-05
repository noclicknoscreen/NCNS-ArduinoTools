#include "NCNS-NFCWrapper.h"

NFCMifareWrapper::NFCMifareWrapper() :
nfc(PN532_SS)
{
  //uid = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  numOfSector = 16;                 // Assume Mifare Classic 1K for now (16 4-block sectors)
}

// Setup
void NFCMifareWrapper::setup(){

  Serial.println("Looking for PN532...");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }

  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();

}


// Write
void NFCMifareWrapper::formatMifare(){

  uint8_t success;                          // Flag to check if there was an error with the PN532
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  bool authenticated = false;               // Flag to indicate if the sector is authenticated
  uint8_t blockBuffer[16];                  // Buffer to store block contents
  uint8_t blankAccessBits[3] = { 0xff, 0x07, 0x80 };
  uint8_t idx = 0;

  // Wait for an ISO14443A type card (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success)
  {
    // We seem to have a tag ...
    // Display some basic information about it
    Serial.println("Found an ISO14443A card/tag");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");

    // Make sure this is a Mifare Classic card
    if (uidLength != 4)
    {
      Serial.println("Ooops ... this doesn't seem to be a Mifare Classic card!");
      return;
    }

    Serial.println("Seems to be a Mifare Classic card (4 byte UID)");
    Serial.println("");
    Serial.println("Reformatting card for Mifare Classic (please don't touch it!) ... ");

    // Now run through the card sector by sector
    for (idx = 0; idx < numOfSector; idx++)
    {
      // Step 1: Authenticate the current sector using key B 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
      success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, BLOCK_NUMBER_OF_SECTOR_TRAILER(idx), 1, (uint8_t *)KEY_DEFAULT_KEYAB);
      if (!success)
      {
        Serial.print("Authentication failed for sector "); Serial.println(numOfSector);
        return;
      }

      // Step 2: Write to the other blocks
      if (idx == 16)
      {
        memset(blockBuffer, 0, sizeof(blockBuffer));
        if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3, blockBuffer)))
        {
          Serial.print("Unable to write to sector "); Serial.println(numOfSector);
          return;
        }
      }
      if ((idx == 0) || (idx == 16))
      {
        memset(blockBuffer, 0, sizeof(blockBuffer));
        if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2, blockBuffer)))
        {
          Serial.print("Unable to write to sector "); Serial.println(numOfSector);
          return;
        }
      }
      else
      {
        memset(blockBuffer, 0, sizeof(blockBuffer));
        if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3, blockBuffer)))
        {
          Serial.print("Unable to write to sector "); Serial.println(numOfSector);
          return;
        }
        if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2, blockBuffer)))
        {
          Serial.print("Unable to write to sector "); Serial.println(numOfSector);
          return;
        }
      }
      memset(blockBuffer, 0, sizeof(blockBuffer));
      if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 1, blockBuffer)))
      {
        Serial.print("Unable to write to sector "); Serial.println(numOfSector);
        return;
      }

      // Step 3: Reset both keys to 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
      memcpy(blockBuffer, KEY_DEFAULT_KEYAB, sizeof(KEY_DEFAULT_KEYAB));
      memcpy(blockBuffer + 6, blankAccessBits, sizeof(blankAccessBits));
      blockBuffer[9] = 0x69;
      memcpy(blockBuffer + 10, KEY_DEFAULT_KEYAB, sizeof(KEY_DEFAULT_KEYAB));

      // Step 4: Write the trailer block
      if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)), blockBuffer)))
      {
        Serial.print("Unable to write trailer block of sector "); Serial.println(numOfSector);
        return;
      }
    }
  }

}
/*
void NFCMifareWrapper::writeMifareBlock(int _numBlock, String _blockValue);

// Read
bool NFCMifareWrapper::isTagPresent();
String NFCMifareWrapper::readMifareBlock(int _numBlock);
*/
