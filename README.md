# contourFitElipse
## C++ implementation of python Ellipse detection script
## First principles implementation based on openCV

To include:
### Smoothing filter with 5x5 gausian kernal
### Sobel filter to accent edges (First order differential filter)

  Outputs 2 arrays
    Differental image
    Directional domain image,  recording the relitive angles of each pixel  
    
### Canny edge detection

Second order differential of image using both the first order sobel image and the direction domain image
to return a single pixel thick outline of each boundary

### contourFit

Generates a list of connected pixels that define the contours in the format :

`contourList[ ][ ]` = {contourNumber,x cordinate, y cordinate} 
Where all coordinates with the same contourNumber are considered part of the same contour

