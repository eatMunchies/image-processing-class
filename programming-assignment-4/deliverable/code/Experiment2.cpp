/*
Author: Anthony Silva
Assignment: Programming Assignment 4
Class: Image Processing
Date: 12/10/25
File: Experiment2.cpp
Description:

*/

#include "image.h"
#include "FT.h"
#include "mask.h"
#include "correlationHelper.h"
#include "frequencyHelper.h"
#include <cstring>
#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

int main(int argc, char* argv[]) 
{
    // get image
    int N, M, Q;
    bool type;
    readImageHeader("./lenna.pgm", N, M, Q, type);
    ImageType lenna(N, M, Q);
    readImage("./lenna.pgm", lenna);
    FT lennaFT(lenna);
    lennaFT.fft(false, true);
    lennaFT.fftShift(); // center it
    // visualize
    ImageType lennaMagnitude(lennaFT.paddedRows, lennaFT.paddedCols, 255);
    lennaFT.getSpectrum(lennaMagnitude, MAGNITUDE);
    writeImage("./Experiment2_Lenna_Magnitude.pgm", lennaMagnitude);

    // prepare mask h and H
    Mask sobel("./sobel.txt");
    FT H(sobel, N, M, true);
    H.fftShift();

    // visualize H
    ImageType HMaskVis(H.paddedRows, H.paddedCols, 255);
    H.getSpectrum(HMaskVis, MAGNITUDE);
    writeImage("./Experiment2_HMaskVis.pgm", HMaskVis);

    // perform convolution on lenna image in frequency domain
    FrequencyHelper helper;
    FT sobelFilteredFT(lennaFT.real, lennaFT.imaginary, lennaFT.paddedRows, lennaFT.paddedCols);
    helper.ElementWiseMultiplication(lennaFT, H, sobelFilteredFT);

    // visualize magnitude
    ImageType sobelFilteredMagnitude(lennaFT.paddedRows, lennaFT.paddedCols, 255);
    sobelFilteredFT.getSpectrum(sobelFilteredMagnitude, MAGNITUDE);
    writeImage("./Experiment2_sobelMagnitude.pgm", sobelFilteredMagnitude);

    // reverse
    sobelFilteredFT.fftShift();
    sobelFilteredFT.fft(false, false);

    // visualize filtered lenna
    ImageType sobelFilterResult(lennaFT.paddedRows, lennaFT.paddedCols, 255);
    sobelFilteredFT.fftShift();
    sobelFilteredFT.getSpectrum(sobelFilterResult, REAL);
    writeImage("./Experiment2_SobelFilter_Result.pgm", sobelFilterResult);

    // perform convolution on lenna image in spatial domain
    CorrelationHelper correlator;
    ImageType lennaSobel(N, M, Q);
    sobel.applyMask(lenna, lennaSobel, correlator);
    writeImage("./Experiment2_SobelCorrelation_Result.pgm", lennaSobel);

}