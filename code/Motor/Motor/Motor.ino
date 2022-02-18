//Motor Controller Code

//Required Libraries Importation
#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct buttonState_set {
  bool power = 0;      // 0 for not pressed, 1 for pressed
  bool motorUp = 0;    // 0 for not pressed, 1 for pressed
  bool motorDown = 0;  // 0 for not pressed, 1 for pressed
  bool nextImg = 0;    // 0 for not pressed, 1 for pressed, motor does not care
  bool prevImg = 0;    // 0 for not pressed, 1 for pressed, motor does not care
} struct_message;

// Create a struct_message called myData
buttonState_struct myData;
bool onOff = 0; //0 for off, 1 for on
int motorSpeed = 0; //lowest speed is 0, max is 5?

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  if (MyData.power) {
    //the power button was pressed
    if (!onOff)
    {
      //turn the motor on
    }
    else {
      //turn the motor off
    }

  }
  else if (MyData.MotorUp) {
    //increase the motor speed
    if (maxSpeed < 5) {
      maxSpeed = maxSpeed + 1;
    }
  }
  else if (MyData.MotorDown) {
    //decrease the motor speed
    if (maxSpeed > 0) {
      maxSpeed = maxSpeed - 1;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  // Initialize Serial Monitor
  Serial.begin(115200);
  
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
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
}

void loop() {
  //go to sleep immediately to save power
  esp_deep_sleep_start();
}
