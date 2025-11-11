/*
Author: Anthony Silva
Assignment: Programming Assignment 3
Class: Image Processing
Date: 11/10/25
File: Experiment2.cpp
Description:
2D Fourier Transform Exploration
*/

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

void drawCenterSquare(ImageType& image, int size) {
    int N, M, Q;
    image.getImageInfo(N, M, Q);

    int startN = N / 2 - size / 2;
    int startM = M / 2 - size / 2;

    for (int n = startN; n < startN + size; n++) {
        for (int m = startM; m < startM + size; m++) {
            image.setPixelVal(n, m, Q);
        }
    }
}

int main(int argc, char* argv[]) {

    int Q = 255;

    // part a
    // generate size 32 image
    ImageType partA(512, 512, Q);
    drawCenterSquare(partA, 32);
    FT partAFT(partA);
    ImageType partARectangle(partAFT.paddedRows, partAFT.paddedCols, Q);
    partAFT.getSpectrum(partARectangle, REAL);
    writeImage("./Experiment2PartARectangle.pgm", partARectangle);
    // run dft
    partAFT.fft(false, true);
    ImageType partAMagnitudeNoShift(partAFT.paddedRows, partAFT.paddedCols, Q);
    partAFT.getSpectrum(partAMagnitudeNoShift, MAGNITUDE);
    writeImage("./Experiment2PartAMagnitudeNoShift.pgm", partAMagnitudeNoShift);
    partAFT.fftShift();
    ImageType partAMagnitudeShift(partAFT.paddedRows, partAFT.paddedCols, Q);
    partAFT.getSpectrum(partAMagnitudeShift, MAGNITUDE);
    writeImage("./Experiment2PartAMagnitudeShift.pgm", partAMagnitudeShift);

    // part b 
    // generate size 64 image
    ImageType partB(512, 512, 255);
    drawCenterSquare(partB, 64);
    FT partBFT(partB);
    ImageType partBRectangle(partBFT.paddedRows, partBFT.paddedCols, Q);
    partBFT.getSpectrum(partBRectangle, REAL);
    writeImage("./Experiment2partBRectangle.pgm", partBRectangle);
    // run dft
    partBFT.fft(false, true);
    ImageType partBMagnitudeNoShift(partBFT.paddedRows, partBFT.paddedCols, Q);
    partBFT.getSpectrum(partBMagnitudeNoShift, MAGNITUDE);
    writeImage("./Experiment2partBMagnitudeNoShift.pgm", partBMagnitudeNoShift);
    partBFT.fftShift();
    ImageType partBMagnitudeShift(partBFT.paddedRows, partBFT.paddedCols, Q);
    partBFT.getSpectrum(partBMagnitudeShift, MAGNITUDE);
    writeImage("./Experiment2partBMagnitudeShift.pgm", partBMagnitudeShift);

    // part c
    // generate size 128 image
    ImageType partC(512, 512, 255);
    drawCenterSquare(partC, 128);
    FT partCFT(partC);
    ImageType partCRectangle(partCFT.paddedRows, partCFT.paddedCols, Q);
    partCFT.getSpectrum(partCRectangle, REAL);
    writeImage("./Experiment2partCRectangle.pgm", partCRectangle);
    // run dft
    partCFT.fft(false, true);
    ImageType partCMagnitudeNoShift(partCFT.paddedRows, partCFT.paddedCols, Q);
    partCFT.getSpectrum(partCMagnitudeNoShift, MAGNITUDE);
    writeImage("./Experiment2partCMagnitudeNoShift.pgm", partCMagnitudeNoShift);
    partCFT.fftShift();
    ImageType partCMagnitudeShift(partCFT.paddedRows, partCFT.paddedCols, Q);
    partCFT.getSpectrum(partCMagnitudeShift, MAGNITUDE);
    writeImage("./Experiment2partCMagnitudeShift.pgm", partCMagnitudeShift);


    return 0;
}