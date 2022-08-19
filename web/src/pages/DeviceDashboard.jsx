import React, { useEffect, useState } from "react";
import { useLocation } from "react-router-dom";
import MeasurementCard from "../components/MeasurementCard";
import TopBar from "../components/TopBar";
import ResetButton from "../components/ResetButton";
import RemoveButton from "../components/RemoveButton";
import "./DeviceDashboard.css";
import SplineChart from "../components/SplineChart";
import {ButtonGroup} from "@mui/material";

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
        const id = setInterval(() => {
            const base = process.env.REACT_APP_API_SERVER_URL;
            const url = base + "/plant-data/" + location.state.deviceId;

            fetch(url)
                .then(response => response.json())
                .then(data => setPlantData(data));
        }, 5000);
        return () => clearInterval(id);
    }, [location.state.deviceId]);

    return (
        <div>
            <TopBar />
            <div className="device-dashboard-data">
                <MeasurementCard className="air-temp" title="Air Temperature" value={plantData.length === 0 ? "Nan" : plantData[0].air_temperature + "°C"} />
                <MeasurementCard className="air-humidity" title="Air Humidity" value={plantData.length === 0 ? "Nan" : plantData[0].air_humidity + "%"} />
                <MeasurementCard className="soil-temp" title="Soil Temperature" value={plantData.length === 0 ? "Nan" : plantData[0].soil_temperature + "°C"} />
                <MeasurementCard className="soil-humidity" title="Soil Humidity" value={plantData.length === 0 ? "Nan" : plantData[0].soil_humidity + "%"} />
            </div>
            {/* TODO: pass correct identifying information that is necessary for resetting the device */}
            <div className = "device-dashboard-data">
            <ButtonGroup variant="contained" aria-label="outlined primary button group" >
                <ResetButton deviceId={location.state.deviceId} />
                <RemoveButton deviceId={location.state.deviceId} />
            </ButtonGroup>
            </div>
            <SplineChart data={[
                plantData.map((data) => {
                    return { x: new Date(data.timestamp * 1000), y: data.soil_humidity };
                }),
                plantData.map((data) => {
                    return { x: new Date(data.timestamp * 1000), y: data.air_humidity };
                }),
                plantData.map((data) => {
                    return { x: new Date(data.timestamp * 1000), y: data.air_temperature };
                }),
                plantData.map((data) => {
                    return { x: new Date(data.timestamp * 1000), y: data.soil_temperature };
                }),
            ]
            } />
        </div>
    );
} 
