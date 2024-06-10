#define BLYNK_TEMPLATE_ID "TMPLxxxx"
#define BLYNK_TEMPLATE_NAME "Pertanian Pintar"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char ssid[] = "WiFi";
char pass[] = "Password";

#define SOIL_PIN A0
#define DHT_PIN D4
#define BUZZER_PIN D1
#define LED_MERAH D5
#define LED_HIJAU D6

#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

int batasKering = 600;
float batasPanas = 31.0;

BlynkTimer timer;

void prosesSistemIoT() {
  int nilaiTanah = analogRead(SOIL_PIN);
  float suhu = dht.readTemperature();

  if (isnan(suhu)) {
    Serial.println("Gagal membaca DHT!");
    return;
  }

  bool kondisiKering = (nilaiTanah > batasKering);
  bool kondisiPanas = (suhu >= batasPanas);

  // LED berdasarkan kondisi tanah
  if (kondisiKering) {
    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(LED_HIJAU, LOW);
  } else {
    digitalWrite(LED_MERAH, LOW);
    digitalWrite(LED_HIJAU, HIGH);
  }

  // Buzzer hanya berdasarkan suhu DHT
  if (kondisiPanas) {
    digitalWrite(BUZZER_PIN, LOW);   // buzzer bunyi jika aktif LOW
  } else {
    digitalWrite(BUZZER_PIN, HIGH);  // buzzer mati
  }

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" C | Tanah: ");
  Serial.print(nilaiTanah);

  if (kondisiKering) {
    Serial.print(" | Tanah: KERING");
  } else {
    Serial.print(" | Tanah: LEMBAB");
  }

  if (kondisiPanas) {
    Serial.println(" | Suhu: PANAS | BUZZER NYALA");
  } else {
    Serial.println(" | Suhu: NORMAL | BUZZER MATI");
  }

  Blynk.virtualWrite(V0, nilaiTanah);
  Blynk.virtualWrite(V1, suhu);
  Blynk.virtualWrite(V2, kondisiKering ? 1 : 0);
  Blynk.virtualWrite(V3, kondisiPanas ? "SUHU PANAS!" : "SUHU NORMAL");
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_HIJAU, OUTPUT);

  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED_MERAH, LOW);
  digitalWrite(LED_HIJAU, LOW);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, prosesSistemIoT);
}

void loop() {
  Blynk.run();
  timer.run();
}
