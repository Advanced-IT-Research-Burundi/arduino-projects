#include "DHT.h"

/*#define DHTPIN 2     // Broche où est connecté le capteur
#define DHTTYPE DHT11   // Ou DHT22 si tu utilises un DHT22
DHT dht(DHTPIN, DHTTYPE);
*/

// ou bien faire
DHT dht(2, DHT11); //Capteur branché sur la broche 2

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000); // Le DHT a besoin de 2 secondes entre chaque lecture

  float humidite = dht.readHumidity();
  float temperature = dht.readTemperature(); // En Celsius

  // Vérification si la lecture a échoué
  if (isnan(humidite) || isnan(temperature)) {
    Serial.println("Échec de lecture du capteur DHT !");
    return;
  }

  Serial.print("Humidité: ");
  Serial.print(humidite);
  Serial.print("%\t");
  Serial.print("Température: ");
  Serial.print(temperature);
  Serial.println(" *C");
}