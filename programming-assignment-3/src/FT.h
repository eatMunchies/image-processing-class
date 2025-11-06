#ifndef FT_H
#define FT_H

#include "image.h"

/*
Scheme:
- use ImageType for storing data
- treat 1D functions as ImageType with a dimension as 1
- store magnitude, phase, real, imaginary parts as imagetypes
*/

class FT {
    public:
        ImageType& input;
        float** real;
        float** imaginary;
        int paddedRows;
        int paddedCols;

        FT(ImageType& i);
        FT(float** real, float** imaginary, int rows, int cols);
        ~FT();
        void fftHelper(float data[], unsigned long nn, int isign);
        void fft(bool is1D, bool isForward);
        
        void getMagnitudeSpectrum(ImageType& output);
        void getPhaseSpectrum(ImageType& output);
        void get1DMagnitudeGraph(ImageType& output, int row);
        void get1DPhaseGraph(ImageType& output, int row);

        void fftShift();
};

#endif