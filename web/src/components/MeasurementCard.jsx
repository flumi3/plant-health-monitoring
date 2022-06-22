import * as React from 'react';
import Typography from '@mui/material/Typography';
import "./MeasurementCard.css";

const MeasurementCard = (props) => {
    return (
        <div className="measurement-card">
            <div className="card-title">
                <Typography variant="h5">
                    {props.title}
                </Typography>
            </div>
            <div className="card-value">
                <Typography variant="h3">
                    {props.value}
                </Typography>
            </div>
        </div>
    );
}

export default MeasurementCard;
