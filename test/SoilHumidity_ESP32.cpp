#include <WiFi.h>
#include <PubSubClient.h>
#include <stdio.h>

// 函数声明
void callback(char *topic, byte *payload, unsigned int length);
int getHumidity();

// 针脚定义
#define enableSensor 1
#define analogPin 3 //要用的时候再赋能

// 定义常量
#define INTERVAL 900 // 这是检测间隔，单位为秒

// WiFi初始化
const char *ssid = "俺滴牛牛是钢钻";     // WiFi 的 ssid
const char *password = "cddndldadnnsgz"; // 密码

// MQTT Broker的相关设置
const char *mqtt_broker = "pythagodzilla.pw";
const char *topic = "liuLake/SoilHumiditySensor";
const char *mqtt_username = "pythagodzilla";
const char *mqtt_password = "jtbx2mtblj";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
    // 初始化引脚
    pinMode(enableSensor, OUTPUT);
    pinMode(analogPin, INPUT);

    Serial.begin(115200);
    // 连接WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the Wi-Fi network");

    // 连接MQTT Broker
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

    // 发送上线信息，订阅主题
    client.publish(topic, "Sensor is online!");
    client.subscribe(topic);
}

// 回调函数
void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }

    // 在这边放你收到消息的动作
    Serial.println();
    Serial.println("-----------------------");
}

void loop()
{
    static long pastTime = 0;
    int humidity = 0;
    char strHumidity[16];

    // 每隔INTERVAL秒获取一次土壤湿度
    if ((millis() - pastTime) / 10 > INTERVAL) {
        humidity = getHumidity();
        sprintf(strHumidity, "Humidity = %d", humidity);
        client.publish(topic, strHumidity);
        pastTime = millis();
    }
    client.loop();
}

//这个函数用来获取土壤湿度，独立出来是因为以后可能玩花的所以就先独立出来了
int getHumidity(){
    static int humidity = 0;
    digitalWrite(enableSensor, HIGH);
    humidity = analogRead(analogPin);
    digitalWrite(enableSensor, LOW);

    return humidity;
}
