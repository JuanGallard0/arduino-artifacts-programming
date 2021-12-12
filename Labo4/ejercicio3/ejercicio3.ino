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
Adafruit_MQTT_Publish PUBLICAR2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");  



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
const int pingPin = D2; // Trigger Pin of Ultrasonic Sensor
const int echoPin = D1; // Echo Pin of Ultrasonic Sensor
const int lm35_pin = A0;  /* LM35 O/P pin */

void setup() {
  Serial.begin(9600); // Starting Serial Terminal
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
    
  int duration, inches, cm;

  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  PUBLICAR.publish(cm);

  /*
  int temp_adc_val;
  float temp_val;
  temp_adc_val = analogRead(lm35_pin); // Read Temperature 
  temp_val = (temp_adc_val * 4.88); // Convert adc value to equivalent voltage 
  temp_val = (temp_val/10); // LM35 gives output of 10mv/°C 
  Serial.print("Temperature = ");
  Serial.print(temp_val);
  Serial.print(" Degree Celsius\n");
  PUBLICAR2.publish(temp_val);
   */
  delay(2500);
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

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
