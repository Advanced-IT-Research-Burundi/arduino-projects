# Jour 5–6 — Capteur PIR : détection de mouvement

## Objectif de l'exercice

Apprendre à détecter la présence de mouvement avec un capteur PIR et à déclencher une LED ainsi qu'un message dans le moniteur série en réponse.

À la fin de cet exercice, tu sauras :
- comprendre le fonctionnement d'un capteur de mouvement infrarouge passif
- lire un signal numérique simple avec `digitalRead()`
- afficher des messages conditionnels dans le moniteur série
- créer la base d'un système d'alarme ou d'éclairage automatique

---

## Qu'est-ce qu'un capteur PIR ?

PIR signifie *Passive InfraRed* (infrarouge passif). Ce capteur détecte les variations de rayonnement infrarouge émises par tout corps chaud — notamment les humains et les animaux, dont le corps émet naturellement de la chaleur. Quand quelque chose de chaud se déplace dans son champ de détection, le capteur envoie un signal HIGH ; en l'absence de mouvement, il reste à LOW.

C'est exactement le capteur utilisé dans les détecteurs de présence des systèmes d'éclairage automatique ou d'alarme domestique.

---

## Matériel nécessaire

| Composant | Quantité |
|---|---|
| Arduino Uno | 1 |
| Capteur PIR | 1 |
| LED | 1 |
| Résistance 220 Ω (pour la LED) | 1 |
| Breadboard | 1 |
| Fils de connexion | quelques-uns |

---

## Câblage — étape par étape

| Broche composant | Broche Arduino Uno | Rôle |
|---|---|---|
| PIR — OUT | D7 | Signal de détection (entrée) |
| PIR — VCC | 5V | Alimentation |
| PIR — GND | GND | Masse |
| LED (+) | D2 (via résistance 220 Ω) | Sortie numérique |
| LED (−) | GND | Masse |

### ⚠️ Avertissement important

La plupart des modules PIR ont un petit délai de préchauffage de 30 à 60 secondes après leur mise sous tension, pendant lequel les lectures peuvent être instables ou incorrectes. Si ton capteur semble se déclencher sans raison juste après le démarrage, patiente une minute avant de tester.

---

## Installer la bibliothèque

Aucune installation nécessaire. `digitalRead()` et `digitalWrite()` sont des fonctions standard fournies par défaut avec Arduino IDE.

---

## Code de l'exercice

```cpp
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
```

### Explication ligne par ligne

`int pinPIR = 7;` — déclare la broche utilisée pour lire le signal du capteur PIR.

`pinMode(pinPIR, INPUT);` — configure la broche 7 comme une entrée, car c'est le capteur qui envoie une information à l'Arduino, et non l'inverse.

`int etat = digitalRead(pinPIR);` — lit l'état actuel du capteur : `HIGH` si un mouvement est détecté, `LOW` sinon.

`if (etat == HIGH) { digitalWrite(led, HIGH); Serial.println("Mouvement détecté !"); }` — si un mouvement est détecté, la LED s'allume et un message s'affiche dans le moniteur série.

`else { digitalWrite(led, LOW); Serial.println("RAS"); }` — sinon (aucun mouvement), la LED s'éteint et le message "RAS" (Rien à Signaler) s'affiche à la place.

`delay(1000);` — attend 1 seconde avant de relire le capteur, pour éviter d'inonder le moniteur série de messages répétés à chaque fraction de seconde.

---

## Comment tester

1. Branche l'Arduino à l'ordinateur par câble USB
2. Sélectionne la bonne carte et le bon port dans **Outils**
3. Téléverse le code
4. Ouvre le **Moniteur série** (`Ctrl + Maj + M`), réglé sur 9600 bauds
5. Patiente environ 30-60 secondes pour laisser le capteur se stabiliser
6. Bouge ta main devant le capteur : la LED doit s'allumer et "Mouvement détecté !" doit apparaître

---

## Erreurs fréquentes

| Problème observé | Cause probable | Solution |
|---|---|---|
| La LED reste allumée en permanence | Capteur encore en phase de préchauffage, ou sensibilité réglée trop haute | Attendre 1 minute après la mise sous tension ; ajuster le potentiomètre de sensibilité sur le module |
| Aucune détection même avec du mouvement évident | Câblage incorrect, ou broche OUT mal identifiée sur le module | Vérifier le branchement ; certains modules PIR ont un ordre de broches différent (VCC-OUT-GND ou OUT-VCC-GND selon le modèle) |
| Le message "Mouvement détecté !" reste affiché trop longtemps après le mouvement réel | Délai de maintien du signal réglé sur le module PIR | Ajuster le potentiomètre de délai (souvent une seconde vis à côté de celle de sensibilité) sur le module |

---

## Exercices à réaliser

1. **Compteur de détections** — Ajoute une variable qui compte le nombre total de détections depuis le démarrage, affichée à chaque déclenchement.
2. **Délai d'extinction** — Modifie le code pour que la LED reste allumée 5 secondes après la dernière détection, plutôt que de s'éteindre immédiatement dès qu'il n'y a plus de mouvement.
3. **Alarme sonore** — Ajoute un buzzer qui sonne brièvement à chaque nouvelle détection de mouvement (et pas en continu pendant toute la durée de la détection).

---

## Pour aller plus loin

Ce capteur est la base de nombreux systèmes de sécurité. Combiné à un module relais, il pourrait automatiquement allumer une vraie lampe lorsqu'une présence est détectée — une extension naturelle vers la domotique.
