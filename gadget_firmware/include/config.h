#include <Arduino.h>
class config
{
public:
    const String deviceID;
    const String defaultWifipasswd;
    String mqtt_broker_hostname;
    int mqtt_broker_port;
    String updateServerURL;

    config& operator=(const config &c){
        if(this== &c){
            return *this;
        }
        this->mqtt_broker_hostname = c.mqtt_broker_hostname;
        this->mqtt_broker_port = c.mqtt_broker_port;
        this->updateServerURL = c.updateServerURL;
        return *this;
    }
};

