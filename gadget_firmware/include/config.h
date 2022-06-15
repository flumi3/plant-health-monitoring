#include <Arduino.h>
class config
{
public:
    const String deviceID;
    const String defaultWifipasswd;
    const String mqtt_broker_hostname;
    const int mqtt_broker_port;
    const String updateServerURL;
};

