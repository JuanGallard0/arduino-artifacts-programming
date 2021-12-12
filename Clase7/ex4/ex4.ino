// CONSTANTS
const int poto_pin = A0;
const int led1_pin = 3;

// VARIABLES
double potoValue = 0;
double ledPower = 0;

void setup() {
  Serial.begin(9600);
  pinMode(poto_pin, INPUT);
  pinMode(led1_pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  potoValue = analogRead(poto_pin);
  ledPower = 255 * potoValue / 1024;
  analogWrite(led1_pin, (int)ledPower);
}
