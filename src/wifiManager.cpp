#include "wifiManager.h"
#include "fullRuntimeMode.h"
#include <esp_now.h>

uint8_t remoteMac[6] = {0};  // Set during pairing

void connectToWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
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
