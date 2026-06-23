# Jour 5–6 — Servomoteur : mouvement de balayage automatique

## Objectif de l'exercice

Apprendre à contrôler un servomoteur pour créer un mouvement de balayage automatique entre 0° et 180°, comme un radar ou une caméra de surveillance qui scanne son environnement.

À la fin de cet exercice, tu sauras :
- utiliser la bibliothèque Servo pour contrôler précisément un angle
- faire varier une position progressivement avec une boucle `for`
- comprendre la différence entre un mouvement progressif et un saut direct de position
- contrôler la vitesse du mouvement avec un `delay()` entre chaque pas

---

## Qu'est-ce qu'un servomoteur ?

Un servomoteur est un moteur spécial capable de se positionner précisément à un angle donné (généralement entre 0° et 180°), et de maintenir cette position tant qu'on ne lui demande pas d'en changer. Contrairement à un moteur classique qui tourne en continu, le servomoteur connaît toujours sa position actuelle.

On le retrouve dans énormément d'applications : direction des voitures télécommandées, bras robotiques, volets automatiques, ou systèmes de visée orientables.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Servomoteur (type SG90 ou similaire) | 1 |
| Breadboard | 1 (optionnel selon montage) |
| Fils de connexion | 3 |

---

## Câblage — étape par étape

| Fil servomoteur | Broche Arduino Uno | Rôle |
|---|---|---|
| Fil rouge (alimentation) | 5V | Alimentation |
| Fil marron ou noir (masse) | GND | Masse |
| Fil orange ou jaune (signal) | D9 (PWM ~) | Signal de commande |

### ⚠️ Avertissement important

Un servomoteur peut consommer beaucoup de courant au démarrage, surtout s'il rencontre une résistance mécanique. Si tu observes des redémarrages inattendus de l'Arduino ou un comportement instable, envisage une alimentation externe dédiée pour le servomoteur plutôt que de tout tirer depuis la broche 5V de l'Arduino.

---

## Installer la bibliothèque

Aucune installation nécessaire. La bibliothèque **Servo** est fournie par défaut avec Arduino IDE — elle n'apparaît donc pas dans le gestionnaire de bibliothèques, mais s'utilise directement avec `#include <Servo.h>`.

---

## Code de l'exercice

```cpp
#include <Servo.h>

Servo monServo;  // Création de l'objet Servo
  int i=0;

void setup() {
  monServo.attach(9);  // On lie le servo à la broche 9
}

void loop() {
  for(i; i<180; i++) {
    monServo.write(i);
    delay(15);
  }

  //if (i==180){
    for(i; i>0; i--) {
      monServo.write(i);
      delay(20);
    }
  //}

  /*monServo.write(0);   // Va à 0 degré
  delay(1000);
  
  monServo.write(90);  // Va à 90 degrés
  delay(1000);
  
  monServo.write(180); // Va à 180 degrés
  delay(1000);*/


}
```

### Explication ligne par ligne

`#include <Servo.h>` — importe la bibliothèque qui fournit toutes les fonctions nécessaires pour contrôler un servomoteur.

`Servo monServo;` — crée un objet `monServo` qui représente ton servomoteur physique. Toutes les commandes de mouvement passeront par cet objet.

`monServo.attach(9);` — relie l'objet logiciel `monServo` à la broche physique 9, là où le fil de signal est réellement branché.

`int i=0;` — cette variable est déclarée en dehors de `setup()` et `loop()` (variable globale), ce qui lui permet de conserver sa valeur d'un tour de boucle à l'autre — essentiel ici puisque le mouvement de balayage doit reprendre exactement où il s'est arrêté.

`for(i; i<180; i++) { monServo.write(i); delay(15); }` — cette boucle fait progresser `i` d'une unité à chaque tour, de sa valeur actuelle jusqu'à 180, en demandant au servo de se positionner à chaque angle intermédiaire avec une petite pause de 15 millisecondes. C'est ce qui crée un mouvement fluide et progressif plutôt qu'un saut brusque directement à 180°.

`for(i; i>0; i--) { monServo.write(i); delay(20); }` — une fois arrivé à 180°, cette seconde boucle fait l'inverse : elle ramène progressivement `i` vers 0, créant le mouvement de retour. La pause est légèrement plus longue (20 ms au lieu de 15), ce qui rend le retour un peu plus lent que l'aller.

Les lignes en commentaire à la fin montrent une approche alternative plus simple : aller directement à des positions fixes (0°, 90°, 180°) avec des pauses, sans mouvement progressif intermédiaire — utile pour comparer les deux approches.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Observe le bras du servomoteur : il doit balayer progressivement de 0° à 180°, puis revenir de 180° à 0°, en boucle continue

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| Le servomoteur vibre ou tremble sans se positionner correctement | Alimentation insuffisante ou instable | Utiliser une alimentation externe dédiée pour le servomoteur |
| Le servomoteur saute brusquement de position parfois | Boucle `for` mal réinitialisée, ou `i` qui dépasse les bornes attendues | Vérifier que les conditions `i<180` et `i>0` sont cohérentes avec les incréments/décréments |
| Aucun mouvement, le servo reste figé | Mauvais branchement du fil de signal, ou broche non-PWM utilisée | Vérifier que le fil orange/jaune est bien sur une broche PWM (`~`) comme la broche 9 |

---

## Exercices à réaliser

1. **Vitesse réglable** — Remplace les valeurs fixes de `delay(15)` et `delay(20)` par une variable unique, et teste différentes vitesses de balayage.
2. **Positions aléatoires** — Plutôt qu'un balayage continu, fais aller le servomoteur vers des positions aléatoires avec `random(0, 181)`, avec une pause entre chaque déplacement.
3. **Contrôle par potentiomètre** — Combine cet exercice avec celui du potentiomètre : utilise sa position pour définir l'angle du servomoteur en temps réel, plutôt qu'un mouvement automatique.

---

## Pour aller plus loin

Ce mouvement de balayage automatique est la base de nombreux projets de robotique, notamment les systèmes de détection radar qui combinent un servomoteur avec un capteur ultrasonique fixé sur son bras, pour scanner une zone entière à la recherche d'obstacles.
