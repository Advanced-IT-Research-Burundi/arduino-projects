# RFID Jour 2 — Contrôle d'accès RFID avec LED et Buzzer

## Objectif de l'exercice

Transformer la simple lecture d'UID en un vrai système de contrôle d'accès : une LED verte + bip aigu si le badge est autorisé, une LED rouge + sirène à deux tons si le badge est refusé.

À la fin de cet exercice, tu sauras :
- comparer un UID de façon fiable sur les 4 octets complets (correction de l'exercice précédent)
- utiliser une variable booléenne (`bool`) pour accumuler un résultat de comparaison
- déclencher des sorties différentes (LED verte ou rouge, sons différents) selon le résultat
- créer un effet sonore à deux tons pour simuler une alarme

---

## Qu'est-ce qu'une comparaison booléenne accumulée ?

Dans l'exercice précédent, la boucle de comparaison s'arrêtait dès le premier octet à cause d'un `break` dans les deux branches — ce qui rendait la vérification incomplète. 

Ici, on utilise une variable `accesAccorde = true` initialisée à "vrai", et on la passe à `false` si un seul octet ne correspond pas. La boucle parcourt les 4 octets sans s'arrêter, et c'est seulement après avoir tout vérifié qu'on décide de l'action à effectuer. C'est la façon correcte et robuste de comparer un UID complet.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Module RFID RC522 | 1 |
| Badge ou carte RFID autorisée | 1 |
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
| RC522 — SDA | D10 | Sélection esclave SPI |
| RC522 — SCK | D13 | Horloge SPI |
| RC522 — MOSI | D11 | Données Arduino → RC522 |
| RC522 — MISO | D12 | Données RC522 → Arduino |
| RC522 — RST | D9 | Réinitialisation |
| RC522 — GND | GND | Masse |
| RC522 — 3.3V | **3.3V** | Alimentation (jamais 5V !) |
| LED verte (+) | D7 (via résistance 220 Ω) | Signal accès accordé |
| LED rouge (+) | D6 (via résistance 220 Ω) | Signal accès refusé |
| LED verte (−) et LED rouge (−) | GND | Masse commune |
| Buzzer (+) | D4 | Signal sonore |
| Buzzer (−) | GND | Masse |

### ⚠️ Avertissement important

Le RC522 doit toujours être alimenté en **3.3V**, jamais en 5V. Cette règle s'applique aussi dans les montages plus complexes comme celui-ci où plusieurs composants partagent l'alimentation. Vérifie ce point avant chaque mise sous tension.

---

## Installer la bibliothèque

- **MFRC522** : Outils → Gérer les bibliothèques → rechercher `MFRC522` → installer (GithubCommunity)
- **SPI.h** : déjà incluse dans Arduino IDE, aucune installation nécessaire

---

## Code de l'exercice

```cpp
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
```

### Explication ligne par ligne

`const int ledVerte = 7; const int ledRouge = 6; const int buzzer = 4;` — déclarent les broches des trois composants de sortie avec le mot-clé `const`, car ces numéros ne changeront jamais pendant l'exécution.

`byte badgeAutorise[] = {0X53, 0XB0, 0X88, 0XF};` — contient l'UID de la carte autorisée sous forme de 4 octets hexadécimaux. **Tu dois remplacer ces valeurs par l'UID de ton propre badge**, obtenu en lisant l'exercice précédent dans le moniteur série.

`bool accesAccorde = true;` — cette variable booléenne démarre à `true` (accès autorisé par défaut) et sera passée à `false` si un seul des 4 octets ne correspond pas. C'est une technique classique : on suppose que tout va bien, et on ne change d'avis que si on trouve une différence.

`if (rfid.uid.uidByte[i] != badgeAutorise[i]) { accesAccorde = false; }` — compare octet par octet l'UID lu à l'UID autorisé. Contrairement à l'exercice précédent, la boucle ne s'arrête pas ici — elle continue jusqu'à avoir vérifié tous les 4 octets. Si l'un d'eux est différent, `accesAccorde` passe à `false` et reste à `false` même si les octets suivants correspondent.

`if (accesAccorde) { ... } else { ... }` — c'est seulement **après** avoir parcouru les 4 octets que le résultat final est utilisé pour déclencher la bonne action.

`tone(buzzer, 2500, 200);` — joue un son aigu (2500 Hz) pendant 200 ms pour signaler un accès réussi. Un son aigu est naturellement associé à une confirmation positive.

`tone(buzzer, 1000, 300); delay(300); tone(buzzer, 800, 300);` — crée une sirène à deux tons alternés (1000 Hz puis 800 Hz) pour signaler un refus. L'alternance de deux fréquences proches produit l'effet sonore d'alarme caractéristique.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Vérifie que l'UID dans `badgeAutorise[]` correspond bien à ton badge (utilise l'exercice précédent si tu n'es pas sûr)
4. Téléverse le code
5. Approche ton badge autorisé : la LED verte doit s'allumer avec un bip aigu pendant 2 secondes
6. Approche un badge inconnu (un autre badge ou carte) : la LED rouge s'allume avec la sirène à deux tons

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| Tout badge déclenche "ACCÈS REFUSÉ" | L'UID dans `badgeAutorise[]` ne correspond pas au badge utilisé | Relancer l'exercice précédent pour lire l'UID exact du badge, puis mettre à jour le tableau |
| La LED verte et la LED rouge s'allument toutes les deux | Deux broches confondues dans le câblage | Vérifier que la LED verte est sur D7 et la rouge sur D6 exactement comme dans le code |
| Le buzzer ne fait aucun son mais les LEDs fonctionnent | Buzzer actif utilisé à la place d'un passif, ou broche incorrecte | Vérifier que `buzzer` est bien sur D4, et tester le buzzer séparément avec `digitalWrite(buzzer, HIGH)` |

---

## Exercices à réaliser

1. **Délai d'alerte prolongé** — Fais clignoter la LED rouge 3 fois au lieu de simplement l'allumer une fois, pour un signal d'alarme plus visible.
2. **Deuxième badge autorisé** — Ajoute un deuxième tableau `badgeAutorise2[]` avec un autre UID, et modifie la condition pour accorder l'accès si le badge correspond à l'un **ou** l'autre (`||`).
3. **Compteur de tentatives refusées** — Ajoute une variable qui compte les accès refusés consécutifs, et déclenche une alarme prolongée au bout de 3 tentatives.

---

## Pour aller plus loin

Ce système n'autorise qu'un seul badge et perd sa configuration à chaque redémarrage (le badge autorisé est figé dans le code). L'exercice suivant (UID_stocker_dans_EEPROM) résout ces deux limites : plusieurs badges peuvent être enregistrés et gérés dynamiquement, et leur liste persiste même quand on éteint l'Arduino.
