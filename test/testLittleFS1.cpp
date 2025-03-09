#include <LittleFS.h>
#include <cJSON.h>

int code;

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
        ConfigureData += (char)configureFile.read();
    }

    Serial.println(ConfigureData);

    cJSON *cJSONData = cJSON_Parse(ConfigureData.c_str());
    if (cJSONData == NULL){
        Serial.println("Configure File Parse FAILED! ");
        Serial.println("3");
    }

    cJSON *FIRSTBOOT = cJSON_GetObjectItem(cJSONData,"FIRSTBOOT");
    if (cJSON_IsBool(FIRSTBOOT)){
        code = FIRSTBOOT -> valueint;
    }else{
        Serial.println("Configure File ERROR! Recreating configure File! ");
        Serial.println("3");
    }

    Serial.println(String(code));
}
