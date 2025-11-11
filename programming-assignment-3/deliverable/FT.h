#ifndef FT_H
#define FT_H

#include "image.h"

/*
Scheme:
- use ImageType for storing data
- treat 1D functions as ImageType with a dimension as 1
- store magnitude, phase, real, imaginary parts as imagetypes
*/

enum FFTComponent {
    REAL,
    IMAGINARY,
    MAGNITUDE,
    PHASE
};

class FT {
    public:
        // ImageType input;
        float** real;
        float** imaginary;
        int paddedRows;
        int paddedCols;
        float** phase;
        float** magnitude;

        FT(ImageType& i);
        FT(float** real, float** imaginary, int rows, int cols);
        ~FT();
        void fftHelper(float data[], unsigned long nn, int isign);
        void fft(bool is1D, bool isForward);
        
        void getSpectrum(ImageType& output, FFTComponent component);
        void get1DGraph(ImageType& output, int row, FFTComponent component);

        void fftShift();
};

#endif