// LIBRARIES
#include <ArduinoJson.h>
#include <SimpleDHT.h>
#include <SoftwareSerial.h>
#include <Wire.h>      // libreria de comunicacion por I2C
#include <LCD.h>      // libreria para funciones de LCD
#include <LiquidCrystal_I2C.h>    // libreria para LCD por I2C

// CONSTANTS
#define dht11_pin 5
// These constants, define values needed for the LDR readings and ADC
#define ldr_pin                   A0
#define max_adc_reading           1023
#define adc_ref_voltage           5
#define ref_resistance            1030  // measure this for best results
#define lux_calc_scalar           12518931
#define lux_calc_exponent         -1.405

// CONFIG
SimpleDHT11 dht11(dht11_pin); // setup the dht11 module
SoftwareSerial linkSerial(2, 3); // RX, TX
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7); // DIR, E, RW, RS, D4, D5, D6, D7

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

void setup() {
  Serial.begin(115200);
  linkSerial.begin(4800);
  lcd.setBacklightPin(3, POSITIVE); // puerto P3 de PCF8574 como positivo
  lcd.setBacklight(HIGH);   // habilita iluminacion posterior de LCD
  lcd.begin(16, 2);     // 16 columnas por 2 lineas para LCD 1602A
  lcd.createChar(0, degree_char);
  lcd.createChar(1, cloud1_char);
  lcd.createChar(2, cloud2_char);
  lcd.createChar(3, cloud3_char);
  lcd.clear();      // limpia pantalla
  lcd.setCursor(13, 1);
  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.write(byte(3));
}

void loop() {
  // VARIABLES
  byte humidity = 0;  // stores humidity value
  byte temperature = 0; // stores temperature value
  String message = "";
  bool messageReady = false;
  int ldrRawData;
  float resistorVoltage, ldrVoltage, ldrResistance, ldrLux;

  /*
     TEMPERATURE & HUMIDITY
  */

  dht11.read(&temperature, &humidity, NULL);
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" H");

  /*
     LUMINANCE
  */

  // Perform the analog to digital conversion
  ldrRawData = analogRead(ldr_pin);

  // RESISTOR VOLTAGE_CONVERSION
  // Convert the raw digital data back to the voltage that was measured on the analog pin
  resistorVoltage = (float)ldrRawData / max_adc_reading * adc_ref_voltage;

  // voltage across the LDR is the 5V supply minus the 5k resistor voltage
  ldrVoltage = adc_ref_voltage - resistorVoltage;

  // LDR_RESISTANCE_CONVERSION
  // resistance that the LDR would have for that voltage
  ldrResistance = ldrVoltage / resistorVoltage * ref_resistance;

  // LDR_LUX
  // Change the code below to the proper conversion from ldrResistance to
  // ldrLux
  ldrLux = lux_calc_scalar * pow(ldrResistance, lux_calc_exponent);

  Serial.print("LDR Illuminance: "); Serial.print(ldrLux); Serial.println(" lux");

  /*
     SERIAL COMMUNICATION
  */

  while (linkSerial.available()) {
    message = linkSerial.readString();
    messageReady = true;
  }
  // Only process message if there's one
  if (messageReady) {
    // The only messages we'll parse will be formatted in JSON
    DynamicJsonDocument doc(500); // ArduinoJson version 6+
    // Attempt to deserialize the message
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      messageReady = false;
      return;
    }
    if (doc["type"] == "request") {
      doc["type"] = "response";
      // Get data from analog sensors
      doc["temperature"] = (int)temperature;
      doc["humidity"] = (int)humidity;
      doc["illuminance"] = ldrLux;
      serializeJson(doc, linkSerial);
    }
    messageReady = false;
  }
  
  /*
     LCD
  */

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
  lcd.print("Lux:");
  lcd.print(ldrLux);

  delay(10000);
}
