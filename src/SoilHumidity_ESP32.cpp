#include <WiFi.h>
#include <PubSubClient.h>
#include <stdio.h>

void callback(char *topic, byte *payload, unsigned int length);
int getHumidity();

//pin
#define enableSensor 1
#define analogPin 2

// WiFi
const char *ssid = "俺滴牛牛是钢钻";     // Enter your Wi-Fi name
const char *password = "cddndldadnnsgz"; // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "pythagodzilla.pw";
const char *topic = "liuLake/Soil";
const char *mqtt_username = "pythagodzilla";
const char *mqtt_password = "jtbx2mtblj";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
    // Set software serial baud to 115200;
    Serial.begin(115200);
    // Connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the Wi-Fi network");
    // connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected())
    {
        String client_id = "liuLake";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
        {
            Serial.println("Public EMQX MQTT broker connected");
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    // Publish and subscribe
    client.publish(topic, "Sensor is online!");
    client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void loop()
{
    static long pastTime = 0;
    int humidity = 0;
    char strHumidity[16];

    if (millis() - pastTime > 15000) {
        humidity = getHumidity();
        sprintf(strHumidity, "Humidity = %d", humidity);
        client.publish(topic, strHumidity);
        pastTime = millis();
    }
    client.loop();
}

int getHumidity(){
    static int humidity = 0;
    digitalWrite(enableSensor, HIGH);
    humidity = analogRead(analogPin);
    digitalWrite(enableSensor, LOW);

    return humidity;
}
