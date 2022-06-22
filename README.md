# tibs

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

