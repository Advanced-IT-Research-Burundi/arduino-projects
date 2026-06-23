# Projet — Jeu de mémoire lumineux (style "Simon")

## Objectif de l'exercice

Réaliser un projet complet combinant 4 LEDs, 4 boutons et un buzzer pour recréer le jeu de mémoire classique "Simon" : l'Arduino joue une séquence lumineuse et sonore de plus en plus longue, que le joueur doit reproduire exactement.

À la fin de cet exercice, tu sauras :
- gérer plusieurs entrées et sorties simultanément à l'aide de tableaux
- générer une séquence aléatoire avec `random()`
- structurer un programme complexe en plusieurs fonctions personnalisées
- créer une boucle d'attente active qui surveille plusieurs boutons à la fois
- combiner LED et son pour donner un retour immédiat à l'utilisateur

---

## Comment fonctionne le jeu Simon ?

Le principe est simple : à chaque tour, l'Arduino ajoute une nouvelle étape aléatoire à une séquence (par exemple : rouge, puis bleu, puis rouge encore). Il rejoue ensuite toute la séquence depuis le début, LED par LED, avec un son différent pour chacune. Le joueur doit alors reproduire exactement cette séquence en appuyant sur les bons boutons dans le bon ordre. S'il réussit, le niveau augmente d'une étape ; s'il se trompe, le jeu recommence depuis le début.

C'est un excellent projet de synthèse car il réutilise presque toutes les compétences vues dans les exercices précédents : LEDs, boutons, buzzer, tableaux, et structure en fonctions.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| LED (4 couleurs différentes idéalement) | 4 |
| Résistances 220 Ω (pour les LEDs) | 4 |
| Bouton poussoir | 4 |
| Buzzer (actif ou passif) | 1 |
| Breadboard | 1 |
| Fils de connexion | nombreux |

---

## Câblage — étape par étape

| Composant | Broche Arduino Uno | Rôle |
|---|---|---|
| LED 1 | D12 (via résistance 220 Ω) | Sortie numérique |
| LED 2 | D5 (via résistance 220 Ω) | Sortie numérique |
| LED 3 | D4 (via résistance 220 Ω) | Sortie numérique |
| LED 4 | D3 (via résistance 220 Ω) | Sortie numérique |
| Bouton 1 (associé à LED 1) | D11 | Entrée numérique |
| Bouton 2 (associé à LED 2) | D8 | Entrée numérique |
| Bouton 3 (associé à LED 3) | D7 | Entrée numérique |
| Bouton 4 (associé à LED 4) | D10 | Entrée numérique |
| Buzzer | D2 | Signal sonore |

### ⚠️ Avertissement important

Remarque que l'ordre des broches des boutons (`{11, 8, 7, 10}`) ne suit pas un ordre croissant simple, et que cet ordre doit obligatoirement correspondre à l'ordre des LEDs (`{12, 5, 4, 3}`) au même index. Le bouton à l'index 0 du tableau (`pinsBoutons[0]`, donc D11) doit physiquement être associé à la LED à l'index 0 (`pinsLED[0]`, donc D12) — sinon le jeu deviendra injouable car les bonnes réponses ne correspondront pas aux bonnes LEDs.

---

## Installer la bibliothèque

Aucune installation nécessaire. Toutes les fonctions utilisées (`digitalRead()`, `digitalWrite()`, `tone()`, `random()`) sont fournies par défaut avec Arduino IDE.

---

## Code de l'exercice

