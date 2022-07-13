import "./App.css";
import { createTheme, ThemeProvider } from "@mui/material";
import { Routes, Route } from "react-router-dom";

import Devices from "./pages/Devices";
import DeviceDashboard from "./pages/DeviceDashboard";

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
      <Routes>
        <Route path="/" element={<Devices />} />
        <Route path="plant-data" element={<DeviceDashboard />} />
      </Routes>
    </ThemeProvider>
  );
}

export default App;
