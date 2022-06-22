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
#include <map>
#include "config.h"
#include "measurement.h"

extern int firmware_version;

Measurement make_Measurement(std::function<int()> airTemp, std::function<int()> airhum, std::function<int()> soilTemp, std::function<int()> soilMoisture);
int airTemp();
int airhum();
int soilTemp();
int soilMoisture();

void send_Measurement_MQTT(MQTTClient &client, Measurement data);
void mqtt_loop();
void connect_MQTT();

void connect_Wifi();
bool update_available(const String &url);
HTTPUpdateResult run_update(const String &url, const String &path);

WiFiClient wificlient;
MQTTClient mqttclient;
WiFiManager wifimanager;

const config cfg{(String("ESP-") + String(ESP.getChipId(), HEX)), "0123456789", "broker.hivemq.com", 1883, "http://192.168.178.23:8000"};

constexpr int sleepdelay15s = 15 * 60 * 1000; // 15 Minuten in ms

void setup()
{

#if USEAALEC
  aalec.init();
  aalec.print_line(1, cfg.deviceID);
#endif
  Serial.println("FIRMWARE VERSION:" + String(firmware_version));
  connect_Wifi();
  if (update_available(cfg.updateServerURL + "/firmwareVersion"))
  {
    Serial.println("[UPDATE] Update available");
    delay(1000);
    run_update(cfg.updateServerURL, "/firmware/firmware.bin");
  }

  connect_MQTT();
}

void loop()
{

  mqtt_loop();
  delay(2000);
}

//--------WIFI----------

void connect_Wifi()
{
  WiFi.mode(WIFI_STA);
  if (wifimanager.autoConnect(cfg.deviceID.c_str(), cfg.defaultWifipasswd.c_str()))
  {
    Serial.println("[WIFI] Wifi connection established");
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
    Serial.print(".");
    delay(1000);
  }
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
  auto ret = ESPhttpUpdate.update(wificlient, url+path);
  if (ret == HTTP_UPDATE_OK)
  {
    Serial.println("HTTP_UPDATE_OK");
    delay(1000); // Wait a second and restart
    ESP.restart();
  }
  return ret;
}