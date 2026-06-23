#include <Servo.h>

Servo monServo;  // Création de l'objet Servo
  int i=0;

void setup() {
  monServo.attach(9);  // On lie le servo à la broche 9
}

void loop() {
  for(i; i<180; i++) {
    monServo.write(i);
    delay(15);
  }

  //if (i==180){
    for(i; i>0; i--) {
      monServo.write(i);
      delay(20);
    }
  //}

  /*monServo.write(0);   // Va à 0 degré
  delay(1000);
  
  monServo.write(90);  // Va à 90 degrés
  delay(1000);
  
  monServo.write(180); // Va à 180 degrés
  delay(1000);*/


}