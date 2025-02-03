#include <Arduino.h>
#include <PubSubClient.h>
#include <LittleFS.h>

// 测试时使用的参数
#define FIRST_START 1 // 未来用LITTLEFS存储，也许ArduinoJSON

// 引脚定义
#define ENABLE_SENSOR 1
#define ANALOG_PIN 3 //要用的时候再赋能

#define INTERVAL 900 // 这是检测间隔，单位为秒

// 选择使用的模块，0是不使用，1是使用
#define USE_ESP8266 1
#define USE_ESP32 0

//AP模式设置
const char *AP_NAME = "pythagodzilla";
const char *AP_PASS = "pythagodzilla";
const char* htmlForm = R"rawliteral(
  <html>
    <head>
      <title>Wi-Fi 配置</title>
    </head>
    <body>
      <h2>请输入Wi-Fi配置</h2>
      <form action="/save" method="POST">
        <label for="ssid">SSID:</label><br>
        <input type="text" id="ssid" name="ssid" required><br><br>
        <label for="pass">密码:</label><br>
        <input type="password" id="pass" name="pass" required><br><br>
        <input type="submit" value="保存配置">
      </form>
    </body>
  </html>
)rawliteral";

// WiFi初始化
const char *ssid = "301-5G";
const char *password = "zhou20110625"; // 密码


// MQTT初始设置
const char *mqtt_broker = "pythagodzilla.pw";
const char *topic = "liuLake/SoilHumiditySensor";
const char *mqtt_username = "pythagodzilla";
const char *mqtt_password = "jtbx2mtblj";
const int mqtt_port = 1883;

/************************** 设置 ***********************************/

#if USE_ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#endif

#if USE_ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#endif


WiFiClient espClient;
PubSubClient client(espClient);

// 声明函数
void callback(char *topic, byte *payload, unsigned int length);
int getHumidity();
void connectWiFi();
void connectMQTTBroker();

// setup函数
void setup()
{
    // 初始化引脚
    pinMode(ENABLE_SENSOR, OUTPUT);
    pinMode(ANALOG_PIN, INPUT);

    Serial.begin(115200);

    connectWiFi();
    connectMQTTBroker();
}

// loop函数
void loop()
{
    static long pastTime = 0;
    int humidity = 0;
    char strHumidity[16];

    // 每隔INTERVAL秒获取一次土壤湿度
    if ((millis() - pastTime) / 10 > INTERVAL) {
        humidity = getHumidity();
        sprintf(strHumidity, "\"Humidity\" : %d", humidity);
        client.publish(topic, strHumidity);
        pastTime = millis();
    }
    client.loop();
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
    Serial.println();
}

//这个函数用来获取土壤湿度
int getHumidity()
{
    static int humidity = 0;
    digitalWrite(ENABLE_SENSOR, HIGH);\

    delay(100); // 用delay是因为方便，具体影响再看

    humidity = analogRead(ANALOG_PIN);
    digitalWrite(ENABLE_SENSOR, LOW);

    return humidity;
}

// 顾名思义，连接WiFi
void connectWiFi()
{
    WiFi.begin(ssid, password);

    for (int connectCount = 0; connectCount < 15; connectCount++)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        else
        {
            break;
        }

        if (connectCount == 14)
        {
            Serial.println("WiFi connection failed. Please check your settings and the signal.");
            return;
        }
    }
    Serial.println("Connected to the WiFi network");
}

// 连接MQTT Broker
void connectMQTTBroker()
{
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

    client.publish(topic, "Sensor is online!");
    client.subscribe(topic);
}

void firstStart()
{
    WiFi.mode(WIFI_STA);
    ESP8266WebServer server(80);

    WiFi.softAP(AP_NAME, AP_PASS);

}
