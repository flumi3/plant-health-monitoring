import * as React from 'react';
import Paper from '@mui/material/Paper';
import {
  Chart,
  ArgumentAxis,
  ValueAxis,
  LineSeries,
  Title,
  Legend,
} from '@devexpress/dx-react-chart-material-ui';
import { styled } from '@mui/material/styles';
import { Animation } from '@devexpress/dx-react-chart';

const format = () => tick => tick;
const PREFIX = 'Demo';
const classes = {
    chart: `${PREFIX}-chart`,
};

const StyledChart = styled(Chart)(() => ({
    [`&.${classes.chart}`]: {
      paddingRight: '20px',
    },
}));

const TitleText = props => (
    <Title.Text {...props} sx={{ whiteSpace: 'pre' }} />
);

const ValueLabel = (props) => {
    const { text } = props;
    return (
      <ValueAxis.Label
        {...props}
        text={`${text}%`}
      />
    );
};

const Root = props => (
    <Legend.Root {...props} sx={{ display: 'flex', margin: 'auto', flexDirection: 'row' }} />
);
const Label = props => (
    <Legend.Label sx={{ pt: 1, whiteSpace: 'nowrap' }} {...props} />
);
const Item = props => (
    <Legend.Item sx={{ flexDirection: 'column' }} {...props} />
);

const data = [
    {
        "device_id": 0,
        "air_temperature": 0,
        "air_humidity": 0,
        "soil_temperature": 0,
        "soil_humidity": 0,
        "timestamp": 0
    },
    {
        "device_id": 0,
        "air_temperature": 0,
        "air_humidity": 0,
        "soil_temperature": 0,
        "soil_humidity": 0,
        "timestamp": 0
    }
]

export default function LineChart(props) {

    return (
        <Paper>
            <StyledChart
                data={data}
                className={classes.chart}
            >
                <ArgumentAxis tickFormat={format} />
                <ValueAxis
                    max={100}
                    labelComponent={ValueLabel}
                />
                <LineSeries
                    name="Lufttemperatur"
                    valueField="air_temperature"
                    argumentField="timestamp"
                />
                <LineSeries
                    name="Bodentemperatur"
                    valueField="air_humidity"
                    argumentField="timestamp"
                />
                <Legend position="bottom" rootComponent={Root} itemComponent={Item} labelComponent={Label} />
                <Title
                    text={"Temperaturhistorie"}
                    textComponent={TitleText}
                />
                <Animation />
            </StyledChart>
        </Paper>
      );
}
