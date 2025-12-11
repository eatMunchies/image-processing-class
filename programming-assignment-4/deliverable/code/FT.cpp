#include "FT.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>     
#include <limits>
#include <random>
#include <utility>

using namespace std;

unsigned long nextPowerOf2(unsigned long n) {
    unsigned long power = 1;
    while (power < n) {
        power <<= 1;
    }
    return power;
};

FT::FT(ImageType& i) {
    // image pad the input to be a power of 2
    int rows, cols, q;
    i.getImageInfo(rows, cols, q);

    paddedRows = nextPowerOf2(rows);
    paddedCols = nextPowerOf2(cols);
    
    // Allocate real and imaginary arrays FIRST
    real = new float*[paddedRows];
    imaginary = new float*[paddedRows];
    magnitude = new float*[paddedRows];
    phase = new float*[paddedRows];
    
    for (int r = 0; r < paddedRows; r++) {
        real[r] = new float[paddedCols];
        imaginary[r] = new float[paddedCols];
        magnitude[r] = new float[paddedCols];
        phase[r] = new float[paddedCols];
    }
    
    
    for (int r = 0; r < paddedRows; r++) {
        for (int c = 0; c < paddedCols; c++) {
            if (r < rows && c < cols) {
                int pixelVal;
                i.getPixelVal(r, c, pixelVal);
                real[r][c] = (float)pixelVal;
            } else {
                real[r][c] = 0.0;  // zeor padding
            }
            imaginary[r][c] = 0.0; 
        }
    }
    

}

FT::FT(Mask& mask, int rows, int cols, bool isOdd) {
    paddedRows = rows;
    paddedCols = cols;
    
    // init arrays
    real = new float*[paddedRows];
    imaginary = new float*[paddedRows];
    magnitude = new float*[paddedRows];
    phase = new float*[paddedRows];
    
    for (int r = 0; r < paddedRows; r++) {
        real[r] = new float[paddedCols];
        imaginary[r] = new float[paddedCols];
        magnitude[r] = new float[paddedCols];
        phase[r] = new float[paddedCols];
    }
    
    for (int r = 0; r < paddedRows; r++) {
        for (int c = 0; c < paddedCols; c++) {
            real[r][c] = 0.0f;
            imaginary[r][c] = 0.0f;
            magnitude[r][c] = 0.0f;
            phase[r][c] = 0.0f;
        }
    }
    
    int maskSize = mask.maskSize;
    float** maskData = mask.mask;
    
    // flip the mask for a convolution isntead of correlation (for conv theorem)
    float** rotatedMask = new float*[maskSize];
    for (int r = 0; r < maskSize; r++) {
        rotatedMask[r] = new float[maskSize];
        for (int c = 0; c < maskSize; c++) {
            rotatedMask[r][c] = maskData[maskSize - 1 - r][maskSize - 1 - c];
        }
    }
    
    if (isOdd) {
        // need to ensure theoretical properties by:
        // need to add leading row and column of zeros
        // align centers in padded arrays
        
        int extendedSize = maskSize + 1; 
        
        float** extendedMask = new float*[extendedSize];
        for (int r = 0; r < extendedSize; r++) {
            extendedMask[r] = new float[extendedSize];
            for (int c = 0; c < extendedSize; c++) {
                extendedMask[r][c] = 0.0;
            }
        }
        
        // copy rotated mask into extended (instead of original)
        for (int r = 0; r < maskSize; r++) {
            for (int c = 0; c < maskSize; c++) {
                extendedMask[r + 1][c + 1] = rotatedMask[r][c];
            }
        }
    
        // center of output: paddedRows/2, paddedCols/2
        // center of extended: extendedSize/2, ...
        // align them!
        
        int padCenterR = paddedRows / 2;
        int padCenterC = paddedCols / 2;
        int maskCenterR = extendedSize / 2;
        int maskCenterC = extendedSize / 2;
        
        int startR = padCenterR - maskCenterR;
        int startC = padCenterC - maskCenterC;
        
        for (int r = 0; r < extendedSize; r++) {
            for (int c = 0; c < extendedSize; c++) {
                int destR = startR + r;
                int destC = startC + c;
                
                destR = (destR + paddedRows) % paddedRows;
                destC = (destC + paddedCols) % paddedCols;
                
                real[destR][destC] = extendedMask[r][c];
            }
        }
        
        // cleanup extended mask
        for (int r = 0; r < extendedSize; r++) {
            delete[] extendedMask[r];
        }
        delete[] extendedMask;
        
        // get frequency domain
        fft(false, true);
        
        // zero out real part bc odd only has imaginary
        for (int r = 0; r < paddedRows; r++) {
            for (int c = 0; c < paddedCols; c++) {
                real[r][c] = 0.0f;
            }
        }
        
        // update phase and magnitude
        for (int r = 0; r < paddedRows; r++) {
            for (int c = 0; c < paddedCols; c++) {
                magnitude[r][c] = fabs(imaginary[r][c]);
                if (imaginary[r][c] > 0) {
                    phase[r][c] = M_PI / 2.0;
                } else if (imaginary[r][c] < 0) {
                    phase[r][c] = -M_PI / 2.0;
                } else {
                    phase[r][c] = 0.0;
                }
            }
        }
        
    } else {
        // even filter, can do standard placement
        
        int halfMask = maskSize / 2;
        
        for (int mr = 0; mr < maskSize; mr++) {
            for (int mc = 0; mc < maskSize; mc++) {
                int offsetR = mr - halfMask;
                int offsetC = mc - halfMask;
                
                int destR = (offsetR + paddedRows) % paddedRows;
                int destC = (offsetC + paddedCols) % paddedCols;
                
                // Use rotated mask instead of original
                real[destR][destC] = rotatedMask[mr][mc];
            }
        }
        
        // get frequency
        fft(false, true);
        
        // should only be real, zero out imaginary
        for (int r = 0; r < paddedRows; r++) {
            for (int c = 0; c < paddedCols; c++) {
                imaginary[r][c] = 0.0;
            }
        }
        
        // update magnitude and phase
        for (int r = 0; r < paddedRows; r++) {
            for (int c = 0; c < paddedCols; c++) {
                magnitude[r][c] = fabs(real[r][c]);
                phase[r][c] = (real[r][c] >= 0) ? 0.0 : M_PI;
            }
        }
    }
    
    // Cleanup rotated mask
    for (int r = 0; r < maskSize; r++) {
        delete[] rotatedMask[r];
    }
    delete[] rotatedMask;
}

