//FanPCB Controller Code

//Required Libraries Importation
#include <esp_now.h>
#include <WiFi.h>

#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

#define NUMPIXELS 30 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    4
#define CLOCKPIN   5
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip(NUMPIXELS, DOTSTAR_BRG);

//make everything 7H x 5W which is enough to have three rows of text
//test area for letters
boolean A[]={0,0,1,1,1,1,1, 0,1,0,1,0,0,0, 1,0,0,1,0,0,0, 0,1,0,1,0,0,0, 0,0,1,1,1,1,1};
boolean B[]={1,1,1,1,1,1,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 0,1,1,0,1,1,0};
boolean C[]={1,1,1,1,1,1,1, 1,1,1,1,1,1,1, 1,1,1,1,1,1,1, 1,1,1,1,1,1,1, 1,1,1,1,1,1,1};
boolean D[]={1,1,1,1,1,1,1, 1,0,0,0,0,0,1, 1,0,0,0,0,0,1, 1,0,0,0,0,0,1, 0,1,1,1,1,1,0};
boolean E[]={1,1,1,1,1,1,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 1,0,0,0,0,0,1};
boolean F[]={1,1,1,1,1,1,1, 1,0,0,1,0,0,0, 1,0,0,1,0,0,0, 1,0,0,1,0,0,0, 1,0,0,0,0,0,0};
boolean G[]={0,1,1,1,1,1,0, 1,0,0,0,0,0,1, 1,0,0,0,1,0,1, 1,0,0,0,1,0,1, 0,1,0,0,1,1,0};
boolean H[]={1,1,1,1,1,1,1, 0,0,0,1,0,0,0, 0,0,0,1,0,0,0, 0,0,0,1,0,0,0, 1,1,1,1,1,1,1};
boolean I[]={1,0,0,0,0,0,1, 1,0,0,0,0,0,1, 1,1,1,1,1,1,1, 1,0,0,0,0,0,1, 1,0,0,0,0,0,1};
boolean J[]={0,0,0,0,1,1,0, 0,0,0,0,0,0,1, 0,0,0,0,0,0,1, 0,0,0,0,0,0,1, 1,1,1,1,1,1,0};
boolean K[]={1,1,1,1,1,1,1, 0,0,0,1,0,0,0, 0,0,1,0,1,0,0, 0,1,0,0,0,1,0, 1,0,0,0,0,0,1};
boolean L[]={1,1,1,1,1,1,1, 0,0,0,0,0,0,1, 0,0,0,0,0,0,1, 0,0,0,0,0,0,1, 0,0,0,0,0,0,1};
boolean M[]={1,1,1,1,1,1,1, 0,1,0,0,0,0,0, 0,0,1,0,0,0,0, 0,1,0,0,0,0,0, 1,1,1,1,1,1,1};
boolean N[]={1,1,1,1,1,1,1, 0,0,1,0,0,0,0, 0,0,0,1,0,0,0, 0,0,0,0,1,0,0, 1,1,1,1,1,1,1};
boolean O[]={0,1,1,1,1,1,0, 1,0,0,0,0,0,1, 1,0,0,0,0,0,1, 1,0,0,0,0,0,1, 0,1,1,1,1,1,0};
boolean P[]={1,1,1,1,1,1,1, 1,0,0,1,0,0,0, 1,0,0,1,0,0,0, 1,0,0,1,0,0,0, 0,1,1,0,0,0,0};
boolean Q[]={0,1,1,1,1,1,0, 1,0,0,0,0,0,1, 1,0,0,0,0,1,1, 1,0,0,0,0,0,1, 0,1,1,1,1,1,0};
boolean R[]={1,1,1,1,1,1,1, 1,0,0,1,0,0,0, 1,0,0,1,0,0,0, 1,0,0,1,0,0,0, 0,1,1,0,1,1,1};
boolean S[]={0,1,1,0,0,1,0, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 0,1,0,0,1,1,0};
boolean T[]={1,0,0,0,0,0,0, 1,0,0,0,0,0,0, 1,1,1,1,1,1,1, 1,0,0,0,0,0,0, 1,0,0,0,0,0,0};
boolean U[]={1,1,1,1,1,1,0, 0,0,0,0,0,0,1, 0,0,0,0,0,0,1, 0,0,0,0,0,0,1, 1,1,1,1,1,1,0};
boolean V[]={1,1,1,1,1,0,0, 0,0,0,0,0,1,0, 0,0,0,0,0,0,1, 0,0,0,0,0,1,0, 1,1,1,1,1,0,0};
boolean W[]={1,1,1,1,1,1,1, 0,0,0,0,0,1,0, 0,0,0,0,1,0,0, 0,0,0,0,0,1,0, 1,1,1,1,1,1,1};
boolean X[]={1,1,0,0,0,1,1, 0,0,1,0,1,0,0, 0,0,0,1,0,0,0, 0,0,1,0,1,0,0, 1,1,0,0,0,1,1};
boolean Y[]={1,1,1,0,0,0,0, 0,0,0,1,0,0,0, 0,0,0,0,1,1,1, 0,0,0,1,0,0,0, 1,1,1,0,0,0,0};
boolean Z[]={1,0,0,0,0,1,1, 1,0,0,0,1,0,1, 1,0,0,1,0,0,1, 1,0,1,0,0,0,1, 1,1,0,0,0,0,1};

