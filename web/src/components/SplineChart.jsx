import {CanvasJSChart} from 'canvasjs-react-charts'

const SplineChart = (props) => {


    const options = {
        animationEnabled: true,
        title:{
            text: "Historie"
        },
        axisY: {
            title: "Feuchte",
        },
        axisY2: {
            title: "Temperatur",
        },
        toolTip:{
            shared: true
        },  
        data: [{
            name: 'Bodenfeuchte',
            yValueFormatString: "##,## %",
            xValueFormatString: "DD-MMM",
            type: "spline",
            showInLegend: true,
            dataPoints: props.data[0]
        },
        {
            name: 'Luftfeuchte',
            yValueFormatString: "##,## %",
            xValueFormatString: "DD-MMM",
            type: "spline",
            showInLegend: true,
            dataPoints: props.data[1]
        },
        {
            name: 'Lufttemeratur',
            axisYType: "secondary",
            yValueFormatString: "##,## °C",
            xValueFormatString: "DD-MMM",
            type: "spline",
            showInLegend: true,
            dataPoints: props.data[2]
        },
        {
            name: 'Bodentemeratur',
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
        <div>
			<CanvasJSChart options = {options}
				/* onRef={ref => this.chart = ref} */
			/>
			{/*You can get reference to the chart instance as shown above using onRef. This allows you to access all chart properties and methods*/}
		</div>
    );
}

export default SplineChart;