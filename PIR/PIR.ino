int pinPIR = 7; // Capteur connecté sur D7
int led = 2;

void setup() {
  pinMode(pinPIR, INPUT); // Le PIR envoie une information -> ENTREE
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int etat = digitalRead(pinPIR); // Lecture du signal

  if (etat == HIGH) {
    digitalWrite(led, HIGH);
    Serial.println("Mouvement détecté !");
  } else {
    digitalWrite(led, LOW);
    Serial.println("RAS");
  }
  delay(1000);
}