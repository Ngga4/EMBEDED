#define BLYNK_TEMPLATE_ID "TMPL61BMWSVLy"
#define BLYNK_TEMPLATE_NAME "ESP8266 WIFI"
#define BLYNK_AUTH_TOKEN "wpZ7hgJ8tRQG2xcLoCVQ66RnU5uQueTt"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Servo.h>

// WiFi
char ssid[] = "Munai Serumpun New";
char pass[] = "HonjoMan2015";


// DHT22
#define DHTPIN D1
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// Ultrasonic
#define TRIG_PIN D5
#define ECHO_PIN D6

// LED
#define LED_PIN D2

// Servo
#define SERVO_PIN D4
Servo myServo;

BlynkTimer timer;

// wadah 
const float tinggiWadah = 4.0;

// Fungsi membaca ultrasonic
float readDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if(duration == 0)
  {
    return -1;
  }

  float distance = duration * 0.034 / 2;

  return distance;
}


void sendSensor()
{
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();  
  float jarak = readDistance();

  if (isnan(suhu) || isnan(kelembaban))
  {
    Serial.println("Gagal membaca DHT22");
    return;
  }

  if (jarak < 0)
  {
    Serial.println("Ultrasonic tidak terbaca");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" °C ");

  Serial.print("Kelembaban: ");
  Serial.print(kelembaban);
  Serial.print(" % ");

  float persenPakan =
      ((tinggiWadah - jarak) / tinggiWadah) * 100.0;

  if (persenPakan < 0)
    persenPakan = 0;

  if (persenPakan > 100)
    persenPakan = 100;  

  Serial.print("Jumlah pakan: ");
  Serial.print(persenPakan);
  Serial.println(" %");    

  // Kirim ke Blynk
  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, kelembaban);
  Blynk.virtualWrite(V2, persenPakan);

if (suhu < 32 || suhu > 35)
  {
    digitalWrite(LED_PIN, LOW);
  }
  else
  {
    digitalWrite(LED_PIN, HIGH);
  }
}

BLYNK_WRITE(V3)
{
  int tombol = param.asInt();

  if(tombol == 1)
  {
    // Putar kanan
    myServo.write(180);

    delay(700);   // sesuaikan waktu

    myServo.write(90); // stop

    Serial.println("Pakan Dibuka");
  }
  else
  {
    // Putar kiri
    myServo.write(0);

    delay(700);   // sesuaikan waktu

    myServo.write(90); // stop

    Serial.println("Pakan Ditutup");
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(0);

  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}