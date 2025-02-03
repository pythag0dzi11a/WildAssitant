#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// 设置热点名称和密码
const char* hotspotSSID = "ESP8266_AccessPoint";
const char* hotspotPassword = "12345678";

ESP8266WebServer server(80);

// 用于存储用户输入的SSID和密码
String ssid = "";
String password = "";

// 网页HTML内容
const char* htmlForm = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>WiFi Setup</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
      form { display: inline-block; }
      input { margin: 5px; }
    </style>
  </head>
  <body>
    <h1>Connect to WiFi</h1>
    <form action="/connect" method="POST">
      <label for="ssid">SSID:</label>
      <input type="text" id="ssid" name="ssid" required><br>
      <label for="password">Password:</label>
      <input type="password" id="password" name="password" required><br>
      <input type="submit" value="Connect">
    </form>
  </body>
</html>
)rawliteral";

// 处理根路径请求，显示表单
void handleRoot() {
  server.send(200, "text/html", htmlForm);
}

// 处理表单提交，获取SSID和密码
void handleConnect() {
  if (server.method() == HTTP_POST) {
    ssid = server.arg("ssid");
    password = server.arg("password");

    // 切换到STA模式
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    int connectCount = 0;
    while (WiFi.status() != WL_CONNECTED && connectCount < 30) {
      delay(500);
      connectCount++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      server.send(200, "text/html", "<h1>Connected to WiFi</h1><p>IP Address: " + WiFi.localIP().toString() + "</p>");
    } else {
      server.send(200, "text/html", "<h1>Failed to connect to WiFi</h1><p>Please try again.</p>");
    }
  }
}

void setup() {
  Serial.begin(115200);

  // 设置热点模式
  WiFi.mode(WIFI_AP);
  WiFi.softAP(hotspotSSID, hotspotPassword);

  // 设置服务器路由
  server.on("/", handleRoot);
  server.on("/connect", HTTP_POST, handleConnect);

  server.begin();
  Serial.println("Access Point setup complete. Connect to SSID: " + String(hotspotSSID));
}

void loop() {
  server.handleClient();
}
