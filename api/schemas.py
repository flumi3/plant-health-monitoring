from datetime import datetime
from pydantic import BaseModel

class PlantDataBase(BaseModel):
    air_temperature: float
    air_humidity: float
    soil_temperature: float
    soil_humitidy: float


class PlantDataCreate(PlantDataBase):
    """Schema for posting a new record into the db."""
    timestamp: int

 
class PlantDataRead(PlantDataBase):
    """Schema for reading a record from the db."""
    id: int
    timestamp: int

    class Config:
        orm_mode = True
