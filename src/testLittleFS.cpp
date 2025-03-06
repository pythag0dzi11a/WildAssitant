#include <LittleFS.h>
#include <cJSON.h>

void setup(){
    Serial.begin(115200);
    LittleFS.begin();

    if (LittleFS.exists("/configure.json"))
    {
        Serial.println("File exists");
        /* code */
    }else{
        Serial.println("File does not exist");
    }


}

void loop(){
    if (LittleFS.exists("/configure.json"))
    {
        Serial.println("File exists");
        /* code */
    }else{
        Serial.println("File does not exist");
    }

    delay(1000);

    File configureFile = LittleFS.open("/configure.json", "r");
    if (!configureFile)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    while (configureFile.available())
    {
        Serial.write(configureFile.read());
    }
}
