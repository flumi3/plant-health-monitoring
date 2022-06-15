import json
from fastapi import FastAPI, Depends
from fastapi.staticfiles import StaticFiles
from sqlalchemy.orm import Session

import models
import crud
import schemas
from database import engine, get_db

app = FastAPI()
models.Base.metadata.create_all(bind=engine)


@app.get("/")
async def read_root():
    return "Running on port 8000"


@app.get("/plant-data", response_model=list[schemas.PlantDataRead])
async def read_plant_data(db: Session = Depends(get_db)):
    db_records = crud.get_plant_data(db)
    return db_records


@app.post("/plant-data", response_model=schemas.PlantDataRead)
async def create_plant_data(data: schemas.PlantDataCreate, db: Session = Depends(get_db)):
    db_record = crud.create_plant_data(db, data)
    return db_record


@app.get("/firmwareVersion")
async def check_version_number():
    with open("firmware/version.json") as file:
        data = json.load(file)
        return data['current_version']


app.mount("/firmware", StaticFiles(directory="firmware"), name="firmware")
