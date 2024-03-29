import schemas
import models
from typing import List
from sqlalchemy.orm import Session
from sqlalchemy import desc


def create_device(db: Session, device: schemas.DeviceCreate) -> models.Device:
    """
    Create a new device record in the database.

    Parameters:
        db: Database session to use for creating the record
        device: Device data that shall be stored in the database

    Returns:
        The device record that was created in the database
    """
    print("Creating new device entry in database...")
    device_record = models.Device(**device.dict())
    db.add(device_record)
    db.commit()
    db.refresh(device_record)
    print(device_record)
    return device_record


def get_devices(db: Session) -> List[models.Device]:
    """
    Queries all existing devices from the db.´

    Parameters:
        db: Database session to use for the query

    Returns:
        The list of all devices in the database
    """
    print("Querying all devices from database...")
    return db.query(models.Device).all()


def remove_device(db: Session, device_id: int) -> None:
    """
    Removes a device from the database.

    Parameters:
        db: Database session to use for the query
        device_id: Identifying hash of the device that shall be removed
    """
    print(f"Removing device with id {device_id} from database...")
    db.query(models.PlantData).filter(models.PlantData.device_id == device_id).delete()
    db.query(models.Device).filter(models.Device.id == device_id).delete()
    db.commit()


def get_plant_data_by_id(
    db: Session, device_id: int, limit: int
) -> List[models.PlantData]:
    """
    Get plant data for a specific device from the database.

    Parameters:
        db: Database session to use for the query
        device_id: Identifying hash of the device for which the data shall be queried

    Returns:
        The plant data records that were found for the given devcie id
    """
    print(f"Querying plant data for device with id {device_id} from database...")
    return (
        db.query(models.PlantData)
        .filter(models.PlantData.device_id == device_id)
        .order_by(desc(models.PlantData.timestamp))
        .limit(limit)
        .all()
    )


def create_plant_data(db: Session, data: schemas.PlantDataCreate) -> models.PlantData:
    """
    Create a plant data record in the database.

    Parameters:
        db: Database session to use for creating the record
        data: Sensor data that shall be stored in the database

    Returns:
        The plant data record that was created in the database
    """
    print("Storing new plant data in database...")
    plant_data_record = models.PlantData(**data.dict())
    db.add(plant_data_record)
    db.commit()
    db.refresh(plant_data_record)
    return plant_data_record
