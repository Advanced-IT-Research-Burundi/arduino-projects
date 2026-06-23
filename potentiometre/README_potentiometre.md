# Jour 3–4 — Potentiomètre : variateur de luminosité avec interrupteur

## Objectif de l'exercice

Apprendre à lire un potentiomètre pour contrôler la luminosité d'une LED de façon continue, tout en ajoutant un bouton qui active ou désactive complètement le système.

À la fin de cet exercice, tu sauras :
- lire une valeur analogique avec `analogRead()` et la convertir avec `map()`
- contrôler une intensité lumineuse avec `analogWrite()`
- combiner une entrée analogique (potentiomètre) avec une entrée numérique (bouton)
- calculer une tension réelle à partir d'une valeur analogique brute

---

## Qu'est-ce qu'un potentiomètre ?

Un potentiomètre est une résistance variable que l'on règle manuellement, généralement en tournant un petit axe. Il possède trois broches : les deux extrémités sont reliées à l'alimentation (5V) et à la masse (GND), tandis que la broche centrale (le curseur) renvoie une tension qui varie selon la position de l'axe — c'est cette broche centrale qu'on branche sur une entrée analogique.

C'est exactement le même composant qu'on retrouve dans les boutons de volume ou de réglage de luminosité des appareils électroniques classiques.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Potentiomètre | 1 |
| LED | 1 |
| Résistance 220 Ω (pour la LED) | 1 |
| Bouton poussoir | 1 |
| Breadboard | 1 |
| Fils de connexion | quelques-uns |

---

## Câblage — étape par étape

| Broche composant | Broche Arduino Uno | Rôle |
|---|---|---|
| Potentiomètre — curseur (broche centrale) | A0 | Entrée analogique |
| Potentiomètre — extrémité 1 | 5V | Alimentation |
| Potentiomètre — extrémité 2 | GND | Masse |
| LED (+) | D9 (PWM ~, via résistance 220 Ω) | Sortie analogique |
| LED (−) | GND | Masse |
| Bouton — patte 1 | D2 | Entrée numérique |
| Bouton — patte 2 | GND ou 5V (selon montage) | Référence |

### ⚠️ Avertissement important

La broche utilisée pour la LED doit être une broche PWM (marquée `~` sur l'Arduino Uno, comme 3, 5, 6, 9, 10 ou 11), car `analogWrite()` ne fonctionne que sur ces broches spécifiques.

---

## Installer la bibliothèque

Aucune installation nécessaire. `analogRead()`, `analogWrite()`, `digitalRead()` et `map()` sont des fonctions standard fournies par défaut avec Arduino IDE.

---

## Code de l'exercice

```cpp
int pot = A0;
int led = 9;
int button = 2;      
int lastButtonState = LOW; 

bool systemActif = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pot, INPUT);
  pinMode(led, OUTPUT);
  analogWrite(led, 0);
  pinMode(button, INPUT); 

}

void loop() {
  int buttonState = digitalRead(button);
  int valeurpot = analogRead(pot);
  int ledState = map(valeurpot, 0, 1023, 0, 255);

  if (buttonState == HIGH && lastButtonState == LOW) {
    systemActif = !systemActif;
    
  }
  if (systemActif == true) {
      analogWrite(led, ledState);      

    } else {
      analogWrite(led, 0);      
    }

  lastButtonState = buttonState;


  // put your main code here, to run repeatedly:
  //with one single LED
  //int valeurpot = analogRead(pot);
  float tension = valeurpot * 5.0 / 1023.0 ;
  Serial.println(tension);
  /*int luminosite = map(valeurpot, 0, 1023, 0, 255);
  analogWrite(led, luminosite);
  delay(50);*/

}
```

### Explication ligne par ligne

`bool systemActif = false;` — cette variable mémorise si le système est actuellement activé ou non. Le type `bool` ne peut contenir que deux valeurs : `true` ou `false`.

`int valeurpot = analogRead(pot);` — lit la position actuelle du potentiomètre, sous forme d'une valeur comprise entre 0 (tourné complètement dans un sens) et 1023 (tourné complètement dans l'autre sens).

`int ledState = map(valeurpot, 0, 1023, 0, 255);` — la fonction `map()` convertit une valeur d'une plage vers une autre. Ici, elle transforme la plage de lecture du potentiomètre (0 à 1023) vers la plage acceptée par `analogWrite()` (0 à 255), car ces deux fonctions ne travaillent pas avec la même échelle de valeurs.

`if (buttonState == HIGH && lastButtonState == LOW) { systemActif = !systemActif; }` — détecte une pression sur le bouton (front montant) et inverse l'état du système avec `!systemActif` (l'opérateur `!` signifie "non" : si c'était vrai, ça devient faux, et inversement). C'est la même logique de toggle vue dans l'exercice du bouton simple.

`if (systemActif == true) { analogWrite(led, ledState); } else { analogWrite(led, 0); }` — si le système est actif, la LED suit la position du potentiomètre ; sinon, elle reste éteinte (0) quelle que soit la position du potentiomètre.

`float tension = valeurpot * 5.0 / 1023.0;` — calcule la tension électrique réelle correspondant à la lecture analogique. L'Arduino Uno fonctionne avec une tension de référence de 5V répartie sur 1024 paliers (0 à 1023), donc cette formule convertit la valeur brute en volts.

Les lignes en commentaire à la fin montrent une version simplifiée antérieure du code (sans bouton ni système actif/inactif), conservée à titre de référence ou pour comparaison.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Ouvre le **Moniteur série** (`Ctrl + Maj + M`), réglé sur 9600 bauds
5. Appuie sur le bouton pour activer le système
6. Tourne le potentiomètre : la luminosité de la LED doit varier en continu
7. Appuie à nouveau sur le bouton : la LED doit s'éteindre, quelle que soit la position du potentiomètre
8. Observe la tension affichée dans le moniteur série, qui varie entre 0 et 5V selon la position du potentiomètre

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| La LED ne s'allume jamais, même après avoir activé le système | Broche LED non-PWM utilisée | Vérifier que la broche est bien marquée `~` sur la carte Arduino |
| Le bouton ne semble rien faire | Câblage en `INPUT` simple sans référence stable | Vérifier le câblage du bouton, envisager `INPUT_PULLUP` avec logique inversée |
| La tension affichée ne correspond pas à ce qui est attendu | Calcul basé sur 1023 plutôt que sur la tension réelle d'alimentation de l'Arduino | Vérifier que l'Arduino est bien alimenté en 5V stable (et non via une source USB faible) |

---

## Exercices à réaliser

1. **Indicateur de seuil** — Allume une seconde LED uniquement lorsque le potentiomètre dépasse 50% de sa course (valeur lue supérieure à 511).
2. **Affichage en pourcentage** — Ajoute une ligne qui affiche la position du potentiomètre sous forme de pourcentage (0 à 100%) plutôt qu'en tension, en utilisant `map()`.
3. **Verrouillage progressif** — Plutôt qu'un système tout ou rien, fais en sorte qu'un appui sur le bouton active une transition douce (fade-in) de la LED jusqu'à la luminosité actuelle du potentiomètre.

---

## Pour aller plus loin

La fonction `map()` utilisée ici pour convertir une plage de valeurs est une fonction extrêmement utile que tu retrouveras dans presque tous les projets impliquant des capteurs analogiques, comme le servomoteur qui utilise une logique similaire pour convertir un angle en position physique.
