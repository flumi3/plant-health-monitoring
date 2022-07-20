import React, { useEffect, useState } from "react";
import { useLocation } from "react-router-dom";
import MeasurementCard from "../components/MeasurementCard";
import TopBar from "../components/TopBar";
import "./DeviceDashboard.css";

export default function DeviceDashboard() {
    const [plantData, setPlantData] = useState([
        {
            "device_id": null,
            "air_temperature": null,
            "air_humidity": null,
            "soil_temperature": null,
            "soil_humidity": null,
            "timestamp": null
        }
    ]);
    const location = useLocation();

    useEffect(() => {
        // TODO: query in a certain interval to keep the data up to date
        const getData = async () => {
            const base = process.env.REACT_APP_API_SERVER_URL;
            const url = base + "/plant-data/" + location.state.deviceId;
            fetch(url)
                .then(response => response.json())
                .then(data => setPlantData(data));
        }
        getData();
    }, [location.state.deviceId]);

    return (
        <div>
            <TopBar />
            <div className="device-dashboard-data">
                <MeasurementCard className="air-temp" title="Air Temperature" value={plantData[0].air_temperature + "°C"} />
                <MeasurementCard className="air-humidity" title="Air Humidity" value={plantData[0].air_humidity} />
                <MeasurementCard className="soil-temp" title="Soil Temperature" value={plantData[0].soil_temperature + "°C"} />
                <MeasurementCard className="soil-humidity" title="Soil Humidity" value={plantData[0].soil_humidity} />
            </div>
        </div>
    );
} 
