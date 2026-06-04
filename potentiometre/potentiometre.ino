int pot = A0;
int led = 9;
int button = 2;      
int lastButtonState = LOW; 

bool systemActif = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pot, INPUT);
  pinMode(led, OUTPUT);
  analogWrite(led, 0);
  pinMode(button, INPUT); 

}

void loop() {
  int buttonState = digitalRead(button);
  int valeurpot = analogRead(pot);
  int ledState = map(valeurpot, 0, 1023, 0, 255);

  if (buttonState == HIGH && lastButtonState == LOW) {
    systemActif = !systemActif;
    
  }
  if (systemActif == true) {
      analogWrite(led, ledState);      

    } else {
      analogWrite(led, 0);      
    }

  lastButtonState = buttonState;


  // put your main code here, to run repeatedly:
  //with one single LED
  //int valeurpot = analogRead(pot);
  float tension = valeurpot * 5.0 / 1023.0 ;
  Serial.println(tension);
  /*int luminosite = map(valeurpot, 0, 1023, 0, 255);
  analogWrite(led, luminosite);
  delay(50);*/

}