```cpp
// --- Configuration des Broches ---
const int pinsLED[] = {12, 5, 4, 3};       // Broches des 4 LED
const int pinsBoutons[] = {11, 8, 7, 10}; // Broches des 4 Boutons correspondants
const int pinBuzzer = 2;                  // Broche du Buzzer

// --- Paramètres du Jeu ---
const int MAX_SEQUENCE = 100; // Longueur maximale du jeu
int sequence[MAX_SEQUENCE];   // Tableau stockant la séquence de LED
int niveauActuel = 0;        // Niveau/Round en cours

// Fréquences sonores pour chaque LED (Buzzer)
const int tonsLED[] = {262, 330, 392, 494}; // Do, Mi, Sol, Si

void setup() {
  Serial.begin(9600);
  
  // Initialisation des broches
  for (int i = 0; i < 4; i++) {
    pinMode(pinsLED[i], OUTPUT);
    pinMode(pinsBoutons[i], INPUT);
  }
  for (int i = 0; i < 4; i++) {
    digitalWrite(pinsLED[i], LOW);
  }
  pinMode(pinBuzzer, OUTPUT);
  
  // Initialisation du générateur aléatoire sur une broche analogique vide
  randomSeed(analogRead(0)); 
  
  lancerAnimationDemarrage();
}

void loop() {
  // 1. Ajouter une nouvelle étape aléatoire à la séquence (valeur entre 0 et 3)
  sequence[niveauActuel] = random(0, 4);
  niveauActuel++;
  
  // 2. Jouer la séquence actuelle pour le joueur
  jouerSequence();
  
  // 3. Attendre et vérifier les réponses du joueur
  if (!attendreReponsesJoueur()) {
    // Si le joueur fait une erreur
    jouerSonPerdu();
    niveauActuel = 0; // Réinitialisation du jeu
    delay(2000);
    lancerAnimationDemarrage();
  } else {
    // Si le joueur réussit le round
    delay(1000); // Petite pause avant le prochain round
  }
}

// --- Fonctions Utilitaires ---

// Allume une LED et joue son son associé
void activerLED(int indexLED, int duree) {
  digitalWrite(pinsLED[indexLED], HIGH);
  tone(pinBuzzer, tonsLED[indexLED], duree);
  delay(duree);
  digitalWrite(pinsLED[indexLED], LOW);
}

// Affiche toute la séquence mémorisée
void jouerSequence() {
  for (int i = 0; i < niveauActuel; i++) {
    activerLED(sequence[i], 400);
    delay(200); // Pause entre deux LED
  }
}

// Attend l'action du joueur et vérifie en temps réel
bool attendreReponsesJoueur() {
  for (int i = 0; i < niveauActuel; i++) {
    int boutonPresse = -1;
    
    // Attente active du bouton
    while (boutonPresse == -1) {
      for (int b = 0; b < 4; b++) {
        if (digitalRead(pinsBoutons[b]) == HIGH) {
          boutonPresse = b;
          
          // 1. ALLUMAGE IMMÉDIAT (sans bloquer avec un delay)
          digitalWrite(pinsLED[boutonPresse], HIGH);
          tone(pinBuzzer, tonsLED[boutonPresse]); // Active le son
          
          // 2. On attend que le joueur RELÂCHE le bouton
          // Pendant qu'il reste appuyé, la LED brille et le son joue
          while (digitalRead(pinsBoutons[b]) == HIGH); 
          
          // 3. EXTINCTION IMMÉDIATE dès le relâchement
          digitalWrite(pinsLED[boutonPresse], LOW);
          noTone(pinBuzzer); // Coupe le son
          
          delay(50); // Un mini anti-rebond physique (50ms) presque imperceptible
        }
      }
    }
    
    // Vérification du bouton pressé
    if (boutonPresse != sequence[i]) {
      return false; // Erreur !
    }
  }
  return true; // Séquence validée
}

// Petit effet de lumière au démarrage ou reset
void lancerAnimationDemarrage() {
  for (int r = 0; r < 2; r++) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(pinsLED[i], HIGH);
      delay(100);
      digitalWrite(pinsLED[i], LOW);
    }
  }
  delay(500);
}

// Signal sonore et visuel en cas d'échec
void jouerSonPerdu() {
  digitalWrite(pinsLED[0], HIGH);
  digitalWrite(pinsLED[1], HIGH);
  digitalWrite(pinsLED[2], HIGH);
  digitalWrite(pinsLED[3], HIGH);
  
  tone(pinBuzzer, 130, 800); // Son grave d'erreur
  delay(800);
  
  digitalWrite(pinsLED[0], LOW);
  digitalWrite(pinsLED[1], LOW);
  digitalWrite(pinsLED[2], LOW);
  digitalWrite(pinsLED[3], LOW);
}
```

### Explication ligne par ligne

`const int pinsLED[] = {12, 5, 4, 3};` — au lieu de créer 4 variables séparées (`led1`, `led2`...), ce tableau regroupe les 4 broches de LEDs. On peut ainsi accéder à chacune via son index : `pinsLED[0]` vaut 12, `pinsLED[1]` vaut 5, etc. C'est beaucoup plus pratique pour répéter une action sur les 4 LEDs avec une boucle plutôt que d'écrire 4 fois le même code.

`int sequence[MAX_SEQUENCE];` — ce tableau va stocker la séquence complète du jeu au fur et à mesure qu'elle s'allonge, jusqu'à un maximum de 100 étapes.

