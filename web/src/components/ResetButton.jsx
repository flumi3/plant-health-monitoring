import * as React from 'react';
import { Button } from "@mui/material";
import "./ResetButton.css";

export default function ResetButton(props) {

    const resetDevice = () => {
        console.log("reseted device")
        const id = props.deviceId;
        const base = process.env.REACT_APP_API_SERVER_URL;
        const url = base + "/devices/reset/" + id;
        fetch(url, {
            method: "POST",
        });
    }

    return (
        <Button className="reset-button" fullWidth variant="contained" onClick={() => resetDevice()}>
            Reset device
        </Button>
    )

}
