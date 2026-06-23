#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h> // Bibliothèque pour utiliser la mémoire cachée de l'Arduino

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);
Servo maSerrure;
LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int ledVerte = 2;
const int ledRouge = 3;
const int buzzer = 4;
const int pinServo = 5;

boolean modeProgrammation = false; // Mode pour ajouter/supprimer des badges
byte badgeMaitre[4];               // Stocke l'UID du badge Maître en mémoire vive
byte badgeLu[4];                   // Stocke l'UID du badge en cours de lecture

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  
  lcd.init();
  lcd.backlight();
  
  pinMode(ledVerte, OUTPUT);
  pinMode(ledRouge, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  maSerrure.attach(pinServo);
  maSerrure.write(0); 

  // --- GESTION DE L'EEPROM AU DÉMARRAGE ---
  // On vérifie si un badge Maître a déjà été configuré (on utilise l'adresse 0 comme marqueur)
  if (EEPROM.read(0) != 143) { 
    lcd.setCursor(0, 0);
    lcd.print("PAS DE MAITRE !");
    lcd.setCursor(0, 1);
    lcd.print("SCANNEZ UN BADGE");
    
    // Attente du premier badge qui deviendra le Maître
    while (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
      delay(50);
    }
    
    // Enregistrement du Maître dans l'EEPROM (Adresses 1 à 4)
    for (byte i = 0; i < 4; i++) {
      EEPROM.write(i + 1, rfid.uid.uidByte[i]);
      badgeMaitre[i] = rfid.uid.uidByte[i];
    }
    EEPROM.write(0, 143); // Écrit le marqueur pour dire "Maître configuré"
    
    lcd.clear();
    lcd.print("MAITRE ENREGISTRE");
    tone(buzzer, 2000, 500);
    delay(2000);
  } else {
    // Si le Maître existe déjà, on le charge depuis l'EEPROM
    for (byte i = 0; i < 4; i++) {
      badgeMaitre[i] = EEPROM.read(i + 1);
    }
  }

  afficherMessageAttente();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }
  
  // Stockage de l'UID lu
  for (byte i = 0; i < 4; i++) {
    badgeLu[i] = rfid.uid.uidByte[i];
  }

  // Vérification : Est-ce le badge Maître ?
  if (estEgal(badgeLu, badgeMaitre)) {
    modeProgrammation = !modeProgrammation; // Alterne le mode config
    lcd.clear();
    if (modeProgrammation) {
      lcd.print("MODE CONFIG...");
      lcd.setCursor(0, 1);
      lcd.print("APPROCHEZ CARTE");
      tone(buzzer, 1500, 100); delay(150); tone(buzzer, 1500, 100);
    } else {
      lcd.print("SORTIE CONFIG");
      tone(buzzer, 1000, 250);
      delay(1000);
      afficherMessageAttente();
    }
    rfid.PICC_HaltA();
    return;
  }

  // PROTOCOLE EN MODE CONFIGURATION
  if (modeProgrammation) {
    int adresseBadge = trouverAdresseEEPROM(badgeLu);
    lcd.clear();
    
    if (adresseBadge != -1) {
      // Le badge existe déjà, on le supprime (on efface ses 4 octets)
      for (int i = 0; i < 4; i++) {
        EEPROM.write(adresseBadge + i, 0);
      }
      lcd.print("BADGE SUPPRIME !");
      digitalWrite(ledRouge, HIGH); tone(buzzer, 800, 500); delay(500); digitalWrite(ledRouge, LOW);
    } else {
      // Le badge est inconnu, on cherche une place vide pour l'ajouter
      int placeLibre = trouverPlaceLibre();
      if (placeLibre != -1) {
        for (int i = 0; i < 4; i++) {
          EEPROM.write(placeLibre + i, badgeLu[i]);
        }
        lcd.print("BADGE AJOUTE !");
        digitalWrite(ledVerte, HIGH); tone(buzzer, 2500, 200); delay(200); tone(buzzer, 2500, 200); delay(300); digitalWrite(ledVerte, LOW);
      } else {
        lcd.print("MEMOIRE PLEINE !");
        tone(buzzer, 500, 1000); delay(1000);
      }
    }
    lcd.setCursor(0, 1); lcd.print("APPROCHEZ CARTE");
  } 
  // PROTOCOLE EN MODE NORMAL (CONTRÔLE D'ACCÈS)
  else {
    lcd.clear();
    if (trouverAdresseEEPROM(badgeLu) != -1) {
      lcd.print("ACCES ACCORDE !");
      lcd.setCursor(0, 1); lcd.print("BIENVENUE :)");
      
      digitalWrite(ledVerte, HIGH);
      tone(buzzer, 2500, 200);
      maSerrure.write(90); 
      delay(3000);
      maSerrure.write(0);  
      digitalWrite(ledVerte, LOW);
    } else {
      lcd.print("ACCES REFUSE !");
      lcd.setCursor(0, 1); lcd.print("BADGE INCONNU");
      
      digitalWrite(ledRouge, HIGH);
      tone(buzzer, 1000, 300); delay(300); tone(buzzer, 800, 300); delay(300);
      digitalWrite(ledRouge, LOW);
    }
    afficherMessageAttente();
  }

  rfid.PICC_HaltA();
}

// --- FONCTIONS OUTILS ---

void afficherMessageAttente() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("  CONTR. ACCES  ");
  lcd.setCursor(0, 1); lcd.print(" SCANNEZ BADGE  ");
}

boolean estEgal(byte a[], byte b[]) {
  for (byte i = 0; i < 4; i++) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

// Cherche si l'UID est dans l'EEPROM (on commence après le Maître, donc adresse 5)
int trouverAdresseEEPROM(byte uid[]) {
  for (int i = 5; i < EEPROM.length() - 4; i += 4) {
    byte correspondant = 0;
    for (int j = 0; j < 4; j++) {
      if (EEPROM.read(i + j) == uid[j]) correspondant++;
    }
    if (correspondant == 4) return i; // Renvoie l'adresse de début du badge trouvé
  }
  return -1; // Badge inconnu
}

// Cherche 4 octets vides (égaux à 0) consécutifs pour enregistrer un nouveau badge
int trouverPlaceLibre() {
  for (int i = 5; i < EEPROM.length() - 4; i += 4) {
    byte vide = 0;
    for (int j = 0; j < 4; j++) {
      if (EEPROM.read(i + j) == 0) vide++;
    }
    if (vide == 4) return i;
  }
  return -1; 
}