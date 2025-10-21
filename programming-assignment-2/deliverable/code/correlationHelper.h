#ifndef CORRELATION_HELPER_H
#define CORRELATION_HELPER_H

#include "image.h"

class CorrelationHelper {
    public:
        CorrelationHelper();
        void correlation(float**, ImageType&, ImageType&, int, int, int, int, int, int);
        void correlateImage(float**, ImageType&, ImageType&);
        void correlation(float**, ImageType&, float**, int, int, int, int, int, int);
        void correlateImage(float**, ImageType&, float**, int, int);
        void scaleImage(float**, ImageType&);
};

#endif