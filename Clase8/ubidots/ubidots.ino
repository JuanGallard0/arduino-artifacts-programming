/****************************************

 * Include Libraries

 ****************************************/

#include "UbidotsESPMQTT.h"



/****************************************

 * Define Constants

 ****************************************/

#define TOKEN "BBFF-RsBdF1wsf3TxzR1zNB2ErzxnHQNHJs"     // Your Ubidots TOKEN

#define WIFINAME "TigoWifi bridge"  // Your SSID

#define WIFIPASS "Yeahjesus9!"  // Your Wifi Pass



Ubidots client(TOKEN);



/****************************************

 * Auxiliar Functions

 ****************************************/



void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");

  Serial.print(topic);

  Serial.print("] ");

  for (int i = 0; i < length; i++) {

    Serial.print((char)payload[i]);

  }

  if ((char)payload[0] == '1') {

    digitalWrite(16, HIGH);

  } else {

    digitalWrite(16, LOW);

  }

  Serial.println();

}



/****************************************

 * Main Functions

 ****************************************/



void setup() {

  // put your setup code here, to run once:

  Serial.begin(115200);

  client.setDebug(true);  // Pass a true or false bool value to activate debug messages

  WiFi.begin(WIFINAME, WIFIPASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    ESP.wdtFeed();
  }
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  client.begin(callback);

  pinMode(16, OUTPUT);

  client.ubidotsSubscribe("esp8266", "temperature");  // Insert the dataSource and Variable's Labels

}



void loop() {

  // put your main code here, to run repeatedly:

  if (!client.connected()) {

    client.reconnect();

    client.ubidotsSubscribe("esp8266", "temperature");  // Insert the dataSource and Variable's Labels

  }

  client.add("stuff", 10);

  client.ubidotsPublish("source1");

  client.loop();

  delay(5000);

}
