# Technische internetbasierte Systeme - Projekt zur Überwachung der Pflanzengesundheit
Axel Stark, MATRIKELNUMMER  
Sebastian Flum, 76855

## TODO:

1. Generelle Funktionsbeschreibung / Übersicht vom Projekt
2. Architektur erklären
3. Auf Komponenten eingehen
4. Benutzerhandbuch mit Bilder (swagger docs auch mit rein)
5. Getting started
  - Entwicklungsumgebung
  - Produktumgebung

<br>

# Projektbeschreibung

Ziel dieses Projekts ist die Überwachung der Gesundheit von Pflanzen durch
ein technisches, internetbasiertes System.

Das hierbei entwickelte System ermöglicht es dem Nutzer, einen mit Sensoren
bestückten Mikrokontroller bei der Pflanze zu platzieren, welcher wichtige
Informationen zum Pflanzenstatus abfrägt. Folgende Daten werden von dem
Mikrokontroller gesammelt und in einem Web-Dashboard angezeigt:

- Bodentemperatur
- Bodenfeuchte
- Lufttemperatur
- Luftfeuchte

Durch die Registrierung des Geräts in einer Web-Applikation, kann der Nutzer
dabei die aktuellen Messwerte auslesen und historische Messwerte in einem
Liniendiagramm anzeigen lassen.

Neben dem Abfragen der Daten durch die Web-Applikation hat der Nutzer
außerdem die Möglichkeit, die oben genannten Daten durch einen Alexa-Skill
abzufragen, wodurch er beispielsweise schnell und einfach entscheiden kann,
ob die Pflanze gegossen werden muss und ob die Pflanze sich in ihrer optimalen
Wachstumsumgebung befindet.

<br>

# Systemarchitektur

<br>

# Installation und Ausführung

<br>

# Nutzerhandbuch





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