// Incluimos librería
#include <DHT.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
 
// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 2
// pin ventilador
#define green_led 4
// pin foco incandescente
#define red_led 5
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11

/************************* NETWORK */
#define WLAN_SSID "TigoWifi bridge"
#define WLAN_PASS "secreto"

/************************* ADAFRUIT SETUP */
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "Jonitox"
#define AIO_KEY "aio_Fgbv19lP3JqIMXEWN5DACle5OpHN"

/************************* GLOBAL STATE */
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/************************* ADAFRUIT PUBLISH */
Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
 
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  // Inicializamos comunicación serie
  Serial.begin(9600);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  
  // Comenzamos el sensor DHT
  dht.begin();

  // inicializar pines led
  pinMode(green_led, OUTPUT);  
  pinMode(red_led, OUTPUT);  //D3 es ledverde
}
 
void loop() {
    // Esperamos 5 segundos entre medidas
  delay(5000);

  /*
     CONNECTION
  */
  MQTT_connect();
  if (! mqtt.ping()) {
    mqtt.disconnect();
  }
 
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahreheit
  float f = dht.readTemperature(true);
 
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
 
  // Calcular el índice de calor en Fahreheit
  float hif = dht.computeHeatIndex(f, h);
  // Calcular el índice de calor en grados centígrados
  float hic = dht.computeHeatIndex(t, h, false);
 
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Índice de calor: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

  if (t > 50.0) {
    digitalWrite(green_led, HIGH);
    digitalWrite(red_led, LOW);
  } 
  else if (t < 40.0) {
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, HIGH);    
  }
  else {
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, LOW);    
  }

  /*
     PUBLISH
  */
  if (! Temperature.publish((int)t))
    Serial.println(F("Failed"));
}

/************************* FUNCTIONS */
void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect();
    delay(10000);
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
}
