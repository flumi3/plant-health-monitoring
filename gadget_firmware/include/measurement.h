#include <Arduino.h>
class Measurement
{
public:
  int airTemp{};
  int airhum{};
  int soilTemp{};
  int soilMoisture{};
};


inline String measurement_to_json(const Measurement &data)
{
  std::vector<std::pair<String, int>> m = {
      {"airtemp", data.airTemp},
      {"airhumidity", data.airhum},
      {"soiltemp", data.soilTemp},
      {"soilmoisture", data.soilMoisture}};

  // Construct JSON object by hand
  String json = "{";
  for (size_t i = 0; i < m.size(); i++)
  {
    json += (m[i].first + " : " + m[i].second);
    if (i < m.size() - 1)
    {
      json += ",";
    }
    json += "\n";
  }
  json += "}";
  //
  return json;
}