import json
from typing import List
from fastapi import FastAPI, Depends
from fastapi.staticfiles import StaticFiles
from fastapi.middleware.cors import CORSMiddleware
from sqlalchemy.orm import Session
from paho.mqtt.client import Client

import models
import crud
import schemas
from settings import settings
from database import engine, get_db

app = FastAPI()
models.Base.metadata.create_all(bind=engine)

origins = ["http://localhost", "http://localhost:3000", "http://193.197.229.59:3000"]
app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,  # origins permitted to make cross-origin requests
    allow_methods=["*"],  # allowed http methods
    allow_headers=["*"],  # allowed http headers
)


@app.get("/")
async def read_root():
    """Test route."""
    return "Running on port 8000"


@app.get("/devices", response_model=List[schemas.Device])
async def read_devices(db: Session = Depends(get_db)):
    """Get all devices."""
    return crud.get_devices(db)


@app.post("/devices", response_model=schemas.Device)
async def create_device(device: schemas.DeviceCreate, db: Session = Depends(get_db)):
    """Create a new device."""
    return crud.create_device(db, device)


@app.delete("/devices/{device_id}")
async def remove_device(device_id, db: Session = Depends(get_db)):
    """Remove a device."""
    crud.remove_device(db, device_id)


@app.post("/devices/reset/{device_id}")
async def reset_device(device_id, db: Session = Depends(get_db)):
    """Reset a device."""
    client = Client("emulator")
    client.connect(settings.BROKER_IP_ADDRESS)
    devices = await read_devices(db)
    topic = None
    for device in devices:
        if device.id == int(device_id):
            topic = device.device_hash
    client.publish(f"/{topic}/reset", "reset")


@app.get("/plant-data/{device_id}", response_model=List[schemas.PlantData])
async def read_plant_data(device_id, db: Session = Depends(get_db), limit: int = 10):
    """Get plant data for a given device id."""
    return crud.get_plant_data_by_id(db, device_id, limit)


@app.post("/plant-data")
async def create_plant_data(
    data: schemas.PlantDataCreate, db: Session = Depends(get_db)
):
    """Create a new plant data entry."""
    crud.create_plant_data(db, data)


@app.get("/firmwareVersion")
async def check_version_number():
    """Check the version number of the firmware."""
    with open("firmware/version.json") as file:
        data = json.load(file)
        return data["current_version"]


app.mount("/firmware", StaticFiles(directory="firmware"), name="firmware")
