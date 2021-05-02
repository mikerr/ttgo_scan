
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

int y,MAXy;

void printline (String message) {
  if (y == 0) tft.fillScreen(TFT_BLACK);
  tft.setCursor (0, y);
  tft.fillRect(0,y,128,9,TFT_BLACK);
  tft.print(message);
  y += 9;
  if (y > MAXy) y = 0;
}


void setup() {
int ROTATION=0;

  if (ROTATION == 0) MAXy = 240;
  else MAXy = 128;

  tft.init();
  tft.setRotation(ROTATION);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  
  pinMode(button, INPUT); 
  
  printline("Scanning...");
}

void scanwifi() {
  int num = WiFi.scanNetworks();
  printline("Found " + String(num) + " Wifi networks");
  for (int i = 0; i < min(num,13); i++)
  {
    printline(WiFi.SSID(i));
    tft.drawLine(128,y-6, 28 + abs(WiFi.RSSI(i)),y-6,TFT_GREEN);
  }
}

void scanble() {
bool device_found;
int scanTime = 5; //In seconds
BLEScan* pBLEScan;

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100); // set Scan interval
  pBLEScan->setWindow(99);  // less or equal setInterval value

  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);

  int num = foundDevices.getCount();
  
  printline("Found " + String(num) + " BLE devices");

  for (int i = 0; i < num; i++)
  {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    String name = device.getName().c_str();
    String address = device.getAddress().toString().c_str();
    int signal =  device.getRSSI();
    
    tft.setTextColor(TFT_WHITE);
    if (name != "") printline (name);
    else printline(address);
    tft.drawLine(128,y-6, 28 + abs(signal),y-6,TFT_GREEN);
   }
   pBLEScan->clearResults(); 
}
void loop() {
   y = 0;
   scanwifi();
   printline("");
   scanble();
   delay (30000);
  }
