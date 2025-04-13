#include "wifiManager.h"
#include "fullRuntimeMode.h"
#include <esp_now.h>
#include <ArduinoOTA.h>

const char* ssid = "homesweethome";
const char* password = "johnandamy";
const char* deviceHostname = "Water_Tank";

uint8_t remoteMac[6] = {0};  // Set during pairing

void connectToWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    WiFi.setHostname(deviceHostname);
    
    Serial.print("Connecting to WiFi");
    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 30) {
        delay(500);
        Serial.print(".");
        retry++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nFailed to connect to WiFi");
    }
}

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void connectToEspNow() {
    WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  // Set remote MAC address (replace with your actual controller MAC)
  uint8_t controllerMac[6] = {0x24, 0x6F, 0x28, 0xAB, 0xCD, 0xEF};  // <-- update this

  memcpy(remoteMac, controllerMac, 6);  // store for telemetry sending

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, controllerMac, 6);
  peerInfo.channel = 0;  // same channel as current WiFi
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add ESP-NOW peer");
  } else {
    Serial.println("Controller paired via ESP-NOW");
  }
}


void setupOTA() {
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  ArduinoOTA.begin();
}
