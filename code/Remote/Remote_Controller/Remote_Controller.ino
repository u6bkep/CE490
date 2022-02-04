//Remote Controller Code

// Required Libraries Importation
#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH THE MAC Address of other two boards (recievers) 
uint8_t broadcastAddress1[] = {0x40, 0x91, 0x51, 0x9B, 0xE6, 0x9C};
uint8_t broadcastAddress2[] = {0xAC, 0x67, 0xB2, 0x2B, 0x64, 0xF4};

//data wanting to send (ll data in one structure as unsure which board needs what info)
/*{typedef struct data_set {
  int motorSpeed;
  int fanIncrement;
  int onOFF;
} data_struct;

// variable for storing the values to send to board
data_set outputs; */

//for testing
typedef struct struct_message {
    int id; // must be unique for each sender board
    int x;
    int y;
} struct_message;

struct_message myData;

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
 
  // testing
  esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
  myData.id = 1;
  myData.x = random(0,50);
  myData.y = random(0,50);
  
 //check to see if messge was sent successfully
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(10000);
}
