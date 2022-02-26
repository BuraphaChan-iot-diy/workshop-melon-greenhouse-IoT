#include <DHT.h>  //Adafruit DHT22 1.3.4 + Adafuit Unified Sensor 1.0.3
#include <ESP8266WiFi.h>
#include<PubSubClient.h>

const char *ssid = "OPPO_A31";//“********”; // cannot be longer than 32 characters!
const char *password = "0813470881";//“*********”;
const char *mqtt_server = "broker.hivemq.com";//“m14.cloudmqtt.com”;
const int mqtt_port = 1883;//19405;
const char *mqttuser = "";//“******”;
const char *mqttpass = "";//“*********”;

#define FAN D0  // ขา D0 ของบอร์ด NodeMCU เป็นขาที่ต่อกับ LED 
#define VALVE1 D1 //D2  // D0 controls solinoid VALVE
#define VALVE2 D2 // D3 controls light
#define SPRAY D3 //D1
#define DHTTYPE DHT22          //Define sensor type
#define DHTPIN D4              // Define sensor pin
#define SOIL_SENSOR A0  /* Connect Soil moisture analog sensor pin to A0 of NodeMCU */

#define ALIAS   "KBHouse_3"
#define time_delay 1000

int timer = 0, timer2 = 0;
unsigned long t1, t2, t_soil, t_solar;;
char str[32];
bool flag = true;

int state = 0;
int stateOutdated = 0;
char buff[16];

unsigned long oldTime;


DHT dht(DHTPIN, DHTTYPE, 15);   //Initialize DHT sensor 

float humid = 0;
float temp = 0;

WiFiClient espclient;
PubSubClient client(mqtt_server, mqtt_port, espclient);

void callback(String topic,byte* payload,unsigned int length1){ 
  Serial.print("message arrived ==> [");
  Serial.print(topic);
  Serial.print("] ");
  String msgg;
  
  for(int i=0;i<length1;i++){
   Serial.print((char)payload[i]);
   msgg += (char)payload[i]; 
   
  }
  Serial.print("\n");
  if(topic == "/house1/control/valve1"){
    if(msgg == "ON_VALVE1"){
       digitalWrite(VALVE1,HIGH); 
       Serial.println("publish(/house1/state/valve1,ON)");
       client.publish("/buu/house1/state/valve1","ON");
       //cli();
       delay(2000);         
    }else if(msgg == "OFF_VALVE1"){
       digitalWrite(VALVE1,LOW);  
       client.publish("/house1/state/valve1","OFF");
       Serial.println("publsh(/house1/state/valve1,OFF)");
       //cli();
       delay(2000);
    }
  }else if(topic=="/house1/control/fan"){
    if(msgg=="ON_FAN"){
      digitalWrite(FAN,HIGH);
      Serial.println("publish(/house1/state/fan,ON)");
      client.publish("/house1/state/fan","ON");
    }else if(msgg="OFF_FAN"){
      digitalWrite(FAN,LOW);
      Serial.println("publsh(/house1/state/fan,OFF)");
      client.publish("/house1/state/fan","OFF");
    }
  }else if(topic=="/house1/control/valve2"){
    if(msgg=="ON_VALVE2"){
      digitalWrite(VALVE2,HIGH);
      Serial.println("publish(/house1/state/valve2,ON)");
      client.publish("/house1/state/valve2","ON");
    }else if(msgg="OFF_VALVE2"){
      digitalWrite(VALVE2,LOW);
      Serial.println("publish(/house1/state/valve2,OFF)");
      client.publish("/house1/state/valve2","OFF");
    }
  }else if(topic=="/house1/control/spray"){
    if(msgg=="ON_SPRAY"){
      digitalWrite(SPRAY,HIGH);
      Serial.println("publish(/house1/state/spray,ON)");
      client.publish("/house1/state/spray","ON");
    }else{
      digitalWrite(SPRAY,LOW);
      Serial.println("publish(/house1/state/spray,OFF)");
      client.publish("/house1/state/spray","OFF");
    }
  }
}

void reconnect(){
   while (!client.connected()) {
     Serial.println("Connecting to MQTT…\n");
     if (client.connect(ALIAS, mqttuser, mqttpass )) {
       Serial.println("connected"); 
       client.subscribe("/house1/control/#");
     } else {
       Serial.print("failed with state ");
       Serial.print(client.state());
       delay(3000);
     }
  }
}
void setup(){
    dht.begin();
    
    Serial.begin(115200);
    Serial.println("Starting...");
    pinMode(SPRAY, OUTPUT);
    pinMode(FAN, OUTPUT);
    pinMode(VALVE1, OUTPUT);
    pinMode(VALVE2,OUTPUT);
   
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

 
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    delay(1000);
    reconnect();
    t1 = millis();
    t2 = millis();
    t_soil = millis()+5000;
    t_solar = millis()+8000;
    flag = false;
}

void loop(){
    if(!client.connected()){
      reconnect();
    }
    client.loop();

    if (millis()-t1 >= 10000) {
            humid = dht.readHumidity(); //อ่านค่าความชื้นจาก DHT
            temp = dht.readTemperature(); //อ่านค่าอุณหภูมิจาก DHT

            if (isnan(humid) || isnan(temp)) { //ตรวจสอบค่าที่อ่านได้ว่าเป็น NaN หรือไม่
              Serial.println("Failed to read from DHT sensor!");
              client.publish("/house1/state/error","Failed to read from DHT!!!");
              delay(1000);
              //return;
            }else{ 
              String data = String(humid)+","+String(temp);
              data.toCharArray(str,data.length()+1);
              client.publish("/house1/state/dht",str); //ส่งข้อมูล ความชื้น และ อุณหภูมิ ไปยัง topic ชื่อ /dht
  
              char s[32];
              data = "publish(/house1/state/dht,["+String(humid)+","+String(temp)+"]))";
              data.toCharArray(s,data.length()+1);
              Serial.println(s);
  
              
           }
           t1 = millis();
        }

        if(millis()-t_soil>=10000){
            float moisture_percentage,moisture;
            moisture = analogRead(SOIL_SENSOR);
            //moisture_percentage = (100.00 - (moisture * 100.00));
            moisture_percentage = map(moisture,0,1023,0,100);
            if (isnan(moisture_percentage)) { //ตรวจสอบค่าที่อ่านได้ว่าเป็น NaN หรือไม่
              Serial.println("Failed to read from Moisture sensor!");
              client.publish("/house1/state/error","Failed to read from SOIL!!!");
              delay(1000);
            }else{
              Serial.print("Moisture value:= ");Serial.println(100-moisture_percentage);
              String data = String(100-moisture_percentage);
              data.toCharArray(str,data.length()+1);
              client.publish("/house1/state/soil",str); //ส่งข้อมูล ความชื้น และ อุณหภูมิ ไปยัง topic ชื่อ /dht
  
              char s[32];
              data = "publish(/house1/state/soil,["+String(100-moisture_percentage)+"]) --> "+String(moisture);
              data.toCharArray(s,data.length()+1);
              Serial.println(s);

            }
            t_soil = millis();
        }
    delay(500);
}
