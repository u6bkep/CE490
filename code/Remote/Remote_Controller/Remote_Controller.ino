//Remote Controller Code

// Required Libraries Importation
#include <esp_now.h>
#include <WiFi.h>

//Define the buttons
#define powerButton_pin 34 // pin 34 connected to Red button
#define motorUpButton_pin 21 // pin 21 connected to Yellow button
#define motorDownButton_pin 18 // pin 18 connected to Black button
#define fanUpButton_pin 23 // pin 23 connected to Blue button
#define fanDownButton_pin 22 // pin 22 connected to Green button

#define BUTTON_PIN_BITMASK ((1UL << powerButton_pin) | (1UL << motorUpButton_pin) | (1UL << motorDownButton_pin) | (1UL << fanUpButton_pin) | (1UL << fanDownButton_pin)) 

bool boardPowered = 0;      // variable for PCB LED board being powered

//data to send for Button Status
typedef struct buttonState_set {
  bool power = 0;      // 0 for not pressed, 1 for pressed
  bool motorUp = 0;    // 0 for not pressed, 1 for pressed
  bool motorDown = 0;  // 0 for not pressed, 1 for pressed
  bool nextImg = 0;    // 0 for not pressed, 1 for pressed
  bool prevImg = 0;    // 0 for not pressed, 1 for pressed
} buttonState_struct;

//variable for storing values send to board
buttonState_struct pButtonState;

// REPLACE WITH THE MAC Address of other two boards (recievers)
uint8_t broadcastAddressMotor[] = {0x40, 0x91, 0x51, 0x9B, 0x60, 0x44}; //motor 40:91:51:9B:60:44
uint8_t broadcastAddressFan[] = {0x40, 0x91, 0x51, 0x9A, 0xDF, 0x14}; //fan 40:91:51:9A:DF:14
//uint8_t broadcastAddressRemote[] = {0x40, 0x91, 0x51, 0x9B, 0xE6, 0x9C} //remote 40:91:51:9B:E6:9C

//need to get MAC addresses again, these may not be the correct ones

//data to send for Motor microcrontroller
typedef struct powerMotor_set {
  int id; // must be unique for each sender board
  int motorSpeed = 0;
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

  // Register Motor peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddressMotor, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add Motor MCU as a peer   
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Register Fan peer
  memcpy(peerInfo.peer_addr, broadcastAddressFan, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add Fan MCU as a peer   
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }


  //*****this is a test area**********
  // initialize these pins as inputs:
  pinMode(powerButton_pin, INPUT);
  pinMode(motorUpButton_pin, INPUT);
  pinMode(motorDownButton_pin, INPUT);
  pinMode(fanUpButton_pin, INPUT);
  pinMode(fanDownButton_pin, INPUT);

  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
}

int iValueToSend = 0; //1 power, 2 motorDown, 3 motorUp, 4 prevImage, 5 nextImage

void loop() {
  //go to sleep immediately to save power
  esp_deep_sleep_start();

  // read the state of the pushbutton value:
  pButtonState.power = digitalRead(powerButton_pin);
  pButtonState.motorUp = digitalRead(powerButton_pin);
  pButtonState.motorDown = digitalRead(powerButton_pin);
  pButtonState.nextImg = digitalRead(powerButton_pin);
  pButtonState.prevImg = digitalRead(powerButton_pin);

  esp_err_t result;

  //check to see if mesage sent and variable of check is result
  //esp_now_send (0 ... is to send message to all 'peers' if want to
  //send to individual 'peer' put broadcastAddress# where # is their #
  //esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) & variable, sizeof(variabl_struct));

  //get values of what is currently on the fan
  //pMotorData = ???;
  //pFanData = ???;
  //boardPowered = ???;

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (pButtonState.power == HIGH) {
    // check if device is already on turn the device on: (ask microcontroller on PCB what the current status is
    if (boardPowered) {
      //turn on the LED PCB, send signal to start the fan with lowest speed and first image
      result = esp_now_send(0, (uint8_t *) &iValueToSend, sizeof(uint8_t)); //address of peer, address of data sent, length of data being sent
    }
    else {
      //turn off the LED PCB, send signal to stop spinning and turn LEDs off
      result = esp_now_send(0, (uint8_t *) &iValueToSend, sizeof(uint8_t)); //address of peer, address of data sent, length of data being sent
    }
  }
  else if (pButtonState.motorDown == HIGH) {
    // check if device is already at lowest speed
    if (pMotorData.motorSpeed > 0) { //***********need to get value for lowest speed possible*************
      //Send signal to lower the speed of the board by one step
      result = esp_now_send(0, (uint8_t *) &iValueToSend, sizeof(uint8_t)); //address of peer, address of data sent, length of data being sent
    }
  }
  else if (pButtonState.motorUp == HIGH) {
    // check if device is already on highest speed
    if (pMotorData.motorSpeed < 5) { //***********need to get value for highest speed possible*************
      //Send signal to raise the fan blade speed by one step
      result = esp_now_send(0, (uint8_t *) &iValueToSend, sizeof(uint8_t)); //address of peer, address of data sent, length of data being sent
    }
  }
  else if (pButtonState.prevImg == HIGH) {
    //Send Signal to go to the previous image
    result = esp_now_send(0, (uint8_t *) &iValueToSend, sizeof(uint8_t)); //address of peer, address of data sent, length of data being sent
  }
  else if (pButtonState.nextImg == HIGH) {
    //Send signal to go to the next image
    result = esp_now_send(0, (uint8_t *) &iValueToSend, sizeof(uint8_t)); //address of peer, address of data sent, length of data being sent
  }

  //check to see if messge was sent successfully
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(1000);//eventually be removed
}
