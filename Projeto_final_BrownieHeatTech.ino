#include <Wire.h>
#include <max6675.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

#define pinoSO 19
#define pinoCS 23
#define pinoCLK 5
#define OLED_RESET -1

#define FIREBASE_HOST "https://brownieheat-e79ed-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "5G1IphPVSHkLgKVhW6pYsRuorP8xHFtYTyCHWYP5"
#define ssid "capitalismo_4_reais_a_hora"
#define password "batata123"

MAX6675 termopar(pinoCLK, pinoCS, pinoSO);
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(2);

  Serial.begin(9600);

  // Conectar ao WiFi
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi.");

  // Conectar ao Firebase
  Serial.println("Conectando ao Firebase...");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  if (Firebase.failed()) {
    Serial.print("Falha na conexão com o Firebase. Código de erro: ");
    Serial.println(Firebase.error());
  } else {
    Serial.println("Conexão com o Firebase bem-sucedida...");
  }
}

void loop() {
  // Leitura do termopar
  float tempTermopar = termopar.readCelsius();

  // Exibição no display OLED
  display.clearDisplay();
  display.setCursor(5, 10);
  //display.print("Temp Termopar: ");
  display.print(tempTermopar);
  display.print(" C");
  display.display();

  // Enviar dados para o Firebase
  String data = "{\"Temperature_Termopar\": " + String(tempTermopar) + "}";
  Firebase.pushString("/data", data);

  delay(1000);
}
