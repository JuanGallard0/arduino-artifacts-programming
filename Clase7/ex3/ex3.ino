// CONSTANTS
const int poto_pin = A0;
const int led1_pin = 2;
const int led2_pin = 3;
const int led3_pin = 4;
const int led4_pin = 5;
const int btn_pin = 6;

// VARIABLES
int potoValue = 0;
int currentLed = 1;
int btn_press = 0;
bool rotationDirection = true;

void setup() {
  Serial.begin(9600);
  pinMode(poto_pin, INPUT);
  pinMode(led1_pin, OUTPUT);
  pinMode(led2_pin, OUTPUT);
  pinMode(led3_pin, OUTPUT);
  pinMode(led4_pin, OUTPUT);
  pinMode(btn_pin, INPUT_PULLUP);
}

void loop() {
  btn_press = digitalRead(btn_pin);
  if (btn_press == LOW)
  {
    switch (rotationDirection) {
      case true:
        rotationDirection = false;
        break;
      default:
        rotationDirection = true;  
    } 
  }
  potoValue = analogRead(poto_pin);

  if (rotationDirection) {
    switch(currentLed) {
      case 1: 
        digitalWrite(led1_pin, HIGH);
        delay(potoValue);
        digitalWrite(led1_pin, LOW);
        currentLed = 2;
        break;  
      case 2: 
        digitalWrite(led2_pin, HIGH);
        delay(potoValue);
        digitalWrite(led2_pin, LOW);
        currentLed = 3;
        break;
      case 3: 
        digitalWrite(led3_pin, HIGH);
        delay(potoValue);
        digitalWrite(led3_pin, LOW);
        currentLed = 4;
        break;
      case 4: 
        digitalWrite(led4_pin, HIGH);
        delay(potoValue);
        digitalWrite(led4_pin, LOW);
        currentLed = 1;
        break;
    }
  }
  else {
    switch(currentLed) {
      case 1: 
        digitalWrite(led1_pin, HIGH);
        delay(potoValue);
        digitalWrite(led1_pin, LOW);
        currentLed = 4;
        break;  
      case 2: 
        digitalWrite(led2_pin, HIGH);
        delay(potoValue);
        digitalWrite(led2_pin, LOW);
        currentLed = 1;
        break;
      case 3: 
        digitalWrite(led3_pin, HIGH);
        delay(potoValue);
        digitalWrite(led3_pin, LOW);
        currentLed = 2;
        break;
      case 4: 
        digitalWrite(led4_pin, HIGH);
        delay(potoValue);
        digitalWrite(led4_pin, LOW);
        currentLed = 3;
        break;
    }
  }
}
