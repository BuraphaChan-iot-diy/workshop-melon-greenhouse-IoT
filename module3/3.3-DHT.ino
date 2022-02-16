#include <DHT.h>  //Adafruit DHT22 1.3.4 + Adafuit Unified Sensor 1.0.3
#define DHTTYPE DHT22          //Define sensor type
#define DHTPIN D4              // Define sensor pin

DHT dht(DHTPIN, DHTTYPE);   //Initialize DHT sensor 

void setup(){
    dht.begin();    
    Serial.begin(9600);
    Serial.println("Starting...");
}

void loop(){
    float humid = dht.readHumidity(); //อ่านค่าความชื้นจาก DHT
    float temp = dht.readTemperature(); //อ่านค่าอุณหภูมิจาก DHT
    if (isnan(humid) || isnan(temp)) { //ตรวจสอบค่าที่อ่านได้ว่าเป็น NaN หรือไม่
      Serial.println("Failed to read from DHT sensor!");
    }else{
      Serial.print("Temperature: ");Serial.println(temp);
      Serial.print("Humidity: ");Serial.println(humid);
    }
    delay(3000);
}
