from datetime import datetime
from sqlalchemy import Boolean, Column, Integer, Float
from sqlalchemy.orm import relationship

from database import Base

class PlantData(Base):
    __tablename__ = "Plant Data"

    id = Column(Integer, primary_key=True, index=True)
    air_temperature = Column(Float, nullable=False)
    air_humidity = Column(Float)
    soil_temperature = Column(Float, nullable=False)
    soil_humitidy = Column(Float, nullable=False)
    timestamp = Column(Integer, nullable=False)
