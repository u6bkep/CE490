#include <Arduino.h>

class PngRenderer
{
    public:
    void init();
    void chooseImage(int imageNum);
    uint8_t getNumberOfImages();
    void renderTask();

    private:
    uint8_t numberOfPixels;
    int selectedImage = 1;
    PNG png;
    uint8_t *image = NULL;
    static void startTaskImpl(void*);
    static void chooseImageStatic(void * _this);
    struct pngFile_t
    {
        uint8_t *start;
        uint8_t *end;
    };
    pngFile_t images[];
    SemaphoreHandle_t xdecodeSemaphore;
    

};


