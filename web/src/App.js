import './App.css';
import './Models/plant_data'
const axios = require('axios').default;

const API_URL = 'http://localhost:8000'

const axios_instance = axios.create({
  baseURL: API_URL,
  headers: { 'Access-Control-Allow-Origin': '*' }
});

async function fetchData() {
  var response = await axios_instance.get('/plant-data');
  console.log(response.data)
}
fetchData();

async function App() {
  return (
    <div className="App">
      <header className="App-header">
        <p>
          hello World
        </p>
      </header>
    </div>
  );
}

export default App;
