#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

const char* ssid = "EdNet";
const char* password = "Huawei@123";

#define VERSION_URL  "https://raw.githubusercontent.com/Elisee-M/ota-update/main/version.txt"
#define FIRMWARE_URL "https://raw.githubusercontent.com/Elisee-M/ota-update/main/firmware.bin"

int currentVersion = 1;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nBooting...");
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  checkForUpdates();
}

void loop() {
  // Your normal program here
  delay(1000);
}



// --------------------------------------------------
// CHECK FOR UPDATE
// --------------------------------------------------
void checkForUpdates() {
  Serial.println("Checking for updates...");

  WiFiClient client;
  HTTPClient http;

  // NEW REQUIRED FORMAT for ESP8266 core 3.x:
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



// --------------------------------------------------
// OTA UPDATE (NEW API FORMAT)
// --------------------------------------------------
void doOTAUpdate() {
  WiFiClient client;

  Serial.println("Starting OTA update...");

  t_httpUpdate_return ret = ESPhttpUpdate.update(
      client,
      FIRMWARE_URL,
      String(currentVersion)
  );

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
