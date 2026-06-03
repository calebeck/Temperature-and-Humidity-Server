#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

// --- Wi-Fi Credentials ---
const char* ssid = "Your WiFi ssid";
const char* password = "Your WiFi password";

// --- Sensor Setup ---
#define DHTPIN 4       // The GPIO pin connected to the DHT11 data pin
#define DHTTYPE DHT11  // Defining the sensor type
DHT dht(DHTPIN, DHTTYPE);

// --- Web Server Setup ---
WebServer server(80); // Start server on standard HTTP port 80

// --- HTML Page (Raw String Literal) ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32-S3 Room Monitor</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; background-color: #f4f4f9; color: #333; }
    h1 { color: #0056b3; }
    .card { background: white; padding: 20px; border-radius: 10px; box-shadow: 0px 4px 8px rgba(0,0,0,0.1); display: inline-block; margin: 10px; width: 200px; }
    .value { font-size: 2.5rem; font-weight: bold; margin: 10px 0; }
    .label { font-size: 1.2rem; color: #666; }
  </style>
</head>
<body>
  <h1>Current Environment</h1>
  <div class="card">
    <div class="label">Temperature</div>
    <div class="value"><span id="temp">--</span> &deg;C</div>
  </div>
  <div class="card">
    <div class="label">Humidity</div>
    <div class="value"><span id="hum">--</span> %</div>
  </div>
  <div class="card">
    <div class="label">Temp (F)</div>
    <div class="value"><span id="tempF">--</span> &deg;F</div>
  </div>

  <script>
    // This function fetches new data from the /data endpoint every 5 seconds
    setInterval(function ( ) {
      fetch('/data')
        .then(response => response.json())
        .then(data => {
          document.getElementById('temp').innerText = data.temperature;
          document.getElementById('hum').innerText = data.humidity;
          document.getElementById('tempF').innerText = data.temperatureF;
        })
        .catch(error => console.error('Error fetching data:', error));
    }, 3000); 
  </script>
</body>
</html>)rawliteral";

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", []() {
    server.send(200, "text/html", index_html);
  });

  // Route to send JSON data to the JavaScript fetch request
  server.on("/data", []() {
    float t = dht.readTemperature(); // Read temperature as Celsius
    float h = dht.readHumidity();    // Read humidity
    float f = dht.readTemperature(true); // Read Fahrenheit

    // Check if any reads failed and return early
    if (isnan(t) || isnan(h)) {
      server.send(500, "application/json", "{\"error\":\"Failed to read sensor\"}");
      return;
    }

    // Format the data as JSON
    String jsonStr = "{\"temperature\":\"" + String(t, 2) + "\", \"temperatureF\":\"" + String(f, 2) + "\", \"humidity\":\"" + String(h, 2) + "\"}";
    server.send(200, "application/json", jsonStr);
  });

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Listen for incoming clients
  server.handleClient();
  delay(2); // Small delay to allow the CPU to handle background Wi-Fi tasks
}
