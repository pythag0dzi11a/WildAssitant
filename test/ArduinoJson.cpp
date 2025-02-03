#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

void initRun();

void setup(){
    Serial.begin(115200);

    if (!LittleFS.begin())
    {
        Serial.println("File system initialization failed.");
        return;
    }

    initRun();
}

void loop(){
    initRun();
}

void initRun(){
    if (LittleFS.exists("/config.json"))
    {
        Serial.println("Config file exists.");

        File configFile = LittleFS.open("/config.json", "r");
        String config;

        if (configFile)
        {
            Serial.println("Config file opened.");

            while (configFile.available())
            {
                config += (char)configFile.read();
            }

            configFile.close();

            //准备解析json
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, config);
            if (error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                return;
            }

            //获取json中的数据
            bool firstStart = doc["firstStart"];

            if (firstStart)
            {
                Serial.println("First start.");
            }
            }else{
                Serial.println("Config file open failed.");
            }
    }
}
