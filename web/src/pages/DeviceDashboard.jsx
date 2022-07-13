import React, { useEffect, useState } from "react";
import { useLocation } from "react-router-dom";
import MeasurementCard from "../components/MeasurementCard";
import TopBar from "../components/TopBar";
import "./DeviceDashboard.css";

export default function DeviceDashboard() {
    const [ plantData, setPlantData ] = useState([]);
    const location = useLocation();

    useEffect(() => {
        // TODO: query in a certain interval to keep the data up to date
        // TODO: fix bug where data does not get fetched
        const getData = async () => {
            const base = process.env.REACT_APP_API_SERVER_URL;
            const url = base + "/plant-data/" + location.state.deviceId;
            fetch(url)
                .then(response => response.json())
                .then(data => {
                    console.log(data);
                    setPlantData(data);
                });
        }
        getData();
    }, [location.state.deviceId]);

    return (
        <div>
            <TopBar />
            <div className="device-dashboard-data">
                <MeasurementCard className="air-temp" title="Air Temperature" value={plantData.air_temperature + "°C"} />
                <MeasurementCard className="air-humidity" title="Air Humidity" value={plantData.air_humidity} />
                <MeasurementCard className="soil-temp" title="Soil Temperature" value={plantData.soil_temperature + "°C"} />
                <MeasurementCard className="soil-humidity" title="Soil Humidity" value={plantData.soil_humidity} />
            </div>
        </div>
    );
} 
