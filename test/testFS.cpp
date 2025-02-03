#include <Arduino.h>
#include "LittleFS.h"

void setup()
{
    Serial.begin(115200);
    Serial.println();
    // 挂载文件系统
    if (!LittleFS.begin())
    {
        Serial.println("LittleFS 文件系统挂载失败！");
        return;
    }
    Serial.println("LittleFS 文件系统挂载成功！");

    // 打开或创建文件并写入数据
    File file = LittleFS.open("/test.txt", "w");
    if (!file)
    {
        Serial.println("打开/创建文件失败！");
        return;
    }
    Serial.println("打开/创建文件成功！");
    // 写入数据
    char data[] = "Hello, World!\r\n";
    file.write((uint8_t *)data, strlen(data));
    file.close();
    Serial.println("数据写入成功！");

    // 读取文件内容
    file = LittleFS.open("/test.txt", "r");
    if (!file)
    {
        Serial.println("打开文件失败！");
        return;
    }
    Serial.println("打开文件成功！");
    Serial.println("文件内容：");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}

void loop()
{
    // 空循环
}
