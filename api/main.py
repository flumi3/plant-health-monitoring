from http import HTTPStatus
from fastapi import FastAPI, Depends

from sqlalchemy.orm import Session

import models
import crud
import schemas
from database import SessionLocal, engine, get_db

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
