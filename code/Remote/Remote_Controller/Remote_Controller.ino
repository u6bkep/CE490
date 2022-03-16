//Remote Controller Code

// Required Libraries Importation
#include <esp_now.h>
#include <WiFi.h>

//Define the buttons
//Only RTC IO can be used as a source for external wake
//source. They are pins: 0,2,4,12-15,25-27,32-39.
#define powerButton_pin 33 // pin 34 connected to Red button
#define motorUpButton_pin 21 // pin 21 connected to Yellow button
#define motorDownButton_pin 18 // pin 18 connected to Black button
#define fanUpButton_pin 22 // pin 23 connected to Blue button
#define fanDownButton_pin 23 // pin 22 connected to Green button

//#define BUTTON_PIN_BITMASK 0x30E000000

//RTC_DATA_ATTR int bootCount = 0;
bool boardPowered = 0;      // variable for PCB LED board being powered
//int iValueToSend = 0; //1 power, 2 motorDown, 3 motorUp, 4 prevImage, 5 nextImage

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

/*
  Method to print the reason by which ESP32
  has been awaken from sleep

  void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
  }*/


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
  while (!Serial){}

  //Increment boot number and print it every reboot
  //++bootCount;
  //Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  //print_wakeup_reason();

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

  //fill structs with zeros
  memset(&peerInfo, 0, sizeof(esp_now_peer_info_t));

  memcpy(peerInfo.peer_addr, broadcastAddressMotor, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  peerInfo.ifidx = (wifi_interface_t)0;

  // Add Motor MCU as a peer
  esp_err_t rv = esp_now_add_peer(&peerInfo);
  if (rv != ESP_OK) {
    Serial.print("Failed to add motor peer: ");
    Serial.println(esp_err_to_name(rv));
    //return;
  }
  else
  {
    char S[40];
    snprintf(S, sizeof(S), "added motor peer @: %02x:%02x:%02x:%02x:%02x:%02x",
           broadcastAddressMotor[0], broadcastAddressMotor[1], broadcastAddressMotor[2], broadcastAddressMotor[3], broadcastAddressMotor[4], broadcastAddressMotor[5]);
    Serial.println(S);
  }

  // Register Fan peer
  memset(&peerInfo, 0, sizeof(esp_now_peer_info_t));
  memcpy(peerInfo.peer_addr, broadcastAddressFan, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  peerInfo.ifidx = (wifi_interface_t)0;

  // Add Fan MCU as a peer
  rv = esp_now_add_peer(&peerInfo);
  if (rv != ESP_OK) {
    Serial.print("Failed to add fan peer: ");
    Serial.println(esp_err_to_name(rv));
    //return;
  }
  else
  {
    char S[40];
    snprintf(S, sizeof(S), "added fan peer @: %02x:%02x:%02x:%02x:%02x:%02x",
           broadcastAddressFan[0], broadcastAddressFan[1], broadcastAddressFan[2], broadcastAddressFan[3], broadcastAddressFan[4], broadcastAddressFan[5]);
    Serial.println(S);
  }

  esp_now_peer_num_t n;
  esp_now_get_peer_num(&n);
  Serial.print("number of configured peerd: ");
  Serial.println(n.total_num);


  //*****this is a test area**********
  // initialize these pins as inputs:
  pinMode(powerButton_pin, INPUT_PULLUP);
  pinMode(motorUpButton_pin, INPUT_PULLUP);
  pinMode(motorDownButton_pin, INPUT_PULLUP);
  pinMode(fanUpButton_pin, INPUT_PULLUP);
  pinMode(fanDownButton_pin, INPUT_PULLUP);

  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_25,0); //1 = High, 0 = Low
  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_26,0); //1 = High, 0 = Low
  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_27,0); //1 = High, 0 = Low
  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_32,0); //1 = High, 0 = Low
  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0); //1 = High, 0 = Low

  //esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

  //go to sleep now
  //Serial.println("Going to sleep now");
  delay(500);
  //esp_deep_sleep_start();
  //anything past this will not be used
}

