import React, { useEffect, useState } from "react";
import { Typography } from "@mui/material";
import Table from '@mui/material/Table';
import TableBody from '@mui/material/TableBody';
import TableCell from '@mui/material/TableCell';
import TableContainer from '@mui/material/TableContainer';
import TableHead from '@mui/material/TableHead';
import TableRow from '@mui/material/TableRow';
import { useNavigate } from "react-router-dom";
import TopBar from "../components/TopBar";
import "./Devices.css"

export default function Devices() {
    const [ devices, setDevices ] = useState([]);
    const navigate = useNavigate();

    useEffect(() => {
        // get devices from backend
        const getDevices = async () => {
            const base = process.env.REACT_APP_API_SERVER_URL;
            const url = base + "/devices";
            fetch(url)
                .then(response => response.json())
                .then(data => setDevices(data));
        }
        getDevices();
    }, []);

    return (
        // TODO: Add submit form to add new devices
        <div>
            <TopBar />
            <Typography className="devices-header" variant="h4" fontWeight="bold">
                Devices
            </Typography>
            <div className="devices-content">
                <TableContainer>
                    <Table>
                        <TableHead>
                            <TableRow>
                                <TableCell>
                                    <Typography fontWeight="bold">
                                        Device Name
                                    </Typography>
                                </TableCell>
                                <TableCell align="right">
                                    <Typography fontWeight="bold">
                                        Device ID
                                    </Typography>
                                </TableCell>
                            </TableRow>
                        </TableHead>
                        <TableBody>
                            {devices.map((device) => (
                                <TableRow
                                    key={device.name}
                                    sx={{ '&:last-child td, &:last-child th': { border: 0 } }}
                                    hover
                                    onClick={() => navigate("/plant-data", {state: {deviceId: device.id}})}
                                >
                                    <TableCell component="th" scope="row">
                                        {device.name}
                                    </TableCell>
                                    <TableCell align="right">{device.device_hash}</TableCell>
                                </TableRow>
                            ))}
                        </TableBody>
                    </Table>
                </TableContainer>
            </div>
        </div>
    );
};
