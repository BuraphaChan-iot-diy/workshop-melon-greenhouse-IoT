void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A0);
  Serial.print("SensorValue:= ");
  Serial.println(sensorValue);
  int moisture = map(sensorValue,0,1024,0,100);
  moisture = 100-moisture;
  Serial.print("Moisture:= ");
  Serial.println(moisture);
  if(moisture<30){
    Serial.println("Dry");
  }else if(moisture>=30 && moisture <70){
    Serial.println("Normal");
  }else{
    Serial.println("Wet");
  }
  delay(3000);
}
