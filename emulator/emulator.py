import time
import json
from random import randint

from paho.mqtt.client import Client, MQTTMessage

CLIENT_ID = "EMULATOR_A"
MESSAGE = None


def create_random_data():
    """Create random device data in order to mock a device."""
    return {
        "airtemp": randint(-10, 30),
        "airhumidity": randint(0, 100),
        "soiltemp": randint(-10, 30),
        "soilmoisture": randint(0, 30),
    }


def on_message(client: Client, userdata, message: MQTTMessage):
    """Callback for when a message is received from the broker."""
    global MESSAGE
    MESSAGE = message


def on_connect(client: Client, userdata, flags: dict, rc: int):
    """Callback for when the client connects to the broker."""
    client.subscribe(f"/{CLIENT_ID}/reset")
    print(rc)


client = Client("emulator")
client.on_message = on_message
client.on_connect = on_connect
client.connect("193.197.229.59")

while True:
    data = create_random_data()
    print("Publish....")
    client.publish(f"/{CLIENT_ID}/plant_data", payload=json.dumps(data))
    client.loop()
    time.sleep(5)
