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