esp_err_t result;
void loop() {
  //go to sleep immediately to save power

  // read the state of the pushbutton value:
  pButtonState.power = digitalRead(powerButton_pin);
  pButtonState.motorUp = digitalRead(motorUpButton_pin);
  pButtonState.motorDown = digitalRead(motorDownButton_pin);
  pButtonState.nextImg = digitalRead(fanUpButton_pin);
  pButtonState.prevImg = digitalRead(fanDownButton_pin);

  while(
    (!digitalRead(powerButton_pin) ) ||
    (!digitalRead(motorUpButton_pin) ) ||
    (!digitalRead(motorDownButton_pin) ) ||
    (!digitalRead(fanUpButton_pin) ) ||
    (!digitalRead(fanDownButton_pin) ) )
        {
          delay(50);
        }

  
  //result = -1;

  //check to see if mesage sent and variable of check is result
  //esp_now_send (0 ... is to send message to all 'peers' if want to
  //send to individual 'peer' put broadcastAddress# where # is their #
  //esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) & variable, sizeof(variabl_struct));

  //get values of what is currently on the fan
  //pMotorData = ???;
  //pFanData = ???;
  //boardPowered = ???;

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (pButtonState.power == LOW) {
    pButtonState.power = 1; pButtonState.motorUp = 0;
    pButtonState.motorDown = 0; pButtonState.nextImg = 0; pButtonState.prevImg = 0;
    // check if device is already on turn the device on: (ask microcontroller on PCB what the current status is
    //turn on the LED PCB, send signal to start the fan with lowest speed and first image
    Serial.println("Attempting to turn on fan and loading first image");
    Serial.print("power button = ");
    Serial.println(pButtonState.power);
    //result = esp_now_send(0, (uint8_t *) &pButtonState, sizeof(pButtonState)); //address of peer, address of data sent, length of data being sent
    result = esp_now_send(broadcastAddressFan, (uint8_t *) &pButtonState, sizeof(buttonState_struct));
    delay(50);
    result = esp_now_send(broadcastAddressMotor, (uint8_t *) &pButtonState, sizeof(buttonState_struct));
  }
  else if (pButtonState.motorDown == LOW) {
    pButtonState.power = 0; pButtonState.motorUp = 0;
    pButtonState.motorDown = 1; pButtonState.nextImg = 0; pButtonState.prevImg = 0;
    //Send signal to lower the speed of the board by one step
    Serial.println("Decreasing motor speed.");
    /*Serial.printf("pButtonState.power = %d\npButtonState.motorUp = %d\npButtonState.motorDown = %d\npButtonState.nextImg = %d\npButtonState.prevImg = %d\n", 
                 pButtonState.power, 
                 pButtonState.motorUp,
                 pButtonState.motorDown,
                 pButtonState.nextImg, 
                 pButtonState.prevImg);*/
    result = esp_now_send(broadcastAddressMotor, (uint8_t *) &pButtonState, sizeof(buttonState_struct)); //address of peer, address of data sent, length of data being sent
  }
  else if (pButtonState.motorUp == LOW) {
    pButtonState.power = 0; pButtonState.motorUp = 1;
    pButtonState.motorDown = 0; pButtonState.nextImg = 0; pButtonState.prevImg = 0;
    //Send signal to raise the fan blade speed by one step
    Serial.println("Increasing motor speed.");
    /*Serial.printf("pButtonState.power = %d\npButtonState.motorUp = %d\npButtonState.motorDown = %d\npButtonState.nextImg = %d\npButtonState.prevImg = %d\n", 
                 pButtonState.power, 
                 pButtonState.motorUp,
                 pButtonState.motorDown,
                 pButtonState.nextImg, 
                 pButtonState.prevImg);*/
    result = esp_now_send(broadcastAddressMotor, (uint8_t *) &pButtonState, sizeof(buttonState_struct)); //address of peer, address of data sent, length of data being sent
  }
  else if (pButtonState.prevImg == LOW) {
    pButtonState.power = 0; pButtonState.motorUp = 0;
    pButtonState.motorDown = 0; pButtonState.nextImg = 0; pButtonState.prevImg = 1;
    //Send Signal to go to the previous image
    Serial.println("Going to previous image.");
    /*Serial.printf("pButtonState.power = %d\npButtonState.motorUp = %d\npButtonState.motorDown = %d\npButtonState.nextImg = %d\npButtonState.prevImg = %d\n", 
                 pButtonState.power, 
                 pButtonState.motorUp,
                 pButtonState.motorDown,
                 pButtonState.nextImg, 
                 pButtonState.prevImg);*/
    result = esp_now_send(broadcastAddressFan, (uint8_t *) &pButtonState, sizeof(buttonState_struct)); //address of peer, address of data sent, length of data being sent
    delay(500);
  }
  else if (pButtonState.nextImg == LOW) {
    pButtonState.power = 0; pButtonState.motorUp = 0;
    pButtonState.motorDown = 0; pButtonState.nextImg = 1; pButtonState.prevImg = 0;
    //Send signal to go to the next image
    Serial.println("Going to next image.");
    /*Serial.printf("pButtonState.power = %d\npButtonState.motorUp = %d\npButtonState.motorDown = %d\npButtonState.nextImg = %d\npButtonState.prevImg = %d\n", 
                 pButtonState.power, 
                 pButtonState.motorUp,
                 pButtonState.motorDown,
                 pButtonState.nextImg, 
                 pButtonState.prevImg);*/
    result = esp_now_send(broadcastAddressFan, (uint8_t *) &pButtonState, sizeof(buttonState_struct)); //address of peer, address of data sent, length of data being sent
    delay(500);
  }

  //check to see if messge was sent successfully
  if (result == ESP_OK) {
    //Serial.println("Sent with success");
    delay(50);//for debugging purposes
  }
  else
  {
    Serial.println(esp_err_to_name(result));
    result = ESP_OK;
    delay(500);
  }
}
