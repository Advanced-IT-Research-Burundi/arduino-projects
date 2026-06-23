# Jour 3–4 — LED RGB et mélange de couleurs

## Objectif de l'exercice

Apprendre à contrôler une LED RGB pour créer différentes couleurs en mélangeant rouge, vert et bleu, et comprendre la différence entre allumer/éteindre et faire varier une intensité.

À la fin de cet exercice, tu sauras :
- utiliser `analogWrite()` pour faire varier l'intensité lumineuse (de 0 à 255)
- créer une fonction personnalisée réutilisable (`setColor`)
- composer des couleurs en mélangeant rouge, vert et bleu
- reconnaître les broches PWM nécessaires pour `analogWrite()`

---

## Qu'est-ce qu'une LED RGB ?

Une LED RGB est en réalité 3 LEDs (rouge, verte, bleue) regroupées dans un même boîtier. En faisant varier l'intensité de chacune indépendamment, on peut produire presque n'importe quelle couleur — exactement comme un écran de télévision compose ses images à partir de pixels rouges, verts et bleus.

`analogWrite()` est comme un variateur de lumière (dimmer) : au lieu d'un simple "allumé/éteint", on choisit une valeur entre 0 (éteint) et 255 (intensité maximale).

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| LED RGB (ou 3 LEDs simples rouge/vert/bleu) | 1 |
| Résistances 220 Ω | 3 |
| Breadboard | 1 |
| Fils de connexion | quelques-uns |

---

## Câblage — étape par étape

| Broche LED RGB | Broche Arduino Uno | Rôle |
|---|---|---|
| Rouge | D3 (PWM ~) | Intensité du rouge |
| Vert | D5 (PWM ~) | Intensité du vert |
| Bleu | D6 (PWM ~) | Intensité du bleu |
| Patte commune (cathode ou anode selon le modèle) | GND ou 5V selon le type de LED | Référence commune |

### ⚠️ Avertissement important

`analogWrite()` ne fonctionne que sur certaines broches, marquées par un symbole `~` sur la carte Arduino Uno (broches 3, 5, 6, 9, 10, 11). Si tu utilises une broche sans `~`, le code peut compiler mais la luminosité ne variera pas correctement.

---

## Installer la bibliothèque

Aucune installation nécessaire. `analogWrite()` est une fonction standard fournie par défaut avec Arduino IDE.

---

## Code de l'exercice

```cpp
// *Interfacing RGB LED with Arduino 
// * Author: Osama Ahmed 

//Defining  variable and the GPIO pin on Arduino
int redPin= 3;
int greenPin = 5;
int  bluePin = 6;

void setup() {
  //Defining the pins as OUTPUT
  pinMode(redPin,  OUTPUT);              
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}
void  loop() {
  setColor(255, 0, 0); // Red Color
  delay(1000);
  setColor(0,  255, 0); // Green Color
  delay(1000);
  setColor(0, 0, 255); // Blue Color
  delay(1000);
  setColor(255, 255, 255); // White Color
  delay(1000);
  setColor(170, 0, 255); // Purple Color
  delay(1000);
  setColor(127, 127,  127); // Light Blue
  delay(1000);
  setColor(255, 255, 0);
  delay(10000);
}
void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin,  greenValue);
  analogWrite(bluePin, blueValue);
}
```

### Explication ligne par ligne

`int redPin=3; int greenPin=5; int bluePin=6;` — déclarent les trois broches PWM utilisées, une par couleur primaire.

`void setColor(int redValue, int greenValue, int blueValue)` — c'est une fonction personnalisée créée pour ne pas répéter trois lignes `analogWrite()` chaque fois qu'on veut changer de couleur. On lui donne trois valeurs (rouge, vert, bleu) et elle s'occupe d'appliquer chacune à la bonne broche.

`setColor(255, 0, 0);` — appelle la fonction avec rouge à pleine intensité (255) et vert/bleu éteints (0) → couleur rouge pure.

`setColor(255, 255, 255);` — les trois couleurs à fond en même temps produisent du blanc, comme en peinture lumineuse.

`setColor(170, 0, 255);` — un mélange partiel de rouge et de bleu à pleine puissance donne du violet. On peut ainsi composer n'importe quelle teinte en jouant sur les trois valeurs.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Observe le défilé de couleurs : rouge, vert, bleu, blanc, violet, bleu clair, jaune
5. La dernière couleur (jaune) reste affichée 10 secondes avant de recommencer le cycle

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| Une seule couleur apparaît, jamais de mélange | Une des trois broches n'est pas une broche PWM (`~`) | Vérifier que les broches utilisées sont bien 3, 5, 6, 9, 10 ou 11 |
| Les couleurs semblent inversées (cyan au lieu de rouge, etc.) | LED RGB à cathode commune câblée comme à anode commune (ou l'inverse) | Vérifier le type de ta LED RGB et adapter le câblage de la patte commune |
| Le blanc paraît plutôt jaunâtre ou bleuté | Déséquilibre naturel entre les trois diodes internes | Ajuster légèrement les valeurs (par exemple 255, 230, 255) pour un blanc plus neutre |

---

## Exercices à réaliser

1. **Effet respiration** — Remplace les couleurs fixes par une boucle `for` qui fait varier une seule couleur de 0 à 255 puis de 255 à 0, pour un effet de pulsation douce.
2. **Couleur aléatoire** — Utilise `random(0, 256)` pour générer des valeurs rouge/vert/bleu aléatoires à chaque tour de boucle.
3. **Contrôle par bouton** — Ajoute un bouton qui passe à la couleur suivante de la liste à chaque pression, au lieu d'un défilement automatique.

---

## Pour aller plus loin

La fonction personnalisée `setColor()` est une bonne introduction à l'écriture de tes propres fonctions — une compétence que tu réutiliseras énormément dans des projets plus complexes comme le jeu de lumière (Simon).
