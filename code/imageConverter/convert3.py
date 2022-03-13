from PIL import Image
import sys
import pdb;

#convert an image to 2d c array compatable with adafruit dotstar library
#now with commandline usage!

#im = Image.open('./pineapple small.png')
im = Image.open(sys.argv[1])

pixels = list(Image.Image.getdata(im))
#pixels = im.getdata()
width, height = im.size
#pdb.set_trace()
print("//{} {}x{}".format(sys.argv[1], width, height))
print("#define IMAGE_WIDTH {}".format(width))
print("#define IMAGE_HEIGHT {}".format(height))
print("const uint32_t image[][IMAGE_WIDTH] = {\n{",end='')

i = 1
for pixel in pixels:
    print("0x{0:02x}{1:02x}{2:02x}".format(pixel[1], pixel[0], pixel[2]), end='')
    if(i % (width) != 0):
        print(", ", end='')
    else:
        if(((i / width) % height) != 0):
            print("}, \n{", end='')
        else:
            print("}};", end='')
    i +=1
