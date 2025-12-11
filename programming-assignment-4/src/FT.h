#ifndef FT_H
#define FT_H

#include "image.h"
#include "mask.h"
#include <vector>
#include <utility>

using namespace std;

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

enum FrequencyFilterType {
    bandReject,
    notchReject,
    bandPass,
    hfe,
    blur
};

struct FilterParams {
    // D0
    float cutoffFrequency = 30.0;
    // W
    float bandwidth = 10.0;
    // HFE
    float gammaH = 2.0;
    float gammaL = 0.5;
    float sharpness = 1.0;
    // notch params
    int numNotches = 0;
    vector<pair<float, float>> notchLocations;
    // blur
    float a = 0.1;
    float b = 0.1;
    float T = 1;

    FilterParams() {}

    static FilterParams BandReject(float d0, float w) {
        FilterParams p;
        p.cutoffFrequency = d0;
        p.bandwidth = w;
        return p;
    }

    static FilterParams BandPass(float d0, float w) {
        FilterParams p;
        p.cutoffFrequency = d0;
        p.bandwidth = w;
        return p;
    }

    static FilterParams NotchReject(float d0, vector<pair<float, float>> notches) {
        FilterParams p;
        p.cutoffFrequency = d0;
        p.numNotches = notches.size();
        p.notchLocations = notches;
        return p;
    }

    static FilterParams HighFrequencyEmphasis(float d0, float gH, float gL, float c) {
        FilterParams p;
        p.cutoffFrequency = d0;
        p.gammaH = gH;
        p.gammaL = gL;
        p.sharpness = c;
        return p;
    }

    static FilterParams MotionBlur(float a, float b, float T) {
        FilterParams p;
        p.a = a;
        p.b = b;
        p.T = T;
        return p;
    }
};

class FT {
    friend class FrequencyHelper;
    public:
        // ImageType input;
        float** real;
        float** imaginary;
        int paddedRows;
        int paddedCols;
        float** phase;
        float** magnitude;

        FT(ImageType& i);
        FT(Mask& mask, int rows, int cols, bool shifted);
        FT(FrequencyFilterType type, int rows, int cols, FilterParams params = FilterParams());
        FT(float** real, float** imaginary, int rows, int cols);
        ~FT();
        void fftHelper(float data[], unsigned long nn, int isign);
        void fft(bool is1D, bool isForward);
        
        void getSpectrum(ImageType& output, FFTComponent component);
        void get1DGraph(ImageType& output, int row, FFTComponent component);

        void fftShift();
        void addGaussianNoise(float mu, float sigma);
};

#endif