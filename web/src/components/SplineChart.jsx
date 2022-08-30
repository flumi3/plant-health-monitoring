import {CanvasJSChart} from 'canvasjs-react-charts'

const SplineChart = (props) => {


    const options = {
        animationEnabled: true,
        title:{
            text: "Device Data History"
        },
        axisY: {
            title: "Humidity",
        },
        axisY2: {
            title: "Temperature",
        },
        toolTip:{
            shared: true
        },  
        data: [{
            name: 'Soil Humidity',
            yValueFormatString: "##,## %",
            xValueFormatString: "DD-MMM",
            type: "spline",
            showInLegend: true,
            dataPoints: props.data[0]
        },
        {
            name: 'Air Humidity',
            yValueFormatString: "##,## %",
            xValueFormatString: "DD-MMM",
            type: "spline",
            showInLegend: true,
            dataPoints: props.data[1]
        },
        {
            name: 'Air Temperature',
            axisYType: "secondary",
            yValueFormatString: "##,## °C",
            xValueFormatString: "DD-MMM",
            type: "spline",
            showInLegend: true,
            dataPoints: props.data[2]
        },
        {
            name: 'Soil Temperature',
            axisYType: "secondary",
            yValueFormatString: "##,## °C",
            xValueFormatString: "DD-MMM",
            type: "spline",
            showInLegend: true,
            dataPoints: props.data[3]
        },
    ]
    }
    return (
        <div className="device-data-chart">
			<CanvasJSChart options = {options}
				/* onRef={ref => this.chart = ref} */
			/>
			{/*You can get reference to the chart instance as shown above using onRef. This allows you to access all chart properties and methods*/}
		</div>
    );
}

export default SplineChart;