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
#include <queue>
#include <DFRobot_SHT20.h>
#include "config.h"
#include "measurement.h"

extern int firmware_version;
const String deviceID = "ESP-" + String(ESP.getChipId(), HEX);
const String configFilePath = "/config.ini";

Measurement take_Measurement(std::function<int()> airTemp, std::function<int()> airhum, std::function<int()> soilTemp, std::function<int()> soilMoisture);
int airTemp();
int airhum();
int soilTemp();
int soilMoisture();

void mqtt_loop();
void connect_MQTT();
void mqtt_callback(const String &topic, const String &payload);

void connect_Wifi();

void update();
bool update_available(const String &url);
HTTPUpdateResult run_update(const String &url, const String &path);

const config default_config{deviceID, "0123456789", "broker.hivemq.com", 1883, "http://192.168.178.23:8000"};

constexpr int sleepdelay15s = 15 * 60 * 1000; // 15 Minuten in ms

WiFiClient wificlient;
MQTTClient mqttclient;
WiFiManager wifimanager;
config cfg = default_config;

using mqtt_message = std::pair<String, String>;

std::queue<mqtt_message> mqtt_message_queue;

const String reset_topic = "/"+deviceID+"/reset";
void reset();

DFRobot_SHT20 sht20(&Wire);

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
      try
      {
        cfg = readConfig(configFile);
      }
      catch (...)
      {
        cfg = default_config;
      }
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

  mqttclient.onMessage(mqtt_callback);
  mqttclient.subscribe(reset_topic);

  sht20.initSHT20();
  Serial.println("[SHT20] Sensor init finish!");

}

void loop()
{
  Measurement data = take_Measurement(airTemp, airhum, soilTemp, soilMoisture);
  String payload = measurement_to_json(data);
  mqtt_message_queue.push({"/" + cfg.deviceID + "/plant_data", payload});
  mqtt_loop();
  delay(2000);
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

  while (!mqtt_message_queue.empty())
  {
    auto &&[topic, payload] = mqtt_message_queue.front();
    mqttclient.publish(topic, payload);
    mqtt_message_queue.pop();
  }
}

void mqtt_callback(const String &topic, const String &payload){
  if(topic == reset_topic){
    reset();
  }

}
//----------MQTT-------------

//---------MEASURE---------------------
Measurement take_Measurement(std::function<int()> airTemp, std::function<int()> airhum, std::function<int()> soilTemp, std::function<int()> soilMoisture)
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
  return sht20.readTemperature();
#endif
  return 6;
}

int soilMoisture()
{
#if USEAALEC
  return sht20.readHumidity();
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

//-------Reset----------

void reset()
{
  wifimanager.resetSettings();
  Serial.println("[WIFI] Wifi data deleted");
  auto del = LittleFS.remove(configFilePath);
  if(del)
    Serial.println("[CONFIG] Config file removed");

  delay(1000);
  ESP.restart();
}