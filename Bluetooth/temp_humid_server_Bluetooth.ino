#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h> // Needed for notifications
#include "DHT.h"

// --- Sensor Setup ---
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// --- BLE Global Variables ---
BLEServer* pServer          = NULL;
BLECharacteristic* pTempChar  = NULL;
BLECharacteristic* pTempFChar = NULL; // New Characteristic for Fahrenheit
BLECharacteristic* pHumidChar = NULL;
bool deviceConnected        = false;
bool oldDeviceConnected     = false;

// --- Custom UUIDs ---
#define SERVICE_UUID        "19b10000-e8f2-537e-4f6c-d104768a1214"
#define TEMP_CHAR_UUID      "19b10001-e8f2-537e-4f6c-d104768a1214"
#define HUMID_CHAR_UUID     "19b10002-e8f2-537e-4f6c-d104768a1214"
#define TEMPF_CHAR_UUID     "19b10003-e8f2-537e-4f6c-d104768a1214" // New UUID for Fahrenheit

// --- Connection Callbacks ---
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Device connected!");
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Device disconnected.");
    }
};

void setup() {
  Serial.begin(115200);
  dht.begin();

  // 1. Create the BLE Device
  BLEDevice::init("ESP32-S3-Room-BLE"); // Fixed name to match your HTML

  // 2. Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // 3. Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // 4. Create the Celsius Temperature Characteristic
  pTempChar = pService->createCharacteristic(
                TEMP_CHAR_UUID,
                BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
              );
  pTempChar->addDescriptor(new BLE2902()); 

  // 5. Create the Fahrenheit Temperature Characteristic
  pTempFChar = pService->createCharacteristic(
                TEMPF_CHAR_UUID,
                BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
              );
  pTempFChar->addDescriptor(new BLE2902());

  // 6. Create the Humidity Characteristic
  pHumidChar = pService->createCharacteristic(
                HUMID_CHAR_UUID,
                BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
              );
  pHumidChar->addDescriptor(new BLE2902());

  // 7. Start the service and start broadcasting
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
  
  Serial.println("Waiting for a client connection to notify...");
}

void loop() {
  // If a computer is connected, read the sensor and send the data
  if (deviceConnected) {
    float t = dht.readTemperature();
    float f = dht.readTemperature(true); // Read Fahrenheit
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(f) && !isnan(h)) {
      // Convert the numbers to standard strings
      String tempString  = String(t, 2);
      String tempFString = String(f, 2);
      String humidString = String(h, 2);

      // Symmetrically push the notifications to the client
      pTempChar->setValue(tempString.c_str());
      pTempChar->notify();

      pTempFChar->setValue(tempFString.c_str());
      pTempFChar->notify();

      pHumidChar->setValue(humidString.c_str());
      pHumidChar->notify();

      Serial.print("Notified -> Temp (C): "); Serial.print(tempString);
      Serial.print(" | Temp (F): "); Serial.print(tempFString);
      Serial.print(" | Humid: "); Serial.println(humidString);
    }
    delay(3000); 
  }

  // Auto-restart advertising if the computer disconnects
  if (!deviceConnected && oldDeviceConnected) {
      delay(500);
      pServer->startAdvertising(); 
      Serial.println("Restarting advertising...");
      oldDeviceConnected = deviceConnected;
  }
  
  // Track new connections
  if (deviceConnected && !oldDeviceConnected) {
      oldDeviceConnected = deviceConnected;
  }
}