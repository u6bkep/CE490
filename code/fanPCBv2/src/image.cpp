#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <PNGdec.h>
#include <FreeRTOS.h>
#include "image.hpp"

//settup pointers for files fullrgb.png Umaruchan.png
extern uint8_t bulldog_png_start[] asm("_binary_src_bulldog_png_start");
extern uint8_t bulldog_png_end[] asm("_binary_src_bulldog_png_end");

extern uint8_t pineapple_png_start[] asm("_binary_src_pineapple_png_start");
extern uint8_t pineapple_png_end[] asm("_binary_src_pineapple_png_end");

extern uint8_t fullrgb_png_start[] asm("_binary_src_fullrgb_png_start");
extern uint8_t fullrgb_png_end[] asm("_binary_src_fullrgb_png_end");

extern uint8_t black_png_start[] asm("_binary_src_black_png_start");
extern uint8_t black_png_end[] asm("_binary_src_black_png_end");

extern uint8_t Umaruchan_png_start[] asm("_binary_src_Umaruchan_png_start");
extern uint8_t Umaruchan_png_end[] asm("_binary_src_Umaruchan_png_end");

uint8_t imageWidth = 0;
uint8_t imageHeight = 0;



xTaskHandle imageRenderingTaskHandle;
xTaskHandle imageDecodingTaskHandle;


#define NUMPIXELS 25 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN 5
#define CLOCKPIN 15
// pin for jall effect sensor
#define HALLPIN 34

Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_RGB);

struct pngFile_t
{
    uint8_t *start;
    uint8_t *end;
};

pngFile_t images[] =
    {
        {black_png_start, black_png_end},
        {bulldog_png_start, bulldog_png_end},
        {pineapple_png_start, pineapple_png_end},
        {fullrgb_png_start, fullrgb_png_end,},
        {Umaruchan_png_start, Umaruchan_png_end}
 };

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

void PngRenderer::init()
{

    //setup hall effect interupt
    pinMode(HALLPIN, INPUT_PULLUP);
    attachInterrupt(HALLPIN, isr, FALLING);

    xdecodeSemaphore = xSemaphoreCreateMutex();

    //setup adafruit dotstart
    strip.begin(); // Initialize pins for output
    //strip.setBrightness(255/2);
    strip.fill(0, 0, numberOfPixels);
    strip.show(); // Turn all LEDs off

    chooseImage(selectedImage);
}

void PngRenderer::chooseImage(int imageNum)
{
        //setup png image
    png.openFLASH(images[imageNum].start, images[imageNum].end - images[imageNum].start, NULL);

        Serial.print("file lenth: ");
    Serial.println(images[imageNum].end - images[imageNum].start);

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

        //if image is already allocated, free
        if (image != NULL)
        {
        	free(image);
        }
        image = (uint8_t *)malloc(png.getBufferSize());
        if(image != NULL)
        {
            Serial.print("image buffer location: ");
            Serial.println(image);
            png.setBuffer(image);
            png.decode(NULL, 0);
        }
        else
        {
            Serial.println("image failed to malloc");
        }
        

        Serial.print("PNG.decode.getLastError: ");
        Serial.println(png.getLastError());
    }
}

uint8_t PngRenderer::getNumberOfImages()
{
    return(sizeof(images)/sizeof(images[0]));
}

unsigned long delayCalc = 0;
unsigned long pixelPeriod = 0;

void PngRenderer::renderTask()
{
    while (true)
    {
        while (triggered == 0);
        triggered = 0;
        pixelPeriod = spinPeriod / (imageHeight);
        for (int line = 0; line < imageHeight; line++)
        {
            delayCalc = micros();
            for (int pixel = 0; pixel < imageWidth; pixel++)
            {
                strip.setPixelColor(pixel, Adafruit_DotStar::gamma8(((pixel_t *)image)[pixel + (line * imageWidth)].r),
                                    Adafruit_DotStar::gamma8(((pixel_t *)image)[pixel + (line * imageWidth)].g),
                                    Adafruit_DotStar::gamma8(((pixel_t *)image)[pixel + (line * imageWidth)].b));
            }
            strip.show();
            delayMicroseconds(pixelPeriod - (micros() - delayCalc) - 10);
        }
    }
}
