/*
Author: Anthony Silva
Assignment: Programming Assignment 4
Class: Image Processing
Date: 12/10/25
File: Experiment4.cpp
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
    readImageHeader("./lenna.pgm", N, M, Q, type);
    ImageType lenna(N, M, Q);
    readImage("./lenna.pgm", lenna);
    FT lennaFT(lenna);
    lennaFT.fft(false, true);
    lennaFT.fftShift();

    // apply motion blur
    FilterParams BlurParams = FilterParams::MotionBlur(0.1, 0.1, 1);
    FT MotionBlurFT(blur, N, M, BlurParams);

    // visualize motion blur
    ImageType MotionBlurMag(MotionBlurFT.paddedRows, MotionBlurFT.paddedCols, 255);
    MotionBlurFT.getSpectrum(MotionBlurMag, MAGNITUDE);
    writeImage("./Experiment4_MotionBlur.pgm", MotionBlurMag);

    FrequencyHelper helper;
    FT blurredLenna(lennaFT.real, lennaFT.imaginary, lennaFT.paddedRows, lennaFT.paddedCols);
    helper.ElementWiseMultiplication(lennaFT, MotionBlurFT, blurredLenna);

    // visualize blurred lenna
    ImageType LennaMotionBlurMag(blurredLenna.paddedRows, blurredLenna.paddedCols, 255);
    blurredLenna.getSpectrum(LennaMotionBlurMag, MAGNITUDE);
    writeImage("./Experiment4_LennaBlur.pgm", LennaMotionBlurMag);

    // add gaussian noise, experimenting with various Gaussian distributions
    FT blurredLenna2(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT blurredLenna3(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    blurredLenna.addGaussianNoise(0, 1);
    blurredLenna2.addGaussianNoise(0, 10);
    blurredLenna3.addGaussianNoise(0, 100);

    // visualize degraded magnitudes
    ImageType degradedLenna1(blurredLenna.paddedRows, blurredLenna.paddedCols, 255);
    ImageType degradedLenna2(blurredLenna.paddedRows, blurredLenna.paddedCols, 255);
    ImageType degradedLenna3(blurredLenna.paddedRows, blurredLenna.paddedCols, 255);
    blurredLenna.getSpectrum(degradedLenna1, MAGNITUDE);
    blurredLenna2.getSpectrum(degradedLenna2, MAGNITUDE);
    blurredLenna3.getSpectrum(degradedLenna3, MAGNITUDE);
    writeImage("./Experiment4_Degraded1.pgm", degradedLenna1);
    writeImage("./Experiment4_Degraded2.pgm", degradedLenna2);
    writeImage("./Experiment4_Degraded3.pgm", degradedLenna3);

    // experimental inverse filtering with various radiuses (full, 40, 70, 85)
    FT inverse11(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT inverse21(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT inverse31(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT inverse41(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT inverse12(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT inverse22(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT inverse32(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT inverse42(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT inverse13(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT inverse23(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT inverse33(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT inverse43(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    
    helper.InverseFilter(blurredLenna, MotionBlurFT, inverse11, 1000000);
    helper.InverseFilter(blurredLenna, MotionBlurFT, inverse21, 40);
    helper.InverseFilter(blurredLenna, MotionBlurFT, inverse31, 70);
    helper.InverseFilter(blurredLenna, MotionBlurFT, inverse41, 85);
    helper.InverseFilter(blurredLenna2, MotionBlurFT, inverse12, 1000000);
    helper.InverseFilter(blurredLenna2, MotionBlurFT, inverse22, 40);
    helper.InverseFilter(blurredLenna2, MotionBlurFT, inverse32, 70);
    helper.InverseFilter(blurredLenna2, MotionBlurFT, inverse42, 85);
    helper.InverseFilter(blurredLenna3, MotionBlurFT, inverse13, 1000000);
    helper.InverseFilter(blurredLenna3, MotionBlurFT, inverse23, 40);
    helper.InverseFilter(blurredLenna3, MotionBlurFT, inverse33, 70);
    helper.InverseFilter(blurredLenna3, MotionBlurFT, inverse43, 85);

    // visualize
    inverse11.fftShift();
    inverse21.fftShift();
    inverse31.fftShift();
    inverse41.fftShift();
    inverse12.fftShift();
    inverse22.fftShift();
    inverse32.fftShift();
    inverse42.fftShift();
    inverse13.fftShift();
    inverse23.fftShift();
    inverse33.fftShift();
    inverse43.fftShift();

    inverse11.fft(false, false);
    inverse21.fft(false, false);
    inverse31.fft(false, false);
    inverse41.fft(false, false);
    inverse12.fft(false, false);
    inverse22.fft(false, false);
    inverse32.fft(false, false);
    inverse42.fft(false, false);
    inverse13.fft(false, false);
    inverse23.fft(false, false);
    inverse33.fft(false, false);
    inverse43.fft(false, false);

    ImageType inverse11Image(inverse11.paddedRows, inverse11.paddedCols, 255);
    ImageType inverse21Image(inverse11.paddedRows, inverse11.paddedCols, 255);
    ImageType inverse31Image(inverse11.paddedRows, inverse11.paddedCols, 255);
    ImageType inverse41Image(inverse11.paddedRows, inverse11.paddedCols, 255);
    ImageType inverse12Image(inverse11.paddedRows, inverse11.paddedCols, 255);
    ImageType inverse22Image(inverse11.paddedRows, inverse11.paddedCols, 255);
    ImageType inverse32Image(inverse11.paddedRows, inverse11.paddedCols, 255);
    ImageType inverse42Image(inverse11.paddedRows, inverse11.paddedCols, 255);
    ImageType inverse13Image(inverse11.paddedRows, inverse11.paddedCols, 255);
    ImageType inverse23Image(inverse11.paddedRows, inverse11.paddedCols, 255);
    ImageType inverse33Image(inverse11.paddedRows, inverse11.paddedCols, 255);
    ImageType inverse43Image(inverse11.paddedRows, inverse11.paddedCols, 255);

    inverse11.getSpectrum(inverse11Image, MAGNITUDE);
    inverse21.getSpectrum(inverse21Image, MAGNITUDE);
    inverse31.getSpectrum(inverse31Image, MAGNITUDE);
    inverse41.getSpectrum(inverse41Image, MAGNITUDE);
    inverse12.getSpectrum(inverse12Image, MAGNITUDE);
    inverse22.getSpectrum(inverse22Image, MAGNITUDE);
    inverse32.getSpectrum(inverse32Image, MAGNITUDE);
    inverse42.getSpectrum(inverse42Image, MAGNITUDE);
    inverse13.getSpectrum(inverse13Image, MAGNITUDE);
    inverse23.getSpectrum(inverse23Image, MAGNITUDE);
    inverse33.getSpectrum(inverse33Image, MAGNITUDE);
    inverse43.getSpectrum(inverse43Image, MAGNITUDE);

    writeImage("./Experiment4_Inverse11.pgm", inverse11Image);
    writeImage("./Experiment4_Inverse21.pgm", inverse21Image);
    writeImage("./Experiment4_Inverse31.pgm", inverse31Image);
    writeImage("./Experiment4_Inverse41.pgm", inverse41Image);
    writeImage("./Experiment4_Inverse12.pgm", inverse12Image);
    writeImage("./Experiment4_Inverse22.pgm", inverse22Image);
    writeImage("./Experiment4_Inverse32.pgm", inverse32Image);
    writeImage("./Experiment4_Inverse42.pgm", inverse42Image);
    writeImage("./Experiment4_Inverse13.pgm", inverse13Image);
    writeImage("./Experiment4_Inverse23.pgm", inverse23Image);
    writeImage("./Experiment4_Inverse33.pgm", inverse33Image);
    writeImage("./Experiment4_Inverse43.pgm", inverse43Image);

    // experimental wiener filtering with different K values
    FT weiner11(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT weiner21(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT weiner31(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT weiner41(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT weiner12(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT weiner22(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT weiner32(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT weiner42(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT weiner13(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT weiner23(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT weiner33(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    FT weiner43(blurredLenna.real, blurredLenna.imaginary, blurredLenna.paddedRows, blurredLenna.paddedCols);
    
    helper.WienerFilter(blurredLenna, MotionBlurFT, weiner11, 0.0001);
    helper.WienerFilter(blurredLenna, MotionBlurFT, weiner21, 0.001);
    helper.WienerFilter(blurredLenna, MotionBlurFT, weiner31, 0.01);
    helper.WienerFilter(blurredLenna, MotionBlurFT, weiner41, 0.1);
    helper.WienerFilter(blurredLenna2, MotionBlurFT, weiner12, 0.0001);
    helper.WienerFilter(blurredLenna2, MotionBlurFT, weiner22, 0.001);
    helper.WienerFilter(blurredLenna2, MotionBlurFT, weiner32, 0.01);
    helper.WienerFilter(blurredLenna2, MotionBlurFT, weiner42, 0.1);
    helper.WienerFilter(blurredLenna3, MotionBlurFT, weiner13, 0.0001);
    helper.WienerFilter(blurredLenna3, MotionBlurFT, weiner23, 0.001);
    helper.WienerFilter(blurredLenna3, MotionBlurFT, weiner33, 0.01);
    helper.WienerFilter(blurredLenna3, MotionBlurFT, weiner43, 0.1);

    weiner11.fftShift();
    weiner21.fftShift();
    weiner31.fftShift();
    weiner41.fftShift();
    weiner12.fftShift();
    weiner22.fftShift();
    weiner32.fftShift();
    weiner42.fftShift();
    weiner13.fftShift();
    weiner23.fftShift();
    weiner33.fftShift();
    weiner43.fftShift();

    weiner11.fft(false, false);
    weiner21.fft(false, false);
    weiner31.fft(false, false);
    weiner41.fft(false, false);
    weiner12.fft(false, false);
    weiner22.fft(false, false);
    weiner32.fft(false, false);
    weiner42.fft(false, false);
    weiner13.fft(false, false);
    weiner23.fft(false, false);
    weiner33.fft(false, false);
    weiner43.fft(false, false);

    ImageType weiner11Image(weiner11.paddedRows, weiner11.paddedCols, 255);
    ImageType weiner21Image(weiner11.paddedRows, weiner11.paddedCols, 255);
    ImageType weiner31Image(weiner11.paddedRows, weiner11.paddedCols, 255);
    ImageType weiner41Image(weiner11.paddedRows, weiner11.paddedCols, 255);
    ImageType weiner12Image(weiner11.paddedRows, weiner11.paddedCols, 255);
    ImageType weiner22Image(weiner11.paddedRows, weiner11.paddedCols, 255);
    ImageType weiner32Image(weiner11.paddedRows, weiner11.paddedCols, 255);
    ImageType weiner42Image(weiner11.paddedRows, weiner11.paddedCols, 255);
    ImageType weiner13Image(weiner11.paddedRows, weiner11.paddedCols, 255);
    ImageType weiner23Image(weiner11.paddedRows, weiner11.paddedCols, 255);
    ImageType weiner33Image(weiner11.paddedRows, weiner11.paddedCols, 255);
    ImageType weiner43Image(weiner11.paddedRows, weiner11.paddedCols, 255);

    weiner11.getSpectrum(weiner11Image, MAGNITUDE);
    weiner21.getSpectrum(weiner21Image, MAGNITUDE);
    weiner31.getSpectrum(weiner31Image, MAGNITUDE);
    weiner41.getSpectrum(weiner41Image, MAGNITUDE);
    weiner12.getSpectrum(weiner12Image, MAGNITUDE);
    weiner22.getSpectrum(weiner22Image, MAGNITUDE);
    weiner32.getSpectrum(weiner32Image, MAGNITUDE);
    weiner42.getSpectrum(weiner42Image, MAGNITUDE);
    weiner13.getSpectrum(weiner13Image, MAGNITUDE);
    weiner23.getSpectrum(weiner23Image, MAGNITUDE);
    weiner33.getSpectrum(weiner33Image, MAGNITUDE);
    weiner43.getSpectrum(weiner43Image, MAGNITUDE);

    writeImage("./Experiment4_Weiner11.pgm", weiner11Image);
    writeImage("./Experiment4_Weiner21.pgm", weiner21Image);
    writeImage("./Experiment4_Weiner31.pgm", weiner31Image);
    writeImage("./Experiment4_Weiner41.pgm", weiner41Image);
    writeImage("./Experiment4_Weiner12.pgm", weiner12Image);
    writeImage("./Experiment4_Weiner22.pgm", weiner22Image);
    writeImage("./Experiment4_Weiner32.pgm", weiner32Image);
    writeImage("./Experiment4_Weiner42.pgm", weiner42Image);
    writeImage("./Experiment4_Weiner13.pgm", weiner13Image);
    writeImage("./Experiment4_Weiner23.pgm", weiner23Image);
    writeImage("./Experiment4_Weiner33.pgm", weiner33Image);
    writeImage("./Experiment4_Weiner43.pgm", weiner43Image);

    // visualize degraded images
    blurredLenna.fftShift();
    blurredLenna.fft(false, false);
    blurredLenna2.fftShift();
    blurredLenna2.fft(false, false);
    blurredLenna3.fftShift();
    blurredLenna3.fft(false, false);
    ImageType blurredLennaMag(blurredLenna.paddedRows, blurredLenna.paddedCols, 255);
    ImageType blurredLennaMag2(blurredLenna.paddedRows, blurredLenna.paddedCols, 255);
    ImageType blurredLennaMag3(blurredLenna.paddedRows, blurredLenna.paddedCols, 255);
    blurredLenna.getSpectrum(blurredLennaMag, MAGNITUDE);
    blurredLenna2.getSpectrum(blurredLennaMag2, MAGNITUDE);
    blurredLenna3.getSpectrum(blurredLennaMag3, MAGNITUDE);
    writeImage("./Experiment4_BlurredLenna1.pgm", blurredLennaMag);
    writeImage("./Experiment4_BlurredLenna2.pgm", blurredLennaMag2);
    writeImage("./Experiment4_BlurredLenna3.pgm", blurredLennaMag3);
    
}