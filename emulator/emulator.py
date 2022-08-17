from paho.mqtt.client import Client, MQTTMessage
from random import randint
import json
import time

def create_random_data():
    return {
        "airtemp": randint(-10,30),
        "airhumidity": randint(0,100),
        "soiltemp": randint(-10,30),
        "soilmoisture": randint(0,100)
    }

CLIENT_ID = "EMULATOR_A"

client = Client("emulator")

client.connect("193.197.229.59")

while True: 
    data = create_random_data()
    print("Publish....")
    client.publish(f'/{CLIENT_ID}/plant_data',payload = json.dumps(data))
    time.sleep(5)