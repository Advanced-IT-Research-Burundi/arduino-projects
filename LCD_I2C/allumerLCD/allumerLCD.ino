#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// On utilise l'adresse 0x27 que le scanner a trouvée !
LiquidCrystal_I2C lcd(0x27, 16, 2); 
  int temps = 0;

void setup() {
  lcd.init();          // Initialisation du LCD
  lcd.backlight();     // Allume le rétroéclairage
  
  lcd.setCursor(0, 0); // Ligne du haut, colonne 0
  lcd.print("Bonjour Reine !");
  
  lcd.setCursor(0, 1); // Ligne du bas, colonne 0
  lcd.print("I2C Fonctionne !");
lcd.clear(); // On efface l'écran avant d'écrire

}

void loop() {
  delay(1000);
  if (temps != -1) {
    lcd.setCursor(0, 0); // Ligne du haut, colonne 0
    lcd.print("temps ecoulee:");
      lcd.setCursor(0, 1); // Ligne du bas, colonne 0
    lcd.print(temps);
    lcd.print(" secondes");
  }  
  temps++;
}