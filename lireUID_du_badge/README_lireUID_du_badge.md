# RFID Jour 1 — Lire l'UID d'un badge et vérifier l'accès

## Objectif de l'exercice

Apprendre à lire l'identifiant unique (UID) d'un badge ou d'une carte RFID, à le comparer avec un UID connu codé dans le programme, et à afficher "ACCÈS AUTORISÉ" ou "ACCÈS REFUSÉ" dans le moniteur série.

À la fin de cet exercice, tu sauras :
- initialiser et utiliser le module RFID RC522 avec la bibliothèque MFRC522
- lire et afficher l'UID d'un badge en hexadécimal
- stocker un UID dans un tableau d'octets (`byte[]`) pour le comparer
- comprendre comment fonctionne une comparaison octet par octet

---

## Qu'est-ce qu'un UID RFID ?

Chaque carte ou badge RFID contient une puce avec un identifiant unique (UID — *Unique IDentifier*), gravé en usine et impossible à modifier. Il est composé de 4 octets (pour les cartes standard), chacun représenté en hexadécimal (base 16 : les chiffres vont de 0 à F).

Par exemple, l'UID `53 B0 88 0F` identifie une carte précise parmi des milliards d'autres — c'est l'équivalent d'une empreinte digitale numérique pour chaque carte.

Dans cet exercice, ton UID personnel (`0X53, 0XB0, 0X88, 0XF`) est déjà enregistré dans le tableau `maCarte[]`. Pour fonctionner avec ta propre carte, tu devras d'abord lire ton UID dans le moniteur série, puis remplacer ces valeurs.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Module RFID RC522 | 1 |
| Badge ou carte RFID | 1 ou plusieurs |
| Breadboard | 1 |
| Fils de connexion | 7 |

---

## Câblage — étape par étape

| Broche RC522 | Broche Arduino Uno | Rôle |
|---|---|---|
| SDA (SS) | D10 | Sélection esclave SPI |
| SCK | D13 | Horloge SPI |
| MOSI | D11 | Données Arduino → RC522 |
| MISO | D12 | Données RC522 → Arduino |
| RST | D9 | Réinitialisation du module |
| GND | GND | Masse |
| 3.3V | **3.3V uniquement** | Alimentation |

### ⚠️ Avertissement important

Le RC522 fonctionne obligatoirement en **3.3V**. Le brancher sur 5V peut le détruire définitivement et instantanément. Sur l'Arduino Uno, la broche 3.3V se trouve juste à côté de la broche 5V — vérifie deux fois avant de mettre sous tension.

---

## Installer la bibliothèque

1. Ouvre Arduino IDE
2. Va dans **Outils → Gérer les bibliothèques** (`Ctrl + Maj + I`)
3. Recherche `MFRC522` et installe la version de **GithubCommunity** (ou miguelbalboa)

La bibliothèque `SPI.h` est fournie par défaut avec Arduino IDE — aucune installation nécessaire pour elle.

---

## Code de l'exercice

```cpp
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
```

### Explication ligne par ligne

`#define SS_PIN 10` et `#define RST_PIN 9` — définissent les broches utilisées pour la communication SPI avec le RC522. Ces deux broches sont les seules "configurables" ; les autres (MOSI, MISO, SCK) sont fixées par le protocole SPI et ne se changent pas.

`MFRC522 rfid(SS_PIN, RST_PIN);` — crée l'objet logiciel `rfid` qui représente ton module physique. Toutes les actions de lecture passeront par cet objet.

`SPI.begin();` — initialise le bus de communication SPI (les 4 fils qui relient l'Arduino au RC522). C'est obligatoire avant d'utiliser le module.

`rfid.PCD_Init();` — initialise le module RC522 lui-même (*PCD* = Proximity Coupling Device, le lecteur).

`if (!rfid.PICC_IsNewCardPresent()) return;` — vérifie si une nouvelle carte est à portée du lecteur. Si non (`!`), on sort immédiatement de `loop()` et on recommence. Le *PICC* désigne la carte elle-même (*Proximity Integrated Circuit Card*).

`byte newCarte[rfid.uid.size];` et `byte maCarte[rfid.uid.size] = {0X53, 0XB0, 0X88, 0XF};` — déclarent deux tableaux d'octets (`byte`) : `newCarte` recevra l'UID de la carte scannée, et `maCarte` contient l'UID de la carte autorisée (à remplacer par le tien).

`newCarte[i] = rfid.uid.uidByte[i];` — à chaque tour de la boucle, copie l'octet numéro `i` de l'UID lu dans le tableau `newCarte`.

`if (maCarte[i] != newCarte[i])` — compare le premier octet des deux tableaux. Si différent → "ACCÈS REFUSÉ", sinon → "ACCÈS AUTORISÉ". **Attention** : ce code ne vérifie en réalité que le premier octet à cause du `break` dans les deux branches du `if/else`. C'est une limitation de cette version — l'exercice suivant corrige cela avec une comparaison complète.

`rfid.PICC_HaltA();` — met la carte en veille après la lecture pour libérer le lecteur et se préparer à une prochaine détection.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Ouvre le **Moniteur série** (`Ctrl + Maj + M`), réglé sur **9600 bauds**
5. Approche ton badge RFID : l'UID s'affiche, puis le résultat "ACCÈS AUTORISÉ" ou "ACCÈS REFUSÉ"
6. Si ton UID ne correspond pas encore à `maCarte[]`, copie l'UID affiché et remplace les valeurs dans le tableau

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| Rien ne s'affiche dans le moniteur série | Module alimenté en 5V ou câblage SPI incorrect | Vérifier que le RC522 est bien sur 3.3V et que les broches MOSI/MISO/SCK/SDA sont correctement câblées |
| Le code affiche toujours "ACCÈS AUTORISÉ" même avec un mauvais badge | Le `break` dans le `else` stoppe la boucle dès le premier octet correct | C'est un comportement connu de cette version du code — l'exercice suivant (UID_LED_BUZZER) corrige ce problème |
| L'UID affiché change à chaque scan du même badge | Contact instable entre le badge et le lecteur | Maintenir le badge immobile quelques secondes, vérifier les connexions du module |

---

## Exercices à réaliser

1. **Trouver ton UID** — Approche plusieurs badges différents et note leurs UIDs dans le moniteur série. Mets à jour `maCarte[]` avec ton badge personnel.
2. **Corriger la comparaison** — Modifie la boucle de comparaison pour qu'elle vérifie les 4 octets (et non seulement le premier), avant d'afficher le résultat final.
3. **Affichage amélioré** — Ajoute un affichage formaté de l'UID avec des tirets entre chaque octet : `53-B0-88-0F` plutôt que `53 B0 88 F`.

---

## Pour aller plus loin

La comparaison octet par octet de cet exercice est la base de tout système de contrôle d'accès RFID. L'exercice suivant (UID_LED_BUZZER) corrige la logique de comparaison et ajoute des retours visuels et sonores pour en faire un vrai système d'accès.
