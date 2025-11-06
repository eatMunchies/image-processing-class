#include "FT.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>

unsigned long nextPowerOf2(unsigned long n) {
    unsigned long power = 1;
    while (power < 2) {
        power <<= 1;
    }
    return power;
}

FT::FT(ImageType& i) {
    // image pad the input to be a power of 2
    int rows, cols, q;
    i.getImageInfo(rows, cols, q);

    paddedRows = nextPowerOf2(rows);
    paddedCols = nextPowerOf2(cols);
    input.setImageInfo(paddedRows, paddedCols, q);
    for (int r = 0; r < paddedRows; r++) {
        for (int c = 0; c < paddedCols; c++) {
            if (r < rows && c < cols) {
                int pixelVal;
                i.getPixelVal(r, c, pixelVal);
                input.setPixelVal(r, c, pixelVal);
            } else {
                input.setPixelVal(r, c, 0);
            }
        }
    }

    real = new float*[paddedRows];
    imaginary = new float*[paddedRows];
    for (int r = 0; r < paddedRows; r++) {
        real[r] = new float[paddedCols];
        imaginary[r] = new float[paddedCols];

        for (int c = 0; c < paddedCols; c++) {
            int pixelVal;
            input.getPixelVal(r, c, pixelVal);
            real[r][c] = (float) pixelVal;
            imaginary[r][c] = 0.0;
        }
    }
}

FT::FT(float** real, float** imaginary, int rows, int cols) {

}

FT::~FT() {
    if (real) {
        for (int r = 0; r < paddedRows; r++) {
            delete[] real[r];
        }
        delete[] real;
    }
    if (imaginary) {
        for (int r = 0; r < paddedRows; r++) {
            delete[] imaginary[r];
        }
        delete[] imaginary;
    }
}

void FT::fftHelper(float data[], unsigned long nn, int isign) {
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    float tempr, tempi;

    n = nn << 1;
    j = 1;

    for (i = 1; i < n + 1; i += 2) {
        if (j > i) {
            tempr = data[j];
            data[j] = data[i];
            data[i] = tempr;

            tempr = data[j+1];
            data[j+1] = data[i+1];
            data[i+1] = tempr;
        }
        m = n >> 1;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }

    mmax = 2;
    while (n > mmax) {
        istep = mmax << 1;
        theta = isign * (6.28318530717959 / mmax);
        wtemp = sin(0.5 * theta);
        wpr = -2.0 * wtemp * wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;

        for (m = 1; m < mmax; m += 2) {
            for (i = m; i <= n; i += istep) {
                j = i + mmax;
                tempr = wr * data[j] - wi * data[j+1];
                tempi = wr * data[j+1] + wi * data[j];
                data[j] = data[i] - tempr;
                data[j+1] = data[i+1] - tempi;
                data[i] += tempr;
                data[i+1] += tempi;
            }
            wr = (wtemp = wr) * wpr - wi * wpi + wr;
            wi = wi * wpr + wtemp * wpi + wi;
        }
        mmax = istep;
    }
}

void FT::fft(bool is1D, bool isForward) {
    int isign = isForward ? 1 : -1;
    unsigned long nn;
    float* data;

    // 1D FFT on rows only;
    nn = paddedCols;
    data = new float[2 * nn + 1];
    for (int r = 0; r < paddedRows; r++) {
        for (int c = 0; c < paddedCols; c++) {
            data[2*c + 1] = real[r][c];
            data[2*c + 2] = imaginary[r][c];
        }

        fftHelper(data, nn, isign);

        for (int c = 0; c < paddedCols; c++) {
            real[r][c] = data[2*c + 1];
            imaginary[r][c] = data[2*c + 2];
        }
    }
    delete[] data;

    if (!is1D) {
        // 1D FFT on cols (second part)
        nn = paddedRows;
        data = new float[2 * nn + 1];
        
        for (int c = 0; c < paddedCols; c++) {
            for (int r = 0; r < paddedRows; r++) {
                data[2*r + 1] = real[r][c];
                data[2*r + 2] = imaginary[r][c];
            }

            fftHelper(data, nn, isign);

            for (int r = 0; r < paddedRows; r++) {
                real[r][c] = data[2*r + 1];
                imaginary[r][c] = data[2*r + 2];
            }
        }
        delete[] data;
    }

    if (!isForward) {
        float normFactor = is1D ? (float) paddedCols : (float) (paddedRows * paddedCols);
        for (int r = 0; r < paddedRows; r++) {
            for (int c = 0; c < paddedCols; c++) {
                real[r][c] /= normFactor;
                imaginary[r][c] /= normFactor;
            }
        }
    }
}

