# Jour 1–2 — Écran LCD I2C : trouver l'adresse et afficher du texte

## Objectif de l'exercice

Apprendre à identifier l'adresse I2C d'un écran LCD avec un scanner, puis à afficher du texte statique et dynamique sur cet écran.

À la fin de cet exercice, tu sauras :
- comprendre ce qu'est le protocole I2C et pourquoi chaque appareil a une adresse
- utiliser un scanner I2C pour détecter l'adresse exacte de ton module
- initialiser un écran LCD avec la bibliothèque LiquidCrystal_I2C
- positionner le curseur et afficher du texte sur les deux lignes de l'écran
- afficher une valeur qui change au fil du temps (compteur de secondes)

---

## Qu'est-ce que le protocole I2C ?

I2C (*Inter-Integrated Circuit*) est un protocole de communication qui permet à plusieurs appareils électroniques de communiquer avec l'Arduino en utilisant seulement deux fils (SDA pour les données, SCL pour l'horloge), au lieu d'un fil dédié par appareil.

Pour que l'Arduino sache à qui il parle, chaque appareil I2C possède une adresse unique, un peu comme un numéro de maison dans une rue. Avant d'utiliser un écran LCD I2C, il faut donc connaître son adresse — généralement `0x27` ou `0x3F`, mais cela peut varier selon le fabricant du module.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Écran LCD 1602 avec module I2C | 1 |
| Breadboard | 1 |
| Fils de connexion | 4 |

---

## Câblage — étape par étape

| Broche LCD I2C | Broche Arduino Uno | Rôle |
|---|---|---|
| VCC | 5V | Alimentation |
| GND | GND | Masse |
| SDA | A4 | Ligne de données I2C |
| SCL | A5 | Ligne d'horloge I2C |

### ⚠️ Avertissement important

Ne saute pas l'étape du scanner I2C. Si tu utilises directement l'adresse `0x27` dans ton code sans avoir vérifié qu'elle correspond réellement à ton module, l'écran peut rester complètement blanc ou noir sans qu'aucun message d'erreur ne t'indique pourquoi.

---

## Installer la bibliothèque

1. Ouvre Arduino IDE
2. Va dans **Outils → Gérer les bibliothèques** (ou `Ctrl + Maj + I`)
3. Recherche `LiquidCrystal I2C`
4. Installe celle de **Frank de Brabander** (la plus utilisée et compatible)

La bibliothèque `Wire.h`, utilisée pour la communication I2C elle-même, n'a pas besoin d'être installée : elle est fournie par défaut avec Arduino IDE.

---

## Étape 1 — Code du scanner I2C (trouver l'adresse)

```cpp
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);             
  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address, HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           
}
```

### Explication ligne par ligne

`for(address = 1; address < 127; address++)` — l'I2C utilise des adresses comprises entre 1 et 126 ; cette boucle teste donc chacune d'entre elles, une par une.

`Wire.beginTransmission(address);` et `error = Wire.endTransmission();` — tente d'envoyer un signal à l'adresse en cours de test. Si un appareil répond à cette adresse, `endTransmission()` retournera `0` (pas d'erreur) ; sinon, une autre valeur d'erreur.

`if (error == 0)` — si aucune erreur n'est survenue, cela signifie qu'un appareil a effectivement répondu à cette adresse : on l'affiche dans le moniteur série au format hexadécimal, qui est la convention standard pour les adresses I2C.

---

## Étape 2 — Code d'affichage sur le LCD

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// On utilise l'adresse 0x27 que le scanner a trouvée !
LiquidCrystal_I2C lcd(0x27, 16, 2); 
  int temps = 0;

void setup() {
  lcd.init();          // Initialisation du LCD
  lcd.backlight();     // Allume le rétroéclairage
  
  lcd.setCursor(0, 0); // Ligne du haut, colonne 0
  lcd.print("Bonjour Reine !");
  
  lcd.setCursor(0, 1); // Ligne du bas, colonne 0
  lcd.print("I2C Fonctionne !");
lcd.clear(); // On efface l'écran avant d'écrire

}

void loop() {
  delay(1000);
  if (temps != -1) {
    lcd.setCursor(0, 0); // Ligne du haut, colonne 0
    lcd.print("temps ecoulee:");
      lcd.setCursor(0, 1); // Ligne du bas, colonne 0
    lcd.print(temps);
    lcd.print(" secondes");
  }  
  temps++;
}
```

### Explication ligne par ligne

`LiquidCrystal_I2C lcd(0x27, 16, 2);` — crée l'objet `lcd` représentant ton écran physique, en précisant son adresse I2C (trouvée à l'étape 1), le nombre de colonnes (16) et le nombre de lignes (2).

`lcd.init();` et `lcd.backlight();` — initialisent l'écran et allument son rétroéclairage. Sans `backlight()`, l'écran peut sembler éteint même s'il fonctionne correctement.

`lcd.setCursor(0, 0);` — positionne le curseur d'écriture à la colonne 0, ligne 0 (donc en haut à gauche). Le LCD ne sait pas faire de retour à la ligne automatique : il faut explicitement repositionner le curseur pour chaque ligne.

`lcd.print("Bonjour Reine !");` — affiche le texte à partir de la position actuelle du curseur.

`lcd.clear();` — efface tout le contenu de l'écran. Ici, il intervient juste après l'affichage du message de bienvenue, donc ce message ne sera visible que très brièvement avant d'être effacé au démarrage.

`if (temps != -1)` — cette condition est toujours vraie ici puisque `temps` ne devient jamais -1 dans ce code ; elle ne change donc rien au comportement actuel, mais pourrait servir de point d'extension (par exemple, arrêter le compteur en mettant `temps` à -1).

`temps++;` — incrémente le compteur de 1 à chaque tour de boucle, donc environ chaque seconde (en tenant compte du `delay(1000)`).

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Téléverse d'abord le **code du scanner** (étape 1)
3. Ouvre le **Moniteur série** (`Ctrl + Maj + M`), réglé sur 9600 bauds
4. Note l'adresse affichée (par exemple `0x27`)
5. Modifie le **code d'affichage** (étape 2) si ton adresse est différente de `0x27`
6. Téléverse le code d'affichage
7. Observe l'écran : il doit afficher un compteur de secondes qui s'incrémente

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| Le scanner n'affiche aucun appareil trouvé | Câblage SDA/SCL inversé ou mal branché | Vérifier que SDA va sur A4 et SCL sur A5 |
| L'écran reste complètement blanc ou noir | Contraste mal réglé, ou mauvaise adresse I2C | Tourner le potentiomètre au dos du module I2C ; vérifier l'adresse avec le scanner |
| Le texte affiché est correct mais illisible (caractères bizarres) | Mauvaise initialisation, ou bibliothèque incompatible | Vérifier que tu utilises bien `LiquidCrystal_I2C` de Frank de Brabander |

---

## Exercices à réaliser

1. **Message de bienvenue persistant** — Modifie le code pour que le message "Bonjour Reine !" reste affiché pendant 3 secondes avant de basculer sur le compteur, plutôt que d'être effacé immédiatement.
2. **Compteur à rebours** — Transforme le compteur croissant en compte à rebours qui part de 60 et descend vers 0.
3. **Affichage sur deux lignes différentes** — Affiche les minutes sur la ligne du haut et les secondes sur la ligne du bas, calculées à partir de la variable `temps`.

---

## Pour aller plus loin

Une fois l'écran LCD maîtrisé, l'étape suivante consiste à y afficher des données de capteurs en temps réel plutôt qu'un simple compteur — c'est exactement ce que fait l'exercice combiné DHT11 + LCD.
