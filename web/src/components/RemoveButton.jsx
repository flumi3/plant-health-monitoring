import * as React from 'react';
import { Button } from "@mui/material";
import { useNavigate } from "react-router-dom";


export default function RemoveButton(props) {
    const navigate = useNavigate();

    const removeDevice = () => {
        console.log("removed device")
        const id = props.deviceId;
        const base = process.env.REACT_APP_API_SERVER_URL;
        const url = base + "/devices/" + id;
        fetch(url, {
            method: "DELETE"
        });
        navigate("/", {replace: true});
    }

    return (
        <Button className="remove-button" fullWidth variant="contained" color="error" onClick={() => removeDevice()}>
            Remove device
        </Button>
    )

}