`randomSeed(analogRead(0));` — la fonction `random()` d'Arduino produit en réalité des nombres pseudo-aléatoires qui suivent toujours la même séquence si on ne l'initialise pas différemment à chaque démarrage. En lisant le bruit électrique naturel sur une broche analogique non connectée (A0), on obtient une valeur imprévisible qui sert à initialiser le générateur aléatoire de façon réellement différente à chaque lancement du programme.

`sequence[niveauActuel] = random(0, 4);` — ajoute une nouvelle étape à la fin de la séquence : un nombre aléatoire entre 0 et 3 (4 exclu), qui correspond à l'index d'une des 4 LEDs.

`void activerLED(int indexLED, int duree)` — cette fonction personnalisée regroupe en une seule instruction réutilisable les 4 actions nécessaires pour "jouer" une LED : l'allumer, jouer son son, attendre, puis l'éteindre. Cela évite de répéter ces 4 lignes partout dans le code où on a besoin d'allumer une LED.

`void jouerSequence()` — parcourt le tableau `sequence` du début jusqu'à `niveauActuel` (la longueur actuelle de la séquence), et active chaque LED dans l'ordre, avec une pause entre chacune pour que le joueur puisse bien distinguer chaque étape.

`bool attendreReponsesJoueur()` — cette fonction retourne soit `true` (le joueur a reproduit correctement toute la séquence) soit `false` (il s'est trompé à un moment). Le type `bool` est parfaitement adapté ici puisqu'il n'y a que deux résultats possibles.

`while (boutonPresse == -1) { ... }` — c'est une boucle d'attente active : le programme reste bloqué ici, en testant continuellement les 4 boutons, jusqu'à ce que l'un d'eux soit pressé. La variable `boutonPresse` reste à -1 (valeur impossible pour un index réel) tant qu'aucun bouton n'a été détecté.

`while (digitalRead(pinsBoutons[b]) == HIGH);` — remarque le point-virgule directement après cette ligne, sans accolades : c'est une boucle vide qui ne fait rien d'autre qu'attendre que la condition devienne fausse (donc que le bouton soit relâché). Le programme reste "coincé" ici aussi longtemps que le bouton reste appuyé.

`if (boutonPresse != sequence[i]) { return false; }` — compare le bouton réellement pressé par le joueur à ce qui était attendu à cette étape de la séquence. S'ils ne correspondent pas, la fonction s'arrête immédiatement et retourne `false`, sans vérifier le reste de la séquence — l'erreur est déjà confirmée.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Observe l'animation de démarrage (les 4 LEDs s'allument brièvement deux fois)
5. Une LED s'allume avec son son associé : c'est la séquence à reproduire
6. Appuie sur le bouton correspondant à cette LED
7. Si c'est correct, le niveau augmente et une nouvelle séquence (plus longue) est jouée
8. Continue jusqu'à faire une erreur : le jeu joue alors un son grave et redémarre au niveau 1

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| Le jeu indique toujours une erreur même en appuyant sur le bon bouton | Tableaux `pinsLED[]` et `pinsBoutons[]` non alignés (mauvaise correspondance entre une LED et son bouton) | Vérifier que la LED et le bouton physiquement associés sont bien au même index dans les deux tableaux |
| Le jeu se bloque complètement et ne réagit plus à aucun bouton | Un bouton est resté bloqué en position HIGH en raison d'un câblage en `INPUT` simple instable | Vérifier le câblage de tous les boutons, envisager `INPUT_PULLUP` avec adaptation du code |
| La séquence jouée par l'Arduino est toujours identique à chaque redémarrage | `randomSeed()` mal initialisé, ou broche A0 connectée à quelque chose qui fausse la lecture | Vérifier que la broche A0 est bien libre (non connectée) pour que `analogRead(0)` capte un bruit réellement aléatoire |

---

## Exercices à réaliser

1. **Affichage du score** — Ajoute `Serial.println(niveauActuel)` à chaque niveau réussi, pour suivre le score dans le moniteur série.
2. **Vitesse croissante** — Réduis progressivement la durée de chaque LED (`activerLED(sequence[i], duree)`) au fil des niveaux, pour rendre le jeu plus difficile à mesure qu'il avance.
3. **Mode deux joueurs** — Imagine et implémente une variante où deux joueurs s'affrontent en alternance, chacun devant reproduire une séquence de plus en plus longue.

---

## Pour aller plus loin

Ce projet est un excellent exemple de programme structuré en plusieurs fonctions, chacune avec une responsabilité claire et unique. C'est une compétence essentielle à mesure que tes projets Arduino deviendront plus ambitieux — notamment pour le projet de fin de formation combinant plusieurs capteurs et actionneurs.