FT::FT(FrequencyFilterType type, int rows, int cols, FilterParams params) {
    paddedRows = rows;
    paddedCols = cols;
    
    this->real = new float*[paddedRows];
    this->imaginary = new float*[paddedRows];
    this->magnitude = new float*[paddedRows];
    this->phase = new float*[paddedRows];
    
    for (int r = 0; r < paddedRows; r++) {
        this->real[r] = new float[paddedCols];
        this->imaginary[r] = new float[paddedCols];
        this->magnitude[r] = new float[paddedCols];
        this->phase[r] = new float[paddedCols];
    }

    // lets create it centered at N/2, N/2
    int centerR = (int) (paddedRows / 2.0);
    int centerC = (int) (paddedCols / 2.0);
    float D0, W, c, gammaH, gammaL, a, b, T; 

    switch (type) {
        case bandPass:
            D0 = params.cutoffFrequency;
            W = params.bandwidth;
            
            for (int r = 0; r < paddedRows; r++) {
                for (int c = 0; c < paddedCols; c++) {
                    float u = r - centerR;
                    float v = c - centerC;
                    float D = sqrt(u * u + v * v);
                    
                    float H;
                    if (D < 1e-10f) {
                        H = (D0 < 1e-10f) ? 1.0 : 0.0;
                    } else {
                        float term = (D * D - D0 * D0) / (D * W);
                        H = exp(-term * term);
                    }
                    
                    this->real[r][c] = H;
                    this->imaginary[r][c] = 0.0;
                    this->magnitude[r][c] = H;
                    this->phase[r][c] = 0.0;
                }
            }
            break;
        case bandReject:
            // gaussian: H(u, v) = 1 - e^-[(D^2-D0^2)/DW]^2
            // W is width of the band
            // D is distance from center of the filter
            // D0 is cutoff frequency
            D0 = params.cutoffFrequency;
            W = params.bandwidth;
            
            for (int r = 0; r < paddedRows; r++) {
                for (int c = 0; c < paddedCols; c++) {
                    float u = r - centerR;
                    float v = c - centerC;
                    float D = sqrt(u * u + v * v);
                    
                    float H;
                    if (D < 1e-10f) {
                        H = (D0 < 1e-10f) ? 0.0 : 1.0;
                    } else {
                        float term = (D * D - D0 * D0) / (D * W);
                        H = 1.0 - exp(-term * term);
                    }
                    
                    this->real[r][c] = H;
                    this->imaginary[r][c] = 0.0;
                    this->magnitude[r][c] = H;
                    this->phase[r][c] = 0.0;
                }
            }
            break;
        case notchReject:
            // gaussian: H(u, v) = 1 - e^[-(D1(u, v) * D2(u, v)) / (2 * D0^2)]
            // D1(u, v) = sqrt[(u - N/2 - u1)^2 + (v - N/2 - v1)^2]
            // D2(u, v) = sqrt[(u - N/2 + u1)^2 + (v - N/2 + v1)^2]
            // Need to pick (+/- u1, v1)
            D0 = params.cutoffFrequency;  // notch radius
            
            // init to 1
            for (int r = 0; r < paddedRows; r++) {
                for (int c = 0; c < paddedCols; c++) {
                    this->real[r][c] = 1.0;
                    this->imaginary[r][c] = 0.0f;
                }
            }
            
            // do each notch
            for (int n = 0; n < params.numNotches; n++) {
                float u0 = params.notchLocations[n].first;   
                float v0 = params.notchLocations[n].second;
                
                for (int r = 0; r < paddedRows; r++) {
                    for (int c = 0; c < paddedCols; c++) {
                        float u = r - centerR;
                        float v = c - centerC;
                        
                        // +u,+v
                        float D1 = sqrt((u - u0) * (u - u0) + (v - v0) * (v - v0));
                        // -u,-v
                        float D2 = sqrt((u + u0) * (u + u0) + (v + v0) * (v + v0));
                        
                        float Hn;
                        if (D0 < 1e-10f) {
                            Hn = 1.0;
                        } else {
                            Hn = 1.0 - exp(-(D1 * D2) / (2.0 * D0 * D0));
                        }
                        
                        // combine!
                        this->real[r][c] *= Hn;
                    }
                }
            }
            
            // update magnitude
            for (int r = 0; r < paddedRows; r++) {
                for (int c = 0; c < paddedCols; c++) {
                    this->magnitude[r][c] = this->real[r][c];
                    this->phase[r][c] = 0.0;
                }
            }
            break;
        case hfe:
            // H(u, v) = (yh - yl)[1 - e ^ [-c * (D(u, v) / D0^2)]] + yl
            // yh is upper limit
            // yl is lower limit
            // c controls slope
            gammaH = params.gammaH;
            gammaL = params.gammaL;
            c = params.sharpness;
            D0 = params.cutoffFrequency;
            
            for (int r = 0; r < paddedRows; r++) {
                for (int c_idx = 0; c_idx < paddedCols; c_idx++) {
                    float u = r - centerR;
                    float v = c_idx - centerC;
                    float D = sqrt(u * u + v * v);
                    
                    float H;
                    if (D0 < 1e-10f) {
                        H = gammaH; 
                    } else {
                        float D2 = D * D;
                        float D0_2 = D0 * D0;
                        H = (gammaH - gammaL) * (1.0 - exp(-c * D2 / D0_2)) + gammaL;
                    }
                    
                    this->real[r][c_idx] = H;
                    this->imaginary[r][c_idx] = 0.0;
                    this->magnitude[r][c_idx] = H;
                    this->phase[r][c_idx] = 0.0;
                }
            }

            break;
        case blur:
            a = params.a;
            b = params.b;
            T = params.T;

            for (int r = 0; r < paddedRows; r++) {
                for (int c_idx = 0; c_idx < paddedCols; c_idx++) {
                    float u = r - centerR;
                    float v = c_idx - centerC;
                    
                    float kernel = M_PI * u * a + v * b;
                    float H_mag;
                    float H_real;
                    float H_imag;
                    
                    // divide by 0 handling
                    if (fabs(kernel) < 1e-10f) {
                        H_mag = T;
                        H_real = T;
                        H_imag = 0.0;
                    } else {                        
                        float sinc_term = T * sinf(kernel) / kernel;
                        
                        float exp_real = cosf(kernel);
                        float exp_imag = -sinf(kernel);
                        
                        H_real = sinc_term * exp_real;
                        H_imag = sinc_term * exp_imag;
                        H_mag = fabs(sinc_term);
                    }
                    
                    this->real[r][c_idx] = H_real;
                    this->imaginary[r][c_idx] = H_imag;
                    this->magnitude[r][c_idx] = H_mag;
                    this->phase[r][c_idx] = atan2(H_imag, H_real);
                }
            }
            break;
        default:
            // identity filter
            for (int r = 0; r < paddedRows; r++) {
                for (int c = 0; c < paddedCols; c++) {
                    this->real[r][c] = 1.0;
                    this->imaginary[r][c] = 0.0;
                    this->magnitude[r][c] = 1.0;
                    this->phase[r][c] = 0.0;
                }
            }


    }
}

