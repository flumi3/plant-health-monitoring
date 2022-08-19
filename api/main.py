import json
from fastapi import FastAPI, Depends
from fastapi.staticfiles import StaticFiles
from fastapi.middleware.cors import CORSMiddleware
from sqlalchemy.orm import Session

import models
import crud
import schemas
from database import engine, get_db

from paho.mqtt.client import Client, MQTTMessage


app = FastAPI()
models.Base.metadata.create_all(bind=engine)

origins = [
    "http://localhost:3000",
    "http://193.197.229.59:3000"
]   
app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,  # origins permitted to make cross-origin requests
    allow_methods=["*"],  # allowed http methods
    allow_headers=["*"],  # allowed http headers
)


@app.get("/")
async def read_root():
    return "Running on port 8000"


@app.get("/devices", response_model=list[schemas.Device])
async def read_devices(db: Session = Depends(get_db)):
    return crud.get_devices(db)


@app.post("/devices")
async def create_device(device: schemas.DeviceCreate, db: Session = Depends(get_db)):
    crud.create_device(db, device)

@app.post("/remove_device/{device_id}")
async def remove_device(device_id, db: Session = Depends(get_db)):
    crud.remove_device(db,device_id)

@app.post("/reset_device/{device_id}")
async def remove_device(device_id,db: Session = Depends(get_db)):
    client = Client("emulator")
    client.connect("193.197.229.59")
    devices = await read_devices(db)
    topic = None
    for device in devices: 
        if device.id == int(device_id):
            topic = device.device_hash
    client.publish(f'/{topic}/reset',"reset")

@app.get("/plant-data/{device_id}", response_model=list[schemas.PlantData])
async def read_plant_data(device_id, db: Session = Depends(get_db), limit: int = 10):
    return crud.get_plant_data_by_id(db, device_id, limit)


@app.post("/plant-data")
async def create_plant_data(data: schemas.PlantDataCreate, db: Session = Depends(get_db)):
    crud.create_plant_data(db, data)



@app.get("/firmwareVersion")
async def check_version_number():
    with open("firmware/version.json") as file:
        data = json.load(file)
        return data['current_version']


app.mount("/firmware", StaticFiles(directory="firmware"), name="firmware")
