from fastapi import FastApi

app = FastApi()

@app.get("/")
async def read_root():
    return "Running on port 8000"
