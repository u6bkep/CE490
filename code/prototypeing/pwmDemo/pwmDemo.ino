// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 34;
#define ledPin 32

// variable for storing the potentiometer value
int potValue = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
}

void loop() {
  // Reading potentiometer value
  potValue = analogRead(potPin);
  //potValue = potValue >> 4;
  potValue = map(potValue, 0, 4095, 0, 255);
  analogWrite(ledPin, potValue);
  Serial.println(potValue);
  
  delay(500);
}
