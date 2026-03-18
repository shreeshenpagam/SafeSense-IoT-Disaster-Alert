# SafeSense-IoT-Disaster-Alert
IoT-based multi-disaster early warning system using ESP32, MQ-2, DHT11, and SW-420 sensors with Blynk alerts
 SafeSense – IoT-Based Disaster Alert System

A real-time multi-disaster early warning system using ESP32, 
integrating gas, temperature, and vibration sensors with Blynk IoT alerts.

Features
-Temperature monitoring (alerts above 50°C)
- Gas leak detection (MQ-2 sensor)
- Vibration/earthquake sensing (SW-420)
- Real-time alerts via Blynk mobile app
- Local buzzer alarm

Components
| Component | Purpose |
|-----------|---------|
| ESP32 | Main microcontroller + WiFi |
| MQ-2 | Gas sensor |
| DHT11 | Temperature & Humidity |
| SW-420 | Vibration sensor |
| Buzzer | Local alert |

Setup Instructions
1. Install Arduino IDE
2. Install libraries: `BlynkSimpleEsp32`, `DHT`, `Adafruit_Sensor`
3. Add your WiFi credentials and Blynk Auth Token in the code
4. Upload `SafeSense_Code.ino` to ESP32
5. Open Blynk app and monitor alerts

Performance
- Alert response time: **1–2 seconds**
- Temperature accuracy: **±2°C**
- Humidity accuracy: **±5% RH**

Author
**Shree Shenpagam** – August 2025
