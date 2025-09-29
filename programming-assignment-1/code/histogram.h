#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "image.h"

class Histogram {
    public:
        Histogram();
        Histogram(ImageType&);
        void getFrequency(const int, int&);
        void addFrequency(const int); 
        void setFrequency(const int, const int);
        void convertToImage(ImageType*);
        void getMaxFrequency(int&);
        void convertToImage(const int, const int, ImageType&);
        void processCDF(int);
        void applyCDF(ImageType&);
    private:
        unsigned int frequencies[256];
        float cdf[256];
};

#endif