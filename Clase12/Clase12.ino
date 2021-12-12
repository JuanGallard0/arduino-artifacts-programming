int LED = 0;           // Pin al que LED está conectado
int LDR = A0;           // Pin al que LDR está conectado
int pulsador = 5;         // Pin al que pulsadsor está conectado
int ledrojo = 4;        // Pin al que ledrojo está conectado
int entrada = 0;        // Variable para guardar valor de LDR
int boton;            // Variable para guardar valor de LDR
void setup()
{
  Serial.begin(115200);     // Iniciar velocidad de comunicación serial
  pinMode(LED, OUTPUT);     //se declara como salida
  pinMode(LDR, INPUT);      //se declara como entrada
  pinMode(pulsador,INPUT);    //se declara como entrada
  pinMode(ledrojo, OUTPUT);   //se declara como salida
}
void loop()
{
entrada = analogRead(LDR);    //lectura del LDR
Serial.println(entrada);    // Valor que será mostrado en serial Monitor
boton = digitalRead(pulsador);  //lectura del LDR
Serial.println(boton);
///   Inicia estructura de control del LDR  /////  
  if (entrada < 500) {      //comparación de valor ldr
  digitalWrite(LED, HIGH);}   // Encender LED
  else {
  digitalWrite(LED, LOW);   // Apagar LED
  delay(100);}
 ///    Inicia estructura de control del boton  ///// 
  if(boton == 1){       //comparación de valor boton
   digitalWrite(ledrojo, HIGH);}// Encender ledrojo
  else{  
    digitalWrite(ledrojo, LOW);}// Apagar ledrojo
  delay(100);
  }
