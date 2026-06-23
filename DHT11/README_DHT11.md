# Jour 1–2 — Capteur DHT11 : température et humidité

## Objectif de l'exercice

Apprendre à lire les valeurs de température et d'humidité ambiante grâce au capteur DHT11, et les afficher dans le moniteur série.

À la fin de cet exercice, tu sauras :
- installer et utiliser une bibliothèque externe (DHT)
- lire une température et une humidité avec `readTemperature()` et `readHumidity()`
- vérifier qu'une lecture de capteur est valide avant de l'utiliser
- formater un affichage clair dans le moniteur série avec `Serial.print()`

---

## Qu'est-ce que le capteur DHT11 ?

Le DHT11 est un capteur combiné qui mesure à la fois la température ambiante (en degrés Celsius) et le taux d'humidité de l'air (en pourcentage). Il communique avec l'Arduino via un seul fil de données, en envoyant un signal numérique codé qu'une bibliothèque dédiée sait interpréter.

C'est le même type de capteur qu'on retrouve dans les petites stations météo domestiques.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Capteur DHT11 (module 3 broches) | 1 |
| Breadboard | 1 |
| Fils de connexion | 3 |

---

## Câblage — étape par étape

| Broche DHT11 | Broche Arduino Uno | Rôle |
|---|---|---|
| VCC | 5V | Alimentation |
| OUT (ou DATA) | D2 | Signal de données |
| GND | GND | Masse |

### ⚠️ Avertissement important

Le DHT11 a besoin d'au moins 2 secondes entre deux lectures pour fonctionner correctement. Si tu essaies de le lire plus fréquemment, tu obtiendras des valeurs invalides ou répétées — c'est pourquoi le code attend toujours avec `delay(2000)` avant chaque lecture.

---

## Installer la bibliothèque

1. Ouvre Arduino IDE
2. Va dans **Outils → Gérer les bibliothèques** (ou `Ctrl + Maj + I`)
3. Dans la barre de recherche, tape `DHT sensor library`
4. Installe celle proposée par **Adafruit** (elle te proposera peut-être aussi d'installer `Adafruit Unified Sensor` — accepte cette dépendance)

---

## Code de l'exercice

```cpp
#include "DHT.h"

/*#define DHTPIN 2     // Broche où est connecté le capteur
#define DHTTYPE DHT11   // Ou DHT22 si tu utilises un DHT22
DHT dht(DHTPIN, DHTTYPE);
*/

// ou bien faire
DHT dht(2, DHT11); //Capteur branché sur la broche 2

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000); // Le DHT a besoin de 2 secondes entre chaque lecture

  float humidite = dht.readHumidity();
  float temperature = dht.readTemperature(); // En Celsius

  // Vérification si la lecture a échoué
  if (isnan(humidite) || isnan(temperature)) {
    Serial.println("Échec de lecture du capteur DHT !");
    return;
  }

  Serial.print("Humidité: ");
  Serial.print(humidite);
  Serial.print("%\t");
  Serial.print("Température: ");
  Serial.print(temperature);
  Serial.println(" *C");
}
```

### Explication ligne par ligne

`#include "DHT.h"` — importe la bibliothèque qui contient toutes les fonctions nécessaires pour communiquer avec le capteur DHT11.

`DHT dht(2, DHT11);` — crée un objet `dht` représentant le capteur physique, en précisant qu'il est branché sur la broche 2 et qu'il s'agit d'un modèle DHT11 (et non DHT22, qui est plus précis mais utilise la même bibliothèque).

Les lignes en commentaire `/* ... */` montrent une syntaxe alternative équivalente, avec des constantes nommées (`DHTPIN`, `DHTTYPE`) plutôt que des valeurs écrites directement — les deux fonctionnent, c'est une question de préférence de style.

`dht.begin();` — initialise la communication avec le capteur au démarrage du programme.

`float humidite = dht.readHumidity();` et `float temperature = dht.readTemperature();` — lisent respectivement le taux d'humidité et la température actuelle. Le type `float` est utilisé car ces valeurs peuvent comporter des décimales.

`if (isnan(humidite) || isnan(temperature))` — `isnan()` signifie *is not a number* (n'est pas un nombre). Si la lecture du capteur échoue (mauvais contact, capteur défectueux, lecture trop rapprochée), la fonction retourne une valeur invalide que `isnan()` permet de détecter, pour éviter d'afficher des données fausses.

`return;` — si la lecture a échoué, cette instruction arrête immédiatement le tour de boucle actuel et recommence au début de `loop()`, sans essayer d'afficher des valeurs invalides.

`Serial.print("%\t");` — le `\t` insère une tabulation, ce qui aligne proprement l'affichage entre l'humidité et la température dans le moniteur série.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Ouvre le **Moniteur série** (`Ctrl + Maj + M`), réglé sur 9600 bauds
5. Observe l'affichage de l'humidité et de la température, mis à jour toutes les 2 secondes

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| "Échec de lecture du capteur DHT !" en boucle | Mauvais câblage, ou capteur branché sur la mauvaise broche | Vérifier que la broche OUT est bien sur D2, comme indiqué dans `DHT dht(2, DHT11);` |
| Erreur de compilation `DHT.h: No such file` | Bibliothèque non installée | Réinstaller "DHT sensor library" par Adafruit |
| Les valeurs affichées ne changent jamais | Lecture trop rapide (moins de 2 secondes entre deux lectures) | Vérifier la présence du `delay(2000)` en début de `loop()` |

---

## Exercices à réaliser

1. **Seuils d'alerte** — Affiche un message supplémentaire "Trop chaud !" si la température dépasse 30°C, ou "Trop humide !" si l'humidité dépasse 70%.
2. **Conversion Fahrenheit** — Ajoute une ligne qui convertit et affiche également la température en degrés Fahrenheit (formule : F = C × 9/5 + 32).
3. **Moyenne sur 5 lectures** — Stocke les 5 dernières températures lues dans un tableau et calcule leur moyenne, pour lisser les petites variations.

---

## Pour aller plus loin

Une fois cet exercice maîtrisé, l'étape naturelle est de combiner ce capteur avec un écran LCD pour afficher température et humidité directement sur l'objet, sans dépendre du moniteur série — c'est exactement ce que fait l'exercice "LCD_DHT".
