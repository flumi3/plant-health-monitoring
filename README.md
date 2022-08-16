# tibs

## TODO:

## Getting started

### Building the app
```shell
docker compose -f docker-compose.dev.yaml build  # dev
docker compose build  # prod
```

### Starting the app
```shell
docker compose -f docker-compose.dev.yaml up  # dev
docker compose up  # prod
```

### Backend
- Add device table to db:
  - Attributes: Name, Device ID und irgend einen primary key für Mapping mit
    den Daten
  - Endpoint für Hinzufügen von neuem Gerät

### Frontend
- Drawer mit 2 Drawern die einen zu den folgenden Seiten führen:
  - Geräteübersicht wo man Geräte hinzufügen und auswählen kann (Startseite)
  - Dashboard eines Geräts wo man die jeweiligen Daten sieht

### Alexa
- Find out how development works

### Docker compose
- create docker compose for prod and mostly since we need to set different environment variables in prod
  (backend url, database connection, etc...)


## Pflanzen Überwachung

### Alexa

- Errinerung zum  gießen
  - Statusleuchte von Alexa
- Gesundheitsstatus abfragen
  - Bodenfeuchte
  - Bodentemperatur
  - Lufttemperatur

### Web-UI

- Daten anzeigen im zeitlichen verlauf (Dashboard)
- Aktuelle Werte Anzeigen
- Gießhistorie

### IOT-Gerät

- Gießen registrieren (Wenn Bodenfeuchte schnell steigt)
- Werte erfassen (15 min Takt)
  - Lufttemp.
  - Bodentemp.
  - Bodenfeuchte
- MQTT an DB versenden
- OTA-Update
  - Wöchentlich/Täglich fragen ob neue Firmware version vorhanden, gegebenenfalls updaten
- WIFI-Config
  - Hotspot aufmachen -> mit Hotspot verbinden -> WIFI-Cridentials eingeben

### Datenbank

- SQL

### HTTP-Server

- Python
  - FAST-API
  - SQL-Alchemy

### MQTT Broker

- Self-Hosted oder Bantel-Server

### Update-Server

- evtl. einfacher File-Server

### Backend allgemein

- Microservices in Docker-Containern

## Architektur
![alt text](./docs/Architecture.png)

https://www.dlr.rlp.de/Internet/global/themen.nsf/b81d6f06b181d7e7c1256e920051ac19/6449e62b480fcb10c1257d5f0034e7b5?OpenDocument


## MQTT 
User: plantData
Pass: plantdatatibs