//GLOBAL
int pulse_pin = 2;
int led_pin = 3;
int pulse_var = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(pulse_pin, INPUT);
  pinMode(led_pin, OUTPUT);
}

void loop()
{
  pulse_var = digitalRead(pulse_pin);
  if (pulse_var == HIGH) 
    digitalWrite(led_pin, HIGH);
  else
    digitalWrite(led_pin, LOW);
  Serial.println(pulse_var);
}
