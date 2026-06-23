# Jour 5–6 — Jouer une mélodie complète (Joyeux Anniversaire)

## Objectif de l'exercice

Apprendre à stocker une mélodie complète dans des tableaux (notes et durées) et à la jouer automatiquement avec une boucle, en utilisant des fréquences musicales précises.

À la fin de cet exercice, tu sauras :
- définir des constantes nommées pour les fréquences des notes avec `#define`
- stocker une séquence de notes dans un tableau de type `float`
- associer chaque note à sa durée musicale dans un tableau séparé
- parcourir les deux tableaux en parallèle avec une boucle `for`
- calculer une durée en millisecondes à partir d'une fraction musicale (noire, croche...)

---

## Qu'est-ce qu'une mélodie en code ?

Une mélodie est simplement une liste de notes jouées les unes après les autres, chacune avec sa propre hauteur (fréquence) et sa propre durée. En programmation, on représente naturellement cette liste avec un tableau (array) : une case par note.

Pour respecter le rythme musical, on utilise la convention classique : 4 = noire (durée normale), 8 = croche (deux fois plus courte), 2 = blanche (deux fois plus longue), etc. Le programme convertit ensuite ces valeurs en millisecondes.

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
| Broche positive | D9 | Signal sonore (`tone()`) |
| Broche négative (GND) | GND | Masse |

### ⚠️ Avertissement important

Ce code utilise des fréquences en `float` (nombres à virgule, ex : 261.625 Hz) pour plus de précision musicale. Vérifie que tu utilises bien `float melodie[]` et non `int melodie[]`, sinon les fréquences seront arrondies et la mélodie sonnera légèrement fausse.

---

## Installer la bibliothèque

Aucune installation nécessaire. `tone()` et `noTone()` sont des fonctions standard fournies par défaut avec Arduino IDE.

---

## Code de l'exercice

```cpp
// Définition des fréquences des notes (en Hz)
#define C4  261.625 // Do
#define D4  293.664 // Ré
#define E4  329.627 // Mi
#define F4  349.228 // Fa
#define G4  391.995 // Sol
#define A4  440 // La
#define Bb4 466.163 //Sib
#define B4  493.883 // Si
#define C5  523.2 // Do (octave supérieure)
#define D5  587.4 // Ré (octave supérieure)

const int buzzerPin = 9; // Broche connectée au buzzer passif

// Tableau des notes de la mélodie
float melodie[] = {
  C4, C4, D4, C4, F4, E4, // Joyeux an-ni-ver-saire
  C4, C4, D4, C4, G4, F4, // Joyeux an-ni-ver-saire
  C4, C4, C5, A4, F4, E4, D4, // Joyeux an-ni-ver-saire...
  Bb4, Bb4, A4, F4, G4, F4  // ... Chéri(e) !
};

// Durée des notes : 4 = noire, 8 = croche, 2 = blanche
// Pour l'anniversaire, le rythme a de petites subtilités
int durees[] = {
  4, 8, 8/3, 8/3, 8/3, 8/6,
  4, 8, 8/3, 8/3, 8/3, 8/6,
  4, 8, 4, 4, 4, 4, 4,
  8, 8, 4, 4, 4, 2
};

void setup() {
  // Pas besoin de pinMode pour la fonction tone()
}

void loop() {
  int tailleMelodie = sizeof(melodie) / sizeof(int);

  for (int i = 0; i < tailleMelodie; i++) {
    // Calcul de la durée de la note en millisecondes
    // 1000 ms correspond au temps d'une note entière
    int dureeNote = 1000 / durees[i];
    
    // Joue la note sur la broche spécifiée
    tone(buzzerPin, melodie[i], dureeNote);

    // Pour distinguer les notes, on ajoute une petite pause entre elles
    // 30% du temps de la note fonctionne généralement bien
    int pauseEntreNotes = dureeNote * 1.30;
    delay(pauseEntreNotes);
    
    // On arrête le son avant la note suivante
    noTone(buzzerPin);
  }

  // Attend 5 secondes avant de rejouer la mélodie
  delay(5000);
}
```

### Explication ligne par ligne

`#define C4 261.625` — crée une constante nommée `C4` qui représente la fréquence exacte de la note Do. Utiliser des noms de notes plutôt que des nombres directement rend le tableau de mélodie beaucoup plus lisible.

`float melodie[] = { C4, C4, D4, ... };` — stocke toute la séquence de notes de la chanson dans l'ordre où elles doivent être jouées. Chaque virgule sépare une note de la suivante.

`int durees[] = { 4, 8, 8/3, ... };` — stocke la durée correspondant à chaque note, dans le même ordre. La position `durees[0]` correspond donc à la durée de `melodie[0]`, et ainsi de suite.

`int tailleMelodie = sizeof(melodie) / sizeof(int);` — calcule automatiquement le nombre de notes dans le tableau, pour ne pas avoir à le compter et l'écrire à la main (ce qui éviterait des erreurs si on modifie la mélodie plus tard).

`int dureeNote = 1000 / durees[i];` — convertit la valeur musicale (4, 8, 2...) en une durée réelle en millisecondes. Une noire (4) dure 1000/4 = 250 ms ; une blanche (2) dure 1000/2 = 500 ms, soit deux fois plus longtemps.

`tone(buzzerPin, melodie[i], dureeNote);` — joue la note numéro `i` du tableau, pendant la durée calculée. Ici, `tone()` reçoit directement une durée en troisième paramètre, donc elle s'arrête automatiquement sans qu'on ait besoin de `delay()` pour la couper.

`int pauseEntreNotes = dureeNote * 1.30;` — ajoute une petite pause supplémentaire (30% de plus que la durée de la note) pour qu'on entende clairement la séparation entre deux notes, plutôt qu'un son continu.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Écoute la mélodie complète de "Joyeux Anniversaire" jouée du début à la fin
5. Une pause de 5 secondes survient avant qu'elle ne soit rejouée

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| La mélodie semble fausse ou désaccordée | Tableau `melodie[]` déclaré en `int` au lieu de `float` | Vérifier que la déclaration utilise bien `float melodie[]` |
| Les notes s'enchaînent sans aucune pause perceptible | `pauseEntreNotes` mal calculé ou absent | Vérifier la présence du `delay(pauseEntreNotes)` après chaque `tone()` |
| Le rythme semble haché ou inégal | Erreur dans le tableau `durees[]`, valeurs non cohérentes avec `melodie[]` | Vérifier que chaque note a bien une durée correspondante au même index |

---

## Exercices à réaliser

1. **Nouvelle mélodie** — Remplace le contenu des tableaux `melodie[]` et `durees[]` par les notes d'une chanson simple de ton choix (cherche une partition simplifiée en ligne).
2. **Tempo réglable** — Ajoute une variable `tempo` qui multiplie ou divise toutes les durées, pour jouer la chanson plus vite ou plus lentement.
3. **Silences** — Ajoute une fréquence spéciale `0` dans le tableau de mélodie pour représenter un silence, et adapte le code pour qu'il appelle `noTone()` au lieu de `tone()` dans ce cas.

---

## Pour aller plus loin

Cette structure à deux tableaux parallèles (notes + durées) est directement réutilisée dans le jeu de lumière (Simon), où elle associe cette fois chaque LED à sa propre fréquence sonore.
