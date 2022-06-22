import './App.css';
import { createTheme, ThemeProvider } from "@mui/material";
import TopBar from './components/TopBar';
import MeasurementCard from './components/MeasurementCard';

const App = () => {

  const theme = createTheme({
    palette: {
        mode: "light",
        background: {
            default: "#eeeeee",
        },
    },
    typography: {
        fontFamily: [
            "Inter",
            "-apple-system",
            "BlinkMacSystemFont",
            "'Segoe UI'",
            "Helvetica",
            "Arial",
            "sans-serif",
            "'Apple Color Emoji'",
            "'Segoe UI Emoji'",
        ].join(","),
    }
  });

  return (
    <ThemeProvider theme={theme}>
      <TopBar />
      <div className="container">
        <MeasurementCard className="air-temp" title="Air Temperature" value="23°" />
        <MeasurementCard className="air-humidity" title="Air Humidity" value="0" />
        <MeasurementCard className="soil-temp" title="Soil Temperature" value="15°" />
        <MeasurementCard className="soil-humidity" title="Soil Humidity" value="0" />
      </div>
    </ThemeProvider>
  );
}

// const axios = require('axios').default;

// const API_URL = 'http://localhost:8000'

// const axios_instance = axios.create({
//   baseURL: API_URL,
//   headers: { 'Access-Control-Allow-Origin': '*' }
// });

// async function fetchData() {
//   var response = await axios_instance.get('/plant-data');
//   console.log(response.data)
// }
// fetchData();

// async function App() {
//   return (
//     <div className="App">
//       <header className="App-header">
//         <p>
//           hello World
//         </p>
//       </header>
//     </div>
//   );
// }

export default App;
