#include <LittleFS.h>
#include <cJSON.h>

/*0 for not init. 1 for inited. 3 for error. 4 for not set*/
short firstBootCode = 4;

void isFirstBoot();

void setup(){

    Serial.begin(115200);



}

void loop(){

LittleFS.begin();

isFirstBoot();

if (LittleFS.exists("/configure.json"))
{
    Serial.println("File exists");
    /* code */
}else{
    Serial.println("File does not exist");
}
    delay(1000);

/*    String ConfigureData;

    if (LittleFS.exists("/configure.json"))
    {
        Serial.println("File exists");

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
    }*/
}
void isFirstBoot(){
    String configureData;
    LittleFS.begin();

    //读取configure.json内容，并保存到configureData中。
    if(LittleFS.exists("/configure.json")){
        File metaConfigureData = LittleFS.open("/configure.json","r");

        if ( metaConfigureData ){
            while ( metaConfigureData.available() ){
                Serial.write(metaConfigureData.read());
                configureData += (char)metaConfigureData.read();
            }

            metaConfigureData.close();
        }else{
            Serial.println("Configure File Open FAILED! ");

            metaConfigureData.close();
        }
    }else{
        Serial.println("Configure File Doesn't EXISTS! ");

    }
//

    //使用cJSON解析configureData内容，并匹配FIRST_BOOT的值。
    cJSON *cJSONData = cJSON_Parse(configureData.c_str());
    if (cJSONData == NULL){
        Serial.println("Configure File Parse FAILED! ");
        firstBootCode = 3;
    }

    cJSON *FIRSTBOOT = cJSON_GetObjectItem(cJSONData,"FIRST_BOOT");
    if (cJSON_IsBool(FIRSTBOOT)){
        firstBootCode = FIRSTBOOT -> valueint;
    }else{
        Serial.println("Configure File ERROR! Recreating configure File! ");
        firstBootCode = 3;
    }

    cJSON_Delete(cJSONData);

    Serial.println(String(firstBootCode));
}