FT::FT(float** real, float** imaginary, int rows, int cols) {
    paddedRows = rows;
    paddedCols = cols;
    
    this->real = new float*[paddedRows];
    this->imaginary = new float*[paddedRows];
    this->magnitude = new float*[paddedRows];
    this->phase = new float*[paddedRows];
    
    for (int r = 0; r < paddedRows; r++) {
        this->real[r] = new float[paddedCols];
        this->imaginary[r] = new float[paddedCols];
        this->magnitude[r] = new float[paddedCols];
        this->phase[r] = new float[paddedCols];
    }
        for (int r = 0; r < paddedRows; r++) {
        for (int c = 0; c < paddedCols; c++) {
            this->real[r][c] = real[r][c];
            this->imaginary[r][c] = imaginary[r][c];
            
            this->magnitude[r][c] = sqrt(real[r][c] * real[r][c] + 
                                        imaginary[r][c] * imaginary[r][c]);
            this->phase[r][c] = atan2(imaginary[r][c], real[r][c]);
        }
    }
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
    if (magnitude) {
        for (int r = 0; r < paddedRows; r++) {
            delete[] magnitude[r];
        }
        delete[] magnitude;
    }
    if (phase) {
        for (int r = 0; r < paddedRows; r++) {
            delete[] phase[r];
        }
        delete[] phase;
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

    for (int r = 0; r < paddedRows; r++) {
        for (int c = 0; c < paddedCols; c++) {
            magnitude[r][c] = sqrt(real[r][c] * real[r][c] + 
                            imaginary[r][c] * imaginary[r][c]);
            phase[r][c] = atan2(imaginary[r][c], real[r][c]);
        }
    }
}

void FT::getSpectrum(ImageType& output, FFTComponent component) {
    output.setImageInfo(paddedRows, paddedCols, 255);
    
    float** data = nullptr;
    float minVal = 0.0, maxVal = 0.0;
    bool useLogScale = false;
    
    switch(component) {
        case REAL:
            data = real;
            break;
        case IMAGINARY:
            data = imaginary;
            break;
        case MAGNITUDE:
            useLogScale = true;
            break;
        case PHASE:
            break;
    }
    
    if (component == MAGNITUDE) {
        float maxMag = 0.0;
        for (int r = 0; r < paddedRows; r++) {
            for (int c = 0; c < paddedCols; c++) {
                float mag = sqrt(real[r][c] * real[r][c] + 
                               imaginary[r][c] * imaginary[r][c]);
                if (mag > maxMag) maxMag = mag;
            }
        }
        
        // log scale
        for (int r = 0; r < paddedRows; r++) {
            for (int c = 0; c < paddedCols; c++) {
                float mag = sqrt(real[r][c] * real[r][c] + 
                               imaginary[r][c] * imaginary[r][c]);
                float logMag = log(1.0 + mag);
                int pixelVal = (int)(255.0 * logMag / (log(1.0 + maxMag)));
                output.setPixelVal(r, c, pixelVal);
            }
        }
    } else if (component == PHASE) {
        for (int r = 0; r < paddedRows; r++) {
            for (int c = 0; c < paddedCols; c++) {
                float phase = atan2(imaginary[r][c], real[r][c]);
                int pixelVal = (int)((phase + M_PI) * 255.0 / (2.0 * M_PI));
                output.setPixelVal(r, c, pixelVal);
            }
        }
    } else { // real or imaginary
        minVal = data[0][0];
        maxVal = data[0][0];
        
        for (int r = 0; r < paddedRows; r++) {
            for (int c = 0; c < paddedCols; c++) {
                if (data[r][c] < minVal) minVal = data[r][c];
                if (data[r][c] > maxVal) maxVal = data[r][c];
            }
        }
        
        float range = maxVal - minVal;
        if (range < 1e-10f) range = 1.0; 
        
        for (int r = 0; r < paddedRows; r++) {
            for (int c = 0; c < paddedCols; c++) {
                int pixelVal = (int)((data[r][c] - minVal) * 255.0 / range);
                output.setPixelVal(r, c, pixelVal);
            }
        }
    }
}

void FT::get1DGraph(ImageType& output, int row, FFTComponent component) {
    int graphHeight, col, q;
    output.getImageInfo(graphHeight, col, q);
    
    // init
    for (int r = 0; r < graphHeight; r++) {
        for (int c = 0; c < paddedCols; c++) {
            output.setPixelVal(r, c, 255);
        }
    }
    
    float* values = new float[paddedCols];
    float minVal = 0.0, maxVal = 0.0;
    
    for (int c = 0; c < paddedCols; c++) {
        switch(component) {
            case REAL:
                values[c] = real[row][c];
                break;
            case IMAGINARY:
                values[c] = imaginary[row][c];
                break;
            case MAGNITUDE:
                values[c] = sqrt(real[row][c] * real[row][c] + imaginary[row][c] * imaginary[row][c]);
                // log scale
                values[c] = log(1.0 + values[c]);
                break;
            case PHASE:
                values[c] = atan2(imaginary[row][c], real[row][c]);
                break;
        }
    }
    
    // min and max
    minVal = values[0];
    maxVal = values[0];
    for (int c = 1; c < paddedCols; c++) {
        if (values[c] < minVal) minVal = values[c];
        if (values[c] > maxVal) maxVal = values[c];
    }
    
    // 10% buffer to top and bottom for better vis
    float dataRange = maxVal - minVal;
    if (dataRange < 1e-10) {
        dataRange = 1.0;
    }
    
    float buffer = dataRange * 0.1;  // 10% buffer for min/max values
    float plotMin = minVal - buffer;
    float plotMax = maxVal + buffer;
    float plotRange = plotMax - plotMin;
    
    // fixed range for phase
    if (component == PHASE) {
        plotMin = -M_PI;
        plotMax = M_PI;
        plotRange = 2.0 * M_PI;
    }
    
    // where y=0 should be drawn
    int zeroLine = graphHeight - 1;  
    if (plotMin < 0 && plotMax > 0) {
        zeroLine = graphHeight - 1 - (int)((0.0f - plotMin) / plotRange * (graphHeight - 1));
        if (zeroLine < 0) zeroLine = 0;
        if (zeroLine >= graphHeight) zeroLine = graphHeight - 1;
    } else if (plotMax <= 0) {
        zeroLine = 0;
    }
    
    // draw the graph
    for (int c = 0; c < paddedCols; c++) {
        float normalized = (values[c] - plotMin) / plotRange;
        int y = graphHeight - 1 - (int)(normalized * (graphHeight - 1));
        
        if (y < 0) y = 0;
        if (y >= graphHeight) y = graphHeight - 1;
        
        if (y < zeroLine) {
            for (int r = y; r <= zeroLine; r++) {
                output.setPixelVal(r, c, 0);
            }
        } else {
            for (int r = zeroLine; r <= y; r++) {
                output.setPixelVal(r, c, 0);
            }
        }
    }
    
    // zero line for pos and neg reference
    for (int c = 0; c < paddedCols; c++) {
        output.setPixelVal(zeroLine, c, 128);
    }
    
    delete[] values;
}

void FT::fftShift() {
    // for 1D, only shift columns
    if (paddedRows == 1) {
        int halfCols = paddedCols / 2;
        
        for (int c = 0; c < halfCols; c++) {
            float tempReal = real[0][c];
            float tempImag = imaginary[0][c];
            real[0][c] = real[0][c + halfCols];
            imaginary[0][c] = imaginary[0][c + halfCols];
            real[0][c + halfCols] = tempReal;
            imaginary[0][c + halfCols] = tempImag;
        }
        return;
    }
    
    // for 2D data, swap quadrants
    int halfRows = paddedRows / 2;
    int halfCols = paddedCols / 2;

    for (int r = 0; r < halfRows; r++) {
        for (int c = 0; c < halfCols; c++) {
            // swap quadrant 1 with quadrant 3
            float tempReal = real[r][c];
            float tempImag = imaginary[r][c];
            real[r][c] = real[r + halfRows][c + halfCols];
            imaginary[r][c] = imaginary[r + halfRows][c + halfCols];
            real[r + halfRows][c + halfCols] = tempReal;
            imaginary[r + halfRows][c + halfCols] = tempImag;
            
            // swap quadrant 2 with quadrant 4
            tempReal = real[r][c + halfCols];
            tempImag = imaginary[r][c + halfCols];
            real[r][c + halfCols] = real[r + halfRows][c];
            imaginary[r][c + halfCols] = imaginary[r + halfRows][c];
            real[r + halfRows][c] = tempReal;
            imaginary[r + halfRows][c] = tempImag;
        }
    }
}

void FT::addGaussianNoise(float mu, float sigma) {
    // using box muller method
    // with two uniform random variables U1, U2 in (0, 1]:
    // Z0 = sqrt(-2 * ln(U1)) * cos(2π * U2)
    // Z1 = sqrt(-2 * ln(U1)) * sin(2π * U2)
    // Z0 and Z1 are independent standard normal variables N(0,1)
    // to get N(mu, sigma): X = mu + sigma * Z

    // from wikipedia: https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
    constexpr double two_pi = 2.0 * M_PI;

    // initialize the random uniform number generator (runif) in a range 0 to 1
    static std::mt19937 rng(std::random_device{}()); // Standard mersenne_twister_engine seeded with rd()
    static std::uniform_real_distribution<> runif(0.0, 1.0);

    for (int r = 0; r < paddedRows; r++) {
        for (int c = 0; c < paddedCols; c += 2) {

            //create two random numbers, make sure u1 is greater than zero
            double u1, u2;
            do
            {
                u1 = runif(rng);
            }
            while (u1 == 0);
            u2 = runif(rng);

            //compute z0 and z1
            auto mag = sigma * sqrt(-2.0 * log(u1));
            auto z0  = mag * cos(two_pi * u2) + mu;
            auto z1  = mag * sin(two_pi * u2) + mu;
            
            // transform to N
            float noise0 = mu + sigma * z0;
            float noise1 = mu + sigma * z1;
            
            // add noise to real component
            real[r][c] += noise0;
            
            // handle odd columns
            if (c + 1 < paddedCols) {
                real[r][c + 1] += noise1;
            }
        }
    }
    
    // update magnitude and phase after adding noise
    for (int r = 0; r < paddedRows; r++) {
        for (int c = 0; c < paddedCols; c++) {
            magnitude[r][c] = sqrt(real[r][c] * real[r][c] + 
                                   imaginary[r][c] * imaginary[r][c]);
            phase[r][c] = atan2(imaginary[r][c], real[r][c]);
        }
    }
}