// LIBRARIES
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>z

// CONSTANTS
// WiFi parameters
#define WLAN_SSID       "TigoWifi bridge"
#define WLAN_PASS       "Yeahjesus9!"
// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "Jonitox"
#define AIO_KEY         "aio_Fgbv19lP3JqIMXEWN5DACle5OpHN"

// CONFIG
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
Adafruit_MQTT_Publish Illuminance = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Illuminance");
SoftwareSerial linkSerial(5, 4); // RX, TX

void setup()
{
  Serial.begin(115200);
  linkSerial.begin(4800);
  Serial.println(F("Adafruit IO Example"));
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  // connect to adafruit io
  connect();
}

/*
   FUNCION DE CONEXION A ADAFRUIT
*/

void connect() {
  Serial.print(F("Connecting to Adafruit IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if (ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(10000);
  }
  Serial.println(F("Adafruit IO Connected!"));
}

void loop()
{
  // VARIABLES
  boolean messageReady = false;
  String message = "";
  int temperature = 0, humidity = 0;
  float illuminance = 0.0;

  // ping adafruit io a few times to make sure we remain connected
  if (! mqtt.ping(3)) {
    // reconnect to adafruit io
    if (! mqtt.connected())
      connect();
  }

  /*
     SERIAL COMMUNICATION
  */

  // Send a JSON-formatted request with key "type" and value "request"
  // then parse the JSON-formatted response with keys "gas" and "distance"
  DynamicJsonDocument doc(500);
  temperature = 0;
  humidity = 0;
  illuminance = 0.0;
  // Sending the request
  doc["type"] = "request";
  serializeJson(doc, linkSerial);
  messageReady = false;
  message = "";
  // Reading the response
  while (messageReady == false) { // blocking but that's ok
    if (linkSerial.available()) {
      message = linkSerial.readString();
      messageReady = true;
    }
  }
  // Attempt to deserialize the JSON-formatted message
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  temperature = doc["temperature"];
  humidity = doc["humidity"];
  illuminance = doc["illuminance"];

  /*
     PUBLISH TO CLOUD
  */

  // Prepare the data for serving it over Adafruit
  if (! Temperature.publish(temperature))
    Serial.println(F("Failed"));
  if (! Humidity.publish(humidity))
    Serial.println(F("Failed"));
  if (! Illuminance.publish(illuminance))
    Serial.println(F("Failed"));
  else
    Serial.println(F("Sent!"));
}
