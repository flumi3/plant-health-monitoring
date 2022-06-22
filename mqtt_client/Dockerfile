FROM python:3.10

ENV PYTHONPATH "${PYTHONPATH}:/mqtt_client"

WORKDIR /mqtt_client

COPY ./requirements.txt .
COPY mqtt_client.py .

RUN pip install --no-cache-dir --upgrade -r requirements.txt
