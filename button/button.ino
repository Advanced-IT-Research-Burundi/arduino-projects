int button = 2; 
int led = 13;   
int ledState = LOW;          
int buttonState;       
int lastButtonState = LOW; 

void setup() {
  pinMode(led, OUTPUT);
  pinMode(button, INPUT); 
  digitalWrite(led, ledState); 
}

void loop() {
  buttonState = digitalRead(button);

  if (buttonState == HIGH && lastButtonState == LOW) {
    
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    
    digitalWrite(led, ledState);
    delay(50); // Anti-rebond
  }

  lastButtonState = buttonState;
}