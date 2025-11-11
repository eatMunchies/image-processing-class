/*
Author: Anthony Silva
Assignment: Programming Assignment 3
Class: Image Processing
Date: 11/10/25
File: Experiment3.cpp
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

int main (int argc, char* argv[]) 
{
    // compute dft of lenna
    // get lenna
    int N, M, Q;
    bool type;
    readImageHeader("./lenna.pgm", N, M, Q, type);
    ImageType lenna(N, M, Q);
    readImage("./lenna.pgm", lenna);

    // compute dft
    FT lennaFT(lenna);
    lennaFT.fft(false, true);

    // part a
    // reconstruct from magnitude
    // set real part to be magnitude, imaginary to 0
    // do this by creating a new FT with the real part from the magnitude of the FT and the imaginary part as 0s
    ImageType lennaMagnitude(lennaFT.paddedRows, lennaFT.paddedCols, 255);
    lennaFT.getSpectrum(lennaMagnitude, MAGNITUDE);
    writeImage("./Experiment3PartAMagnitude.pgm", lennaMagnitude);
    // now we can take the inverse FT of the magnitude
    float** zeroArray = new float*[lennaFT.paddedRows];
    for (int i = 0; i < lennaFT.paddedRows; i++) {
        zeroArray[i] = new float[lennaFT.paddedCols];
        for (int j = 0; j < lennaFT.paddedCols; j++) {
            zeroArray[i][j] = 0.0;
        }
    }
    FT lennaMagnitudeFT(lennaFT.magnitude, zeroArray, lennaFT.paddedRows, lennaFT.paddedCols);
    lennaMagnitudeFT.fft(false, false);
    lennaMagnitudeFT.fftShift();
    ImageType lennaMagnitudeReverse(lennaMagnitudeFT.paddedRows, lennaMagnitudeFT.paddedCols, 255);
    lennaMagnitudeFT.getSpectrum(lennaMagnitudeReverse, REAL);
    writeImage("./Experiment3PartAReverseMagnitude.pgm", lennaMagnitudeReverse);

    // part b
    // reconstruct from phase
    ImageType lennaPhase(lennaFT.paddedRows, lennaFT.paddedCols, 255);
    lennaFT.getSpectrum(lennaPhase, PHASE);
    writeImage("./Experiment3PartBPhase.pgm", lennaPhase);
    float** cosTheta = new float*[lennaFT.paddedRows];
    float** sinTheta = new float*[lennaFT.paddedRows];
    for (int i = 0; i < lennaFT.paddedRows; i++) {
        cosTheta[i] = new float[lennaFT.paddedCols];
        sinTheta[i] = new float[lennaFT.paddedCols];
    }
    for (int r = 0; r < lennaFT.paddedRows; r++) {
        for (int c = 0; c < lennaFT.paddedCols; c++) {
            cosTheta[r][c] = cos(lennaFT.phase[r][c]);
            sinTheta[r][c] = sin(lennaFT.phase[r][c]);
        }
    }
    FT lennaPhaseFT(cosTheta, sinTheta, lennaFT.paddedRows, lennaFT.paddedCols);
    lennaPhaseFT.fft(false, false);
    ImageType lennaPhaseReverse(lennaPhaseFT.paddedRows, lennaPhaseFT.paddedCols, 255);
    lennaPhaseFT.getSpectrum(lennaPhaseReverse, REAL);
    writeImage("./Experiment3PartBReversePhase.pgm", lennaPhaseReverse);

    // cleanup
    for (int i = 0; i < lennaFT.paddedRows; i++) {
        delete[] zeroArray[i];
        delete[] cosTheta[i];
        delete[] sinTheta[i];
    }
    delete[] zeroArray;
    delete[] cosTheta;
    delete[] sinTheta;
    
}