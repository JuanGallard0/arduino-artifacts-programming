#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "ReynosaPhone"     //your SSID
#define WLAN_PASS       "12345678"         //your password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                                // use 8883 for SSL
#define AIO_USERNAME  "greynosa"                             //your AIO username at ADAFRUIT
#define AIO_KEY       "aio_clGT28K3NQbKRJxf8ywqvgYCENtV"     //your AIO Key at ADAFRUIT

/******************* para declarar pines y variables *************************/
int entrada;                                //sirve para guardar el dato leído de la LDR
int boton;                                  //sirve para guardar el dato leído del pulsador

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/****************************** Feeds for Publishing***************************************/
// Setup a feed called 'indicador' for publishing. led verde
Adafruit_MQTT_Publish indicador = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/indicador");

// Setup a feed called 'indicador2' for publishing.led rojo
Adafruit_MQTT_Publish indicador2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/indicador2");

/****************************** Feeds for Subscribing***************************************/
// Setup a feed called 'pulsador' for subscribing to changes on the slider
Adafruit_MQTT_Subscribe pulsador = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/pulsador", MQTT_QOS_1);

//// Setup a feed called 'onoff' for subscribing to changes to the button
//Adafruit_MQTT_Subscribe boton_led = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/boton_led", MQTT_QOS_1);

/*************************** Sketch Code ************************************/
//
//void slidercallback(double x) {
//  Serial.print("Hey we're in a slider callback, the slider value is: ");
//  Serial.println(x);
//}

void pulsadorcallback(char *data, uint16_t len) {
  Serial.print("Hey we're in a pulsador callback, the button value is: ");
  Serial.println(data);
  String message = String(data);
      message.trim();
      if (message == "1") {
        digitalWrite(D2, HIGH);       //Encendemos el puerto D2
        indicador2.publish(1);}       //actualizamos el indicador an adafruit.
      if (message == "0") {
        digitalWrite(D2, LOW);        //Apagamos el puerto D2
        indicador2.publish(0);}}      //actualizamos el indicador an adafruit.

//*********************************************** SetUp *****************************/
void setup() {
  //para declarar si son entradas o salidas
  pinMode(D2, OUTPUT);  //D2 es ledrojo
  pinMode(D3, OUTPUT);  //D3 es ledverde
  pinMode(D1, INPUT);   //D1 es el pulsador
  pinMode(A0, INPUT);   //A0 es el fotoresistencia
  Serial.begin(115200); //inicio de comunicacion serie
  delay(10);
  //****************************************** Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  //********************************************* Callback Functions
//  slider.setCallback(slidercallback);
  pulsador.setCallback(pulsadorcallback);
  
  // Setup MQTT subscription for feed's.
//  mqtt.subscribe(&slider);
  mqtt.subscribe(&pulsador);

}

uint32_t x=0;
//***************************************************** Loop ********************************/
void loop() {
  MQTT_connect();
  mqtt.processPackets(5000);
  if(! mqtt.ping()) {   // ping the server to keep the mqtt connection alive
    mqtt.disconnect();}

// Now we can publish stuff!
  Serial.print(F("\nSending indicador val "));
  Serial.print(x);
  Serial.print("...");
  if (! indicador.publish(x++)) {
    Serial.println(F("Failed"));}
    else {
    Serial.println(F("OK!"));}
//para indicador 2
  Serial.print(F("\nSending indicador2 val "));
  Serial.print(x);
  Serial.print("...");
  if (! indicador2.publish(x++)) {
    Serial.println(F("Failed"));}
    else {
    Serial.println(F("OK!"));}
    
//Espacio para código de funcionamiento adicional 
      //código para lectura de ldr
  entrada = analogRead(A0);    //lectura del LDR
  Serial.print("El valor del LDR es: "); //mensaje
  Serial.println(entrada); 
   if (entrada < 500) {      //comparación de valor ldr
    digitalWrite(D3, HIGH);   // Encender LED
    indicador.publish(1);}   //Actualizamos el indicador en adafruit
    else {
    digitalWrite(D3, LOW);   // Apagar LED
    indicador.publish(3);}   //Actualizamos el indicador en adafruit

  //código para lectura de pulsador fisico (D1)
  boton = digitalRead(D1);    //lectura del boton(pulsador fisico) 
  Serial.print("El estado del pulsador fisico es: ");  //mensaje
  Serial.println(boton); 
    if (boton == 1){
      digitalWrite(D2, HIGH);   // Encender LED
      indicador2.publish(1);
      }
    else {
      digitalWrite(D2,  LOW);   // Apagar LED
      indicador2.publish(0);
      }
    }


// Function to connect and reconnect as necessary to the MQTT server.
void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;}
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 10 seconds...");
       mqtt.disconnect();
       delay(10000);  // wait 10 seconds
       retries--;
       if (retries == 0) {       // basically die and wait for WDT to reset me
         while (1);}}
  Serial.println("MQTT Connected!");}
