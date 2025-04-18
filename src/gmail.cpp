 //ADC_MODE(ADC_VCC);
 /*
const char* ssid = "homesweethome";
const char* password = "johnandamy";

const char* smtpServer = "smtp.gmail.com";
const int smtpPort = 465;

// Replace these with your info
const char* emailSender = "john.jensen.2@gmail.com";
const char* appPassword = ""; // 16-digit App Password from Google no spaces
const char* emailRecipient = "john.jensen.2@gmail.com";
const char* deviceHostname = "esp-beacon";

WiFiClientSecure client;
ESP8266WebServer server(80);

      float lat = 30.709600;
      float lng = 101.399002;

float readBatteryVoltage() {
  float volts = ESP.getVcc();
  //return analogRead(A0) * (4.2 / 1023.0); // Adjust scaling if needed
  return volts/1000;
}  
void startCaptivePortal() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Beacon-Recovery");

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", []() {
    String html = R"(
      <html>
      <head><title>Beacon Recovery Mode</title></head>
      <body style='text-align:center;font-family:sans-serif;'>
        <h2>Beacon Recovery Mode</h2>
        <p>No Wi-Fi found. Connect manually to recover or trigger actions.</p>
        <p><a href='/sos'>Send SOS</a> | <a href='/reboot'>Reboot</a></p>
      </body>
      </html>
    )";
    server.send(200, "text/html", html);
  });

  server.on("/sos", []() {
    // You could manually call sendEmail() here or trigger a relay
    server.send(200, "text/plain", "SOS Triggered.");
  });

  server.on("/reboot", []() {
    server.send(200, "text/plain", "Rebooting...");
    delay(1000);
    ESP.restart();
  });

  server.begin();
  Serial.println("Recovery server started.");
}
   
bool connectToWiFi() {
  Serial.println("Connecting to preferred WiFi...");

  WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();

  // Try for 2 minutes
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 120000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to home network.");
    return true;
  }

  Serial.println("\nFailed to connect to home. Scanning for open networks...");

  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    String ssid = WiFi.SSID(i);
    int encryption = WiFi.encryptionType(i);
    if (encryption == ENC_TYPE_NONE) {
      Serial.print("Trying open network: ");
      Serial.println(ssid);
      WiFi.begin(ssid.c_str());
      unsigned long openStart = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - openStart < 30000) {
        delay(500);
        Serial.print(".");
      }
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to open network.");
        return true;
      }
    }
  }

  Serial.println("Failed to connect to any open networks.");
  return false;
}


void setup() {
  Serial.begin(115200);

  bool wifiOK = connectToWiFi();
  if (!wifiOK) {
  startCaptivePortal();
  }
  
  WiFi.setHostname(deviceHostname);
  Serial.print("Connecting to WiFi");
  ArduinoOTA.setHostname(deviceHostname);  // optional, or use your own name
 ArduinoOTA.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  client.setInsecure();  // Skip certificate validation (optional, but useful for testing)

  Serial.println(F("Ready. Press 'e' to send email."));
   if (sendEmail()) Serial.println(F("Email sent"));
      else Serial.println(F("Email failed"));
}

void loop() {
  ArduinoOTA.handle();

  if (Serial.available() > 0) {
    char inChar = Serial.read();
    if (inChar == 'e') {
      if (sendEmail()) Serial.println(F("Email sent"));
      else Serial.println(F("Email failed"));
    }
  }
}

bool sendEmail() {
  float voltage = readBatteryVoltage();
  if (!client.connect(smtpServer, smtpPort)) {
    Serial.println(F("Connection to SMTP server failed"));
    return false;
  }

  if (!eRcv()) return false;

  client.println("EHLO esp8266");
  if (!eRcv()) return false;

  client.println("AUTH LOGIN");
  if (!eRcv()) return false;

  client.println(base64Encode(emailSender));     // base64 of email
  if (!eRcv()) return false;

  client.println(base64Encode(appPassword));     // base64 of app password
  if (!eRcv()) return false;

  client.print("MAIL FROM: <"); client.print(emailSender); client.println(">");
  if (!eRcv()) return false;

  client.print("RCPT TO: <"); client.print(emailRecipient); client.println(">");
  if (!eRcv()) return false;

  client.println("DATA");
  if (!eRcv()) return false;

  client.print("To: "); client.println(emailRecipient);
  client.print("From: "); client.println(emailSender);
client.println("Subject: 🚨 SOS: RC Boat/Tank Lost!");
  client.println("Content-Type: text/plain\r\n");

  String mapLink = "https://maps.google.com/?q=" + String(lat, 6) + "," + String(lng, 6);
  client.println("🚨 SOS Beacon Triggered!");
  client.printf("Battery Voltage: %.2fV\n", voltage);
  client.printf("Location:\nLatitude: %.6f\nLongitude: %.6f\n", lat, lng);
  client.println("\nGoogle Maps Link:");
  client.println(mapLink);
  client.println(".");
  if (!eRcv()) return false;

  client.println("QUIT");
  if (!eRcv()) return false;

  client.stop();
  Serial.println(F("Email complete, disconnected."));
  return true;
}

bool eRcv() {
  int loopCount = 0;
  while (!client.available()) {
    delay(1);
    if (++loopCount > 10000) {
      Serial.println(F("Timeout"));
      client.stop();
      return false;
    }
  }

  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line.startsWith("4") || line.startsWith("5")) return false;
  }

  return true;
}

// Simple Base64 encoder (no need for external libraries)
String base64Encode(const char* input) {
  const char* base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String output;
  int i = 0;
  int len = strlen(input);
  unsigned char array3[3];
  unsigned char array4[4];

  while (len--) {
    array3[i++] = *(input++);
    if (i == 3) {
      array4[0] = (array3[0] & 0xfc) >> 2;
      array4[1] = ((array3[0] & 0x03) << 4) + ((array3[1] & 0xf0) >> 4);
      array4[2] = ((array3[1] & 0x0f) << 2) + ((array3[2] & 0xc0) >> 6);
      array4[3] = array3[2] & 0x3f;

      for (i = 0; i < 4; i++) output += base64Chars[array4[i]];
      i = 0;
    }
  }

  if (i) {
    for (int j = i; j < 3; j++) array3[j] = '\0';

    array4[0] = (array3[0] & 0xfc) >> 2;
    array4[1] = ((array3[0] & 0x03) << 4) + ((array3[1] & 0xf0) >> 4);
    array4[2] = ((array3[1] & 0x0f) << 2) + ((array3[2] & 0xc0) >> 6);
    array4[3] = array3[2] & 0x3f;

    for (int j = 0; j < i + 1; j++) output += base64Chars[array4[j]];
    while (i++ < 3) output += '=';
  }

  return output;
}*/