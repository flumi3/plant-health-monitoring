import os
import json
import requests
import time
from typing import Any
from datetime import datetime
from paho.mqtt.client import Client, MQTTMessage


BROKER_IP_ADDRESS = os.getenv("BROKER_IP_ADDRESS")
API_SERVER_URL = os.getenv("API_SERVER_URL")
print(BROKER_IP_ADDRESS)
print(API_SERVER_URL)

DEVICES = []
BROKER_TOPICS = []

def get_devices():
    global BROKER_TOPICS
    global DEVICES
    response = requests.get(API_SERVER_URL + "/devices")
    DEVICES= response.json()
    BROKER_TOPICS = [f'/{device["device_hash"]}/plant_data' for device in DEVICES]

def subscribe(client: Client):
    global BROKER_TOPICS
    for topic in BROKER_TOPICS:
        client.subscribe(topic)


def on_connect(client: Client, userdata: Any, flags: dict, rc: int):
    print("Connected with result code " + str(rc))
    print(f"Subscribing to broker topics...")
    subscribe(client)


def on_message(client: Client, userdata: Any, message: MQTTMessage):
    global DEVICES
    measurement_values: dict = json.loads(message.payload.decode("utf-8"))
    print("Message received: " + str(measurement_values))
    
    topic = str(message.topic)
    dev_id = None
    for dev in DEVICES: 
        if dev["device_hash"] == topic.split("/")[1]:
            dev_id = dev['id']

    payload = {
        "device_id" : dev_id,
        "air_temperature": measurement_values.get("airtemp"),
        "air_humidity": measurement_values.get("airhumidity"),
        "soil_temperature": measurement_values.get("soiltemp"),
        "soil_humidity": measurement_values.get("soilmoisture"),
        "timestamp": datetime.timestamp(datetime.now())
    }
    print(f"Sending data to {API_SERVER_URL}...")
    response = requests.post(url=API_SERVER_URL+"/plant-data", json=payload)
    if response.status_code == 200:
        print(f"Successful")
    elif response.status_code >= 400:
        print(f"Unsuccessful") 

# get device ids which are used as broker topics
get_devices()

# setup mqtt client
client = Client("tibs-mqtt-client")
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER_IP_ADDRESS)
client.loop_start()
while True:
    get_devices()
    subscribe(client)
    time.sleep(5)
