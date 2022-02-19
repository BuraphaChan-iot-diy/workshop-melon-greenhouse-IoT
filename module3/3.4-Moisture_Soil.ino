int sensorPin = A0; // select the input pin for the Soil moisture sensor
int sensorValue = 0; // variable to store the value coming from the sensor

void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  int humid = map(sensorValue, 0, 1024, 0, 100);
  humid = 100 - humid;
  Serial.print("sensor = " );Serial.println(sensorValue);
  Serial.print("humid = " );Serial.println(humid);

  if(humid <= 30){
    Serial.println("Dry Soil");
  }else if(humid > 30 && humid <= 70){
    Serial.println("Humid Soil");
  }else{
    Serial.println("in Water");
  }
  delay(3000);
}
