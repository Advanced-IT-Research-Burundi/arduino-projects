# Jour 3–6 — Détecteur de proximité sonore (HC-SR04 + Buzzer)

## Objectif de l'exercice

Combiner un capteur de distance à ultrasons avec un buzzer pour créer un système d'alerte sonore qui se déclenche lorsqu'un objet s'approche trop près — le même principe qu'un radar de recul de voiture.

À la fin de cet exercice, tu sauras :
- mesurer une distance avec le capteur HC-SR04 grâce à `pulseIn()`
- convertir un temps de propagation du son en distance réelle
- déclencher une action sonore selon une condition de seuil
- afficher des mesures en continu dans le moniteur série

---

## Comment fonctionne un capteur ultrasonique ?

Le HC-SR04 envoie une onde ultrasonique inaudible (comme un sonar de sous-marin) et mesure le temps qu'elle met pour revenir après avoir rebondi sur un obstacle. Connaissant la vitesse du son dans l'air (340 mètres par seconde), on peut calculer la distance parcourue, donc la distance jusqu'à l'objet.

C'est exactement le même principe que l'écholocation utilisée par les chauves-souris ou les dauphins.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Capteur ultrasonique HC-SR04 | 1 |
| Buzzer passif | 1 |
| Breadboard | 1 |
| Fils de connexion | 6 |

---

## Câblage — étape par étape

| Broche composant | Broche Arduino Uno | Rôle |
|---|---|---|
| HC-SR04 — VCC | 5V | Alimentation |
| HC-SR04 — Trig | D3 | Émission de l'impulsion (sortie) |
| HC-SR04 — Echo | D6 | Réception du signal de retour (entrée) |
| HC-SR04 — GND | GND | Masse |
| Buzzer — positif | D9 | Signal sonore |
| Buzzer — négatif | GND | Masse |

### ⚠️ Avertissement important

Ne jamais inverser les broches Trig et Echo : Trig doit être déclarée en `OUTPUT` (elle envoie le signal) et Echo en `INPUT` (elle le reçoit). Une inversion empêchera toute mesure correcte et peut, dans certains cas, endommager le capteur.

---

## Installer la bibliothèque

Aucune installation nécessaire. `pulseIn()`, `tone()` et `noTone()` sont des fonctions standard fournies par défaut avec Arduino IDE.

---

## Code de l'exercice

```cpp
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
```

### Explication ligne par ligne

`digitalWrite(trigPin, LOW); delayMicroseconds(2);` — s'assure que la broche Trig démarre dans un état stable et bas avant d'envoyer le signal, pour éviter toute interférence avec une mesure précédente.

`digitalWrite(trigPin, HIGH); delayMicroseconds(10); digitalWrite(trigPin, LOW);` — ces trois lignes envoient une impulsion électrique extrêmement brève (10 microsecondes) qui déclenche l'émission de l'onde ultrasonique par le capteur.

`duree = pulseIn(echoPin, HIGH);` — cette fonction attend que la broche Echo passe à HIGH (le capteur a reçu l'écho), puis mesure combien de temps elle reste à HIGH avant de redescendre. Ce temps, exprimé en microsecondes, correspond exactement à la durée de l'aller-retour de l'onde sonore.

`distance = duree * 0.034 / 2;` — applique la formule physique de conversion : la vitesse du son étant de 340 m/s (soit 0,034 cm par microseconde), on multiplie le temps par cette vitesse, puis on divise par 2 car le son a parcouru la distance deux fois (aller + retour).

`if (distance<10) { tone(pinBuzzer, 2000); } else { noTone(pinBuzzer); }` — c'est la logique d'alerte : si la distance mesurée est inférieure à 10 cm, le buzzer émet un son continu à 2000 Hz ; sinon, il reste silencieux.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Ouvre le **Moniteur série** (`Ctrl + Maj + M`), réglé sur 9600 bauds
5. Observe les valeurs de distance qui s'affichent en continu
6. Approche ta main à moins de 10 cm du capteur : le buzzer doit se déclencher

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| La distance affichée est toujours 0 ou une valeur absurde | Câblage Trig/Echo inversé, ou fils mal connectés | Revérifier le câblage selon le tableau ci-dessus |
| Le buzzer sonne en continu même sans objet proche | Capteur orienté vers une surface très proche (table, mur) | Dégager l'espace devant le capteur, vérifier le seuil de 10 cm |
| Les mesures sont instables, varient beaucoup d'un coup à l'autre | Surface réfléchissante irrégulière (tissu, angle), ou alimentation instable | Tester sur une surface plane et dure, vérifier les connexions d'alimentation |

---

## Exercices à réaliser

1. **Seuil ajustable** — Remplace la valeur fixe `10` par une variable, et teste différents seuils de déclenchement (5 cm, 20 cm, 50 cm).
2. **Son progressif** — Fais varier la fréquence du buzzer selon la distance : plus l'objet est proche, plus le son devient aigu (comme un vrai radar de recul).
3. **Ajout d'une LED** — Allume une LED rouge en plus du son lorsque l'objet est très proche (moins de 5 cm), pour un double signal visuel et sonore.

---

## Pour aller plus loin

Cette mesure de distance par ultrasons est une porte d'entrée vers la robotique : combinée à un moteur ou un servomoteur, elle permettrait à un petit robot d'éviter automatiquement les obstacles.
