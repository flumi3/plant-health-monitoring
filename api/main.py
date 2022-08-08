import json
from fastapi import FastAPI, Depends
from fastapi.staticfiles import StaticFiles
from fastapi.middleware.cors import CORSMiddleware
from sqlalchemy.orm import Session

import models
import crud
import schemas
from database import engine, get_db

app = FastAPI()
models.Base.metadata.create_all(bind=engine)

origins = [
    "http://localhost:3000"
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

@app.get("/get_device_name/{device_id}",response_model=schemas.Device)
async def read_device_name(device_id, db: Session = Depends(get_db)):
    return crud.get_name_by_id(db,device_id)

@app.get("/plant-data/{device_id}", response_model=list[schemas.PlantData])
async def read_plant_data(device_id, db: Session = Depends(get_db)):
    return crud.get_plant_data_by_id(db, device_id)

@app.get("/get_critical_plants", response_model=list[int])
async def read_critical_plant(db: Session = Depends(get_db)):
    return crud.get_by_critical_humidity(db)

@app.post("/plant-data")
async def create_plant_data(data: schemas.PlantDataCreate, db: Session = Depends(get_db)):
    crud.create_plant_data(db, data)


@app.get("/firmwareVersion")
async def check_version_number():
    with open("firmware/version.json") as file:
        data = json.load(file)
        return data['current_version']


app.mount("/firmware", StaticFiles(directory="firmware"), name="firmware")
