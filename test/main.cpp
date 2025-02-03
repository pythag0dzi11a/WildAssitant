#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>

void connectWiFi();
void connectMQTTServer();
void pubMQTTMessage();
void tickerCount();

const char* ssid = "pythagodzilla";
const char *password = "zzxzzxzzx";
const char *mqtt_server = "http://pythagodzilla.pw";
const char *addr = "esp/test";

Ticker ticker;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

int count;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  connectWiFi();

  mqttClient.setServer(mqtt_server, 1883);

  connectMQTTServer();

  ticker.attach(1, tickerCount);

}

void loop() {
  if (!mqttClient.connected()){
    connectMQTTServer();
  }

  mqttClient.loop();

  if (count == 10){
    pubMQTTMessage();
    count = 0;
  }
}

void connectWiFi(){
  WiFi.begin(ssid, password);

  for (int connectCounter = 0; connectCounter < 10 && WiFi.status() != WL_CONNECTED; connectCounter++){
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

}

void pubMQTTMessage(){
  static int value;

  String topicString = addr + WiFi.macAddress();

  char publishTopic[topicString.length() + 1];
  strcpy(publishTopic, topicString.c_str());

  String messageString = "Message " + String(value);
  char publishMessage[messageString.length() + 1];
  strcpy(publishMessage, messageString.c_str());

  if (mqttClient.publish(publishTopic, publishMessage)){
    Serial.println("Message published");
  } else {
    Serial.println("Message not published");
  }
}

void connectMQTTServer() {
  String clientId = "ESP8266Client-" + String(WiFi.macAddress());

  if (mqttClient.connect(clientId.c_str())){
    Serial.println("Connected to MQTT server");
  } else {
    Serial.println("Not connected to MQTT server");
  }
}

void tickerCount(){
  count++;
}
