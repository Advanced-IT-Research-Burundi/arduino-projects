# Jour 1–2 — Station météo : DHT11 affiché sur écran LCD

## Objectif de l'exercice

Combiner le capteur DHT11 et l'écran LCD I2C pour créer une mini station météo qui affiche température et humidité directement sur l'écran, sans dépendre de l'ordinateur.

À la fin de cet exercice, tu sauras :
- faire fonctionner ensemble deux composants qui utilisent des protocoles différents (1 fil pour le DHT, I2C pour le LCD)
- afficher des valeurs numériques de capteur sur un écran physique
- structurer du texte sur plusieurs lignes avec `setCursor()`
- effacer l'écran proprement entre deux mises à jour avec `lcd.clear()`

---

## Pourquoi combiner ces deux composants ?

Le DHT11 mesure les données mais ne peut pas les afficher lui-même. Le LCD peut afficher du texte mais ne mesure rien. En les combinant, on obtient un véritable petit appareil autonome — comme une station météo de salon — qui n'a plus besoin d'être branché à un ordinateur pour être utile.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Capteur DHT11 | 1 |
| Écran LCD 1602 avec module I2C | 1 |
| Breadboard | 1 |
| Fils de connexion | 7 |

---

## Câblage — étape par étape

| Broche composant | Broche Arduino Uno | Rôle |
|---|---|---|
| DHT11 — VCC | 5V | Alimentation |
| DHT11 — OUT | D2 | Signal de données |
| DHT11 — GND | GND | Masse |
| LCD I2C — VCC | 5V | Alimentation |
| LCD I2C — GND | GND | Masse |
| LCD I2C — SDA | A4 | Données I2C |
| LCD I2C — SCL | A5 | Horloge I2C |

### ⚠️ Avertissement important

Avant de lancer ce code, assure-toi d'avoir déjà identifié l'adresse I2C exacte de ton écran (avec le scanner I2C vu dans l'exercice précédent). Si l'adresse `0x27` utilisée ici ne correspond pas à ton module, l'écran restera vide même si le reste du circuit fonctionne correctement.

---

## Installer la bibliothèque

Deux bibliothèques externes sont nécessaires :

1. **DHT sensor library** par Adafruit (rechercher `DHT sensor library` dans le gestionnaire de bibliothèques)
2. **LiquidCrystal I2C** par Frank de Brabander (rechercher `LiquidCrystal I2C`)

`Wire.h` n'a pas besoin d'être installée : elle est fournie par défaut avec Arduino IDE.

---

## Code de l'exercice

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  delay(2000);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd.clear(); // On efface l'écran avant d'écrire
  
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Hum:  ");
  lcd.print(h);
  lcd.print(" %");
}
```

### Explication ligne par ligne

`#define DHTPIN 2` et `#define DHTTYPE DHT11` — définissent des constantes nommées pour la broche et le modèle du capteur, ce qui rend le code plus facile à adapter si tu changes de broche ou de modèle plus tard.

`DHT dht(DHTPIN, DHTTYPE);` et `LiquidCrystal_I2C lcd(0x27, 16, 2);` — créent les deux objets représentant respectivement le capteur et l'écran. Ces deux objets fonctionnent de façon totalement indépendante, même s'ils sont utilisés ensemble dans le même programme.

`dht.begin();` et `lcd.init(); lcd.backlight();` — initialisent chacun des deux composants au démarrage, l'un à la suite de l'autre. L'ordre n'a pas d'importance ici car ils n'interagissent pas directement entre eux.

`delay(2000);` — comme pour l'exercice DHT seul, cette pause est nécessaire car le capteur a besoin d'au moins 2 secondes entre deux lectures.

`lcd.clear();` — efface tout le contenu de l'écran avant d'afficher les nouvelles valeurs. Sans cette ligne, les anciens chiffres pourraient se superposer visuellement aux nouveaux si le nombre de caractères change (par exemple "21.5" qui devient "9.8" laisserait des traces du chiffre précédent).

`lcd.setCursor(0, 0); lcd.print("Temp: "); lcd.print(t); lcd.print(" C");` — ces trois appels à `print()` s'enchaînent sur la même ligne : d'abord le texte fixe "Temp: ", puis la valeur numérique de la température, puis l'unité " C". Le LCD les affiche les uns après les autres, sans retour à la ligne automatique.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Observe l'écran LCD : il doit afficher la température sur la ligne du haut et l'humidité sur la ligne du bas
5. Les valeurs se mettent à jour toutes les 2 secondes

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| L'écran reste vide alors que le programme semble fonctionner | Mauvaise adresse I2C | Relancer le scanner I2C de l'exercice précédent pour vérifier l'adresse exacte |
| Les valeurs affichées sont des nombres incohérents (très grands ou négatifs) | Lecture du DHT11 échouée mais non vérifiée | Ajouter une vérification avec `isnan()` comme dans l'exercice DHT11 seul |
| L'écran clignote ou semble instable | `lcd.clear()` exécuté trop fréquemment, créant un effet visuel saccadé | C'est normal avec un `delay(2000)` ; pour réduire l'effet, n'efface que les caractères qui changent réellement |

---

## Exercices à réaliser

1. **Vérification de lecture** — Ajoute la vérification `isnan()` vue dans l'exercice DHT11 seul, pour éviter d'afficher des valeurs invalides sur l'écran en cas d'échec de lecture.
2. **Alerte visuelle** — Ajoute une LED qui s'allume automatiquement si la température dépasse un certain seuil, en plus de l'affichage LCD.
3. **Affichage alterné** — Au lieu d'afficher température et humidité simultanément sur deux lignes, fais-les alterner toutes les 3 secondes sur une seule ligne en gros caractères (en réutilisant la même position de curseur).

---

## Pour aller plus loin

Cette station météo de base peut évoluer vers un véritable système d'alerte environnementale en y ajoutant un buzzer ou un relais qui se déclenche automatiquement selon les conditions mesurées — une logique très proche de celle utilisée dans l'exercice combinant le capteur ultrasonique et le buzzer.
