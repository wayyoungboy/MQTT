#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "mzz";
const char* password = "mzzmqtta";
const char* mqtt_server = "47.101.214.72";
const char* client_id = "clientId-ApjJZcy9Dh"; 
const int mqttPort = 1883;


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client1")) {
      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }

 //client.publish("mzz", "Hello from ESP8266");


}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

}
char *comdata;
String data = "";
void loop() {
  if (Serial.available() > 0) {
    data = "";
     delay(2);
    while (Serial.available())   //时刻读取硬件串口数据
    {
      data += char(Serial.read());
     
    }
    comdata = &data[0];
    Serial.println(data);
    client.publish("mzz",  comdata);

  }
}
