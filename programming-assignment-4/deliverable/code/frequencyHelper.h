#ifndef FREQUENCY_HELPER_H
#define FREQUENCY_HELPER_H

#include "FT.h"
#include <cmath>

class FrequencyHelper {
public:
    FrequencyHelper();
    
    void ElementWiseMultiplication(FT& a, FT& b, FT& output);
    void ElementWiseDivision(FT& a, FT& b, FT& output, float epsilon = 1e-10f);
    void InverseFilter(FT& degradedImage, FT& degradationFunction, FT& output, float radius = 0.0, float epsilon = 1e-10f);
    void WienerFilter(FT& degradedImage, FT& degradationFunction, FT& output, float K = 0.01);

private:
    // helper to update magnitude and phase from real/imaginary
    void UpdateMagnitudePhase(FT& ft);
};

#endif