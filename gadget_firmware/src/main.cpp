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

extern const char* firmware_version;


Measurement make_Measurement(std::function<int()> airTemp, std::function<int()> soilTemp, std::function<int()> soilMoisture);
int airTemp();
int soilTemp();
int soilMoisture();

void send_Measurement_MQTT(MQTTClient &client, Measurement data);
void mqtt_loop();
void connect_MQTT();

void connect_Wifi();
bool update_available(const String &url);
HTTPUpdateResult run_update(const String &url, int port, const String &path);

WiFiClient wificlient;
MQTTClient mqttclient;
WiFiManager wifimanager;

const config cfg{(String("ESP-") + String(ESP.getChipId(), HEX)), "0123456789", "broker.hivemq.com", 1883, "http://192.168.79.1:8000"};

constexpr int sleepdelay15s = 15 * 60 * 1000; // 15 Minuten in ms

void setup()
{

#if USEAALEC
  aalec.init();
  aalec.print_line(1, cfg.deviceID);
#endif
  Serial.println("FIRMWARE VERSION:" + String(firmware_version));
  connect_Wifi();
  connect_MQTT();

  if (update_available(cfg.updateServerURL+"/firmwareVersion"))
  {
    run_update(cfg.updateServerURL, 8080, "/firmware/firmware.bin");
  }
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

  Measurement data = make_Measurement(airTemp, soilTemp, soilMoisture);
  send_Measurement_MQTT(mqttclient, data);
}

void send_Measurement_MQTT(MQTTClient &client, Measurement data)
{
  std::map<String, int> m = {
      {"airtemp", data.airTemp},
      {"soiltemp", data.soilTemp},
      {"soilmoisture", data.soilMoisture}};

  for (auto const &x : m)
  {
    client.publish("/" + cfg.deviceID + "/" + x.first, String(x.second));
  }
}

//----------MQTT-------------

//---------MEASURE---------------------
Measurement make_Measurement(std::function<int()> airTemp, std::function<int()> soilTemp, std::function<int()> soilMoisture)
{
  return {airTemp(), soilTemp(), soilMoisture()};
}

int airTemp()
{
#if USEAALEC

  return aalec.get_temp();

#endif
  return 2;
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
  return 6;
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

  String available_version_number =  http_res == 200 ?  http.getString() : firmware_version;
  http.end();
  return String(firmware_version) != available_version_number;
}

HTTPUpdateResult run_update(const String &url, int port, const String &path)
{
  return ESPhttpUpdate.update(wificlient, url, port, path);
}