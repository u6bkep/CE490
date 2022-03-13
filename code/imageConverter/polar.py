import polarTransform
import matplotlib.pyplot as plt
import imageio
import numpy as np
import sys
import os

print(os.getcwd())
print(sys.argv)

Image = imageio.imread(sys.argv[1])

#polarImage, ptSettings = polarTransform.convertToPolarImage(Image, initialRadius=0,
#                                                    finalRadius=120, initialAngle=0,
#                                                    finalAngle=2 * np.pi)

polarImage, ptSettings = polarTransform.convertToPolarImage(Image, hasColor=True)


#cartesianImage = ptSettings.convertToCartesianImage(polarImage)

#import pdb; pdb.set_trace()

imageio.imwrite(sys.argv[2], polarImage)

#plt.figure()
#plt.imshow(polarImage, origin='lower')

#plt.figure()
#plt.imshow(cartesianImage, origin='lower')