import * as React from 'react';
import { useState } from "react";
import TextField from '@mui/material/TextField';
import { Button } from "@mui/material";
import Grid from "@mui/material/Grid";

export default function DeviceForm() {
    const [deviceName, setDeviceName] = useState("");
    const [deviceId, setDeviceId] = useState("");

    const onSubmit = () => {
        if (deviceName === "" && deviceId === ""){
            return;
        }
        const base = process.env.REACT_APP_API_SERVER_URL;
        const url = base + "/devices"
        const payload = {
            "name": deviceName,
            "device_hash": deviceId
        }
        fetch(url, {
            method: "POST",
            body: JSON.stringify(payload),
            headers: {
                "Content-Type": "application/json"
            }
        }).then(
            ()=>window.location.reload(false)
        )

    }

    return (
        <div>
            <Grid container spacing={1} columnSpacing={{ xs: 1, sm: 2, md: 3 }} >
                <Grid item sm={6}>
                    <TextField
                        fullWidth
                        required
                        id="first-name"
                        label="Device name"
                        variant="outlined"
                        value={deviceName}
                        onChange={event => setDeviceName(event.target.value)}
                    />
                </Grid>
                <Grid item sm={6}>
                    <TextField
                        fullWidth
                        required
                        id="first-name"
                        label="Device ID"
                        variant="outlined"
                        value={deviceId}
                        onChange={event => setDeviceId(event.target.value)}
                    />
                </Grid>
                <Grid item sm={12}>
                    <Button fullWidth type="submit" variant="contained" onClick={() => onSubmit()}>
                        Add new device
                    </Button>
                </Grid>
            </Grid>
        </div>
    );
}
