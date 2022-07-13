from pydantic import BaseModel


class DeviceBase(BaseModel):
    """Base schema of the device."""
    name: str
    device_hash: str


class DeviceCreate(DeviceBase):
    """Schema for creating a device record in the db."""
    pass


class Device(DeviceBase):
    """Schema for reading device records from the db."""
    id: int

    class Config:
        orm_mode = True


class PlantDataBase(BaseModel):
    """Base schema of the plant data."""
    device_id: int
    air_temperature: float
    air_humidity: float
    soil_temperature: float
    soil_humidity: float
    timestamp: int


class PlantDataCreate(PlantDataBase):
    """Schema for creating a device record in the db."""
    pass

 
class PlantData(PlantDataBase):
    """Schema for reading plant data records from the db."""
    id: int

    class Config:
        orm_mode = True
