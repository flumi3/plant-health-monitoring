import json
from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles

app = FastAPI()

@app.get("/")
async def read_root():
    return "Running on port 8000"


@app.get("/firmwareVersion")
async def check_version_number():
    with open("firmware/version.json") as file:
        data = json.load(file)
        return data['current_version']

app.mount("/firmware",StaticFiles(directory="firmware"), name="firmware")