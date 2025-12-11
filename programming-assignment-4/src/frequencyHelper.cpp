#include "frequencyHelper.h"
#include <iostream>
#include <cmath>

FrequencyHelper::FrequencyHelper() {
    // Default constructor
}

void FrequencyHelper::UpdateMagnitudePhase(FT& ft) {
    for (int r = 0; r < ft.paddedRows; r++) {
        for (int c = 0; c < ft.paddedCols; c++) {
            ft.magnitude[r][c] = sqrt(ft.real[r][c] * ft.real[r][c] + 
                                      ft.imaginary[r][c] * ft.imaginary[r][c]);
            ft.phase[r][c] = atan2(ft.imaginary[r][c], ft.real[r][c]);
        }
    }
}

void FrequencyHelper::ElementWiseMultiplication(FT& a, FT& b, FT& output) {
    // complex multiplication: (a + bi)(c + di) = (ac - bd) + (ad + bc)i
    
    for (int r = 0; r < a.paddedRows; r++) {
        for (int c = 0; c < a.paddedCols; c++) {
            float aReal = a.real[r][c];
            float aImag = a.imaginary[r][c];
            float bReal = b.real[r][c];
            float bImag = b.imaginary[r][c];
            
            output.real[r][c] = aReal * bReal - aImag * bImag;
            output.imaginary[r][c] = aReal * bImag + aImag * bReal;
        }
    }
    
    UpdateMagnitudePhase(output);
}

void FrequencyHelper::ElementWiseDivision(FT& a, FT& b, FT& output, float epsilon) {
    // complex division: (a + bi) / (c + di) = [(ac + bd) + (bc - ad)i] / (c^2 + d^2)
    
    for (int r = 0; r < a.paddedRows; r++) {
        for (int c = 0; c < a.paddedCols; c++) {
            float aReal = a.real[r][c];
            float aImag = a.imaginary[r][c];
            float bReal = b.real[r][c];
            float bImag = b.imaginary[r][c];
            
            float denominator = bReal * bReal + bImag * bImag;
            
            // stop divide by 0
            if (denominator < epsilon) {
                output.real[r][c] = 0.0;
                output.imaginary[r][c] = 0.0;
            } else {
                output.real[r][c] = (aReal * bReal + aImag * bImag) / denominator;
                output.imaginary[r][c] = (aImag * bReal - aReal * bImag) / denominator;
            }
        }
    }
    
    UpdateMagnitudePhase(output);
}

void FrequencyHelper::InverseFilter(FT& degradedImage, FT& degradationFunction, FT& output, float radius, float epsilon) {
    // inverse filtering: F_hat(u,v) = G(u,v) / H(u,v)
    // this occurs only within the specified radius
    
    float centerR = degradedImage.paddedRows / 2.0;
    float centerC = degradedImage.paddedCols / 2.0;
    bool useRadius = (radius > 0.0);
    
    for (int r = 0; r < degradedImage.paddedRows; r++) {
        for (int c = 0; c < degradedImage.paddedCols; c++) {
            float u = r - centerR;
            float v = c - centerC;
            float D = sqrt(u * u + v * v);
            
            if (useRadius && D > radius) {
                // outside range, just copy the value
                output.real[r][c] = degradedImage.real[r][c];
                output.imaginary[r][c] = degradedImage.imaginary[r][c];
            } else {
                // inside the range, apply the transform
                float gReal = degradedImage.real[r][c];
                float gImag = degradedImage.imaginary[r][c];
                float hReal = degradationFunction.real[r][c];
                float hImag = degradationFunction.imaginary[r][c];
                
                float hMagSquared = hReal * hReal + hImag * hImag;
                
                if (hMagSquared < epsilon) {
                    // val is too close to 0, just assume 0. 
                    output.real[r][c] = 0.0;
                    output.imaginary[r][c] = 0.0;
                } else {
                    // complex division
                    output.real[r][c] = (gReal * hReal + gImag * hImag) / hMagSquared;
                    output.imaginary[r][c] = (gImag * hReal - gReal * hImag) / hMagSquared;
                }
            }
        }
    }
    
    UpdateMagnitudePhase(output);
}

void FrequencyHelper::WienerFilter(FT& degradedImage, FT& degradationFunction, FT& output, float K) {
    // F_hat(u,v) = [H*(u,v) / (|H(u,v)|^2 + K)] * G(u,v)
    // K typically 0.001 to 0.1
    // larger k is more smoothing, less noise more blur
    // smaller k is less smoothing and approaches inverse filter
    
    for (int r = 0; r < degradedImage.paddedRows; r++) {
        for (int c = 0; c < degradedImage.paddedCols; c++) {
            float gReal = degradedImage.real[r][c];
            float gImag = degradedImage.imaginary[r][c];
            float hReal = degradationFunction.real[r][c];
            float hImag = degradationFunction.imaginary[r][c];
            
            float hMagSquared = hReal * hReal + hImag * hImag;
            
            // H*(u,v)
            float hConjReal = hReal;
            float hConjImag = -hImag;
            
            // denom
            float denominator = hMagSquared + K;
            
            // wiener filter transfer function: H* / (|H|^2 + K)
            float wReal = hConjReal / denominator;
            float wImag = hConjImag / denominator;
            
            // multiply by G
            output.real[r][c] = wReal * gReal - wImag * gImag;
            output.imaginary[r][c] = wReal * gImag + wImag * gReal;
        }
    }
    
    UpdateMagnitudePhase(output);
}
