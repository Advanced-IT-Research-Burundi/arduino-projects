# Jour 5–6 — Buzzer : jouer des sons simples avec tone()

## Objectif de l'exercice

Apprendre à utiliser un buzzer passif pour produire des sons de fréquences différentes à l'aide de la fonction `tone()`.

À la fin de cet exercice, tu sauras :
- utiliser `tone()` pour jouer un son à une fréquence précise
- utiliser `noTone()` pour arrêter un son
- comprendre la relation entre fréquence (Hz) et hauteur du son (aigu/grave)
- enchaîner plusieurs sons différents dans `loop()`

---

## Qu'est-ce qu'un buzzer passif ?

Un buzzer passif ne produit aucun son par lui-même : il vibre à la fréquence exacte qu'on lui demande via la fonction `tone()`. Plus la fréquence est élevée (en Hertz, Hz), plus le son est aigu — exactement comme une corde de guitare courte vibre plus vite et produit un son plus haut qu'une corde longue.

C'est différent d'un buzzer actif, qui ne sait produire qu'un seul bip fixe dès qu'on lui envoie du courant, sans contrôle de la fréquence.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Buzzer passif | 1 |
| Breadboard | 1 |
| Fils de connexion | 2 |

---

## Câblage — étape par étape

| Broche buzzer | Broche Arduino Uno | Rôle |
|---|---|---|
| Broche positive | D8 | Signal sonore (`tone()`) |
| Broche négative (GND) | GND | Masse |

### ⚠️ Avertissement important

Il n'y a pas de risque de tension ici, mais attention à ne pas confondre buzzer actif et passif : si jamais tu utilises un buzzer actif avec ce même code, il produira toujours le même son quelle que soit la fréquence demandée, car il n'est pas conçu pour varier sa fréquence.

---

## Installer la bibliothèque

Aucune installation nécessaire. `tone()` et `noTone()` sont des fonctions standard fournies par défaut avec Arduino IDE.

---

## Code de l'exercice

```cpp
const int pinBuzzer = 8;

void setup() {
  // La fonction tone() gère la configuration de la broche
}

void loop() {
  // 1. Émission d'un son aigu (2000 Hz) pendant 1 seconde
  tone(pinBuzzer, 2000);
  delay(1000);
  
  // 2. Émission d'un son grave (400 Hz) pendant 1 seconde
  tone(pinBuzzer, 400);
  delay(1000);
  
  // 3. Pause de 1 seconde
  noTone(pinBuzzer);
  delay(1000);
}
```

### Explication ligne par ligne

`const int pinBuzzer = 8;` — déclare la broche utilisée pour le buzzer. Le mot-clé `const` indique que cette valeur ne changera jamais pendant l'exécution du programme — une bonne pratique pour les broches fixes.

`void setup() { }` — ici, `setup()` est vide car `tone()` n'a pas besoin de `pinMode()` au préalable : elle configure elle-même la broche en sortie au moment où elle est appelée.

`tone(pinBuzzer, 2000);` — démarre un son continu à 2000 Hz sur la broche du buzzer. Contrairement à une version avec une durée précisée, ce son continue de jouer indéfiniment jusqu'à ce qu'on appelle `noTone()` ou qu'on lance un nouveau `tone()`.

`delay(1000);` — laisse le son jouer pendant 1 seconde avant de passer à l'instruction suivante.

`tone(pinBuzzer, 400);` — change immédiatement la fréquence à 400 Hz (un son plus grave), sans avoir besoin d'arrêter le son précédent au préalable.

`noTone(pinBuzzer);` — arrête complètement le son sur cette broche.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Écoute : un son aigu pendant 1 seconde, puis un son grave pendant 1 seconde, puis 1 seconde de silence, en boucle

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| Aucun son ne sort | Buzzer branché à l'envers, ou mauvaise broche | Vérifier la polarité et que `pinBuzzer` correspond au fil réellement branché |
| Le son ne s'arrête jamais, même après `noTone()` | Buzzer actif utilisé à la place d'un passif | Vérifier le type de buzzer ; un actif ignore la fréquence donnée à `tone()` |
| Le son est très faible | Buzzer mal inséré dans le breadboard, contact instable | Vérifier que les deux pattes sont bien enfoncées dans les bonnes rangées |

---

## Exercices à réaliser

1. **Gamme musicale** — Remplace les deux fréquences fixes par une suite de 5 ou 6 fréquences différentes pour créer une mini-gamme montante.
2. **Sirène** — Crée un effet de sirène en faisant varier la fréquence progressivement avec une boucle `for`, plutôt que par sauts brusques.
3. **Buzzer + bouton** — Combine ce code avec un bouton poussoir : le son ne joue que lorsque le bouton est maintenu appuyé.

---

## Pour aller plus loin

Une fois la fonction `tone()` maîtrisée, l'étape suivante consiste à jouer une vraie mélodie en stockant plusieurs notes dans un tableau — c'est exactement ce que fait l'exercice "buzzer-melodie".
