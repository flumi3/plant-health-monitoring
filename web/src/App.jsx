import "./App.css";
import { createTheme, ThemeProvider } from "@mui/material";
import TopBar from "./components/TopBar";
import Devices from "./pages/Devices";

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
      <Devices />
    </ThemeProvider>
  );
}

export default App;
