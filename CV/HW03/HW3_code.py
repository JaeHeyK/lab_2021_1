from PIL import Image
import numpy as np
import math


# In[4]:

# return gauss 1-d array based on sigma value
def gauss1d(sigma):
    d = math.ceil(sigma*6)//2*2+1

    g1 = np.exp(-(np.arange(d)-d//2)**2/(2*sigma**2))

    return g1/np.sum(g1)
# In[5]:

def gauss2d(sigma):
    return np.outer(gauss1d(sigma), gauss1d(sigma))
# In[6]:

def convolve2d(array, filter):
    array.astype(np.float32)
    
    m = filter.shape[0]//2

    pad = np.pad(array,m,"constant", constant_values=0)

    conv = filter[:,::-1][::-1]

    result = np.zeros(array.shape)

    for i in range(m,array.shape[0]+m):
        for j in range(m, array.shape[1]+m):
            result[i-m][j-m] = np.sum(conv * pad[i-m:i+m+1,j-m:j+m+1])

    return result
# In[7]:

def gaussconvolve2d(array, sigma):
    return convolve2d(array, gauss2d(sigma))


# In[39]:


# Part 1.4.c
# Open dog image, convert it to greyscale
dogIm = Image.open('2b_dog.bmp')
dogIm = dogIm.convert('L')

# convert it to numpy array, and apply gaussian convolution with sigma 3
dogImArray = np.asarray(dogIm)
dogImGaussArray = gaussconvolve2d(dogImArray, 3)

# convert its type to uint8, for saving image file
dogImGaussArray = dogImGaussArray.astype('uint8')
# convert this array to image, and save it
dogImGauss = Image.fromarray(dogImGaussArray)
dogImGauss.save('dog_gaussian.bmp', 'PNG')

# Part 1.4.d
# show both original image and filtered image using Image.show()
dogIm.show()
dogImGauss.show()


# In[8]:


# Befor Part2, create low pass functoin and array-image convertig function

# this function get target image and sigma
# and with them, it will return low pass filtered RGB channel list
# this list can be used by another filter functions
def colorLowPassChannels(im, sigma):
    # split each RGB channel from imported image
    r,g,b = im.split()
    channels = [r, g, b]

    # filtered channel arrays will be stored here
    gaussChannels = []
    
    # for each channel, convert it to numpy array
    # apply gaussian filter(with sigma), and store it
    for channel in channels:
        chArray = np.asarray(channel)
        gaussChArray = gaussconvolve2d(chArray, sigma)
        gaussChannels.append(gaussChArray)
    return gaussChannels


# In[9]:


# this function will convert RGB channel arrays to image
# after whole filtering process, this function should be called
# to save or show image file
def imageFromChArray(channels):
    # each converted channel will be stored here
    resultChannels = []

    for channel in channels:
        # convert channel array to image, and store it
        resultChannels.append(Image.fromarray(channel.astype('uint8')))
    # return merged RGB image
    return Image.merge("RGB", resultChannels)


# In[10]:


# Part 2.1
# I choose dog image for low pass filtering,
# so open dog image file
dogIm = Image.open('2b_dog.bmp')

# use previous functions to get low pass filtered color image
dogLowPassIm = imageFromChArray(colorLowPassChannels(dogIm, 3))

# show filterd image, and save it as file
dogLowPassIm.show()
dogLowPassIm.save("dog_lowpass.bmp", "PNG")


# In[11]:


# Before Part 2.2, create high pass filter function
# this function will return high pass filterd RGB channel list
# it takes image, sigma value,
# and v for determining whether visualize high path filter or not
# if v is 0, filter will not visualized,
# else if v is 1, filter will visualized by adding 128 to each pixel
def colorHighPassChannels(im, sigma, v):
    # prepare original and low pass filterd channels
    r, g, b = im.split()
    channels = [r, g, b]
    lpChannels = colorLowPassChannels(im, sigma)
    
    # high pass filterd channels will be stored here
    hpChannels = []

    # I used zip() to iterate both channels at once
    for channel, lpChannel in zip(channels, lpChannels):
        # convert channels to arrays,
        # and subtract low pass filter to original
        # visualization is up to v value
        # and store filtered channel
        chArray = np.asarray(channel)
        hpChArray = chArray - lpChannel + 128*v
        hpChannels.append(hpChArray)
    return hpChannels


# In[12]:


# Part 2.2
# I choose dog image for low pass filtering
catIm = Image.open('2a_cat.bmp')

# use previous functions to get high pass filtered color image
# finally show and save image file

catHighPassIm = imageFromChArray(colorHighPassChannels(catIm, 3, 1))

catHighPassIm.show()
catHighPassIm.save("cat_highpass.bmp", "PNG")


# In[15]:


# before Part 2.3, create hybrid image function
# it returns RGB channel list of hybrid image
# it takes two image for low pass, and high pass filter
# each filter's sigma value will be given seperately, too
def colorHybrid(im1, im2, sigma1, sigma2):
    # create low pass, high pass filter image
    # for high pass filter, because there's no reason to visualize it, so it's visualize factor(=v) will be 0
    lpChannels = colorLowPassChannels(im1, sigma1)
    hpChannels = colorHighPassChannels(im2, sigma2, 0)

    # hybrid image channels will be stored here
    hybridChannels = []

    for hpChannel, lpChannel in zip(hpChannels, lpChannels):
        # sum each low pass and high pass filter
        hybridChannel = lpChannel + hpChannel
        # clamp outbounded values in previous results
        # limit values to 255 which is larger than 255
        # alse limit values to 0 which is smaller than 0
        hybridChannel[np.where(hybridChannel > 255)] = 255
        hybridChannel[np.where(hybridChannel < 0)] = 0
        # store clamped channel array
        hybridChannels.append(hybridChannel)
        
    return hybridChannels


# In[14]:


# Part 2.3
# create hybrid image consist of low pass filtered dog and high pass filtered cat. sigma values is 3
# it would be look like a cat at far distance,
# and look like a dog in close distance
catDogHybridIm  = imageFromChArray(colorHybrid(dogIm, catIm, 3, 3))

# show hybrid image and save it as file
catDogHybridIm.show()
catDogHybridIm.save("cat_dog_hybrid.bmp","PNG")

