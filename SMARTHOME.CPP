#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Smart Home"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "NAMA_WIFI";
char pass[] = "PASSWORD_WIFI";

#define FLAME_SENSOR D5
#define PIR_SENSOR D6

#define BUZZER D7
#define LED_PIN D4

BlynkTimer timer;

void readSensors() {
  int flameState = digitalRead(FLAME_SENSOR);
  int pirState = digitalRead(PIR_SENSOR);

  if (flameState == LOW) {
    digitalWrite(BUZZER, HIGH);
    Blynk.virtualWrite(V0, 1);
    Blynk.virtualWrite(V1, "API TERDETEKSI!");
    Serial.println("API TERDETEKSI!");
  } else {
    digitalWrite(BUZZER, LOW);
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, "AMAN");
  }

  if (pirState == HIGH) {
    digitalWrite(LED_PIN, HIGH);
    Blynk.virtualWrite(V2, 1);
    Blynk.virtualWrite(V3, "GERAKAN TERDETEKSI");
    Serial.println("GERAKAN TERDETEKSI");
  } else {
    digitalWrite(LED_PIN, LOW);
    Blynk.virtualWrite(V2, 0);
    Blynk.virtualWrite(V3, "TIDAK ADA GERAKAN");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(FLAME_SENSOR, INPUT);
  pinMode(PIR_SENSOR, INPUT);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER, LOW);
  digitalWrite(LED_PIN, LOW);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L, readSensors);
}

void loop() {
  Blynk.run();
  timer.run();
}