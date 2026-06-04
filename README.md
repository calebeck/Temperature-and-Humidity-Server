ESP32-S3 Room Environment Monitor

A flexible environmental monitoring system built with an ESP32-S3 to track temperature and humidity using a DHT11 sensor. Supports two wireless communication methods: Wi-Fi (local hosting) and Bluetooth Low Energy (BLE) for remote data streaming.

## Features
* **Dual Connectivity Options:** Choose between Wi-Fi for local hosting or BLE for wireless data streaming
* **Live Updates:** Real-time temperature and humidity monitoring
* **Decimal Precision:** Formats readings to 2 decimal places for accurate data
* **Local Web Server:** Runs entirely on the ESP32-S3 over your local Wi-Fi network (LAN) using a raw HTML/JS literal
* **Remote Dashboard:** Stream BLE data to a hosted web dashboard via Bluetooth Low Energy

## Hardware Required
* ESP32-S3 Development Board
* DHT11 Temperature & Humidity Sensor (Module or bare sensor)
* Jumper wires
* 10k pull-up resistor (only required if using a bare 4-pin DHT11; most 3-pin modules have this built-in)

## Wiring Guide
| DHT11 Pin | ESP32-S3 Pin |
| :--- | :--- |
| VCC (+) | 3.3V |
| GND (-) | GND |
| DATA (Out) | GPIO 4 |

## Software & Dependencies
1. **Arduino IDE** (with the ESP32 board package installed)
2. **Adafruit DHT Sensor Library** (Install via Arduino Library Manager)
3. **Adafruit Unified Sensor Library** (Required dependency for the DHT library)

## Setup & Installation

### Option 1: Wi-Fi Method (Local Web Server)
1. Open `temp_humid_server.ino` in the Arduino IDE
2. Update the Wi-Fi credentials at the top of the sketch:
   ```cpp
   const char* ssid = "Your WiFi ssid";
   const char* password = "Your WiFi password";
   ```
3. Upload the sketch to your ESP32-S3
4. Open the Serial Monitor (115200 baud) to find the device's local IP address
5. Navigate to that IP address in your browser to view the live dashboard

### Option 2: Bluetooth Low Energy (BLE) Method
1. Open `temp_humid_server_Bluetooth.ino` in the Arduino IDE
2. Upload the sketch to your ESP32-S3
3. Access the remote dashboard at: **https://brilliant-mochi-19de18.netlify.app/**
4. Click the "Connect to ESP32" button on the dashboard
5. Select your ESP32-S3-Room-BLE device from the Bluetooth pairing dialog
6. View live temperature and humidity data streamed directly from your device

## Communication Methods

### Wi-Fi (Local Web Server)
- **File:** `temp_humid_server.ino`
- **UI:** `index.html` (embedded in the sketch)
- **Access:** Local network only via IP address
- **Update Rate:** 3 seconds
- **Advantages:** Direct control, no internet required, faster response

### Bluetooth Low Energy (BLE)
- **File:** `temp_humid_server_Bluetooth.ino`
- **UI:** `index.html` (standalone, hosted on Netlify)
- **Access:** Remote dashboard at https://brilliant-mochi-19de18.netlify.app/
- **Update Rate:** 3 seconds
- **Advantages:** Wireless range up to 100m, no Wi-Fi needed, remote access

## Project Structure
```
.
├── temp_humid_server.ino                 # Wi-Fi version with embedded web server
├── temp_humid_server_Bluetooth.ino       # BLE version for remote streaming
└── index.html                            # Standalone BLE dashboard (also hosted on Netlify)
```

## Notes
- Both sketches output diagnostic information to the Serial Monitor for troubleshooting
- The BLE method requires a modern browser with Web Bluetooth API support (Chrome, Edge, Opera, etc.)
- For the Wi-Fi method, ensure your ESP32-S3 and computer are on the same network
- Data readings are obtained with 2 decimal place precision for both Celsius and Fahrenheit
