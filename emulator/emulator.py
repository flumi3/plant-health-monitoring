from paho.mqtt.client import Client, MQTTMessage
from random import randint
import time
import json

CLIENT_ID = "EMULATOR_A"
def create_random_data():
    return {
        "airtemp": randint(-10,30),
        "airhumidity": randint(0,100),
        "soiltemp": randint(-10,30),
        "soilmoisture": randint(0,30)
    }
MESSAGE = None
def on_message(client: Client, userdata, message: MQTTMessage): 
    global MESSAGE
    MESSAGE = message

def on_connect(client: Client, userdata, flags: dict, rc: int):
    client.subscribe(f'/{CLIENT_ID}/reset')
    print(rc)


client = Client("emulator")
client.on_message = on_message
client.on_connect = on_connect
client.connect("193.197.229.59")

while True: 
    data = create_random_data()
    print("Publish....")
    client.publish(f'/{CLIENT_ID}/plant_data',payload = json.dumps(data))
    client.loop()
    time.sleep(5)