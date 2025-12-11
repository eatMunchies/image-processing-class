/*
Author: Anthony Silva
Assignment: Programming Assignment 4
Class: Image Processing
Date: 12/10/25
File: Experiment3.cpp
Description:

*/

#include "image.h"
#include "FT.h"
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
    readImageHeader("./girl.pgm", N, M, Q, type);
    ImageType girl(N, M, Q);
    readImage("./girl.pgm", girl);
    FT girlFT(girl);
    girlFT.fft(false, true);
    girlFT.fftShift();
    ImageType girlMagVis(girlFT.paddedCols, girlFT.paddedRows, 255);
    girlFT.getSpectrum(girlMagVis, MAGNITUDE);
    writeImage("./Experiment3_girlFTMagVis.pgm", girlMagVis);

    // run high frequency emphasis filter with various parameters
    float D0 = 1.8;
    float c = 1;
    FilterParams HFEParams1 = FilterParams::HighFrequencyEmphasis(D0, 1.0, 0.0, c);
    FilterParams HFEParams2 = FilterParams::HighFrequencyEmphasis(D0, 1.5, 0.0, c);
    FilterParams HFEParams3 = FilterParams::HighFrequencyEmphasis(D0, 2.0, 0.0, c);
    FilterParams HFEParams4 = FilterParams::HighFrequencyEmphasis(D0, 1.0, 0.5, c);
    FilterParams HFEParams5 = FilterParams::HighFrequencyEmphasis(D0, 1.5, 0.5, c);
    FilterParams HFEParams6 = FilterParams::HighFrequencyEmphasis(D0, 2.0, 0.5, c);
    FilterParams HFEParams7 = FilterParams::HighFrequencyEmphasis(D0, 1.0, 1.0, c);
    FilterParams HFEParams8 = FilterParams::HighFrequencyEmphasis(D0, 1.5, 1.0, c);
    FilterParams HFEParams9 = FilterParams::HighFrequencyEmphasis(D0, 2.0, 1.0, c);
    FT HFE1(hfe, N, M, HFEParams1);
    FT HFE2(hfe, N, M, HFEParams2);
    FT HFE3(hfe, N, M, HFEParams3);
    FT HFE4(hfe, N, M, HFEParams4);
    FT HFE5(hfe, N, M, HFEParams5);
    FT HFE6(hfe, N, M, HFEParams6);
    FT HFE7(hfe, N, M, HFEParams7);
    FT HFE8(hfe, N, M, HFEParams8);
    FT HFE9(hfe, N, M, HFEParams9);

    // visualize each HFE
    ImageType HFE1Mag(girlFT.paddedRows, girlFT. paddedCols, 255);
    HFE1.getSpectrum(HFE1Mag, MAGNITUDE);
    writeImage("./Experiment3_HFE_1.pgm", HFE1Mag);
    ImageType HFE2Mag(girlFT.paddedRows, girlFT. paddedCols, 255);
    HFE2.getSpectrum(HFE2Mag, MAGNITUDE);
    writeImage("./Experiment3_HFE_2.pgm", HFE2Mag);
    ImageType HFE3Mag(girlFT.paddedRows, girlFT. paddedCols, 255);
    HFE3.getSpectrum(HFE3Mag, MAGNITUDE);
    writeImage("./Experiment3_HFE_3.pgm", HFE3Mag);
    ImageType HFE4Mag(girlFT.paddedRows, girlFT. paddedCols, 255);
    HFE4.getSpectrum(HFE4Mag, MAGNITUDE);
    writeImage("./Experiment3_HFE_4.pgm", HFE4Mag);
    ImageType HFE5Mag(girlFT.paddedRows, girlFT. paddedCols, 255);
    HFE5.getSpectrum(HFE5Mag, MAGNITUDE);
    writeImage("./Experiment3_HFE_5.pgm", HFE5Mag);
    ImageType HFE6Mag(girlFT.paddedRows, girlFT. paddedCols, 255);
    HFE6.getSpectrum(HFE6Mag, MAGNITUDE);
    writeImage("./Experiment3_HFE_6.pgm", HFE6Mag);
    ImageType HFE7Mag(girlFT.paddedRows, girlFT. paddedCols, 255);
    HFE7.getSpectrum(HFE7Mag, MAGNITUDE);
    writeImage("./Experiment3_HFE_7.pgm", HFE7Mag);
    ImageType HFE8Mag(girlFT.paddedRows, girlFT. paddedCols, 255);
    HFE8.getSpectrum(HFE8Mag, MAGNITUDE);
    writeImage("./Experiment3_HFE_8.pgm", HFE8Mag);
    ImageType HFE9Mag(girlFT.paddedRows, girlFT. paddedCols, 255);
    HFE9.getSpectrum(HFE9Mag, MAGNITUDE);
    writeImage("./Experiment3_HFE_9.pgm", HFE9Mag);
    
    FrequencyHelper helper;

    FT HFT1(girlFT.real, girlFT.imaginary, girlFT.paddedRows, girlFT.paddedCols);
    FT HFT2(girlFT.real, girlFT.imaginary, girlFT.paddedRows, girlFT.paddedCols);
    FT HFT3(girlFT.real, girlFT.imaginary, girlFT.paddedRows, girlFT.paddedCols);
    FT HFT4(girlFT.real, girlFT.imaginary, girlFT.paddedRows, girlFT.paddedCols);
    FT HFT5(girlFT.real, girlFT.imaginary, girlFT.paddedRows, girlFT.paddedCols);
    FT HFT6(girlFT.real, girlFT.imaginary, girlFT.paddedRows, girlFT.paddedCols);
    FT HFT7(girlFT.real, girlFT.imaginary, girlFT.paddedRows, girlFT.paddedCols);
    FT HFT8(girlFT.real, girlFT.imaginary, girlFT.paddedRows, girlFT.paddedCols);
    FT HFT9(girlFT.real, girlFT.imaginary, girlFT.paddedRows, girlFT.paddedCols);

    helper.ElementWiseMultiplication(girlFT, HFE1, HFT1);
    helper.ElementWiseMultiplication(girlFT, HFE2, HFT2);
    helper.ElementWiseMultiplication(girlFT, HFE3, HFT3);
    helper.ElementWiseMultiplication(girlFT, HFE4, HFT4);
    helper.ElementWiseMultiplication(girlFT, HFE5, HFT5);
    helper.ElementWiseMultiplication(girlFT, HFE6, HFT6);
    helper.ElementWiseMultiplication(girlFT, HFE7, HFT7);
    helper.ElementWiseMultiplication(girlFT, HFE8, HFT8);
    helper.ElementWiseMultiplication(girlFT, HFE9, HFT9);

    // see results
    ImageType HFT1Mag(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT2Mag(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT3Mag(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT4Mag(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT5Mag(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT6Mag(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT7Mag(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT8Mag(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT9Mag(girlFT.paddedRows, girlFT.paddedCols, 255);

    HFT1.getSpectrum(HFT1Mag, MAGNITUDE);
    HFT2.getSpectrum(HFT2Mag, MAGNITUDE);
    HFT3.getSpectrum(HFT3Mag, MAGNITUDE);
    HFT4.getSpectrum(HFT4Mag, MAGNITUDE);
    HFT5.getSpectrum(HFT5Mag, MAGNITUDE);
    HFT6.getSpectrum(HFT6Mag, MAGNITUDE);
    HFT7.getSpectrum(HFT7Mag, MAGNITUDE);
    HFT8.getSpectrum(HFT8Mag, MAGNITUDE);
    HFT9.getSpectrum(HFT9Mag, MAGNITUDE);

    writeImage("./Experiment3_HFE_FT_1.pgm", HFT1Mag);
    writeImage("./Experiment3_HFE_FT_2.pgm", HFT2Mag);
    writeImage("./Experiment3_HFE_FT_3.pgm", HFT3Mag);
    writeImage("./Experiment3_HFE_FT_4.pgm", HFT4Mag);
    writeImage("./Experiment3_HFE_FT_5.pgm", HFT5Mag);
    writeImage("./Experiment3_HFE_FT_6.pgm", HFT6Mag);
    writeImage("./Experiment3_HFE_FT_7.pgm", HFT7Mag);
    writeImage("./Experiment3_HFE_FT_8.pgm", HFT8Mag);
    writeImage("./Experiment3_HFE_FT_9.pgm", HFT9Mag);

    // reverse the FT
    HFT1.fftShift();
    HFT1.fft(false, false);
    HFT2.fftShift();
    HFT2.fft(false, false);
    HFT3.fftShift();
    HFT3.fft(false, false);
    HFT4.fftShift();
    HFT4.fft(false, false);
    HFT5.fftShift();
    HFT5.fft(false, false);
    HFT6.fftShift();
    HFT6.fft(false, false);
    HFT7.fftShift();
    HFT7.fft(false, false);
    HFT8.fftShift();
    HFT8.fft(false, false);
    HFT9.fftShift();
    HFT9.fft(false, false);

    // see outputs
    ImageType HFT1Result(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT2Result(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT3Result(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT4Result(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT5Result(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT6Result(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT7Result(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT8Result(girlFT.paddedRows, girlFT.paddedCols, 255);
    ImageType HFT9Result(girlFT.paddedRows, girlFT.paddedCols, 255);
    
    HFT1.getSpectrum(HFT1Result, MAGNITUDE);
    HFT2.getSpectrum(HFT2Result, MAGNITUDE);
    HFT3.getSpectrum(HFT3Result, MAGNITUDE);
    HFT4.getSpectrum(HFT4Result, MAGNITUDE);
    HFT5.getSpectrum(HFT5Result, MAGNITUDE);
    HFT6.getSpectrum(HFT6Result, MAGNITUDE);
    HFT7.getSpectrum(HFT7Result, MAGNITUDE);
    HFT8.getSpectrum(HFT8Result, MAGNITUDE);
    HFT9.getSpectrum(HFT9Result, MAGNITUDE);

    writeImage("./Experiment3_HFE_FT_Result_1.pgm", HFT1Result);
    writeImage("./Experiment3_HFE_FT_Result_2.pgm", HFT2Result);
    writeImage("./Experiment3_HFE_FT_Result_3.pgm", HFT3Result);
    writeImage("./Experiment3_HFE_FT_Result_4.pgm", HFT4Result);
    writeImage("./Experiment3_HFE_FT_Result_5.pgm", HFT5Result);
    writeImage("./Experiment3_HFE_FT_Result_6.pgm", HFT6Result);
    writeImage("./Experiment3_HFE_FT_Result_7.pgm", HFT7Result);
    writeImage("./Experiment3_HFE_FT_Result_8.pgm", HFT8Result);
    writeImage("./Experiment3_HFE_FT_Result_9.pgm", HFT9Result);
}