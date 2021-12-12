// CONSTANTS
const int poto_pin = A5;
const int red_pin = 3;
const int green_pin = 5;
const int blue_pin = 6;
const int btn_pin = 2;

// VARIABLES
double potoValue = 0;
double ledPower = 0;
int ledColor = 1;
int btnPress = 0;

void setup() {
  Serial.begin(9600);
  pinMode(poto_pin, INPUT);
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
  pinMode(btn_pin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  potoValue = analogRead(poto_pin);
  ledPower = 255 * potoValue / 1024;

  btnPress = digitalRead(btn_pin);
  if (btnPress == LOW)
  {
    switch (ledColor) {
      case 1:
        ledColor = 2;
        break;
      case 2:
        ledColor = 3;
        break;
      default:
        ledColor = 1;  
    } 
  }

  switch (ledColor) {
    case 1:
      RGB_color((int)ledPower, 0, 0);
      break;
    case 2:
      RGB_color(0, (int)ledPower, 0);
      break;
    default:
      RGB_color(0, 0, (int)ledPower);
  } 

  delay(100);
}

void RGB_color(int red_value, int green_value, int blue_value)
{
  analogWrite(red_pin, red_value);
  analogWrite(green_pin, green_value);
  analogWrite(blue_pin, blue_value);
}
