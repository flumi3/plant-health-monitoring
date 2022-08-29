from distutils.debug import DEBUG
from lib2to3.pytree import Base
import os
from pydantic import BaseSettings


class Settings(BaseSettings):

    # Database settings
    DB_USER = os.getenv("DB_USER")
    assert DB_USER
    DB_PASSWORD = os.getenv("DB_PASSWORD")
    assert DB_PASSWORD
    DB_HOST = os.getenv("DB_HOST")
    assert DB_HOST
    DB_NAME = os.getenv("DB_NAME")
    assert DB_NAME
    DATABASE_URL = f"postgresql://{DB_USER}:{DB_PASSWORD}@{DB_HOST}/{DB_NAME}"

    # Frontend settings
    WEB_SERVER_URL = os.getenv("WEB_SERVER_URL")
    assert WEB_SERVER_URL

    # MQTT settings
    BROKER_IP_ADDRESS = os.getenv("BROKER_IP_ADDRESS")
    assert BROKER_IP_ADDRESS


settings = Settings()
