#include <WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include "openPortal.h"

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1); // Optional: set a fixed IP for AP

void startCaptivePortal(AsyncWebServer &server, DNSServer &dnsServer) {
  WiFi.softAP("TrashBot-Setup");
  delay(100);  // Give some time for the AP to start

  dnsServer.start(DNS_PORT, "*", apIP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", openportal_html);
  });

  server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request) {
    String ssid = request->getParam("ssid", true)->value();
    String password = request->getParam("password", true)->value();

    // TODO: Store credentials
    request->send(200, "text/html", "<h1>Saved. Rebooting...</h1>");
    delay(1000);
    ESP.restart();
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->redirect("/");
  });

  server.begin();
}

void handleCaptivePortal(DNSServer &dnsServer) {
  dnsServer.processNextRequest();
}
