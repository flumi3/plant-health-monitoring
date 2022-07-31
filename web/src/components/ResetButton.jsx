import * as React from 'react';
import { Button } from "@mui/material";
import "./ResetButton.css";

export default function ResetButton(props) {

    const resetDevice = () => {
        console.log("reseted device")
        // TODO: send request to reset the device (with device id?)
        // props.deviceId
    }

    return (
        <Button className="reset-button" fullWidth variant="contained" onClick={() => resetDevice()}>
            Reset device
        </Button>
    )

}
