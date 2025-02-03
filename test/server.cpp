#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <littleFS.h>

void startAP();

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

ESP8266WebServer server(80);

void setup()
{
    Serial.begin(115200);

    if (!LittleFS.begin())
    {
        Serial.println("File system initialization failed.");
        return;
    }

}

void loop()
{
    server.handleClient();
}

void handleRoot()
{
    server.send(200, "text/plain", htmlForm);
}

void startAP(){
    WiFi.softAP(AP_NAME, AP_PASS);
    Serial.println("AP Started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
}
