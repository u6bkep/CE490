//FanPCB Controller Code

//Required Libraries Importation
#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include <esp_now.h>
//#include <SPI.h>
#include <PNGdec.h>
#include <WiFi.h>
#include "image.hpp"


PngRenderer imageRenderer;


// Structure example to receive data
// Must match the sender structure
typedef struct buttonState_set {
  bool power = 0;      // 0 for not pressed, 1 for pressed
  bool motorUp = 0;    // 0 for not pressed, 1 for pressed, fan does not care
  bool motorDown = 0;  // 0 for not pressed, 1 for pressed, fan does not care
  bool nextImg = 0;    // 0 for not pressed, 1 for pressed
  bool prevImg = 0;    // 0 for not pressed, 1 for pressed
} buttonState_struct;

// Create a struct_message called myData
buttonState_struct myData;
bool onOff = 0; //0 for off, 1 for on
int fanImage = 0; //0 is no image
int totalImages = 0;


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println("recieved button press");
  //Serial.printf("myData.power = %d\nmyData.nextImg = %d\nmyData.prevImg = %d\n", myData.power, myData.nextImg, myData.prevImg);
  if (myData.power) {
    //the power button was pressed
    
      //turn the motor off
      fanImage = 0;//no image
      imageRenderer.chooseImage(fanImage);
  }
  else if (myData.nextImg) {
    //change to the next image
    fanImage++;
    if (fanImage >= totalImages)
    {
      fanImage = 1;
    }
    imageRenderer.chooseImage(fanImage);
    Serial.print("Setting image #");
    Serial.println(fanImage);
  }
  else if (myData.prevImg) {
    //change to the previous image
    fanImage--;
    if (fanImage < 1)
    {
      fanImage = totalImages - 1;
    }
    imageRenderer.chooseImage(fanImage);
    Serial.print("Setting image #");
    Serial.println(fanImage);
  }
}

void setup() {
  // put your setup code here, to run once:

  //setup serial printing
  Serial.begin(115200);
  while(!Serial);
  delay(50);
  Serial.println("booting...");

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

  //init image renderer
  imageRenderer.init();
  totalImages = imageRenderer.getNumberOfImages();
}

void loop() {
  //code to loop forever
  imageRenderer.renderTask();
}

//int timer = 800;    
//int U18 = 7;
//int State = 0;         
//int lastState = 0;     

//int m;
//
//void setup() {
//  for (int Pin = 2; Pin <=6; Pin++) {
//    pinMode(Pin, OUTPUT);}
//  pinMode(U18, INPUT);
//}
//
//void loop() {
//State = digitalRead(U18);
//if (State != lastState){
//  if (State == HIGH){
//delayMicroseconds(5000);
//WriteLetter3Width(H);
//WriteLetter1Width(space);
//WriteLetter3Width(A);
//WriteLetter1Width(space);
//WriteLetter3Width(C);
//WriteLetter1Width(space);
//WriteLetter3Width(K);
//WriteLetter1Width(space);
//WriteLetter3Width(E);
//WriteLetter1Width(space);
//WriteLetter3Width(D);
//WriteLetter1Width(space);
//WriteLetter2Width(exclamationmark);
//WriteLetter1Width(space);}}
//lastState = State;
//}
//
//void WriteLetter4Width(boolean letter[]){
//  m=0;
//  for (int n=0; n<=3; n++){
//  for (int Pin = 2; Pin <=6; Pin++) {
//    digitalWrite(Pin, letter[m]);
//    m=m+1;}
//    delayMicroseconds(timer);}}
//
//void WriteLetter3Width(boolean letter[]){
//  m=0;
//  for (int n=0; n<=2; n++){
//  for (int Pin = 2; Pin <=6; Pin++) {
//    digitalWrite(Pin, letter[m]);
//    m=m+1;}
//    delayMicroseconds(timer);}}
//
// void WriteLetter2Width(boolean letter[]){
//  m=0;
//  for (int n=0; n<=1; n++){
//  for (int Pin = 2; Pin <=6; Pin++) {
//    digitalWrite(Pin, letter[m]);
//    m=m+1;}
//    delayMicroseconds(timer);}}
// 
// void WriteLetter1Width(boolean letter[]){
//  m=0;
//   for (int Pin = 2; Pin <=6; Pin++) {
//    digitalWrite(Pin, space[m]);
//    m=m+1;}
//    delayMicroseconds(timer);}
//*/
