// Définition des broches
const int trigPin = 3;
const int echoPin = 6;
const int pinBuzzer = 9;

// Variables pour la durée et la distance
long duree;
int distance;

void setup() {
  // Initialisation de la communication série
  Serial.begin(9600);
  
  // Configuration des broches
  pinMode(trigPin, OUTPUT); // Le Trig émet le son (Sortie)
  pinMode(echoPin, INPUT);  // L'Echo reçoit le son (Entrée)
}

void loop() {
  // 1. On s'assure que le Trig est bas
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // 2. On envoie une impulsion HIGH de 10 microsecondes pour lancer l'onde
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // 3. On mesure le temps que met l'onde à revenir (en microsecondes)
  duree = pulseIn(echoPin, HIGH);
  
  // 4. Calcul de la distance en cm
  // La vitesse du son est de 340 m/s soit 0,034 cm/microseconde
  // On divise par 2 car l'onde fait un aller-retour
  distance = duree * 0.034 / 2;

  if (distance<10) {
    tone(pinBuzzer, 2000);
  } else {
    noTone(pinBuzzer);
  }
  
  // 5. Affichage du résultat sur le moniteur série
  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Pause de 500ms avant la prochaine mesure
  delay(500);
}