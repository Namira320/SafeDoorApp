// Konfigurasi Token Blynk (Sudah disesuaikan dengan milikmu)
#define BLYNK_TEMPLATE_ID "TMPL6XXo-5pkz"
#define BLYNK_TEMPLATE_NAME "SafeDoor"
#define BLYNK_AUTH_TOKEN "NBsSyZ6xT2kD7lQWLTDMI0qiWqkzQr1f"

#define BLYNK_PRINT Serial 

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WAJIB GANTI: Masukkan nama WiFi dan Password di tempatmu
char ssid[] = "HOME_Guest";
char pass[] = "00112233";

#define SENSOR_PIN D1
#define BUZZER_PIN D2
#define LED_PIN D3

bool notifSent = false;

void setup() {
  Serial.begin(9600);

  // Setup mode pin
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Matikan buzzer dan LED di awal
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  // Mulai koneksi ke WiFi dan server Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  // Menjaga agar alat terus terhubung dengan Blynk
  Blynk.run();

  // Membaca status sensor pintu (HIGH = terbuka, LOW = tertutup)
  bool doorOpen = digitalRead(SENSOR_PIN);

  if (doorOpen == HIGH) {
    // PINTU TERBUKA
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    
    // Menyalakan indikator di Web/Aplikasi (Kirim angka 1 ke V0)
    Blynk.virtualWrite(V0, 1); 

    // Mencegah notifikasi terkirim berkali-kali per detik (spam)
    if (!notifSent) {
      Blynk.logEvent("door_open", "⚠️ Pintu terbuka!");
      notifSent = true;
    }

  } else {
    // PINTU TERTUTUP
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    
    // Mematikan indikator di Web/Aplikasi (Kirim angka 0 ke V0)
    Blynk.virtualWrite(V0, 0); 
    
    // Reset status notifikasi
    notifSent = false;
  }
}