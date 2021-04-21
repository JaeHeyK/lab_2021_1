# In[1]:
from PIL import Image
import math
import numpy as np

"""
Get and use the functions associated with gaussconvolve2d that you used in the last HW02.
"""

# In[2]:
"""
Following functions until gaussconvolve2d() are functions I made in HW02.
"""
def gauss1d(sigma):
    # To get minimum odd number that larger than sigma * 6,
    # Ceil sigma * 6 first
    # If ceil result is odd, that is array length. Else we need to add 1 to result number.
    d = math.ceil(sigma*6)
    d += ( 1 if (d%2 == 0) else 0)

    g1 = np.exp(-(np.arange(d)-d//2)**2/(2*sigma**2))

    return g1/np.sum(g1)

def gauss2d(sigma):
    g = np.outer(gauss1d(sigma), gauss1d(sigma))

    # Nomalize result array by dividing its sum.
    return g/np.sum(g)

def convolve2d(array,filter):
    array.astype(np.float32)
    
    m = filter.shape[0]//2

    pad = np.pad(array,m,"constant", constant_values=0)

    conv = filter[:,::-1][::-1]

    # Prepare result image array except zero padding
    result = np.zeros(array.shape)

    for i in range(m,array.shape[0]+m):
        for j in range(m, array.shape[1]+m):
            result[i-m][j-m] = np.sum(conv * pad[i-m:i+m+1,j-m:j+m+1])

    return result

def gaussconvolve2d(array,sigma):
    return convolve2d(array, gauss2d(sigma))

# For convenient, I'll using image saving function in following assignments.
# It will convert passed array to uint8 array,
# convert that array to image,
# and finally save converted image, using passed filename and mode.
def saveImageFromArray(array, filename, mode):
    img = Image.fromarray(array.astype('uint8'))
    img.save(filename, mode)
# In[3]:
"""
1. Noise reduction
"""
# Open image and show it
iguanaImg = Image.open("iguana.bmp")
iguanaImg.show()

# Convert image to grayscale and apply gaussian filter
iguanaImg = iguanaImg.convert('L')
iguanaImgArr = np.asarray(iguanaImg)
iguanaFImgArr = gaussconvolve2d(iguanaImgArr, 1.6)

# Convert array to image, show it, and save it.
iguanaFImg = Image.fromarray(iguanaFImgArr)
iguanaFImg.show()

saveImageFromArray(iguanaFImgArr, 'iguana_filtered.bmp','PNG')

# In[4]:
"""
2. Finding the intensity gradient of the image
"""
# Define sobel filter function first
def sobel_filters(img):
    # Define sobel x, y filter respectively
    sobelX = np.array([[-1., 0., 1.], [-2., 0., 2.], [-1., 0., 1.]])
    sobelY = np.array([[1., 2., 1.], [0., 0., 0.], [-1., -2., -1.]])

    # Convert image to array
    # And Get intensity x, y value respectively
    imgArr = np.asarray(img)
    iX = convolve2d(imgArr, sobelX)
    iY = convolve2d(imgArr, sobelY)

    # Get gradient and mapping its value from 0 to 255
    # For mapping, divide the array with its max value and multiply 255
    G = np.hypot(iX, iY)
    G = G*(255.0/G.max())
    
    # Get theta value from intensity x, y using arctan.
    theta = np.arctan2(iY, iX)

    return (G, theta)

# Store result gradient and theta respectively
iguanaG, iguanaTheta = sobel_filters(iguanaFImg)

# Show sobel filtered image, and save it.
iguanaGImg = Image.fromarray(iguanaG)
iguanaGImg.show()

saveImageFromArray(iguanaG, 'iguana_gradient.bmp', 'PNG')

# In[5]:
"""
3. Non-Maximum Suppression
"""

def non_max_suppression(G, theta):
    
    # We need to map theta values to one of (0, 45, 90, 180)
    # But instead of mapping theta values to those degree values, I will map them to indices, like (0, 1, 2, 3), which are same with quotients of those angles divided by 45.
    # So there is no need to convert radian to degree, and I will get quotients and just round it.

    # Get remainder of theta(divided by pi).
    theta = theta % math.pi
    # Divide modified theta by pi/4.
    theta = theta / (math.pi/4)
    # Round theta array, so that theta values mapped to indices.
    theta = theta.round()
    # Change 4 to 0, because those 4 were original theta values that should have been mapped to 0.
    theta[theta == 4] = 0
    # And change theta indices to uint8 to use as direction indice at below.
    theta = theta.astype('uint8')

    # Because our convolve2d() already returns image without zero padding, we can apply NMS right away.

    # Copy gradient to base array
    base = G.copy()
    # Create zero-filled result array
    res = np.zeros(np.shape(G))

    # Create direction array which is matched with theta indices.
    direction = np.array([[1,0], [1,1], [0,1], [-1, 1]])

    # Iterate each pixels of base except image's border,
    # to exclude unwanted value generated by zero-padding.
    for i in range(1, res.shape[0]-1):
        for j in range(1, res.shape[1]-1):
            # Get the neighboring pixels' value using theta indices.
            g1 = base[i-direction[theta[i,j],1], j+direction[theta[i,j],0]]
            g2 = base[i+direction[theta[i,j],1], j-direction[theta[i,j],0]]
            # If it is local maximum between its neighbor pixels(along edge direction), then copy it into result array.
            if((base[i][j]>=g1) & (base[i][j]>=g2)):
                res[i][j] = base[i][j]
    
    return res
# Get non maximum suppressed image array
iguanaNMS = non_max_suppression(iguanaG, iguanaTheta)

# Convert array to image, show it, and save it.
iguanaNMSImg = Image.fromarray(iguanaNMS)
iguanaNMSImg.show()

saveImageFromArray(iguanaNMS, 'iguana_NMS.bmp', 'PNG')

# In[6]:
"""
4. Double Threshold
"""
def double_thresholding(img):
    # Calculate diff, high threshold, and low threshod.
    diff = img.max() - img.min()
    Th = img.min() + diff * 0.15
    Tl = img.min() + diff * 0.03

    # Threshold with both Th, Tl
    res = np.zeros(np.shape(img))
    res[img < Tl] = 0
    res[(Tl <= img) & (img < Th)] = 80
    res[Th <= img] = 255

    return res

# Get double thresholded image array
iguanaDT = double_thresholding(iguanaNMS)

# Convert array to image, show it, and save it.
iguanaDTImg = Image.fromarray(iguanaDT)
iguanaDTImg.show()

saveImageFromArray(iguanaDT, 'iguana_DT.bmp', 'PNG')

# In[7]:
"""
5. Edge Tracking by hysteresis
"""
# First, Create graph using given image.
# Actually each node would be weak or strong pixel,
# so get pixels that its intensity is equal or larger than 80, first.
# And iterate through pixels, checking if neighboring pixels are in there, too.
# Finally add pixel in dictonary as key, neighboring pixels list as value.
# That's the graph of weak, strong edge pixels.
def getImageGraph(img):
    # Create empty dictionary
    graph = {}
    # Get index of pixels which have larger(or equal) intensity than 80.
    edgePixels = np.argwhere(img >= 80)

    for i, pivot in enumerate(edgePixels):
        # Get row index of pixels that are in neighboring row of pivot.
        rows = edgePixels[:,:1].flatten()
        rowMask = (pivot[0]-1<=rows) & (rows<=(pivot[0]+1))
        
        # Get column index of pixels that are in neighboring column of pivot.
        cols = edgePixels[:,1:].flatten()
        colMask = (pivot[1]-1<=cols) & (cols<=(pivot[1]+1))

        # Neighboring pixels should be in both neighboring row and column.
        # So get index mask(neighborMask) using logical AND,
        # and remove pivot itself from index mask(make mask's value False).
        # That's why I iterate pixels with enumerate().
        neighborMask = rowMask & colMask
        neighborMask[i] = False
        
        # Get real index of neighbors, through bool indexing.
        neighbors = edgePixels[neighborMask]
        # Add key: pivot's index(tuple) and values: list of neighbors(tuple) to graph.
        graph[tuple(pivot)] = list(map(tuple, neighbors))
    return graph

def hysteresis(img):
    # While DFS, if we visited any pixels, we can say that those pixels will be strong-edge.
    # So, if we assign 1 to visited pixels, 0 to unvisited pixels,
    # and multiply 255 to whole array, we will get result image with strong edges.

    # Get graph of passed image.
    graph = getImageGraph(img)
    # Create array for checking pixels' visit history
    # and list for stacking pixels that are planned to visit.
    visited = np.zeros(np.shape(img))
    willVisit = []

    # For every pixels in graph,
    for node in graph.keys():
        # Check if pixel is belong to strong edge and if so,
        if(img[node] == 255):
            # start DFS with append this pixel.
            willVisit.append(node)
            # While visitlist is not empty,
            while willVisit:
                # Remove last added pixel from visitlist,
                dest = willVisit.pop()
                # and if I didn't visit this pixel,
                if visited[dest] != 1:
                    # record as visited pixel,
                    visited[dest] = 1
                    # and add connected pixels into visitlist.
                    willVisit.extend(graph[dest])
    # After DFS, multiply visited array by 255 to get final edge image.
    res = visited * 255.0
    return res

# Get hysteresised image array
iguanaHyst = hysteresis(iguanaDT)
# Convert array to image, show it, and save it.
iguanaHystImg = Image.fromarray(iguanaHyst)
iguanaHystImg.show()

saveImageFromArray(iguanaHyst, 'iguana_edge.bmp', 'PNG')
# %%
