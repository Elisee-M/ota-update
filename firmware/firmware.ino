#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecureBearSSL.h>

// --------------------
// WiFi credentials
// --------------------
const char* ssid = "EdNet";
const char* password = "Huawei@123";

// --------------------
// GitHub raw URLs
// --------------------
#define VERSION_URL  "https://raw.githubusercontent.com/Elisee-M/ota-update/master/version.txt"
#define FIRMWARE_URL "https://raw.githubusercontent.com/Elisee-M/ota-update/master/firmware.bin"

// --------------------
// Current version
// --------------------
int currentVersion = 1;

// --------------------
// LED pin for demo
// --------------------
#define led D4

// --------------------
// SETUP
// --------------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(led, OUTPUT);

  Serial.println("\nBooting...");
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println("IP: " + WiFi.localIP().toString());

  checkForUpdates();
}

// --------------------
// LOOP
// --------------------
void loop() {
  // Simple LED blink
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
}

// --------------------
// CHECK FOR OTA UPDATE
// --------------------
void checkForUpdates() {
  Serial.println("Checking for updates...");

  WiFiClientSecure client;
  client.setInsecure(); // Skip SSL verification (required for GitHub HTTPS)
  HTTPClient http;

  if (!http.begin(client, VERSION_URL)) {
    Serial.println("❌ Failed to initialize HTTPClient");
    return;
  }

  int httpCode = http.GET();

  if (httpCode != 200) {
    Serial.print("❌ Error fetching version file: ");
    Serial.println(httpCode);
    http.end();
    return;
  }

  String newVersionStr = http.getString();
  http.end();

  int newVersion = newVersionStr.toInt();

  Serial.print("Current version: ");
  Serial.println(currentVersion);
  Serial.print("Online version: ");
  Serial.println(newVersion);

  if (newVersion > currentVersion) {
    Serial.println("🔔 New update found!");
    doOTAUpdate();
  } else {
    Serial.println("👍 No update needed.");
  }
}

// --------------------
// OTA UPDATE
// --------------------
void doOTAUpdate() {
  WiFiClientSecure client;
  client.setInsecure(); // Skip SSL verification for firmware download

  Serial.println("Starting OTA update...");

  t_httpUpdate_return ret = ESPhttpUpdate.update(client, FIRMWARE_URL, String(currentVersion));

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("❌ Update failed [%d]: %s\n",
                    ESPhttpUpdate.getLastError(),
                    ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("No updates available.");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("✔ Update successful!");
      break;
  }
}
