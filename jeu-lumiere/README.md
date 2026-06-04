# 🧠 Jeu de Mémoire "Simon" avec Arduino

Ce guide d'instructions vous accompagne pas à pas dans le montage matériel et l'installation logicielle de votre jeu de mémoire. Le principe est simple : l'Arduino génère une séquence lumineuse et sonore aléatoire qui s'allonge à chaque round. Vous devez la reproduire sans faire d'erreur !

---

## 📋 Matériel Requis

* **1x** Arduino Uno (avec son câble USB)
* **1x** Platine d'essai (Breadboard)
* **4x** LED de couleurs différentes (ex: Bleue, Jaune, Rouge, Verte)
* **4x** Boutons-poussoirs à 4 broches
* **4x** Résistances de **330 Ω** (pour protéger les LED)
* **4x** Résistances de **10 kΩ** (pour le circuit Pull-Down des boutons)
* **1x** Buzzer Piézoélectrique (pour le retour sonore)
* Des fils de connexion (Jumpers)

---

## 🛠️ Guide de Câblage Étape par Étape

### Étape 1 : Alimentation de la Breadboard
1. Reliez la broche **5V** de l'Arduino à la ligne rouge `(+)` de la breadboard avec un fil.
2. Reliez la broche **GND** de l'Arduino à la ligne bleue/noire `(-)` de la breadboard avec un fil.

### Étape 2 : Connexion des LED
Chaque LED possède une patte longue (Anode $+$) et une patte courte (Cathode $-$).
1. Insérez les 4 LED sur la breadboard.
2. Connectez la patte courte (Cathode) de chaque LED à la ligne de masse `(-)` de la breadboard.
3. Placez une **résistance de 330 Ω** entre la patte longue (Anode) de chaque LED et une ligne vide de la breadboard.
4. Reliez l'autre côté de la résistance aux broches de l'Arduino comme suit :
   * **LED 1** ➡️ Broche **12**
   * **LED 2** ➡️ Broche **5**
   * **LED 3** ➡️ Broche **4**
   * **LED 4** ➡️ Broche **3**

### Étape 3 : Connexion des Boutons (Configuration Pull-Down)
*⚠️ **IMPORTANT :** Sans les résistances de 10 kΩ, le signal électrique "flotte" et l'Arduino croit que vous appuyez en continu sur les boutons ou que les LED restent bloquées à l'allumage.*

Pour chaque bouton :
1. Placez le bouton **à cheval sur la rainure centrale** de la breadboard.
2. Connectez une broche du haut directement à la ligne d'alimentation rouge `(+)` (5V).
3. Sur la broche du bas (du même côté) :
   * Connectez une **résistance de 10 kΩ** qui va directement vers la ligne de masse bleue `(-)` (GND).
   * Sur la **même ligne** de la breadboard, connectez un fil de signal vers l'Arduino :
     * **Bouton 1** (contrôle la LED 12) ➡️ Broche **11**
     * **Bouton 2** (contrôle la LED 5)  ➡️ Broche **8**
     * **Bouton 3** (contrôle la LED 4)  ➡️ Broche **7**
     * **Bouton 4** (contrôle la LED 3)  ➡️ Broche **10**

### Étape 4 : Connexion du Buzzer
1. Connectez la patte positive du buzzer (`+` indiqué sur le boîtier) à la broche numérique **2** de l'Arduino.
2. Connectez la patte négative à la ligne de masse bleue `(-)` (GND).

---

## 💻 Code Source Arduino

Copiez et collez ce code optimisé dans l'IDE Arduino. Il intègre une gestion **asynchrone et instantanée** des boutons pour éviter toute latence pendant le jeu.

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

void activerLED(int indexLED, int duree) {
  digitalWrite(pinsLED[indexLED], HIGH);
  tone(pinBuzzer, tonsLED[indexLED], duree);
  delay(duree);
  digitalWrite(pinsLED[indexLED], LOW);
}

void jouerSequence() {
  for (int i = 0; i < niveauActuel; i++) {
    activerLED(sequence[i], 400);
    delay(200); // Pause entre deux LED
  }
}

bool attendreReponsesJoueur() {
  for (int i = 0; i < niveauActuel; i++) {
    int boutonPresse = -1;
    
    while (boutonPresse == -1) {
      for (int b = 0; b < 4; b++) {
        if (digitalRead(pinsBoutons[b]) == HIGH) {
          boutonPresse = b;
          
          // Allumage immédiat dès la pression
          digitalWrite(pinsLED[boutonPresse], HIGH);
          tone(pinBuzzer, tonsLED[boutonPresse]);
          
          // Attend le relâchement complet du bouton
          while (digitalRead(pinsBoutons[b]) == HIGH); 
          
          // Extinction immédiate au relâchement
          digitalWrite(pinsLED[boutonPresse], LOW);
          noTone(pinBuzzer);
          
          delay(50); // Anti-rebond matériel
        }
      }
    }
    
    if (boutonPresse != sequence[i]) {
      return false; // Erreur détectée
    }
  }
  return true; // Round réussi
}

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

void jouerSonPerdu() {
  for (int i = 0; i < 4; i++) digitalWrite(pinsLED[i], HIGH);
  tone(pinBuzzer, 130, 800); // Note grave d'erreur
  delay(800);
  for (int i = 0; i < 4; i++) digitalWrite(pinsLED[i], LOW);
}
