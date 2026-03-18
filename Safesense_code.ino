// Blynk Template Information
#define BLYNK_TEMPLATE_ID "TMPL3qJMfkNsC"
#define BLYNK_TEMPLATE_NAME "DISASTER SENSE"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"  // ⚠️ Replace with your token

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Wi-Fi Credentials
#define WIFI_SSID "YOUR_WIFI_NAME"         // ⚠️ Replace with your WiFi name
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"  // ⚠️ Replace with your WiFi password

// Sensor & Buzzer Pin Definitions
#define DHTPIN 26
#define DHTTYPE DHT11
#define MQ2_A0 34
#define MQ2_D0 35
#define VIBRATION_SENSOR 32
#define BUZZER_PIN 27

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// False Alarm Prevention Variables
static int lastGasLevels[5] = {0, 0, 0, 0, 0};
static int gasIndex = 0;
static float lastTemperature = 0;
static int tempCount = 0;

// Function to check & reconnect Blynk
void reconnectBlynk() {
  if (!Blynk.connected()) {
    Serial.println("Blynk Disconnected! Reconnecting...");
    Blynk.connect();
    if (Blynk.connected()) {
      Serial.println("Reconnected to Blynk!");
    } else {
      Serial.println("Failed to reconnect to Blynk.");
    }
  }
}

// Function to check disaster conditions & send alerts
void sendAlerts() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Stabilize Temperature Readings
  if (abs(temperature - lastTemperature) > 5) {
    tempCount++;
    if (tempCount < 3) {
      temperature = lastTemperature;
    } else {
      tempCount = 0;
    }
  }
  lastTemperature = temperature;

  // Average Gas Sensor Readings
  int gasLevel = analogRead(MQ2_A0);
  lastGasLevels[gasIndex] = gasLevel;
  gasIndex = (gasIndex + 1) % 5;
  int avgGasLevel = 0;
  for (int i = 0; i < 5; i++) {
    avgGasLevel += lastGasLevels[i];
  }
  avgGasLevel /= 5;

  int gasThreshold = digitalRead(MQ2_D0);
  bool vibrationDetected = digitalRead(VIBRATION_SENSOR) == LOW;

  // Print Sensor Readings
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C    Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  Serial.print("Gas Level (A0): ");
  Serial.println(avgGasLevel);

  if (gasThreshold == LOW) {
    Serial.println("High Gas Detected!");
  }

  // Disaster Alert Condition
  if (temperature > 50 || avgGasLevel > 2000 || gasThreshold == LOW || vibrationDetected) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("DISASTER DETECTED!");
    Blynk.logEvent("disaster_alert", "Disaster Detected! Check Your System!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("System Normal.");
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");

  // Connect to Blynk
  Serial.print("Connecting to Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);
  while (!Blynk.connected()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nBlynk Connected!");

  dht.begin();
  pinMode(MQ2_A0, INPUT);
  pinMode(MQ2_D0, INPUT);
  pinMode(VIBRATION_SENSOR, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  timer.setInterval(5000L, sendAlerts);
}

void loop() {
  reconnectBlynk();
  Blynk.run();
  timer.run();
}
