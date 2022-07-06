#define USEAALEC 1

#if USEAALEC
#include <AALeC.h>
#else
#include <Arduino.h>
#endif
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiManager.h>
#include <MQTTClient.h>
#include <vector>
#include <map>
#include "config.h"
#include "measurement.h"
#include <FS.h>
#include <LittleFS.h>
#include <algorithm>

extern int firmware_version;
const String deviceID = "ESP-" + String(ESP.getChipId(), HEX);
const String configFilePath = "/config.ini";

Measurement make_Measurement(std::function<int()> airTemp, std::function<int()> airhum, std::function<int()> soilTemp, std::function<int()> soilMoisture);
int airTemp();
int airhum();
int soilTemp();
int soilMoisture();

void send_Measurement_MQTT(MQTTClient &client, Measurement data);
void mqtt_loop();
void connect_MQTT();

void connect_Wifi();

void update();
bool update_available(const String &url);
HTTPUpdateResult run_update(const String &url, const String &path);

const config default_config{deviceID, "0123456789", "broker.hivemq.com", 1883, "http://192.168.178.23:8000"};
void saveConfig(const config &cfg, File &file);
config readConfig(File &file);
void printConfig(const config &c);

constexpr int sleepdelay15s = 15 * 60 * 1000; // 15 Minuten in ms

WiFiClient wificlient;
MQTTClient mqttclient;
WiFiManager wifimanager;
config cfg = default_config;


void setup()
{

#if USEAALEC
  aalec.init();
  aalec.print_line(1, cfg.deviceID);
  aalec.print_line(2, "Default WIFI-PW:");
  aalec.print_line(3, "    " + cfg.defaultWifipasswd);
#endif

  Serial.println("FIRMWARE VERSION:" + String(firmware_version));

  if (LittleFS.begin())
  {
    Serial.println("[FS] Mounted file system");
    if (LittleFS.exists(configFilePath))
    {
      Serial.println("[FS] Found config file");
      File configFile = LittleFS.open(configFilePath, "r");
      cfg = readConfig(configFile);
      configFile.close();
    }
  }
  else
  {
    Serial.println("[FS] Failed to mount file system");
  }
  WiFiManagerParameter custom_mqtt_server("MQTT_HOST", "MQTT Host", cfg.mqtt_broker_hostname.c_str(), cfg.mqtt_broker_hostname.length());
  WiFiManagerParameter custom_mqtt_port("MQTT_PORT", "MQTT Port", String(cfg.mqtt_broker_port).c_str(), 4);
  wifimanager.addParameter(&custom_mqtt_server);
  wifimanager.addParameter(&custom_mqtt_port);

  connect_Wifi();

  cfg.mqtt_broker_hostname = custom_mqtt_server.getValue();
  cfg.mqtt_broker_port = String(custom_mqtt_port.getValue()).toInt();

  update();
  connect_MQTT();

  File file = LittleFS.open(configFilePath, "w");
  saveConfig(cfg, file);
  file.close();
  printConfig(cfg);
}

void loop()
{

  mqtt_loop();
  delay(2000);
}

void saveConfig(const config &cfg, File &file)
{

  std::vector<String> data{
      cfg.mqtt_broker_hostname,
      String(cfg.mqtt_broker_port),
      cfg.updateServerURL};
  const std::vector<String> names{"MQTThost", "MQTTport",
                                  "updateServerURL"};
  std::vector<String> results;
  std::transform(data.begin(), data.end(), names.begin(), std::back_inserter(results),
                 [](const auto &aa, const auto &bb)
                 {
                   return (bb + "=" + aa);
                 });

  if (file)
  {
    for (auto &r : results)
    {
      file.println(r);
    }
  }
}

config readConfig(File &file)
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
    //Wenn config Datei nicht ok, gib default config zurück
    return default_config;
  }
  const std::vector<String> names{"MQTThost", "MQTTport",
                                  "updateServerURL"};

  for (auto &[key, val] : entries)
  {
    val.trim();
  }
  return config({cfg.deviceID, cfg.defaultWifipasswd, entries["MQTThost"], entries["MQTTport"].toInt(), entries["updateServerURL"]});
}

void printConfig(const config &c){
  std::vector<std::pair<String,String>>data {
    {"DeviceID: ", c.deviceID},
    {"DefaultWiFiPassword: ", c.defaultWifipasswd},
    {"MQTT-Host: ", c.mqtt_broker_hostname},
    {"MOTT-Port: ", String(c.mqtt_broker_port)},
    {"Update-Server-URL: ", c.updateServerURL}
  };
  for(auto&& [key,value] : data){
    Serial.println("[CONFIG] "+ key+ value);
  }
}

