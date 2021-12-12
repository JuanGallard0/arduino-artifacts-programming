/************************* LIBRARIES */
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SimpleDHT.h>

/************************* GLOBAL CONSTANTS */
#define dht11_pin 0
#define ldr_pin A0
#define scl_pin 5
#define sda_pin 4
#define R 1.0 // Valor de la resistencia en serie en kilos.

/************************* GLOBAL VARIABLES */
bool connection = true;

/************************* NETWORK */
#define WLAN_SSID "MotoJG"
#define WLAN_PASS "hotpass99"

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
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
Adafruit_MQTT_Publish Illuminance = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Illuminance");

/************************* HARDWARE CONFIGURATION */
SimpleDHT11 dht11(dht11_pin);
LiquidCrystal_I2C lcd(0x27, 16, 2);

/************************* LCD CHARACTERS */
byte degree_char[] = {
  B00100,
  B01010,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte cloud1_char[] = {
  B00000,
  B00000,
  B00001,
  B00111,
  B01111,
  B01111,
  B11111,
  B11111
};
byte cloud2_char[] = {
  B01110,
  B01111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte cloud3_char[] = {
  B00000,
  B10000,
  B11100,
  B11100,
  B11111,
  B11111,
  B11111,
  B11110
};

/************************* SETUP */
void setup() {
  Serial.begin(115200);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Wire.begin(sda_pin, scl_pin);
  lcd.begin();
  lcd.createChar(0, degree_char);
  lcd.createChar(1, cloud1_char);
  lcd.createChar(2, cloud2_char);
  lcd.createChar(3, cloud3_char);
}

/************************* LOOP */
void loop() {
  /*
     VARIABLES
  */
  byte humidity = 0;
  byte temperature = 0;
  int lux; // Intensidad de luz.

  /*
     CONNECTION
  */
  MQTT_connect();
  if (! mqtt.ping()) {
    mqtt.disconnect();
  }

  /*
     TEMPERATURE AND HUMIDITY
  */
  dht11.read(&temperature, &humidity, NULL);
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" H");

  /*
     ILLUMINANCE
  */
  lux = CalcIlluminance();
  Serial.println(lux);

  /*
     LCD
  */
  PrintLcd(temperature, humidity, lux);

  /*
     PUBLISH
  */
  if (! Temperature.publish((int)temperature))
    Serial.println(F("Failed"));
  if (! Humidity.publish((int)humidity))
    Serial.println(F("Failed"));
  if (! Illuminance.publish(lux))
    Serial.println(F("Failed"));

  delay(10000);
}

/************************* FUNCTIONS */
void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    connection = true;
    return;
  }
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect();
    delay(10000);
    retries--;
    if (retries == 0) {
      connection = false;
      return;
    }
  }
}

int CalcIlluminance() {
  int tension = analogRead(ldr_pin); // Lectura de la señal.
  float valorLDR = ( (1023 * R) / tension) - R;// Encuentra resistencia de LDR.
  //lux = 800.69 * pow(valorLDR, -1.283); //Calcula intensidad luminosa en lx
  // Envia informacion por serie al monitor serial
  return 10.0 * pow(valorLDR / 5.0, -1.3); //Calcula intensidad luminosa en lx
}

void PrintLcd(byte temperature, byte humidity, int lux) {
  lcd.clear();      // limpia pantalla
  if (connection) {
    lcd.setCursor(13, 1);
    lcd.write(byte(1));
    lcd.write(byte(2));
    lcd.write(byte(3));
  }
  else {
    lcd.setCursor(9, 1);    // ubica cursor en columna 0 y linea 0
    lcd.print("OFFLINE");  // escribe el texto
  }
  lcd.setCursor(0, 0);    // ubica cursor en columna 0 y linea 0
  lcd.print("T:");  // escribe el texto
  lcd.print(temperature);
  lcd.write(byte(0));
  lcd.print("C");
  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(lux);
  lcd.print(" lux");
}
