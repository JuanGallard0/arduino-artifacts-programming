// CONSTANTS
const int btn_pin = 2; 
const int led1_pin = 3;
const int led2_pin = 4;
const int long_press_time  = 1500; // 1000 milliseconds

// VARIABLES
int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin
unsigned long pressedTime  = 0;
bool isPressing = false;
bool isLongDetected = false;

void setup() {
  pinMode(btn_pin, INPUT_PULLUP);
  pinMode(led1_pin, OUTPUT);
  pinMode(led2_pin, OUTPUT);
}

void loop() {
  // read the state of the switch/button:
  currentState = digitalRead(btn_pin);

  if(lastState == LOW && currentState == HIGH) {        // button is pressed
    pressedTime = millis();
    isPressing = true;
    isLongDetected = false;
    digitalWrite(led1_pin, HIGH);
  } else if(lastState == HIGH && currentState == LOW) { // button is released
    isPressing = false;
    digitalWrite(led1_pin, LOW);
  }

  if(isPressing == true && isLongDetected == false) {
    long pressDuration = millis() - pressedTime;

    if( pressDuration > long_press_time ) {
      digitalWrite(led1_pin, LOW);
      digitalWrite(led2_pin, HIGH);
      delay(1500);
      digitalWrite(led2_pin, LOW);
    }
  }

  // save the the last state
  lastState = currentState;
}
