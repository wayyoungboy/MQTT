#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "mzz";
const char* password = "mzzmqtta";
const char* mqtt_server = "47.101.214.72";

const int mqttPort = 1883;

 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   // Serial.println("Connecting to WiFi..");
  }
 // Serial.println("Connected to the WiFi network");
 
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
 //   Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client1")) {
     // Serial.println("connected");  
 
    } else {
 
     // Serial.print("failed with state ");
     // Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  
  client.subscribe("mzz");
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
 // Serial.print("Message arrived in topic: ");
 // Serial.println(topic);
 
  //Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
}
 
void loop() {
  client.loop();
 
}
