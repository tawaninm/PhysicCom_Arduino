void setup() {
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
}
void loop() {
  if (digitalRead(4) == LOW){
    digitalWrite(8,0);
  }
  else {
    digitalWrite(8,1);
  }
  if(digitalRead(5) == LOW){
    digitalWrite(9,0);
  }
  else {
    digitalWrite(9,1);
  }
  if(digitalRead(6) == LOW) {
    digitalWrite(10,0);
  }
  else {
    digitalWrite(10,1);
  }
}
