ESP32-S3 Room Environment Monitor

A simple, asynchronous web server built with an ESP32-S3 to monitor temperature and humidity using a DHT11 sensor. (Originally configured to monitor my room in Prague!).

## Features
* **Live Updates:** The web interface automatically fetches new data in the background every 3 seconds without refreshing the page.
* **Decimal Precision:** Formats temperature and humidity readings to 2 decimal places.
* **Local Hosting:** Runs entirely on the ESP32-S3 over your local Wi-Fi network (LAN) using a raw HTML/JS literal.

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
1. Open the `.ino` file in the Arduino IDE.
2. Update the Wi-Fi credentials at the top of the sketch:
