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

- Bild zur Systemarchitektur überarbeiten (Fragezeichen entfernen, etc.)
- Beispielbild mit Mikrokontroller in Pflanze platziert
  


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

Für die Umsetzung des Projekts wurde folgende Systemarchitektur entworfen:

![fig:1](docs/system_architecture.png)

## IOT-Gerät

TODO: eventuell überarbeiten oder ausbessern mit deinem Wissen

Wie bereits erwähnt, ist das IOT-Gerät der bei der Pflanze anzubringende
Mikrokontroller bestehend aus einem ESP32 und einem ESP8266. Am
Mikrokontroller angebracht sind die zur Datensammlung benötigten Sensoren
für Bodenfeuchte und Bodentemperatur sowie Luftfeuchte und Lufttemperatur.
Zusätzlich am Gerät angebracht ist eine Batterie, da es in der Nähe der meisten
Pflanzen keine Möglichkeit zur Spannungsversorgung per Steckdose gibt.

Der Mikrokontroller sammelt die im vorherigen
Kapitel beschriebenen Daten und sendet diese mithilfe des
MQTT-Netzwerkprotokolls an einen zugehörigen MQTT-Broker.

![fig:2](docs/iot_device.png)

## MQTT-Broker

TODO: broker beschreibung mit deinem wissen ergänzen/ausbessern

Der MQTT-Broker dient dem Datenaustausch zwischen dem IOT-Gerät und dem
Backend. Er empfängt veröffentlichte Nachrichten (Sensordaten), filtert die
Nachrichten nach dem Topic und verteilt sie an Clients, welche die
jeweiligen Topics abonniert haben.

## MQTT-Client

Ein in Python entwickelter MQTT-Client fragt die Python API nach den
registrierten Geräten ab und abonniert die zugehörigen Topics beim MQTT-Broker,
wodurch der Client die vom IOT-Gerät gesammelten Daten erhält. Diese Daten
werden kontinuierlich abgefragt und anschließend an die Python API
weitergegeben.

## Python API

Eine Python API stellt die Funktionalität zur Verarbeitung und Verteilung
aller wichtiger Daten dar. Sobald es die vom IOT-Gerät gesammelten Daten
erhält, legt es diese in einer PostgreSQL Datenbank zur späteren Verwendung
ab. Folgende Endpunkte werden von der API bereitgestellt:

![fig:3](docs/api_swagger.png)

### GET /

Root endpoint zum Testen des Servers.

### GET /devices

Fragt alle registrierten Geräte in der Datenbank ab.

### POST /devices

Registriert ein neues Gerät in der Datenbank.

### DELETE /devices/{device_id}

Löscht das IOT-Gerät mit der übergebenen Geräte-ID aus der Datenbank.

### POST /devices/reset/{device_id}

Setzt das IOT-Gerät mit der übergebenen Geräte-ID auf Werkszustand zurück.

### GET /plant-data/{device_id}

Fragt alle gesammelten Daten zu einem bestimmten IOT-Gerät ab.

### POST /plant-data

Legt einen neuen Datensatz über die gesammelten Sensordaten in der Datenbank an.

### GET /firmwareVersion

Fragt die aktuelle Firmware-Version des IOT-Geräts ab.

## PostgreSQL Datenbank

Die PostgreSQL Datenbank dient zur Speicherung und Abfrage der erfassten
Sensordaten. Dafür wurde folgendes Datenbankmodell entworfen:

![fig:4](docs/db_model.png)

## React Web Applikation

Eine in React entwickelte Web-Applikation ermöglicht die Registrierung und
Löschung von IOT-Geräten sowie das Anzeigen der zugehörigen Daten. Um diese
Funktionalität bieten zu können, benutzt die Web-App die von der Python API
bereitgestellten Endpunkte.

## Alexa Skill

TODO: alexa skill beschreiben


<br>

# Installation und Ausführung

<br>

# Nutzerhandbuch

TODO: screenshots im produktivbetrieb machen  
- Hinzufügen von Gerät
- Geräteliste von echtem device
- Detailansicht mit daten von echtem device

TODO: bild von device wie es bei der pflanze angebracht wird oder so lol :D provisorisch würde ja reichen




# OLD STUFF


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