import os
import json
import requests
import time
from typing import Any, List
from datetime import datetime
from paho.mqtt.client import Client, MQTTMessage

BROKER_IP_ADDRESS = os.getenv("BROKER_IP_ADDRESS")
API_SERVER_URL = os.getenv("API_SERVER_URL")
assert BROKER_IP_ADDRESS
assert API_SERVER_URL

DEVICES = []
BROKER_TOPICS = []


def init_broker_topics() -> None:
    """Query the database for all devices and create a list of topics to subscribe to."""
    print("Initializing broker topics...")
    global BROKER_TOPICS
    global DEVICES
    response = requests.get(API_SERVER_URL + "/devices")
    DEVICES = response.json()
    BROKER_TOPICS = [f'/{device["device_hash"]}/plant_data' for device in DEVICES]


def subscribe_to_broker_topics(client: Client) -> None:
    """Subscribe to all topics in the list of topics."""
    print("Subscribing to broker topics...")
    global BROKER_TOPICS
    print(BROKER_TOPICS)
    for topic in BROKER_TOPICS:
        client.subscribe(topic)


def on_connect(client: Client, userdata: Any, flags: dict, rc: int) -> None:
    """Callback for when the client connects to the broker."""
    print("Connected with result code " + str(rc))
    subscribe_to_broker_topics(client)


def on_message(client: Client, userdata: Any, message: MQTTMessage) -> None:
    """Callback for when a message is received from the broker."""
    global DEVICES
    measurement_values: dict = json.loads(message.payload.decode("utf-8"))
    print("Message received: " + str(measurement_values))

    topic = str(message.topic)
    dev_id = None
    for dev in DEVICES:
        if dev["device_hash"] == topic.split("/")[1]:
            dev_id = dev["id"]

    payload = {
        "device_id": dev_id,
        "air_temperature": measurement_values.get("airtemp"),
        "air_humidity": measurement_values.get("airhumidity"),
        "soil_temperature": measurement_values.get("soiltemp"),
        "soil_humidity": measurement_values.get("soilmoisture"),
        "timestamp": datetime.timestamp(datetime.now()),
    }
    print(f"Sending data to {API_SERVER_URL}...")
    response = requests.post(url=API_SERVER_URL + "/plant-data", json=payload)
    if response.status_code == 200:
        print(f"Successful")
    elif response.status_code >= 400:
        print(f"Unsuccessful")


# get device ids which are used as broker topics
init_broker_topics()

# setup mqtt client
client = Client("tibs-mqtt-client")
client.on_connect = on_connect
client.on_message = on_message

# connect to broker and start execution loop
client.connect(BROKER_IP_ADDRESS)
client.loop_start()
old_broker_topics = BROKER_TOPICS
while True:
    init_broker_topics()
    if len(old_broker_topics) != len(BROKER_TOPICS):
        print("Broker topics changed. Subscribing to new topics...")
        subscribe_to_broker_topics(client)
    old_broker_topics = BROKER_TOPICS
    time.sleep(5)