boolean a[]={0,0,1,0,0,1,0, 0,0,1,0,1,0,1, 0,0,1,0,1,0,1, 0,0,1,0,1,0,1, 0,0,0,1,1,1,1};
boolean b[]={0,1,1,1,1,1,1, 0,0,0,1,0,0,1, 0,0,0,1,0,0,1, 0,0,0,1,0,0,1, 0,0,0,0,1,1,0};
boolean c[]={0,0,0,1,1,1,0, 0,0,1,0,0,0,1, 0,0,1,0,0,0,1, 0,0,1,0,0,0,1, 0,0,0,1,0,0,1};
boolean d[]={0,0,0,0,1,1,0, 0,0,0,1,0,0,1, 0,0,0,1,0,0,1, 0,0,0,1,0,0,1, 0,1,1,1,1,1,1};
boolean e[]={0,0,0,1,1,1,0, 0,0,1,0,1,0,1, 0,0,1,0,1,0,1, 0,0,1,0,1,0,1, 0,0,0,1,0,0,1};
boolean f[]={0,0,0,0,1,0,0, 0,0,1,1,1,1,1, 0,1,0,0,1,0,0, 0,1,0,0,1,0,0, 0,1,0,0,0,0,0};
boolean g[]={0,1,1,0,1,1,0, 1,0,1,1,0,0,1, 1,0,1,0,1,0,1, 1,1,1,0,1,0,1, 1,0,0,0,1,1,0};
boolean h[]={1,1,1,1,1,1,1, 0,0,0,1,0,0,0, 0,0,1,0,0,0,0, 0,0,1,0,0,0,0, 0,0,0,1,1,1,1};
boolean i[]={0,0,0,0,0,0,0, 0,0,1,0,0,0,1, 1,0,1,1,1,1,1, 0,0,0,0,0,0,1, 0,0,0,0,0,0,0};
boolean j[]={0,0,0,0,0,1,0, 0,0,0,0,0,0,1, 0,0,0,0,0,0,1, 1,0,1,1,1,1,0, 0,0,0,0,0,0,0};
boolean k[]={1,1,1,1,1,1,1, 0,0,0,1,0,0,0, 0,0,0,1,0,0,0, 0,0,1,0,1,0,0, 0,1,0,0,0,1,1};
boolean l[]={0,0,0,0,0,0,0, 1,0,0,0,0,0,1, 1,1,1,1,1,1,1, 0,0,0,0,0,0,1, 0,0,0,0,0,0,0};
boolean m[]={0,0,1,1,1,1,1, 0,0,1,0,0,0,0, 0,0,0,1,1,1,1, 0,0,1,0,0,0,0, 0,0,0,1,1,1,1};
boolean n[]={0,0,1,1,1,1,1, 0,0,0,1,0,0,0, 0,0,1,0,0,0,0, 0,0,1,0,0,0,0, 0,0,0,1,1,1,1};
boolean o[]={0,0,0,1,1,1,0, 0,0,1,0,0,0,1, 0,0,1,0,0,0,1, 0,0,1,0,0,0,1, 0,0,0,1,1,1,0};
boolean p[]={0,0,1,1,1,1,1, 0,0,1,0,1,0,0, 0,0,1,0,1,0,0, 0,0,0,1,0,0,0, 0,0,0,0,0,0,0};
boolean q[]={0,0,0,1,1,1,0, 0,0,1,0,0,0,1, 0,0,1,0,0,1,1, 0,0,1,0,0,0,1, 0,0,0,1,1,1,0};
boolean r[]={0,0,0,0,0,0,1, 0,0,1,1,1,1,1, 0,0,0,1,0,0,1, 0,0,1,0,0,0,0, 0,0,0,1,0,0,0};
boolean s[]={0,0,0,1,0,0,1, 0,0,1,0,1,0,1, 0,0,1,0,1,0,1, 0,0,1,0,1,0,1, 0,0,1,0,0,1,0};
boolean t[]={0,0,0,0,0,0,0, 1,1,1,1,1,1,0, 0,0,1,0,0,0,1, 0,0,1,0,0,0,1, 0,0,0,0,0,1,0};
boolean u[]={0,0,1,1,1,1,0, 0,0,0,0,0,0,1, 0,0,0,0,0,0,1, 0,0,0,0,0,1,0, 0,0,1,1,1,1,1};
boolean v[]={0,0,1,1,0,0,0, 0,0,0,0,1,1,0, 0,0,0,0,0,0,1, 0,0,0,0,1,1,0, 0,0,1,1,0,0,0};
boolean w[]={0,0,1,1,1,1,0, 0,0,0,0,0,0,1, 0,0,0,0,1,1,0, 0,0,0,0,0,0,1, 0,0,1,1,1,1,0};
boolean x[]={0,0,1,0,0,0,1, 0,0,0,1,0,1,0, 0,0,0,0,1,0,0, 0,0,0,1,0,1,0, 0,0,1,0,0,0,1};
boolean y[]={0,0,1,1,1,0,1, 0,0,0,0,1,0,1, 0,0,0,0,1,0,1, 0,0,0,0,1,0,1, 0,0,1,1,1,1,0};
boolean z[]={0,0,1,0,0,0,1, 0,0,1,0,0,1,1, 0,0,1,0,1,0,1, 0,0,1,1,0,0,1, 0,0,1,0,0,0,1};

//numbers
boolean zero[]  ={0,1,1,1,1,1,0, 1,0,0,0,1,0,1, 1,0,0,1,0,0,1, 1,0,1,0,0,0,1, 0,1,1,1,1,1,0};
boolean one[]   ={0,0,0,0,0,0,0, 1,0,0,0,0,0,1, 1,1,1,1,1,1,1, 0,0,0,0,0,0,1, 0,0,0,0,0,0,0};
boolean two[]   ={0,1,0,0,1,1,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 0,1,1,0,0,0,1};
boolean three[] ={0,1,0,0,0,1,0, 1,0,0,0,0,0,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 0,1,1,0,1,1,0};
boolean four[]  ={1,1,1,1,0,0,0, 0,0,0,1,0,0,0, 0,0,0,1,0,0,0, 1,1,1,1,1,1,1, 0,0,0,1,0,0,0};
boolean five[]  ={1,1,1,1,0,0,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 1,0,0,0,1,1,0};
boolean six[]   ={0,1,1,1,1,1,0, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 1,0,0,0,1,1,0};
boolean seven[] ={1,0,0,0,0,0,0, 1,0,0,0,1,1,1, 1,0,0,1,0,0,0, 1,0,0,1,0,0,0, 1,1,1,0,0,0,0};
boolean eight[] ={0,1,1,0,1,1,0, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 0,1,1,0,1,1,0};
boolean nine[]  ={0,1,1,0,0,1,0, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 0,1,1,1,1,1,0};

