//Remote Controller Code

// Required Libraries Importation
#include <esp_now.h>
#include <WiFi.h>

//Define the buttons
#define powerButton_pin 5 // pin 5 connected to red button
#define motorUpButton_pin 18 // pin 18 connected to blue button
#define motorDownButton_pin 19 // pin 19 connected to green button
#define fanUpButton_pin 25 // pin 25 connected to yellow button
#define fanDownButton_pin 33 // pin 33 connected to black button

#define BUTTON_PIN_BITMASK 0x2020C0020 // pin masks = 2^5 + 2^18 + 2^19 + 2^25 + 2^33 = 8624275488 in dec

bool boardPowered = 0;      // variable for PCB LED board being powered

//data to send for Button Status
typedef struct buttonState_set {
  int power = 0;      // 0 for not pressed, 1 for pressed
  int motorUp = 0;    // 0 for not pressed, 1 for pressed
  int motorDown = 0;  // 0 for not pressed, 1 for pressed
  int nextImg = 0;    // 0 for not pressed, 1 for pressed
  int prevImg = 0;    // 0 for not pressed, 1 for pressed
} buttonState_struct;

//variable for storing values send to board
buttonState_struct pButtonState;

// REPLACE WITH THE MAC Address of other two boards (recievers)
uint8_t broadcastAddress1[] = {0x40, 0x91, 0x51, 0x9B, 0xE6, 0x9C}; //motor
uint8_t broadcastAddress2[] = {0xAC, 0x67, 0xB2, 0x2B, 0x64, 0xF4}; //fan

//data to send for Motor microcrontroller
typedef struct powerMotor_set {
  int id; // must be unique for each sender board
  int motorSpeed = 3;
  bool onOFF;
} powerMotor_struct;

//variable for storing values send to board
powerMotor_struct pMotorData;

//data to send for fan motorcontroller
typedef struct powerFan_set {
  int id; // must be unique for each sender board
  int fanImg;
  bool onOFF;
} powerFan_struct;

//variable for storing values send to board
powerFan_struct pFanData;


// Callback Funtion to know message was recieved or not
// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet from: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {

  //initialise serial monitor for debugging purposes
  Serial.begin(115200);

  // set device as Wi-Fi station
  WiFi.mode(WIFI_STA);

  //initialise ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //registering the call back funtion to ESP-NOW
  esp_now_register_send_cb(OnDataSent);

  // set up to pair wiht other ESP-NOW devices
  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 0;
  peerInfo.encrypt = false;


  // register first ESP32 board
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  //  register second ESP32 board
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }


  //*****this is a test area**********
  // initialize the pins as an input:
  pinMode(powerButton_pin, INPUT);
  pinMode(motorUpButton_pin, INPUT);
  pinMode(motorDownButton_pin, INPUT);
  pinMode(fanUpButton_pin, INPUT);
  pinMode(fanDownButton_pin, INPUT);

  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
}

int iValueToSend = 0; //1 power, 2 motorDown, 3 motorUp, 4 prevImage, 5 nextImage

void loop() {
  //go to sleep
  esp_deep_sleep_start();
  //check to see if mesage sent and variable of check is result
  //esp_now_send (0 ... is to send message to all 'peers' if want to
  //send to individual 'peer' put broadcastAddress# where # is their #
  //esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) & variable, sizeof(variabl_struct));
  
  iValueToSend = 0;

  // read the state of the pushbutton value:
  pButtonState.power = digitalRead(powerButton_pin);
  pButtonState.motorUp = digitalRead(powerButton_pin);
  pButtonState.motorDown = digitalRead(powerButton_pin);
  pButtonState.nextImg = digitalRead(powerButton_pin);
  pButtonState.prevImg = digitalRead(powerButton_pin);

  //get values of what is currently on the fan
  //pMotorData = ???;
  //pFanData = ???;
  //boardPowered = ???;

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (pButtonState.power == HIGH) {
    // check if device is already on turn the device on: (ask microcontroller on PCB what the current status is
    iValueToSend = 1;
    if (boardPowered) {
      //turn on the LED PCB, send signal to start the fan with lowest speed and first image
      esp_err_t result = esp_now_send(0, (uint8_t *) &iValueToSend, sizeof(uint8_t)); //address of peer, address of data sent, length of data being sent
    }
    else {
      //turn off the LED PCB, send signal to stop spinning and turn LEDs off
      esp_err_t result = esp_now_send(0, (uint8_t *) &iValueToSend, sizeof(uint8_t)); //address of peer, address of data sent, length of data being sent
    }
  }
  else if (pButtonState.motorDown == HIGH) {
    // check if device is already at lowest speed
    iValueToSend = 2;
    if (pMotorData.motorSpeed > 0) { //***********need to get value for lowest speed possible*************
      //Send signal to lower the speed of the board by one step
      esp_err_t result = esp_now_send(0, (uint8_t *) &iValueToSend, sizeof(uint8_t)); //address of peer, address of data sent, length of data being sent
    }
  }
  else if (pButtonState.motorUp == HIGH) {
    // check if device is already on highest speed
    iValueToSend = 3;
    if (pMotorData.motorSpeed < 5) { //***********need to get value for highest speed possible*************
      //Send signal to raise the fan blade speed by one step
      esp_err_t result = esp_now_send(0, (uint8_t *) &iValueToSend, sizeof(uint8_t)); //address of peer, address of data sent, length of data being sent
    }
  }
  else if (pButtonState.prevImg == HIGH) {
    //Send Signal to go to the previous image
    iValueToSend = 4;
    esp_err_t result = esp_now_send(0, (uint8_t *) &iValueToSend, sizeof(uint8_t)); //address of peer, address of data sent, length of data being sent
  }
  else if (pButtonState.nextImg == HIGH) {
    //Send signal to go to the next image
    iValueToSend = 5;
    esp_err_t result = esp_now_send(0, (uint8_t *) &iValueToSend, sizeof(uint8_t)); //address of peer, address of data sent, length of data being sent
  }

  //check to see if messge was sent successfully
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(1000);//evenytually be removed
}
