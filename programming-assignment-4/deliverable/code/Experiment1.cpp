/*
Author: Anthony Silva
Assignment: Programming Assignment 4
Class: Image Processing
Date: 12/10/25
File: Experiment1.cpp
Description:
Removing Noise in Frequency Domain
*/

#include "frequencyHelper.h"
#include "image.h"
#include "FT.h"
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
    readImageHeader("./noisy_boy.pgm", N, M, Q, type);
    ImageType noisyBoy(N, M, Q);
    readImage("./noisy_boy.pgm", noisyBoy);

    // get the FT of it
    FT noisyBoyFT(noisyBoy);
    noisyBoyFT.fft(false, true);
    // visualzie it
    ImageType noisyBoyMagnitude(noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, 255);
    noisyBoyFT.fftShift();
    noisyBoyFT.getSpectrum(noisyBoyMagnitude, MAGNITUDE);
    writeImage("./Experiment1_NoisyBoy_Magnitude.pgm", noisyBoyMagnitude);

    // part a - band reject and notch filter compared to gaussian filtering
    float D0 = 35.8;
    float W = 1;
    FrequencyHelper helper;
    FilterParams brParams = FilterParams::BandReject(D0, W);
    FT bandRejectFilter(bandReject, noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, brParams);

    // visualize filter
    ImageType brFilterVis(noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, 255);
    bandRejectFilter.getSpectrum(brFilterVis, MAGNITUDE);
    writeImage("./Experiment1_BandRejectFilter.pgm", brFilterVis);

    // apply filter
    FT bandFilteredFT(noisyBoyFT.real, noisyBoyFT.imaginary, noisyBoyFT.paddedRows, noisyBoyFT.paddedCols);
    helper.ElementWiseMultiplication(noisyBoyFT, bandRejectFilter, bandFilteredFT);

    // visualize filtered magnitude spectrum
    ImageType bandFilteredMag(noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, 255);
    bandFilteredFT.getSpectrum(bandFilteredMag, MAGNITUDE);
    writeImage("./Experiment1_BandReject_FilteredSpectrum.pgm", bandFilteredMag);

    // inverse fft for result
    bandFilteredFT.fftShift();
    bandFilteredFT.fft(false, false);

    // visualize output
    ImageType bandRejectResult(noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, 255);
    bandFilteredFT.getSpectrum(bandRejectResult, REAL);
    writeImage("./Experiment1_BandReject_Result.pgm", bandRejectResult);

    // now lets do notch reject
    vector<pair<float, float>> notchLocations;
    notchLocations.push_back({15.9, 31.87});
    notchLocations.push_back({-15.9, 31.87});
    FilterParams nrParams = FilterParams::NotchReject(13, notchLocations);
    FT notchRejectFilter(notchReject, noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, nrParams);

    // vis filter
    ImageType nrFilterVis(noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, 255);
    notchRejectFilter.getSpectrum(nrFilterVis, MAGNITUDE);
    writeImage("./Experiment1_NotchRejectFilter.pgm", nrFilterVis);

    // apply filter
    FT notchFilteredFT(noisyBoyFT.real, noisyBoyFT.imaginary, noisyBoyFT.paddedRows, noisyBoyFT.paddedCols);
    helper.ElementWiseMultiplication(noisyBoyFT, notchRejectFilter, notchFilteredFT);

    // visualize
    ImageType notchFilteredMag(noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, 255);
    notchFilteredFT.getSpectrum(notchFilteredMag, MAGNITUDE);
    writeImage("./Experiment1_NotchReject_FilteredSpectrum.pgm", notchFilteredMag);

    // inverse fft for result
    notchFilteredFT.fftShift();
    notchFilteredFT.fft(false, false);

    // visualize output
    ImageType notchRejectResult(noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, 255);
    notchFilteredFT.getSpectrum(notchRejectResult, REAL);
    writeImage("./Experiment1_NotchReject_Result.pgm", notchRejectResult);

    // part b - extract noise instead of remove it (band pass)
    FilterParams bpParams = FilterParams::BandPass(D0, W);
    FT bandPassFilter(bandPass, noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, brParams);

    // visualize filter
    ImageType bpFilterVis(noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, 255);
    bandPassFilter.getSpectrum(bpFilterVis, MAGNITUDE);
    writeImage("./Experiment1_BandPassFilter.pgm", bpFilterVis);

    // apply filter
    FT bandPassFilteredFT(noisyBoyFT.real, noisyBoyFT.imaginary, noisyBoyFT.paddedRows, noisyBoyFT.paddedCols);
    helper.ElementWiseMultiplication(noisyBoyFT, bandPassFilter, bandPassFilteredFT);

    // visualize filtered magnitude spectrum
    ImageType bandPassFilteredMag(noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, 255);
    bandPassFilteredFT.getSpectrum(bandPassFilteredMag, MAGNITUDE);
    writeImage("./Experiment1_BandPass_FilteredSpectrum.pgm", bandPassFilteredMag);

    // inverse fft for result
    bandPassFilteredFT.fftShift();
    bandPassFilteredFT.fft(false, false);

    // visualize output
    ImageType bandPassResult(noisyBoyFT.paddedRows, noisyBoyFT.paddedCols, 255);
    bandPassFilteredFT.getSpectrum(bandPassResult, REAL);
    writeImage("./Experiment1_BandPass_Result.pgm", bandPassResult);
    
}