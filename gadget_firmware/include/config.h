#pragma once
#include <Arduino.h>
#include <vector>
#include <map>
#include <exception>
#include <FS.h>
#include <LittleFS.h>

class config
{
public:
    String deviceID;
    String defaultWifipasswd;
    String mqtt_broker_hostname;
    int mqtt_broker_port;
    String updateServerURL;
};

inline std::vector<std::pair<String, String>> create_maping(const config &cfg)
{
    return {
        {"DeviceID", cfg.deviceID},
        {"DefaultWiFiPassword", cfg.defaultWifipasswd},
        {"MQTT-Host", cfg.mqtt_broker_hostname},
        {"MOTT-Port", String(cfg.mqtt_broker_port)},
        {"Update-Server-URL", cfg.updateServerURL}};
}

inline void saveConfig(const config &cfg, File &file)
{

    if (file)
    {
        for (auto &&[key, value]: create_maping(cfg))
        {
            file.println(key + "=" + value);
        }
    }
}

inline config readConfig(File &file)
{

    std::map<String, String> entries;

    if (file)
    {
        while (file.available())
        {
            String data = file.readStringUntil('\n');
            int delim = data.indexOf("=");
            entries.insert({data.substring(0, delim), data.substring(delim + 1, data.length())});
        }
    }
    else
    {
        // Wenn config Datei nicht ok, werfe exception
        throw "Config file not found";
    }
    for (auto &[key, val] : entries)
    {
        val.trim();
    }
    return config({entries["DeviceID"], entries["DefaultWiFiPassword"], entries["MQTT-Host"], entries["MOTT-Port"].toInt(), entries["Update-Server-URL"]});
}

inline void printConfig(const config &c)
{

    for (auto &&[key, value] : create_maping(c))
    {
        Serial.println("[CONFIG] " + key + ": " + value);
    }
}
