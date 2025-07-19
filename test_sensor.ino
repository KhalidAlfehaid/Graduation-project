#include <WiFi.h>
#include <Firebase_ESP_Client.h>


#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "Zain_RT875_FDD3"
#define WIFI_PASSWORD "123456778"

#define API_KEY "AIzaSyDXRkMjWhz4RS-dxc8j20T_pv7toUQ-B7I"
#define DATABASE_URL "https://parkingspot-65d6f-default-rtdb.europe-west1.firebasedatabase.app/"

#define USER_EMAIL "khalidalfuhaid12@gmail.com"
#define USER_PASSWORD "Kk123456-"

#define TRIG_PIN 27
#define ECHO_PIN 33


#define DISTANCE_THRESHOLD 300 
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Connecting to Firebase...");
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; 

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  String status;
  if (distance < DISTANCE_THRESHOLD) {
    status = "occupied"; 
  } else {
    status = "available"; 
  }

  if (Firebase.ready()) {
    if (Firebase.RTDB.setString(&fbdo, "/parking_slots/slot_1/status", status)) {
      Serial.println("Status sent successfully: " + status);
    } else {
      Serial.print("Failed to send status: ");
      Serial.println(fbdo.errorReason());
    }
  }

  delay(2000); 
}