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

//Button State Variables
int PlastState = LOW; //previous state from the power input pin
int PcurrentState; //current reading from power input pin
int MUlastState = LOW; //previous state from the motor up input pin
int MUcurrentState; //current reading from motor up input pin
int MDlastState = LOW; //previous state from the motor down input pin
int MDcurrentState; //current reading from motor down input pin
int FUlastState = LOW; //previous state from the fan up input pin
int FUcurrentState; //current reading from fan up input pin
int FDlastState = LOW; //previous state from the fan down input pin
int FDcurrentState; //current reading from fan down input pin

// REPLACE WITH THE MAC Address of other two boards (recievers) 
uint8_t broadcastAddress1[] = {0x40, 0x91, 0x51, 0x9B, 0xE6, 0x9C}; //motor
uint8_t broadcastAddress2[] = {0xAC, 0x67, 0xB2, 0x2B, 0x64, 0xF4}; //fan

//data to send for Motor microcrontroller
typedef struct powerMotor_set {
  int id; // must be unique for each sender board
  int motorSpeed = 3;
  int onOFF;
} powerMotor_struct;

//variable for storing values send to board
powerMotor_struct pMotorData;

//data to send for fan motorcontroller
typedef struct powerFan_set {
  int id; // must be unique for each sender board
  int fanImg;
  int onOFF;
} powerFan_struct;

//variable for storing values send to board
powerFan_struct pFanData; 


//Callback Funtion to know message was recieved or not
// callback when data is sent
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
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
//  register second ESP32 board 
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } 

}
 
void loop() {

 //check to see if mesage sent and variable of check is result
 //esp_now_send (0 ... is to send message to all 'peers' if want to 
 //send to individual 'peer' put broadcastAddress# where # is their #
 // esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) & variable, sizeof(variabl_struct));

  //send button press code to motor motorcontroller
  esp_err_t result = esp_now_send(0, (uint8_t *) &pMotorData, sizeof(pMotorData));
  pMotorData.id = 1;
  
  // read the state of the On/Off Button:
  PcurrentState = digitalRead(powerButton_pin);
  if (PlastState == LOW && PcurrentState == HIGH)
    pMotorData.onOFF = 1;
  else 
    pMotorData.onOFF = 0;
  // save the the last state
  PlastState = PcurrentState;

  // read the state of the Motor Up Button:
  MUcurrentState = digitalRead(motorUpButton_pin);
  if (MUlastState == LOW && MUcurrentState == HIGH)
    pMotorData.motorSpeed = pMotorData.motorSpeed + 1;
  else 
    pMotorData.motorSpeed = pMotorData.motorSpeed;
  // save the the last state
  MUlastState = MUcurrentState;
  
  // read the state of the On/Off Button:
  MDcurrentState = digitalRead(motorDownButton_pin);
  if (MDlastState == LOW && MDcurrentState == HIGH)
    pMotorData.motorSpeed = pMotorData.motorSpeed - 1;
  else 
    pMotorData.motorSpeed = pMotorData.motorSpeed;
  // save the the last state
  MDlastState = MDcurrentState;
  
 //check to see if messge was sent successfully
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(10000);
}