void FT::getMagnitudeSpectrum(ImageType& output) {
    output.setImageInfo(paddedRows, paddedCols, 255);

    float maxMag = 0.0;
    for (int r = 0; r < paddedRows; r++) {
        for (int c = 0; c < paddedCols; c++) {
            float mag = sqrt(real[r][c] * real[r][c] + imaginary[r][c] * imaginary[r][c]);
            if (mag > maxMag) maxMag = mag;
        }
    }

    for (int r = 0; r < paddedRows; r++) {
        for (int c = 0; c < paddedCols; c++) {
            float mag = sqrt(real[r][c] * real[r][c] + imaginary[r][c] * imaginary[r][c]);
            float logMag = log(1 + mag);
            int pixelVal = (int) (255.0 * logMag / log(1.0 + maxMag));
            output.setPixelVal(r, c, pixelVal);
        }
    }
}

void FT::getPhaseSpectrum(ImageType& output) {
    output.setImageInfo(paddedRows, paddedCols, 255);

    for (int r = 0; r < paddedRows; r++) {
        for (int c = 0; c < paddedCols; c++) {
            float phase = atan2(imaginary[r][c], real[r][c]);
            int pixelVal = (int) ((phase + M_PI) * 255.0 / (2.0 * M_PI));
            output.setPixelVal(r, c, pixelVal);
        }
    }
}

void FT::get1DMagnitudeGraph(ImageType& output, int row) {
    int graphHeight = 256;
    output.setImageInfo(graphHeight, paddedCols, 255);

    for (int r = 0; r < graphHeight; r++) {
        for (int c = 0; c < paddedCols; c++) {
            output.setPixelVal(r, c, 255);
        }
    }

    float* magnitudes = new float[paddedCols];
    float maxMag = 0.0;
    for (int c = 0; c < paddedCols; c++) {
        magnitudes[c] = sqrt(real[row][c] * real[row][c] + imaginary[row][c] * imaginary[row][c]);
        if (magnitudes[c] > maxMag) maxMag = magnitudes[c];
    }

    for (int c = 0; c < paddedCols; c++) {
        magnitudes[c] = log(1.0 + magnitudes[c]);
    }
    float maxLogMag = log(1.0 + maxMag);
    
    for (int c = 0; c < paddedCols; c++) {
        int height = (int) ((magnitudes[c] / maxLogMag) * (graphHeight - 1));

        for (int h = 0; h <= height; h++) {
            int y = graphHeight - 1 - h;
            output.setPixelVal(y, c, 0);
        }
    }

    delete[] magnitudes
}

void FT::get1DPhaseGraph(ImageType& output, int row) {
    int graphHeight = 256;
    output.setImageInfo(graphHeight, paddedCols, 255);

    for (int r = 0; r < graphHeight; r++) {
        for (int c = 0; c < paddedCols; c++) {
            output.setPixelVal(r, c, 255);
        }
    }

    float* phases = new float[paddedCols];
    for (int c = 0; c < paddedCols; c++) {
        phases[c] = atan2(imaginary[row][c], real[row][c]);
    }

    int midLine = graphHeight / 2;

    for (int c = 0; c < paddedCols; c++) {
        int height = (int) ((phases[c] / M_PI) * (graphHeight / 2.0));
        int y = midLine - height;

        if (y < 0) y = 0;
        if (y >= graphHeight) y = graphHeight - 1;

        if (y < midLine) {
            for (int r = y; r <= midLine; r++) {
                output.setPixelVal(r, c, 0);
            }
        } else {
            for (int r = midLine; r <= y; r++) {
                output.setPixelVal(r, c, 0);
            }
        }
    }

    delete[] phases;
}

void FT::fftShift() {
    int halfRows = paddedRows / 2;
    int halfCols = paddedCols / 2;

    for (int r = 0; r < halfRows; r++) {
        for (int c = 0; c < halfCols; c++) {
            float tempReal = realPart[r][c];
            float tempImag = imagPart[r][c];
            realPart[r][c] = realPart[r + halfRows][c + halfCols];
            imagPart[r][c] = imagPart[r + halfRows][c + halfCols];
            realPart[r + halfRows][c + halfCols] = tempReal;
            imagPart[r + halfRows][c + halfCols] = tempImag;
            
            tempReal = realPart[r][c + halfCols];
            tempImag = imagPart[r][c + halfCols];
            realPart[r][c + halfCols] = realPart[r + halfRows][c];
            imagPart[r][c + halfCols] = imagPart[r + halfRows][c];
            realPart[r + halfRows][c] = tempReal;
            imagPart[r + halfRows][c] = tempImag;
        }
    }
}