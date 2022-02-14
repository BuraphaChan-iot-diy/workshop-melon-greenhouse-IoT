void setup() {
  Serial.begin(9600);
  pinMode(D0,OUTPUT);
}

void loop() {
  digitalWrite(D0,HIGH);
  Serial.println("HIGH");
  delay(1000);
  digitalWrite(D0,LOW);
  Serial.println("LOW");
  delay(1000);
}
