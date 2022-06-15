from sqlalchemy.orm import Session

from models import PlantData
from schemas import PlantDataCreate, PlantDataRead


def get_plant_data(db: Session):
    return db.query(PlantData).all()


def create_plant_data(db: Session, data: PlantDataCreate):
    db_record = PlantData(**data.dict())
    db.add(db_record)
    db.commit()
    db.refresh(db_record)
    return db_record
