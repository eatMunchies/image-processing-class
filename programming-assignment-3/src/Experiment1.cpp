/*
Author: Anthony Silva
Assignment: Programming Assignment 3
Class: Image Processing
Date: 11/10/25
File: Experiment1.cpp
Description:
1D Fourier Transform Exploration
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

void generateStatistics(FT& ft) {
    // After running FFT
    float** realPart = ft.real;
    float** imagPart = ft.imaginary;
    int cols = ft.paddedCols;

    float sumReal = 0, sumImag = 0;
    float maxReal = realPart[0][0], maxImag = imagPart[0][0];

    for (int i = 0; i < cols; i++) {
        sumReal += realPart[0][i];
        sumImag += imagPart[0][i];
        
        if (abs(realPart[0][i]) > abs(maxReal)) maxReal = realPart[0][i];
        if (abs(imagPart[0][i]) > abs(maxImag)) maxImag = imagPart[0][i];
    }

    cout << "\n********STATISTICS*********\n" << endl;
    cout << "Average Real: " << sumReal / cols << endl;
    cout << "Average Imag: " << sumImag / cols << endl;
    cout << "Max Real: " << maxReal << endl;
    cout << "Max Imag: " << maxImag << endl;

    // see values at expected impulse locations
    cout << "\nValues at index 8:" << endl;
    cout << "  Real: " << realPart[0][56] << endl;
    cout << "  Imag: " << imagPart[0][56] << endl;
    cout << "  Mag: " << sqrt(realPart[0][56]*realPart[0][56] + imagPart[0][56]*imagPart[0][56]) << endl;

    cout << "\nValues at index 120:" << endl;
    cout << "  Real: " << realPart[0][72] << endl;
    cout << "  Imag: " << imagPart[0][72] << endl;
    cout << "  Mag: " << sqrt(realPart[0][72]*realPart[0][72] + imagPart[0][72]*imagPart[0][72]) << endl;
}

int main(int argc, char* argv[]) 
{
    // part a
    // f = [2, 3, 4, 4]
    int f[] = {2, 3, 4, 4};
    ImageType partA(1, 4, 255);
    for (int i = 0; i < 4; i++) {
        partA.setPixelVal(0, i, f[i]);
    }
    // compute dft
    FT partAFFT(partA);
    int partASize = 16;
    ImageType partAInit(partASize, partAFFT.paddedCols, 255);
    partAFFT.get1DGraph(partAInit, 0, REAL);
    writeImage("./Experiment1PartAInitial.pgm", partAInit);
    partAFFT.fft(true, true);
    // no need to shift for visualization here, we can check the math directly
    // plot real, imaginary, magnitude
    ImageType partAReal(partASize, partAFFT.paddedCols, 255);
    ImageType partAImaginary(partASize, partAFFT.paddedCols, 255);
    ImageType partAMagnitude(partASize, partAFFT.paddedCols, 255);
    ImageType partAPhase(partASize, partAFFT.paddedCols, 255);
    partAFFT.get1DGraph(partAReal, 0, REAL);
    partAFFT.get1DGraph(partAImaginary, 0, IMAGINARY);
    partAFFT.get1DGraph(partAMagnitude, 0, MAGNITUDE);
    partAFFT.get1DGraph(partAPhase, 0, PHASE);
    writeImage("./Experiment1PartAReal.pgm", partAReal);
    writeImage("./Experiment1PartAImaginary.pgm", partAImaginary);
    writeImage("./Experiment1PartAMagnitude.pgm", partAMagnitude);
    writeImage("./Experiment1PartAPhase.pgm", partAPhase);
    cout << "Real Values: [";
    for (int i = 0; i < 4; i++) {
        cout << partAFFT.real[0][i];
        if (i < 3) cout << ", ";
    }
    cout << "]" << endl;
    cout << "Imaginary Values: [";
    for (int i = 0; i < 4; i++) {
        cout << partAFFT.imaginary[0][i];
        if (i < 3) cout << ", ";
    }
    cout << "]" << endl;
    // inverse dft
    partAFFT.fft(true, false);
    cout << "Inverse DFT Values: [";
    for (int i = 0; i < 4; i++) {
        cout << partAFFT.real[0][i];
        if (i < 3) cout << ", ";
    }
    cout << "]" << endl;

    // part b
    // f = cos(2pi * 8x), N = 128
    ImageType partB(1, 128, 255);
    for (int i = 0; i < 128; i++) {
        float deltaX = (float) i * 1/128;
        float sampleVal = cos(2.0 * M_PI * 8.0 * deltaX);
        partB.setPixelVal(0, i, (int) (sampleVal * 127.5)); // normalize
    }
    // compute dft
    FT partBFFT(partB);
    int partBSize = 128;
    // see wave before anything
    ImageType partBCosineWave(partBSize, partBFFT.paddedCols, 255);
    partBFFT.get1DGraph(partBCosineWave, 0, REAL);
    writeImage("Experiment1PartBCosine.pgm", partBCosineWave);
    // run dft
    partBFFT.fft(true, true);
    partBFFT.fftShift(); // shift!
    // plot real, imaginary, magnitude, phase
    ImageType partBReal(partBSize, partBFFT.paddedCols, 255);
    ImageType partBImaginary(partBSize, partBFFT.paddedCols, 255);
    ImageType partBMagnitude(partBSize, partBFFT.paddedCols, 255);
    ImageType partBPhase(partBSize, partBFFT.paddedCols, 255);
    partBFFT.get1DGraph(partBReal, 0, REAL);
    partBFFT.get1DGraph(partBImaginary, 0, IMAGINARY);
    partBFFT.get1DGraph(partBMagnitude, 0, MAGNITUDE);
    partBFFT.get1DGraph(partBPhase, 0, PHASE);
    writeImage("./Experiment1partBReal.pgm", partBReal);
    writeImage("./Experiment1partBImaginary.pgm", partBImaginary);
    writeImage("./Experiment1partBMagnitude.pgm", partBMagnitude);
    writeImage("./Experiment1partBPhase.pgm", partBPhase);

    generateStatistics(partBFFT);

    // After running FFT
   
    // part c
    // f = Rect_128
    ImageType partC(1, 128, 255);
    ifstream file("Rect_128.txt");
    float val = 0;
    int i = 0;
    while (file >> val && i < 128) {
        partC.setPixelVal(0, i, (int) val * 255);
        i++;
    }
    file.close();
    // compute dft
    FT partCFFT(partC);
    int partCSize = 255;
    ImageType partCRectange(partCSize, partCFFT.paddedCols, 255);
    partCFFT.get1DGraph(partCRectange, 0, REAL);
    writeImage("Experiment1PartCRectangle.pgm", partCRectange);
    // run dft
    partCFFT.fft(true, true);
    partCFFT.fftShift(); // shift!
    // plot real, imaginary, magnitude, phase
    generateStatistics(partCFFT);

    ImageType partCReal(partCSize, partCFFT.paddedCols, 255);
    ImageType partCImaginary(partCSize, partCFFT.paddedCols, 255);
    ImageType partCMagnitude(partCSize, partCFFT.paddedCols, 255);
    ImageType partCPhase(partCSize, partCFFT.paddedCols, 255);
    partCFFT.get1DGraph(partCReal, 0, REAL);
    partCFFT.get1DGraph(partCImaginary, 0, IMAGINARY);
    partCFFT.get1DGraph(partCMagnitude, 0, MAGNITUDE);
    partCFFT.get1DGraph(partCPhase, 0, PHASE);
    writeImage("./Experiment1partCReal.pgm", partCReal);
    writeImage("./Experiment1partCImaginary.pgm", partCImaginary);
    writeImage("./Experiment1partCMagnitude.pgm", partCMagnitude);
    writeImage("./Experiment1partCPhase.pgm", partCPhase);
}