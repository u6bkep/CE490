#include <Arduino.h>

class PngRenderer
{
    public:
    void init();
    void chooseImage (int imageNum);
    uint8_t getNumberOfImages();
    void renderTask();

    private:
    uint8_t numberOfPixels;
    int selectedImage = 0;
    

};