//--------WIFI----------

void connect_Wifi()
{
  WiFi.mode(WIFI_STA);
#if USEAALEC
  aalec.set_rgb_strip(4, 255, 0, 0);
#endif
  if (wifimanager.autoConnect(cfg.deviceID.c_str(), cfg.defaultWifipasswd.c_str()))
  {
    Serial.println("[WIFI] Wifi connection established");
#if USEAALEC
    aalec.set_rgb_strip(4, 0, 255, 0);
#endif
  }
  else
  {
    Serial.println("[WIFI] Error while establishing wifi connection");
    ESP.restart();
  }
}
//----------WIFI

//----------MQTT----------------
void connect_MQTT()
{
  Serial.println("[MQTT] Connecting");
  mqttclient.begin(cfg.mqtt_broker_hostname.c_str(), cfg.mqtt_broker_port, wificlient);

  while (!mqttclient.connect(cfg.deviceID.c_str()))
  {
#if USEAALEC
    aalec.set_rgb_strip(0, 255, 0, 0);
    delay(500);
    aalec.set_rgb_strip(0, 0, 0, 0);
    delay(500);
#else
    Serial.print(".");
    delay(1000);
#endif
  }
#if USEAALEC
  aalec.set_rgb_strip(0, 0, 255, 0);
#endif
  Serial.println("[MQTT] connection established");
}

void mqtt_loop()
{
  mqttclient.loop();
  delay(10);

  if (!mqttclient.connected())
  {
    connect_MQTT();
  }

  Measurement data = make_Measurement(airTemp, airhum, soilTemp, soilMoisture);
  send_Measurement_MQTT(mqttclient, data);
}

void send_Measurement_MQTT(MQTTClient &client, Measurement data)
{
  std::vector<std::pair<String, int>> m = {
      {"airtemp", data.airTemp},
      {"airhumidity", data.airhum},
      {"soiltemp", data.soilTemp},
      {"soilmoisture", data.soilMoisture}};

  // Construct JSON object by hand
  String payload = "{";
  for (size_t i = 0; i < m.size(); i++)
  {
    payload += (m[i].first + " : " + m[i].second);
    if (i < m.size() - 1)
    {
      payload += ",";
    }
    payload += "\n";
  }
  payload += "}";
  //

  client.publish("/" + cfg.deviceID + "/plant_data", payload);
}

//----------MQTT-------------

//---------MEASURE---------------------
Measurement make_Measurement(std::function<int()> airTemp, std::function<int()> airhum, std::function<int()> soilTemp, std::function<int()> soilMoisture)
{
  return {airTemp(), airhum(), soilTemp(), soilMoisture()};
}

int airTemp()
{
#if USEAALEC

  return aalec.get_temp();

#endif
  return 2;
}

int airhum()
{
#if USEAALEC
  return aalec.get_humidity();
#endif
  return 4;
}

int soilTemp()
{
#if USEAALEC
  return aalec.get_analog();
#endif
  return 4;
}

int soilMoisture()
{
#if USEAALEC
  return aalec.get_analog();
#endif
  return 8;
}

//-----OTA-UPDATE-----

void update()
{

  if (update_available(cfg.updateServerURL + "/firmwareVersion"))
  {
    Serial.println("[UPDATE] Update available");
    delay(1000);
    run_update(cfg.updateServerURL, "/firmware/firmware.bin");
  }
}

bool update_available(const String &url)
{
  HTTPClient http;

  if (!http.begin(wificlient, url))
  {
    Serial.println("[HTTP] Connection error");
  }
  int http_res = http.GET();
  int http_answer = http.getString().toInt();
  int available_version_number = http_res == 200 ? http_answer : firmware_version;
  http.end();
  return firmware_version < available_version_number;
}

HTTPUpdateResult run_update(const String &url, const String &path)
{
  ESPhttpUpdate.onProgress([](int cur, int total)
                           { Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total); });
  auto ret = ESPhttpUpdate.update(wificlient, url + path);
  if (ret == HTTP_UPDATE_OK)
  {
    Serial.println("HTTP_UPDATE_OK");
    delay(1000); // Wait a second and restart
    ESP.restart();
  }
  return ret;
}