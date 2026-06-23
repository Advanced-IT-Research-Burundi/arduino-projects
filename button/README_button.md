# Jour 3–4 — Bouton poussoir : interrupteur à bascule (toggle)

## Objectif de l'exercice

Apprendre à détecter une pression sur un bouton et à créer un comportement "interrupteur" : chaque appui inverse l'état d'une LED, au lieu de l'allumer seulement pendant que le bouton est maintenu.

À la fin de cet exercice, tu sauras :
- lire l'état d'un bouton avec `digitalRead()`
- détecter une transition (le moment précis où on passe de relâché à appuyé)
- mémoriser un état entre deux tours de boucle avec des variables globales
- comprendre et appliquer un anti-rebond logiciel simple

---

## Qu'est-ce qu'un comportement "toggle" ?

Un bouton classique réagit pendant que tu le maintiens appuyé (comme un buzzer de sonnette). Un comportement *toggle* (bascule) fonctionne comme l'interrupteur d'une chambre : chaque appui inverse l'état, et la lumière reste dans le nouvel état même après avoir relâché le bouton.

Pour cela, l'Arduino doit se souvenir de l'état précédent du bouton afin de détecter le moment exact où il passe de "relâché" à "appuyé" — et non simplement s'il est appuyé ou non.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Bouton poussoir | 1 |
| LED | 1 |
| Résistance 220 Ω (pour la LED) | 1 |
| Breadboard | 1 |
| Fils de connexion | quelques-uns |

---

## Câblage — étape par étape

| Broche | Broche Arduino Uno | Rôle |
|---|---|---|
| Bouton — patte 1 | D2 | Entrée numérique |
| Bouton — patte 2 | GND ou 5V (selon montage) | Référence |
| LED (+) | D13 (via résistance 220 Ω) | Sortie numérique |
| LED (−) | GND | Masse |

### ⚠️ Avertissement important

Ce code utilise `pinMode(button, INPUT);` (entrée simple, sans résistance interne). Avec ce montage, la broche peut "flotter" (lire une valeur instable) si le bouton n'est connecté ni au courant ni à la masse de façon stable. Si tu observes un comportement erratique de la LED, c'est probablement le signe qu'il faut soit ajouter une résistance physique de tirage (pull-down), soit passer à `INPUT_PULLUP` (vu dans d'autres exercices).

---

## Installer la bibliothèque

Aucune installation nécessaire. `digitalRead()` et `digitalWrite()` sont des fonctions standard fournies par défaut avec Arduino IDE.

---

## Code de l'exercice

```cpp
int button = 2; 
int led = 13;   
int ledState = LOW;          
int buttonState;       
int lastButtonState = LOW; 

void setup() {
  pinMode(led, OUTPUT);
  pinMode(button, INPUT); 
  digitalWrite(led, ledState); 
}

void loop() {
  buttonState = digitalRead(button);

  if (buttonState == HIGH && lastButtonState == LOW) {
    
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    
    digitalWrite(led, ledState);
    delay(50); // Anti-rebond
  }

  lastButtonState = buttonState;
}
```

### Explication ligne par ligne

`int ledState = LOW;` et `int lastButtonState = LOW;` — ces variables globales mémorisent un état d'un tour de boucle à l'autre. C'est la clé du comportement toggle : sans elles, l'Arduino "oublierait" ce qui s'est passé juste avant.

`buttonState = digitalRead(button);` — lit l'état actuel du bouton à chaque tour de boucle.

`if (buttonState == HIGH && lastButtonState == LOW)` — cette condition est vraie seulement à l'instant précis où le bouton vient juste d'être appuyé (il était relâché au tour précédent, il est appuyé maintenant). C'est ce qu'on appelle détecter un *front montant*. Sans cette comparaison avec l'état précédent, la LED basculerait en continu tant que le bouton reste appuyé.

`if (ledState == LOW) { ledState = HIGH; } else { ledState = LOW; }` — inverse l'état mémorisé : si la LED était éteinte, on la prévoit allumée, et inversement.

`delay(50); // Anti-rebond` — un bouton mécanique ne change pas d'état instantanément : il "rebondit" électriquement pendant quelques millisecondes. Cette petite pause évite que ces micro-rebonds soient interprétés comme plusieurs appuis distincts.

`lastButtonState = buttonState;` — à la toute fin de chaque tour, on sauvegarde l'état actuel pour pouvoir le comparer au tour suivant.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Appuie une fois sur le bouton : la LED doit s'allumer et **rester** allumée même après avoir relâché
5. Appuie une seconde fois : la LED doit s'éteindre

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| La LED clignote de façon erratique sans bouton appuyé | Broche en `INPUT` simple sans référence stable (flottante) | Vérifier le câblage ou passer en `INPUT_PULLUP` avec une logique inversée |
| Un seul appui bascule la LED plusieurs fois | Rebonds mécaniques non filtrés | Augmenter légèrement le `delay(50)` à 100 ou 150 ms |
| La LED ne réagit jamais | Mauvais branchement du bouton, ou broche 2 déjà utilisée ailleurs | Vérifier le câblage et qu'aucun autre composant n'utilise la broche 2 |

---

## Exercices à réaliser

1. **Compteur d'appuis** — Ajoute une variable qui compte le nombre total de fois où le bouton a été appuyé, et affiche-la dans le moniteur série.
2. **Double LED** — Quand la LED principale est allumée, allume aussi une seconde LED de couleur différente ; éteins les deux ensemble.
3. **Triple état** — Modifie le code pour qu'un cycle de 3 appuis fasse : éteint → faible luminosité → pleine luminosité → éteint (en utilisant `analogWrite()`).

---

## Pour aller plus loin

Cette technique de détection de front montant (comparer l'état actuel à l'état précédent) est réutilisée presque partout en Arduino, y compris dans le jeu de lumière (Simon) pour détecter précisément quand un joueur appuie sur un bouton.
