#!/usr/bin/env python
# coding: utf-8

# In[79]:


get_ipython().system('pip install Image')


# In[80]:


get_ipython().system('pip install numpy')


# In[81]:


from PIL import Image
import numpy as np
import math


# In[82]:


def boxfilter(n):
    assert n%2 == 1, "Dimesion must be odd"
    return np.ones((n,n))/(n**2)


# In[83]:


boxfilter(3)


# In[84]:


boxfilter(4)


# In[85]:


boxfilter(7)


# In[86]:


def gauss1d(sigma):
    d = math.ceil(sigma*6)//2*2+1
    g1 = np.exp(-(np.arange(d)-d//2)**2/(2*sigma**2))
    return g1/np.sum(g1)


# In[87]:


gauss1d(0.3)


# In[88]:


gauss1d(0.5)


# In[89]:


gauss1d(1)


# In[90]:


gauss1d(2)


# In[91]:


def gauss2d(sigma):
    return np.outer(gauss1d(sigma), gauss1d(sigma))


# In[92]:


gauss2d(0.5)


# In[93]:


gauss2d(1)


# In[94]:


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


# In[95]:


def gaussconvolve2d(array, sigma):
    return convolve2d(array, gauss2d(sigma))


# In[96]:


dogIm = Image.open('2b_dog.bmp')
dogIm.show()

dogIm = dogIm.convert('L')
dogImArray = np.asarray(dogIm)
dogImGaussArray = gaussconvolve2d(dogImArray, 3)
dogImGaussArray = dogImGaussArray.astype('uint8')

dogImGauss = Image.fromarray(dogImGaussArray)
dogImGauss.save('dog_gaussian.bmp', 'PNG')

dogImGauss.show()


# In[97]:


def colorLowPassChannels(im, sigma):
    r,g,b = im.split()
    channels = [r, g, b]
    gaussChannels = []
    
    for channel in channels:
        chArray = np.asarray(channel)
        gaussChArray = gaussconvolve2d(chArray, sigma)
        gaussChannels.append(gaussChArray)
    return gaussChannels


# In[98]:


def imageFromChArray(channels):
    resultChannels = []
    for channel in channels:
        resultChannels.append(Image.fromarray(channel.astype('uint8')))
    return Image.merge("RGB", resultChannels)


# In[99]:


dogIm = Image.open('2b_dog.bmp')

dogLowPassIm = imageFromChArray(colorLowPassChannels(dogIm, 5))

dogLowPassIm.show()
dogLowPassIm.save("dog_lowpass.bmp", "PNG")


# In[100]:


def colorHighPassChannels(im, sigma, v):
    r, g, b = im.split()
    channels = [r, g, b]
    hpChannels = []
    for channel in channels:
        chArray = np.asarray(channel)
        hpChArray = chArray - gaussconvolve2d(chArray, sigma) + 128*v
        hpChannels.append(hpChArray)
    return hpChannels


# In[101]:


catIm = Image.open('2a_cat.bmp')

catHighPassIm = imageFromChArray(colorHighPassChannels(catIm, 2, 1))

catHighPassIm.show()

catHighPassIm.save("cat_highpass.bmp", "PNG")


# In[102]:


def colorHybrid(im1, im2, sigma1, sigma2):
    lpChannels = colorLowPassChannels(im1, sigma1)
    hpChannels = colorHighPassChannels(im2, sigma2, 0)
    hybridChannels = []
    for hpChannel, lpChannel in zip(hpChannels, lpChannels):
        hybridChannel = lpChannel + hpChannel
        hybridChannel[np.where(hybridChannel > 255)] = 255
        hybridChannel[np.where(hybridChannel < 0)] = 0
        hybridChannels.append(hybridChannel)
        
    return hybridChannels


# In[105]:


catDogHybridIm  = imageFromChArray(colorHybrid(dogIm, catIm, 5, 3))

catDogHybridIm.show()

catDogHybridIm.save("cat_dog_hybird.bmp","PNG")


# In[ ]:




