#include "EEPROM.h"
#include <Adafruit_DotStar.h>
#include <SPI.h>


//#include "pineapplePolar small.h"
#include "bulldog.h"

#define NUMPIXELS 25

#define HALLPIN 34

#define DATAPIN    5
#define CLOCKPIN   15
Adafruit_DotStar 
strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

/*
struct pixel
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};
*/

volatile unsigned long lastspinMicros = 0;
volatile unsigned long spinMicros = 0;
volatile unsigned long spinPeriod = 20000ul;
volatile bool triggered = 0;

void IRAM_ATTR isr() 
{
  lastspinMicros = spinMicros;
  spinMicros = micros();
  spinPeriod = spinMicros - lastspinMicros;
  triggered = 1;
}

void IRAM_ATTR isr2()
{
  for(int frame = 0; frame < IMAGE_WIDTH; frame++)
  {
    for(int pixel = 0; pixel < NUMPIXELS; pixel++)
    {
      strip.setPixelColor(pixel, image[pixel][frame]);
    }
    strip.show();
    delayMicroseconds(500);
  }
  strip.fill(0, 0, NUMPIXELS);
  strip.show();
}

unsigned long delayCalc = 0;
//unsigned long delayCalc2 = 0;
unsigned long pixelPeriod = 0;

void IRAM_ATTR isr3()
{
  lastspinMicros = spinMicros;
  spinMicros = micros();
  spinPeriod = spinMicros - lastspinMicros;
  pixelPeriod = (spinPeriod / (IMAGE_WIDTH + 1));
  for(int frame = 0; frame < IMAGE_WIDTH; frame++)
  {
    
    for(int pixel = 0; pixel < NUMPIXELS; pixel++)
    {
      strip.setPixelColor(pixel, image[pixel][frame]);
    }
    strip.show();
    delayMicroseconds(pixelPeriod);
  }
  strip.fill(0, 0, NUMPIXELS);
  strip.show();
}

void setup() {

  //setup serial printing
  Serial.begin(115200);
  while(!Serial);
  delay(50);
  Serial.println("booting...");

  //setup hall effect interupt
  pinMode(HALLPIN, INPUT_PULLUP);
  attachInterrupt(HALLPIN, isr, FALLING);

  //setup adafruit dotstart
  strip.begin(); // Initialize pins for output
  //strip.setBrightness(255/2);
  strip.show();  // Turn all LEDs off

  //setup timer

}



void loop() 
{
  
  
  while(triggered == 0);
  triggered = 0;
  //Serial.println(delayCalc);
  unsigned long pixelPeriod = spinPeriod / (IMAGE_WIDTH);
  for(int frame = 0; frame < IMAGE_WIDTH; frame++)
  {
    delayCalc = micros();
    for(int pixel = 0; pixel < NUMPIXELS; pixel++)
    {
      strip.setPixelColor(pixel,Adafruit_DotStar::gamma32(image[pixel][frame]));
    }
    strip.show();
    //delayCalc2 = micros();
    delayMicroseconds(pixelPeriod - (micros() - delayCalc) - 10);
  }
  //strip.fill(0, 0, NUMPIXELS);
  //strip.show();
  //delayMicroseconds(pixelPeriod * 10);
  
 /*
 Serial.print(delayCalc2);
 Serial.print(" - ");
 Serial.print(delayCalc);
 Serial.print(" = ");
 Serial.print(delayCalc2 - delayCalc);
 Serial.print(", pixelPeriod: ");
 Serial.println(pixelPeriod);
*/
}
