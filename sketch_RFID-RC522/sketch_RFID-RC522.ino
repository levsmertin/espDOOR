
#include <ESP8266WiFi.h>
#include <SPI.h>
#include "MFRC522.h"

/* 
RST     = GPIO5    (D1)
SDA(SS) = GPIO4    (D2)
MOSI    = GPIO13   (D7)
MISO    = GPIO12   (D6)
SCK     = GPIO14   (D5)
GND     = GND
3.3V    = 3.3V
*/
#define RST_PIN  5  // RST-PIN für RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN  4  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO4 

MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup() 
{
  Serial.begin(9600);    
  delay(250);
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
  Serial.println("\nHi");
}

void loop() 
{ 
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent()) 
  {
    delay(50);
      if (mfrc522.PICC_ReadCardSerial())
           {
            delay(50);
            Serial.print(F("Card UID:"));
            dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
            Serial.println();
           }
    return;
  }
  // Select one of the cards
//  if ( ! mfrc522.PICC_ReadCardSerial()) 
//  {
//    delay(50);
//    return;
//  }
  delay (4000);
}

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) 
{
  for (byte i = 0; i < bufferSize; i++) 
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
