# RFID Jour 3 — Serrure intelligente avec EEPROM, LCD, Servo et badge Maître

## Objectif de l'exercice

Réaliser un système de contrôle d'accès complet et autonome : les badges autorisés sont stockés dans la mémoire permanente de l'Arduino (EEPROM), un badge Maître permet d'ajouter ou supprimer des badges sans modifier le code, un servomoteur simule le verrou physique, et un écran LCD affiche l'état du système en temps réel.

À la fin de cet exercice, tu sauras :
- utiliser l'EEPROM pour sauvegarder des données qui survivent à un redémarrage
- concevoir un système à deux modes : mode normal et mode configuration
- gérer dynamiquement une liste de badges autorisés (ajout et suppression)
- combiner 5 composants différents (RFID, LCD, servo, LED, buzzer) dans un programme structuré en fonctions
- utiliser un "badge Maître" comme clé d'administration du système

---

## Qu'est-ce que l'EEPROM ?

L'EEPROM (*Electrically Erasable Programmable Read-Only Memory*) est une toute petite mémoire intégrée directement dans l'Arduino. Contrairement à la mémoire RAM utilisée par les variables normales (qui s'efface à chaque redémarrage), l'EEPROM conserve ses données même quand l'Arduino est éteint — exactement comme une clé USB.

L'Arduino Uno dispose de 1024 octets d'EEPROM. Dans cet exercice, on l'utilise comme une "liste de badges autorisés" : chaque badge occupe 4 octets (son UID), et on peut en stocker plus d'une centaine.

**Structure de l'EEPROM utilisée dans ce projet :**

| Adresse | Contenu |
|---|---|
| 0 | Marqueur (valeur 143 = "badge Maître configuré") |
| 1 à 4 | UID du badge Maître (4 octets) |
| 5 à 8 | UID du 1er badge utilisateur |
| 9 à 12 | UID du 2ème badge utilisateur |
| 13 à 16 | UID du 3ème badge utilisateur |
| ... | Suite des badges |

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Module RFID RC522 | 1 |
| Badges ou cartes RFID | 2 minimum (1 Maître + 1 ou plusieurs utilisateurs) |
| Écran LCD 1602 avec module I2C | 1 |
| Servomoteur (SG90 ou similaire) | 1 |
| LED verte | 1 |
| LED rouge | 1 |
| Résistances 220 Ω | 2 |
| Buzzer passif | 1 |
| Breadboard | 1 |
| Fils de connexion | nombreux |

---

## Câblage — étape par étape

| Broche composant | Broche Arduino Uno | Rôle |
|---|---|---|
| RC522 — SDA | D10 | SPI — sélection esclave |
| RC522 — SCK | D13 | SPI — horloge |
| RC522 — MOSI | D11 | SPI — données vers RC522 |
| RC522 — MISO | D12 | SPI — données vers Arduino |
| RC522 — RST | D9 | Réinitialisation RC522 |
| RC522 — GND | GND | Masse |
| RC522 — 3.3V | **3.3V** | Alimentation (jamais 5V !) |
| LCD I2C — SDA | A4 | Données I2C |
| LCD I2C — SCL | A5 | Horloge I2C |
| LCD I2C — VCC | 5V | Alimentation |
| LCD I2C — GND | GND | Masse |
| Servomoteur — signal | D5 (PWM ~) | Commande du servo |
| Servomoteur — rouge | 5V | Alimentation |
| Servomoteur — marron/noir | GND | Masse |
| LED verte (+) | D2 (via résistance 220 Ω) | Accès accordé |
| LED rouge (+) | D3 (via résistance 220 Ω) | Accès refusé |
| LED verte (−) / rouge (−) | GND | Masse commune |
| Buzzer (+) | D4 | Signal sonore |
| Buzzer (−) | GND | Masse |

### ⚠️ Avertissement important

Ce montage utilise simultanément le SPI (pour le RC522), l'I2C (pour le LCD), le PWM (pour le servo) et plusieurs GPIO. Avant de mettre sous tension, vérifie chaque connexion avec le tableau ci-dessus — une confusion entre SDA (A4) et SCL (A5) pour le LCD, ou entre les broches SPI du RC522, peut empêcher l'ensemble du système de fonctionner sans aucun message d'erreur visible.

---

## Installer les bibliothèques

| Bibliothèque | Installation | Utilisation |
|---|---|---|
| SPI.h | Fournie par défaut — rien à installer | Communication SPI avec le RC522 |
| MFRC522.h | Gestionnaire → rechercher `MFRC522` (GithubCommunity) | Lecture RFID |
| Servo.h | Fournie par défaut — rien à installer | Contrôle du servomoteur |
| Wire.h | Fournie par défaut — rien à installer | Bus I2C pour le LCD |
| LiquidCrystal_I2C.h | Gestionnaire → rechercher `LiquidCrystal I2C` (Frank de Brabander) | Affichage LCD |
| EEPROM.h | Fournie par défaut — rien à installer | Mémoire permanente |

---

## Code de l'exercice

```cpp
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
        digitalWrite(ledVerte, HIGH); tone(buzzer, 2500, 200); delay(200); 
        tone(buzzer, 2500, 200); delay(300); digitalWrite(ledVerte, LOW);
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
```

### Explication ligne par ligne

`EEPROM.read(0) != 143` — au démarrage, le programme vérifie si l'adresse 0 de l'EEPROM contient la valeur 143. Ce chiffre est un "marqueur" arbitraire choisi par toi pour signaler que le badge Maître a déjà été configuré. Si la valeur est différente de 143 (par exemple à la première utilisation, l'EEPROM contient 255 partout par défaut), le système entre en mode configuration initiale.

`EEPROM.write(i + 1, rfid.uid.uidByte[i]);` — écrit chaque octet de l'UID du badge Maître dans l'EEPROM à partir de l'adresse 1 (donc adresses 1, 2, 3, 4). L'adresse 0 est réservée au marqueur.

`modeProgrammation = !modeProgrammation;` — à chaque scan du badge Maître, bascule le mode entre "normal" et "configuration". Si on était en mode normal, on passe en mode config ; si on était en mode config, on revient en mode normal. C'est le même opérateur `!` (non logique) vu dans l'exercice du bouton toggle.

`boolean estEgal(byte a[], byte b[])` — cette fonction personnalisée compare deux tableaux de 4 octets. Elle retourne `true` si tous les octets sont identiques, `false` dès qu'un octet diffère. Elle est réutilisée partout dans le code pour éviter de réécrire la même boucle de comparaison plusieurs fois.

`int trouverAdresseEEPROM(byte uid[])` — parcourt l'EEPROM octet par octet (par blocs de 4 à partir de l'adresse 5), en cherchant un bloc dont les 4 octets correspondent à l'UID recherché. Elle retourne l'adresse de ce bloc si trouvé, ou `-1` si le badge est inconnu.

`if (adresseBadge != -1)` — si le badge est trouvé dans l'EEPROM (adresse valide, différente de -1), cela signifie qu'il est déjà enregistré → on le **supprime** en écrivant des 0 à son adresse. Si `-1`, le badge est nouveau → on le **ajoute** dans une place libre.

`int trouverPlaceLibre()` — cherche le premier bloc de 4 octets consécutifs qui valent tous 0 (emplacement libre), à partir de l'adresse 5. Ces octets à 0 sont ceux laissés par une suppression de badge ou jamais utilisés.

`maSerrure.write(90);` — commande le servomoteur d'aller à 90° pour "ouvrir" le verrou simulé. Après 3 secondes, `maSerrure.write(0)` le ramène à 0° (verrou fermé).

---

## Comment tester — procédure complète

### 1ère utilisation (EEPROM vierge)
1. Téléverse le code, branche l'Arduino
2. L'écran affiche "PAS DE MAITRE ! SCANNEZ UN BADGE"
3. Approche le badge que tu veux désigner comme Maître → il est enregistré en EEPROM
4. L'écran affiche "MAITRE ENREGISTRE"

### Ajouter un badge utilisateur
5. Approche le badge Maître → l'écran affiche "MODE CONFIG... APPROCHEZ CARTE"
6. Approche le badge à ajouter → "BADGE AJOUTE !"
7. Approche à nouveau le badge Maître → l'écran revient en mode normal "CONTRÔLE ACCÈS"

### Tester le contrôle d'accès
8. Approche le badge ajouté → LED verte, bip aigu, servo tourne à 90° pendant 3 sec
9. Approche un badge inconnu → LED rouge, sirène à deux tons, servo ne bouge pas

### Supprimer un badge
10. Entrer en mode config (scanner le Maître), puis scanner le badge à supprimer → "BADGE SUPPRIME !"

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| L'écran reste toujours sur "PAS DE MAITRE" | L'EEPROM a déjà un badge Maître dont le marqueur n'a pas été réinitialisé | Téléverser un programme qui écrit `EEPROM.write(0, 0)` dans `setup()` pour effacer le marqueur, puis rétéléverser ton code |
| Le badge Maître ne passe jamais en mode config | UID du badge Maître en EEPROM différent du badge utilisé (par ex. si le badge a changé) | Réinitialiser l'EEPROM (voir ci-dessus) pour reconfigurer un nouveau badge Maître |
| Le servo ne bouge pas lors d'un accès accordé | Broche D5 mal câblée, ou servo insuffisamment alimenté | Vérifier le câblage du servo ; si le servo consomme trop, utiliser une alimentation externe dédiée |
| L'écran affiche des caractères bizarres | Mauvaise adresse I2C — le LCD n'est pas sur `0x27` | Relancer le scanner I2C pour trouver l'adresse exacte de ton module, et mettre à jour `LiquidCrystal_I2C lcd(0x27,...)` |

---

## Exercices à réaliser

1. **Réinitialisation EEPROM** — Écris un programme séparé qui efface toute l'EEPROM (`EEPROM.write(i, 0)` en boucle), pour pouvoir repartir de zéro si tu veux reconfigurer complètement le système.
2. **Compteur de passages** — Stocke dans l'EEPROM le nombre total d'accès accordés depuis la mise en service, et affiche-le sur le LCD à chaque accès réussi.
3. **Plages horaires** — Ajoute une contrainte : le servo ne s'ouvre que si l'accès est accordé ET qu'un bouton physique est maintenu appuyé (simule un agent de sécurité qui valide chaque ouverture manuellement).

---

## Pour aller plus loin

Ce système constitue un projet professionnel complet qui combine l'ensemble des compétences des semaines 2 et 3 : LCD, RFID, servo, LED, buzzer, et maintenant mémoire persistante. Il peut être étendu avec une horloge temps réel (module RTC) pour enregistrer l'heure et la date de chaque passage, ou avec un module WiFi (ESP8266) pour envoyer des alertes à distance.
