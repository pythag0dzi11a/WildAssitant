#include <LittleFS.h>
#include <cJSON.h>

void setup(){

    int isFirstStart;

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

    String ConfigureData;

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
        ConfigureData += (char)configureFile.read();
    }

    cJSON *configuration = cJSON_Parse(ConfigureData.c_str());
    if (configuration == NULL)
    {
        Serial.println("Failed to parse JSON");
        return;
    }

    cJSON *FIRSTSTART = cJSON_GetObjectItem(configuration, "FIRST_BOOT");
    if (cJSON_IsBool(FIRSTSTART))
    {
         = FIRSTSTART->valueint;
        /* code */
    }

}
