#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

// Déclaration des broches pour les composants
const int ledVerte = 7;
const int ledRouge = 6;
const int buzzer = 4;

// ⚠️ REMPLACE CES VALEURS PAR L'UID DE TON BADGE BLEU
byte badgeAutorise[] = {0X53, 0XB0, 0X88, 0XF}; 

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  // Configuration des broches en sorties
  pinMode(ledVerte, OUTPUT);
  pinMode(ledRouge, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.println("Système de contrôle d'accès prêt...");
}

void loop() {
  // S'il n'y a pas de nouvelle carte, on arrête là
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Badge détecté ! UID : ");
  bool accesAccorde = true;

  // Comparaison de l'UID lu avec l'UID autorisé
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
    
    if (rfid.uid.uidByte[i] != badgeAutorise[i]) {
      accesAccorde = false; // Un octet est différent, accès refusé
    }
  }
  Serial.println();

  // Action en fonction du résultat
  if (accesAccorde) {
    Serial.println("=> ACCÈS ACCORDÉ ! Bienvenue.");
    digitalWrite(ledVerte, HIGH);   // Allume la LED verte
    tone(buzzer, 2500, 200);        // Petit bip aigu de succès
    delay(2000);                    // Laisse la porte "ouverte" 2 secondes
    digitalWrite(ledVerte, LOW);
  } 
  else {
    Serial.println("=> ACCÈS REFUSÉ ! Alerte.");
    digitalWrite(ledRouge, HIGH);   // Allume la LED rouge
    // Code pour faire une sirène à deux tons avec le buzzer
    tone(buzzer, 1000, 300);
    delay(300);
    tone(buzzer, 800, 300);
    delay(300);
    digitalWrite(ledRouge, LOW);
  }

  // Met le badge en veille pour la prochaine lecture
  rfid.PICC_HaltA();
}