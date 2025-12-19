#include <WiFi.h>
#include <ThingSpeak.h>
#include <TinyGPSPlus.h>
#include <DHT.h>

// ====== CONFIG WIFI ======
const char* WIFI_SSID     = "Bait_B2_FF30";
const char* WIFI_PASSWORD = "1234567890";

// ====== CONFIG THINGSPEAK ======
unsigned long myChannelNumber = 3200807;   // <<< CAMBIA ESTO POR TU CHANNEL ID NUMÉRICO >>>
const char* myWriteAPIKey     = "25AMBQ340GO0DX9Y";

WiFiClient client;

// ====== DHT22 ======
#define DHTPIN 6
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ====== GPS (NEO-6M) POR UART1 ======
#define GPS_RX 4   // RX del ESP32 (a TX del GPS)
#define GPS_TX 5   // TX del ESP32 (opcional si no mandas comandos)
HardwareSerial GPS(1);
TinyGPSPlus gps;

// ====== TIMING ======
unsigned long lastUpdateTS = 0;
const unsigned long THINGSPEAK_INTERVAL = 20000; // 20 s (mínimo 15 s en free)

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("Conectando a WiFi ");
  Serial.print(WIFI_SSID);
  Serial.println(" ...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\n✅ WiFi conectado. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n⚠️ No se pudo conectar a WiFi.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\nIniciando: ESP32 + DHT22 + GPS + ThingSpeak");

  dht.begin();

  // GPS
  GPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  // WiFi
  connectWiFi();

  // ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // 1) Procesar NMEA del GPS continuamente
  while (GPS.available() > 0) {
    char c = GPS.read();
    gps.encode(c);
  }

  unsigned long now = millis();

  // 2) Cada X segundos mandamos datos a ThingSpeak
  if (now - lastUpdateTS >= THINGSPEAK_INTERVAL) {
    lastUpdateTS = now;

    // Asegurar WiFi
    connectWiFi();

    // Leer DHT22
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("⚠️ Error leyendo DHT22");
      h = -1;
      t = -1;
    }

    // Leer GPS
// ====== CALIDAD GPS ======
int sats = gps.satellites.isValid() ? gps.satellites.value() : 0;
float hdop = gps.hdop.isValid() ? gps.hdop.hdop() : 99.9; // 99.9 = mala precisión

// Evitar mandar 0,0 si se pierde el fix
static float lastLat = NAN;
static float lastLon = NAN;

if (gps.location.isValid()) {
  lastLat = gps.location.lat();
  lastLon = gps.location.lng();
}

float lat = isnan(lastLat) ? 0.0 : lastLat;
float lon = isnan(lastLon) ? 0.0 : lastLon;
    // Log en Serial
    Serial.println("---- NUEVO REGISTRO ----");
    Serial.print("Temp: "); Serial.print(t);   Serial.println(" °C");
    Serial.print("Hum : "); Serial.print(h);   Serial.println(" %");
    Serial.print("Lat : "); Serial.println(lat, 6);
    Serial.print("Lon : "); Serial.println(lon, 6);
    Serial.print("Sats: "); Serial.println(sats);
    Serial.print("HDOP: "); Serial.println(hdop);

    if (WiFi.status() == WL_CONNECTED) {
      // Poner campos en ThingSpeak
      ThingSpeak.setField(1, t);        // temperatura
      ThingSpeak.setField(2, h);        // humedad
      ThingSpeak.setField(3, lat);      // latitud
      ThingSpeak.setField(4, lon);      // longitud
      ThingSpeak.setField(5, sats);  // satélites
      ThingSpeak.setField(6, hdop);  // precisión GPS

      int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

      if (httpCode == 200) {
        Serial.println("📤 Datos enviados a ThingSpeak OK.");
      } else {
        Serial.print("⚠️ Error ThingSpeak, código: ");
        Serial.println(httpCode);
      }
    } else {
      Serial.println("⚠️ Sin WiFi, solo lectura local por ahora.");
    }

    Serial.println("-------------------------");
  }

  delay(10);
}