int vert = 7;
int bleu = 8;
//int jaune = 12;
//int rouge = 4;

void setup() {
  pinMode(vert, OUTPUT);
  pinMode(bleu, OUTPUT);
  //pinMode(jaune, OUTPUT);
  //pinMode(rouge, OUTPUT);

}

void loop() {
  digitalWrite(vert, HIGH); 
  delay(2000);           
  digitalWrite(vert, LOW);  

  digitalWrite(bleu, HIGH); 
  delay(2000);           
  digitalWrite(bleu, LOW); 

  /*digitalWrite(jaune, HIGH); 
  delay(2000);           
  digitalWrite(jaune, LOW);    

  digitalWrite(rouge, HIGH); 
  delay(10000);           
  digitalWrite(rouge, LOW);
*/}
