#include <Arduino.h>
#include "EEPROM.h"
#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <PNGdec.h>


#define NUMPIXELS 25
//#define IMAGE_WIDTH 25
//#define IMAGE_HEIGHT 100

#define HALLPIN 34

#define DATAPIN    5
#define CLOCKPIN   15
Adafruit_DotStar 
strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_RGB);


//settup pointers for files dragons.png
extern uint8_t bulldog_png_start[] asm("_binary_src_bulldog_png_start");
extern uint8_t bulldog_png_end[] asm("_binary_src_bulldog_png_end");

extern uint8_t pikazard_png_start[] asm("_binary_src_pikazard_png_start");
extern uint8_t pikazard_png_end[] asm("_binary_src_pikazard_png_end");

extern uint8_t dragons_png_start[] asm("_binary_src_dragons_png_start");
extern uint8_t dragons_png_end[] asm("_binary_src_dragons_png_end");

uint8_t imageWidth = 0;
uint8_t imageHeight = 0;
uint8_t * image;

PNG png;

struct pngFile_t
{
uint8_t * start;
uint8_t * end;
};

pngFile_t images[] = {{bulldog_png_start, bulldog_png_end}, {pikazard_png_start, pikazard_png_end}, {dragons_png_start, dragons_png_end}};
int selectedImage = 2;

volatile unsigned long lastspinMicros = 0;
volatile unsigned long spinMicros = 0;
volatile unsigned long spinPeriod = 20000ul;
volatile bool triggered = 0;


struct pixel_t
{
  uint8_t b;
  uint8_t g;
  uint8_t r;
};


void IRAM_ATTR isr() 
{
  lastspinMicros = spinMicros;
  spinMicros = micros();
  spinPeriod = spinMicros - lastspinMicros;
  triggered = 1;
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
  strip.fill(0, 0, NUMPIXELS);
  strip.show();  // Turn all LEDs off

  //setup png image
  png.openFLASH(images[selectedImage].start, images[selectedImage].end - images[selectedImage].start, NULL);
  

  Serial.print("file lenth: ");
  Serial.println(images[selectedImage].end - images[selectedImage].start);

  Serial.print("PNG.getLastError: ");
  Serial.println(png.getLastError());
  
  Serial.print("PNG.getHeight: ");
  Serial.println(png.getHeight());
  imageHeight = png.getHeight();

  Serial.print("PNG.getWidth: ");
  Serial.println(png.getWidth());
  imageWidth = png.getWidth();

  Serial.print("PNG.getBpp: ");
  Serial.println(png.getBpp());
  
  Serial.print("PNG.hasAlpha: ");
  Serial.println(png.hasAlpha());

  Serial.print("PNG.isInterlaced: ");
  Serial.println(png.isInterlaced());

  Serial.print("PNG.getPixelType: ");
  Serial.println(png.getPixelType());

  Serial.print("PNG.getBufferSize: ");
  Serial.println(png.getBufferSize());
  
  image = (uint8_t *) malloc(png.getBufferSize());
  Serial.print("image buffer location: ");
  Serial.println(* image);
  png.setBuffer(image);
  png.decode(NULL, 0);

  Serial.print("PNG.decode.getLastError: ");
  Serial.println(png.getLastError());

}

unsigned long delayCalc = 0;
unsigned long pixelPeriod = 0;

void loop() 
{
  while(triggered == 0);
  triggered = 0;
  pixelPeriod = spinPeriod / (imageHeight);
  for(int line = 0; line < imageHeight; line++)
  {
    delayCalc = micros();
    for(int pixel = 0; pixel < imageWidth; pixel++)
    {
      strip.setPixelColor(pixel, Adafruit_DotStar::gamma8(((pixel_t *) image)[pixel + (line * imageWidth)].r),
                                  Adafruit_DotStar::gamma8(((pixel_t *) image)[pixel + (line * imageWidth)].g),
                                  Adafruit_DotStar::gamma8(((pixel_t *) image)[pixel + (line * imageWidth)].b));
    }
    strip.show();
    delayMicroseconds(pixelPeriod - (micros() - delayCalc) - 10);
  }
}
