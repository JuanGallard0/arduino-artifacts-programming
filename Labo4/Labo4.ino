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



/****************************** SE DEFINE DONDE SE PUBLICARA ***************************************/


Adafruit_MQTT_Publish PUBLICAR = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ilumi");  



/****************************** SE DEFINE DE QUIEN SE RECIBEN DATOS ***************************************/


Adafruit_MQTT_Subscribe RECIBIR = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/intensidad", MQTT_QOS_1);



/*************************** EJEMPLOS DE QUE HACER CON LOS DATOS RECIBIDOS ************************************/


void RECIBIRcallback(double x) {
  analogWrite(D1,x);} //ESCRIBIR EN LA SALIDA PWM CON EL VALOR QUE MANDA
          //ALGUN widget VARIABLE.

void RECIBIR1callback(char *data, uint16_t len) {
  String message = String(data);
      message.trim();
      if (message == "MENSAJE 1") {digitalWrite(D0, 0);}    //DEPENDIENDO DEL MENSAJE RECIBIDO SE APAGA
      if (message == "MENSAJE 2") {digitalWrite(D0, 1);}}  //O SE ENCIENDE D0


//*********************************************** SetUp *****************************
//int variable global; //SE DEFINEN VARIABLES A UTILIZAR EN CUALQUIER FUNCION 
float y=0;
void setup() {

  pinMode(D0, OUTPUT); //SE DEFINE LA FUNCION DE LOS PINES
  pinMode(D1, OUTPUT);

  WiFi.begin(WLAN_SSID, WLAN_PASS); //SE INICIA LA CONEXION A LA RED WIFI
  
  RECIBIR.setCallback(RECIBIRcallback); //Callback Functions
  mqtt.subscribe(&RECIBIR);} ////Setup MQTT subscription for feed's. 

  //RECIBIR1.setCallback(RECIBIR1callback); //Callback Functions
  //mqtt.subscribe(&RECIBIR1);} ////Setup MQTT subscription for feed's. 

//***************************************************** Loop ********************************/

void loop() {
 
  MQTT_connect();
  mqtt.processPackets(2800);
  if(! mqtt.ping()) {   // ping the server to keep the mqtt connection alive
    mqtt.disconnect();}   
  y = analogRead(A0); // LECTURA DEL PIN A0.
  PUBLICAR.publish(y);
  delay(5000);
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
