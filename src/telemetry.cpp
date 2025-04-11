#include "telemetry.h"
#include <esp_now.h>
#include <WiFi.h>

// Replace with actual MAC address of the remote
uint8_t remoteAddress[] = {0x24, 0x6F, 0x28, 0xAB, 0xCD, 0xEF};

void initTelemetry() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, remoteAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (!esp_now_add_peer(&peerInfo)) {
    Serial.println("Peer added successfully");
  }
}

void sendTelemetry(const TelemetryPacket &packet) {
  esp_now_send(remoteAddress, (uint8_t *)&packet, sizeof(packet));
  
}

void sendTelemetryPacket(const String& message) {
  esp_now_send(remoteAddress, (uint8_t *)message.c_str(), message.length());
}
