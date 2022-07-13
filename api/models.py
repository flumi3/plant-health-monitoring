from sqlalchemy import Column, ForeignKey, Integer, Float, String
from sqlalchemy.orm import relationship, backref

from database import Base


class Device(Base):
    __tablename__ = "device"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String, nullable=False, unique=True)
    device_hash = Column(String, nullable=False, unique=True)


class PlantData(Base):
    __tablename__ = "plant_data"

    id = Column(Integer, primary_key=True, index=True)
    air_temperature = Column(Float, nullable=False)
    air_humidity = Column(Float)
    soil_temperature = Column(Float, nullable=False)
    soil_humidity = Column(Float, nullable=False)
    timestamp = Column(Integer, nullable=False)
    
    # 1 to 1 relationship with device
    device_id = Column(Integer, ForeignKey("device.id"))
    device = relationship("Device", backref=backref("plant_data", uselist=False))
