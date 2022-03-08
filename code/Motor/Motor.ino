//Motor Controller Code

//Required Libraries Importation
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h> 

Servo myservo;  // create servo object to control a servo

#define servoPin  32      // GPIO pin used to connect the servo control (digital out)
#define minSpeed  75
#define maxSpeed  65

#if defined(ARDUINO_ESP32S2_DEV)
#define potPin 10       // GPIO pin used to connect the potentiometer (analog in)
#else
#define potPin 34       // GPIO pin used to connect the potentiometer (analog in)
#endif
#define ADC_Max 4096    // This is the default ADC max value on the ESP32 (12 bit ADC width);
                          // this width can be set (in low-level oode) from 9-12 bits, for a
                          // a range of max values of 512-4096

//float val;    // variable to read the value from the analog pin

// Structure example to receive data
// Must match the sender structure
typedef struct buttonState_set {
  int power = 0;      // 0 for not pressed, 1 for pressed
  int motorUp = 0;    // 0 for not pressed, 1 for pressed
  int motorDown = 0;  // 0 for not pressed, 1 for pressed
  int nextImg = 0;    // 0 for not pressed, 1 for pressed, motor does not care
  int prevImg = 0;    // 0 for not pressed, 1 for pressed, motor does not care
} buttonState_struct;

// Create a struct_message called myData
buttonState_struct myData;
bool onOff = 0; //0 for off, 1 for on
int motorSpeed = 90; //90 for off, 65 for highest speed desired, 75 for slowest

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  myData.power = 0; myData.motorUp = 0; myData.motorDown = 0;
  myData.nextImg = 0; myData.prevImg = 0;
  memcpy(&myData, incomingData, sizeof(myData));
  myData.nextImg = 0; myData.prevImg = 0;
  Serial.println(myData.power);
  Serial.println(myData.motorUp);
  Serial.println(myData.motorDown);
  Serial.println(myData.nextImg);
  Serial.println(myData.prevImg);
  if (myData.power) {
    //the power button was pressed
    if (!onOff) //if off
    {
      onOff = 1;
      //turn the motor on at slowest speed
      motorSpeed = minSpeed; //75 is the slowest speed we want
    }
    else {
      //turn the motor off
      onOff = 0;
      motorSpeed = 90; //90 is off
    }

  }
  else if (myData.motorUp) 
  {
    //increase the motor speed
    if (motorSpeed > maxSpeed && motorSpeed <= minSpeed) 
    {
      motorSpeed = motorSpeed - 2;
    }
  }
  else if (myData.motorDown)
  {
    //decrease the motor speed
    if (motorSpeed < minSpeed && motorSpeed >= maxSpeed) 
    {
      motorSpeed = motorSpeed + 2;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  // Initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);// Standard 50hz servo
  myservo.attach(servoPin, 500, 2500);   // attaches the servo on pin 18 to the servo object
                                         // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  //val = analogRead(potPin);             // read the value of the potentiometer (value between 0 and 1023)
  //val = map(val, 0, ADC_Max, 0, 180);   // scale it to use it with the servo (value between 0 and 180)
  //val = map(val, 0, ADC_Max, maxSpeed, 90);   //limiting motor speed to values previously discovered experimentaly
  myservo.write(motorSpeed);              // set the servo position according to the scaled value
  Serial.print(F("Using PWM Freq = ")); 
  Serial.println(motorSpeed);
  delay(200);                          // wait for the servo to get there
}
