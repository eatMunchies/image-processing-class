#ifndef MASK_H
#define MASK_H

#include "image.h"
#include "correlationHelper.h"
#include <cstring>
#include <iostream>

class Mask {
    public:
        Mask();
        Mask(char*, int); // gaussian or averaging, mask size
        Mask(ImageType&); // make a mask from an image
        Mask(char*); // read from filepath
        void applyMask(ImageType&, ImageType&, CorrelationHelper&); // use the correlationhelper to apply the mask
    private:
        float** mask; 
        int maskSize;
        void loadGaussianFromFile(char*);
        void normalizeMask();
};

#endif