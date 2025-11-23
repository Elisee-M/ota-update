# ESP8266 OTA Update via Firebase Hosting 🚀

> Auto OTA update for ESP8266 ⚡💡

<details>
<summary>📦 Features</summary>
- Connects ESP8266 to WiFi 🌐  
- Checks version.txt 🔍  
- Downloads firmware.bin if newer ⚡  
- OTA update runs automatically ✅  
- Blinks LED while running 💡
</details>

<details>
<summary>🛠 Requirements</summary>
- ESP8266 (NodeMCU, Wemos D1 mini)  
- Arduino IDE with ESP8266 board package ≥ 3.x  
- Firebase Hosting project 🏠  
- Files: version.txt & firmware.bin
</details>

<details>
<summary>🚀 Firebase Hosting Setup</summary>
1. Create Firebase project → Hosting  
2. Place version.txt & firmware.bin in folder  
3. Deploy via CLI:  
   ```bash
   firebase login
   firebase init hosting
   firebase deploy
