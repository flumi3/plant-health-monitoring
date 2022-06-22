import json
import requests
from typing import Any
from datetime import datetime
from paho.mqtt.client import Client, MQTTMessage

# TODO: add information
BROKER_IP_ADDRESS = ""
BROKER_TOPIC = ""
API_SERVER_URL = ""


def on_connect(client: Client, userdata: Any, flags: dict, rc: int):
    print("Connected with result code " + str(rc))
    print(f"Subscribing to broker topics...")
    client.subscribe(BROKER_TOPIC)


def on_message(client: Client, userdata: Any, message: MQTTMessage):
    measurement_values: dict = json.loads(message.payload.decode("utf-8"))
    print("Message received: " + str(measurement_values))
    
    payload = {
        "air_temperature": measurement_values.get("airtemp"),
        "air_humidity": measurement_values.get("airhumidity", -1),  # TODO: use air humidity if possible with sensor
        "soil_temperature": measurement_values.get("soiltemp"),
        "soil_humidity": measurement_values.get("soilmoisture"),
        "timestamp": datetime.timestamp(datetime.now())
    }
    print(f"Sending data to {API_SERVER_URL}...")
    response = requests.post(url=API_SERVER_URL, data=payload)
    if response.status_code == 200:
        print(f"Successful")
    elif response.status_code >= 400:
        print(f"Unsuccessful") 


# setup mqtt client
client = Client("tibs-mqtt-client")
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER_IP_ADDRESS)
client.loop_forever()
