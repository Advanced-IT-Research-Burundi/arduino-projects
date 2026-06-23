# Jour 1–2 — LED qui clignote (allumage simple)

## Objectif de l'exercice

Apprendre à contrôler une LED en l'allumant et en l'éteignant à intervalle régulier, et étendre cette logique à plusieurs LEDs.

À la fin de cet exercice, tu sauras :
- déclarer une broche comme sortie avec `pinMode()`
- allumer et éteindre une LED avec `digitalWrite()`
- enchaîner plusieurs actions dans `loop()` pour créer une séquence
- commenter du code pour désactiver temporairement une partie sans la supprimer

---

## Qu'est-ce qu'une LED ?

Une LED (*Light Emitting Diode*, diode électroluminescente) est un petit composant qui s'allume quand le courant la traverse dans le bon sens. Contrairement à une ampoule classique, elle ne fonctionne que dans un seul sens — c'est pourquoi elle a une patte longue (+) et une patte courte (−).

On peut la comparer à un interrupteur à sens unique : l'électricité ne peut passer que dans une direction.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| LED (vert, bleu, et éventuellement jaune, rouge) | jusqu'à 4 |
| Résistances 220 Ω | autant que de LEDs |
| Breadboard | 1 |
| Fils de connexion | quelques-uns par LED |

---

## Câblage — étape par étape

| Broche LED | Broche Arduino Uno | Rôle |
|---|---|---|
| Patte longue (+) LED verte | D7 (via résistance 220 Ω) | Sortie numérique |
| Patte longue (+) LED bleue | D8 (via résistance 220 Ω) | Sortie numérique |
| Patte courte (−) de chaque LED | GND | Masse commune |

### ⚠️ Avertissement important

Ne jamais connecter une LED directement à une broche Arduino sans résistance. Sans résistance, le courant est trop fort, la LED brûle en quelques secondes et la broche Arduino peut être endommagée. La résistance 220 Ω limite le courant à une valeur sûre.

---

## Installer la bibliothèque

Aucune installation nécessaire. `pinMode()` et `digitalWrite()` sont des fonctions standard fournies par défaut avec Arduino IDE.

---

## Code de l'exercice

```cpp
int vert = 7;
int bleu = 8;
//int jaune = 12;
//int rouge = 4;

void setup() {
  pinMode(vert, OUTPUT);
  pinMode(bleu, OUTPUT);
  //pinMode(jaune, OUTPUT);
  //pinMode(rouge, OUTPUT);

}

void loop() {
  digitalWrite(vert, HIGH); 
  delay(2000);           
  digitalWrite(vert, LOW);  

  digitalWrite(bleu, HIGH); 
  delay(2000);           
  digitalWrite(bleu, LOW); 

  /*digitalWrite(jaune, HIGH); 
  delay(2000);           
  digitalWrite(jaune, LOW);    

  digitalWrite(rouge, HIGH); 
  delay(10000);           
  digitalWrite(rouge, LOW);
*/}
```

### Explication ligne par ligne

`int vert = 7;` et `int bleu = 8;` — déclarent deux variables qui représentent les numéros de broches utilisées. Donner un nom clair plutôt qu'un simple chiffre rend le code plus facile à lire.

`pinMode(vert, OUTPUT);` — indique à l'Arduino que la broche 7 va servir à envoyer du courant (et non à en recevoir). C'est obligatoire avant d'utiliser `digitalWrite()` sur cette broche.

`digitalWrite(vert, HIGH);` — envoie du courant (5V) sur la broche 7, ce qui allume la LED verte.

`delay(2000);` — met le programme en pause pendant 2000 millisecondes, soit 2 secondes, avant de passer à la ligne suivante.

`digitalWrite(vert, LOW);` — coupe le courant (0V) sur la broche 7, ce qui éteint la LED verte.

Les lignes entre `/* */` sont des commentaires multi-lignes : le code à l'intérieur est ignoré par l'Arduino. C'est une technique pratique pour désactiver temporairement une partie du programme (ici, la LED jaune et la LED rouge) sans avoir à la supprimer — utile pour tester progressivement.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Observe : la LED verte doit s'allumer 2 secondes, puis s'éteindre ; la LED bleue fait ensuite la même chose
5. Le cycle se répète indéfiniment

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| Aucune LED ne s'allume | Patte de la LED inversée, ou résistance manquante | Vérifier le sens (patte longue = +) et la présence de la résistance |
| Une seule LED fonctionne | Mauvais numéro de broche dans le code par rapport au câblage réel | Vérifier que `int vert = 7;` correspond bien au fil branché sur D7 |
| Le programme ne compile pas après avoir décommenté le jaune/rouge | Une ligne `pinMode()` correspondante n'a pas été décommentée aussi | Décommenter à la fois la déclaration de la variable ET le `pinMode()` associé |

---

## Exercices à réaliser

1. **Décommenter progressivement** — Active la LED jaune en retirant les `//` devant sa déclaration et son `pinMode()`. Fais de même avec la rouge.
2. **Chenillard** — Modifie l'ordre et les délais pour créer un effet de LEDs qui s'allument en cascade, comme une vague lumineuse.
3. **Vitesse variable** — Remplace les `delay(2000)` par des valeurs différentes pour chaque LED et observe le nouveau rythme.

---

## Pour aller plus loin

Une fois cet exercice maîtrisé, passe à la LED RGB pour découvrir comment mélanger les couleurs avec `analogWrite()` au lieu de simplement allumer/éteindre.
