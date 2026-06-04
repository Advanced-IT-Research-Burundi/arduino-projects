int photo = A0;
int led = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(photo, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int lecture = analogRead(photo);
  Serial.println(lecture); 
  if (lecture>10) {
    digitalWrite(led, LOW);
  } else {
    digitalWrite(led, HIGH);

  }
  
}
