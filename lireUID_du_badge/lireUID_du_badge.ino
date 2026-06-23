#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Approchez une carte...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  byte newCarte[rfid.uid.size];
  byte maCarte[rfid.uid.size] = {0X53, 0XB0, 0X88, 0XF};


  Serial.print("UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
    newCarte[i]=rfid.uid.uidByte[i];
  }
  Serial.println();

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (maCarte[i] != newCarte[i]) {
      Serial.println("ACCÉS REFUSÉ");
      break;
    } else {
      Serial.println("ACCÉS AUTORISÉ");
      break;
    }
  }

  Serial.println();
  rfid.PICC_HaltA();

}