//special characters if we want them to take up a full area
boolean space[]       ={0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0};
boolean period[]      ={0,0,0,0,0,0,0, 0,0,0,0,0,1,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0};
boolean comma[]       ={0,0,0,0,0,0,0, 0,0,0,0,1,0,1, 0,0,0,0,1,1,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0};
boolean colon[]       ={0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,1,0,0,1,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0};
boolean semicolon[]   ={0,0,0,0,0,0,0, 0,0,0,0,0,0,1, 0,0,1,0,0,1,1, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0};
boolean apostrophe[]  ={0,0,0,0,0,0,0, 1,1,1,0,0,0,0, 0,0,0,0,0,0,0, 1,1,1,0,0,0,0, 0,0,0,0,0,0,0};
boolean exclamation[] ={0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,1,1,1,1,0,1, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0};
boolean question[]    ={0,1,0,0,0,0,0, 1,0,0,0,0,0,0, 1,0,0,1,1,0,1, 1,0,0,1,0,0,0, 0,1,1,0,0,0,0};
boolean at[]          ={0,1,1,1,1,1,0, 1,0,0,0,0,0,1, 1,0,1,1,0,0,1, 1,0,1,1,0,0,1, 0,1,1,1,0,1,0};
boolean pound[]       ={0,1,0,0,1,0,0, 1,1,1,1,1,1,1, 0,1,0,0,1,0,0, 1,1,1,1,1,1,1, 0,1,0,0,1,0,0};
boolean dollar[]      ={0,0,1,0,0,1,0, 0,1,0,1,0,1,0, 1,1,1,1,1,1,1, 0,1,0,1,0,1,0, 0,1,0,0,1,0,0};
boolean percent[]     ={0,1,1,0,0,1,1, 1,0,1,0,1,0,0, 1,1,1,1,0,1,1, 0,0,1,0,1,0,1, 1,1,0,0,1,1,1};
boolean ampersand[]   ={0,1,1,0,1,1,0, 1,0,0,1,0,0,1, 1,0,0,1,0,0,1, 0,1,1,1,1,1,0, 0,0,0,1,0,0,1};
boolean star[]        ={0,0,0,0,0,0,0, 0,1,0,1,0,0,0, 1,1,1,0,0,0,0, 0,1,0,1,0,0,0, 0,0,0,0,0,0,0};
boolean forwardSlash[]={0,0,0,0,0,1,1, 0,0,0,0,1,0,0, 0,0,0,1,0,0,0, 0,0,1,0,0,0,0, 1,1,0,0,0,0,0};
boolean backSlash[]   ={1,1,0,0,0,0,0, 0,0,1,0,0,0,0, 0,0,0,1,0,0,0, 0,0,0,0,1,0,0, 0,0,0,0,0,1,1};

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

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  if (myData.power) {
    //the power button was pressed
    if (!onOff)
    {
      //turn the motor on
      fanImage = 1; //first image
    }
    else {
      //turn the motor off
      fanImage = 0;//no image
    }

  }
  else if (myData.nextImg) {
    //change to the next image
    fanImage = (fanImage + 1) % 3;
    if (fanImage == 0)
    {
      fanImage = 1;
    }
  }
  else if (myData.prevImg) {
    //change to the previous image
    fanImage = (fanImage - 1) % 3;
    if (fanImage == 0)
    {
      fanImage = 1;
    }
  }
}

void setup() {
  // put your setup code here, to run once:

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

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
}

// Runs 25 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

int      head  = 0, tail = -25; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

void loop() {
  //code to loop forever
  strip.setPixelColor(head, color); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(20);                        // Pause 20 milliseconds (~50 FPS)

  if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
    head = 0;                       //  Yes, reset head index to start
    if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
      color = 0xFF0000;             //   Yes, reset to red
  }
  if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
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
