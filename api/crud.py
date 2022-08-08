from sqlalchemy.orm import Session

import schemas
import models


def create_device(db: Session, device: schemas.DeviceCreate) -> models.Device:
    """
    Create a new device record in the database.

    Parameters:
        db: Database session to use for creating the record
        device: Device data that shall be stored in the database

    Returns:
        The device record that was created in the database
    """
    device_record = models.Device(**device.dict())
    db.add(device_record)
    db.commit()
    db.refresh(device_record)
    return device_record


def get_devices(db: Session) -> list[schemas.Device]:
    """Queries all existing devices from the db."""
    return db.query(models.Device).all()


def get_plant_data_by_id(db: Session, device_id: int) -> list[schemas.PlantData]:
    """
    Get plant data for a specific device from the database.
    
    Parameters:
        db: Database session to use for the query
        device_id: Identifying hash of the device for which the data shall be queried
    """
    return db.query(models.PlantData).filter(models.Device.id == device_id).all()


def create_plant_data(db: Session, data: schemas.PlantDataCreate) -> models.PlantData:
    """
    Create a plant data record in the database.
    
    Parameters:
        db: Database session to use for creating the record
        data: Sensor data that shall be stored in the database

    Returns:
        The plant data record that was created in the database
    """
    # TODO: create and link with device record
    plant_data_record = models.PlantData(**data.dict())
    db.add(plant_data_record)
    db.commit()
    db.refresh(plant_data_record)
    return plant_data_record


def get_name_by_id(db:Session, device_id:int)->models.Device:
    return db.query(models.Device).filter(models.Device.id == device_id).first()

def get_by_critical_humidity(db: Session) -> list[int]:
    devices = get_devices(db)
    last_val = [(device.id,get_plant_data_by_id(db,device.id)[-1]) for device in devices]
    return [dev for dev,data in last_val if data.soil_humidity <= 30]