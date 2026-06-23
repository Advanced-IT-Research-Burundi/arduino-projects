# Jour 3–4 — Photorésistance : capteur de luminosité

## Objectif de l'exercice

Apprendre à mesurer le niveau de lumière ambiante avec une photorésistance, et déclencher automatiquement une LED quand il fait sombre — le principe d'un lampadaire automatique.

À la fin de cet exercice, tu sauras :
- lire une valeur analogique avec `analogRead()`
- comprendre la plage de valeurs retournée par une entrée analogique (0 à 1023)
- créer un comportement automatique basé sur un seuil de luminosité
- afficher des valeurs analogiques en continu dans le moniteur série

---

## Qu'est-ce qu'une photorésistance ?

Une photorésistance (aussi appelée LDR, *Light Dependent Resistor*) est un composant dont la résistance électrique varie selon la quantité de lumière qu'elle reçoit : plus il y a de lumière, plus sa résistance diminue, et inversement. En la combinant avec une résistance fixe dans un circuit, on peut mesurer indirectement la luminosité ambiante via une broche analogique de l'Arduino.

C'est le même principe utilisé dans les lampadaires de rue qui s'allument automatiquement à la tombée de la nuit.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Photorésistance (LDR) | 1 |
| Résistance fixe (ex : 10 kΩ, pour le pont diviseur) | 1 |
| LED | 1 |
| Résistance 220 Ω (pour la LED) | 1 |
| Breadboard | 1 |
| Fils de connexion | quelques-uns |

---

## Câblage — étape par étape

| Broche composant | Broche Arduino Uno | Rôle |
|---|---|---|
| Photorésistance (via pont diviseur) | A0 | Entrée analogique |
| LED (+) | D2 (via résistance 220 Ω) | Sortie numérique |
| LED (−) | GND | Masse |

### ⚠️ Avertissement important

Une photorésistance seule ne peut pas être lue directement par l'Arduino : elle doit être montée avec une résistance fixe pour former un "pont diviseur de tension", qui transforme sa variation de résistance en une variation de tension mesurable sur une broche analogique.

---

## Installer la bibliothèque

Aucune installation nécessaire. `analogRead()` et `digitalWrite()` sont des fonctions standard fournies par défaut avec Arduino IDE.

---

## Code de l'exercice

```cpp
int photo = A0;
int led = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(photo, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int lecture = analogRead(photo);
  Serial.println(lecture); 
  if (lecture>10) {
    digitalWrite(led, LOW);
  } else {
    digitalWrite(led, HIGH);

  }
  
}
```

### Explication ligne par ligne

`int photo = A0;` — déclare la broche analogique A0 comme entrée pour la photorésistance. Les broches analogiques sont préfixées par un `A` sur l'Arduino Uno (A0 à A5).

`pinMode(photo, INPUT);` — bien que techniquement optionnel pour les broches analogiques (qui sont en entrée par défaut), c'est une bonne pratique de le préciser explicitement pour la clarté du code.

`int lecture = analogRead(photo);` — lit la valeur analogique sur la broche A0. Contrairement à `digitalRead()` qui retourne uniquement 0 ou 1, `analogRead()` retourne une valeur comprise entre 0 et 1023, offrant une mesure beaucoup plus précise.

`Serial.println(lecture);` — affiche cette valeur brute dans le moniteur série, ce qui est essentiel pour calibrer le bon seuil de déclenchement selon ton environnement lumineux réel.

`if (lecture>10) { digitalWrite(led, LOW); } else { digitalWrite(led, HIGH); }` — cette condition compare la lecture à un seuil fixé à 10. Note que ce seuil est très bas : il faudra probablement l'ajuster selon le montage exact (orientation du pont diviseur, valeur de la résistance fixe utilisée), car selon le câblage, une valeur élevée peut signifier soit "beaucoup de lumière" soit "peu de lumière".

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Ouvre le **Moniteur série** (`Ctrl + Maj + M`), réglé sur 9600 bauds
5. Observe les valeurs affichées en pleine lumière, puis couvre la photorésistance avec ta main
6. Note comment la valeur change selon la luminosité, et ajuste le seuil `10` dans le code si nécessaire

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| La valeur affichée reste toujours à 0 ou toujours à 1023, sans jamais varier | Pont diviseur de tension absent ou mal câblé | Vérifier que la photorésistance est bien montée en série avec une résistance fixe, formant un diviseur de tension |
| La LED s'allume quand il fait clair et s'éteint quand il fait sombre (comportement inversé) | Orientation du pont diviseur inversée par rapport à ce qui est attendu dans le code | Soit inverser le branchement physique, soit inverser la condition `if` dans le code |
| Le seuil de 10 ne correspond à aucune situation réelle observée | Valeurs réelles très différentes de ce seuil selon ton montage et ta luminosité ambiante | Observer les valeurs affichées dans le moniteur série dans tes conditions réelles, puis ajuster le seuil en conséquence |

---

## Exercices à réaliser

1. **Calibration du seuil** — Observe les valeurs en pleine lumière et dans le noir, puis fixe un seuil intermédiaire cohérent (par exemple 500) plutôt que la valeur 10 actuelle.
2. **Luminosité progressive** — Remplace `digitalWrite()` par `analogWrite()` pour faire varier l'intensité de la LED progressivement selon le niveau de luminosité ambiante, plutôt qu'un simple tout ou rien.
3. **Lampadaire avec délai** — Ajoute un délai de quelques secondes avant que la LED ne s'éteigne après le retour de la lumière, pour éviter un clignotement si la luminosité varie rapidement (par exemple un nuage qui passe).

---

## Pour aller plus loin

Cette logique de seuil basée sur une lecture analogique est directement réutilisable avec d'autres capteurs analogiques, comme le potentiomètre vu dans l'exercice suivant — la différence étant que le potentiomètre est contrôlé manuellement plutôt que par les conditions ambiantes.
