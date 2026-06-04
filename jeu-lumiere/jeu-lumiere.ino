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