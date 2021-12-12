#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"         //LIBRERIAS NECESARIAS PARA COMUNICARSE
#include "Adafruit_MQTT_Client.h"


/************************* RED WIFI  *********************************/



#define WLAN_SSID       "TigoWifi bridge"     //NOMBRE DE LA RED

#define WLAN_PASS       "Yeahjesus9!"         //CONTRASEÑA DE LA RED



/************************* Adafruit.io Setup *********************************/



#define AIO_SERVER      "io.adafruit.com"

#define AIO_SERVERPORT  1883                                // use 8883 for SSL

#define AIO_USERNAME  "Jonitox"                            //TU USERNAME DE ADAFRUIT

#define AIO_KEY       "aio_Fgbv19lP3JqIMXEWN5DACle5OpHN"      //TU KEY DE ADAFRUIT



/************ Global State (you don't need to change this!) ******************/



//CREANDO UNA CLASE ESP8266 WiFiClient PARA CONCECTAR CON SERVIDOR MQTT.

WiFiClient client;


// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);



/****************************** SE DEFINE DE QUIEN SE RECIBEN DATOS ***************************************/


Adafruit_MQTT_Subscribe cajita = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/cajita", MQTT_QOS_1);
Adafruit_MQTT_Subscribe fila = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/fila", MQTT_QOS_1);



/*************************** EJEMPLOS DE QUE HACER CON LOS DATOS RECIBIDOS ************************************/
byte x;

void cajitacallback(char *data, uint16_t len) {
  String message = String(data);
      message.trim();
      if (x==1){lcd.setCursor(0,0); lcd.print(message);} //O SE ENCIENDE D0
      else if (x==2){lcd.setCursor(0,1); lcd.print(message);} //O SE ENCIENDE D0
      else if (message == "limpiar"){lcd.clear();}}


void filacallback(char *data, uint16_t len) {
  String message = String(data);
      message.trim();
      if (message == "fila 1") {x=1;}    //DEPENDIENDO DEL MENSAJE RECIBIDO SE APAGA
      else if (message == "fila 2") {x=2;}
}
      
//*********************************************** SetUp *****************************

void setup() {

  pinMode(D0, OUTPUT); //SE DEFINE LA FUNCION DE LOS PINES
  pinMode(D1, OUTPUT);

  WiFi.begin(WLAN_SSID, WLAN_PASS); //SE INICIA LA CONEXION A LA RED WIFI
  
  cajita.setCallback(cajitacallback); //Callback Functions
  mqtt.subscribe(&cajita); ////Setup MQTT subscription for feed's. 
  
  fila.setCallback(filacallback); //Callback Functions
  mqtt.subscribe(&fila);} ////Setup MQTT subscription for feed's. 

//***************************************************** Loop ********************************/

void loop() {
 
  MQTT_connect();
  mqtt.processPackets(2800);
  if(! mqtt.ping()) {   // ping the server to keep the mqtt connection alive
    mqtt.disconnect();}   
    } //SE PUBLICA EN EL widget "PUBLICAR" LA VARIABLE y


// FUNCION PARA CONECTAR Y RECONECTAR AL SERVIDOR. PROPIO DE LA LIBRERIA.

void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;}
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected

       mqtt.disconnect();

       delay(10000);  // wait 10 seconds

       retries--;

       if (retries == 0) {       // basically die and wait for WDT to reset me

         while (1);}}
}
