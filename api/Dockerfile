FROM python:3.10

ENV PYTHONPATH "${PYTHONPATH}:/api"

WORKDIR /api

COPY ./requirements.txt .
COPY *.py /api/

RUN pip install --no-cache-dir --upgrade -r requirements.txt
