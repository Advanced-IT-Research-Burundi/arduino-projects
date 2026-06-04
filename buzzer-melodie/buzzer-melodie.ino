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