/*
Author: Anthony Silva
Assignment: Programming Assignment 2
Class: Image Processing
Date: 10/20/25
File: ExtraCredit.cpp
Description:
Implement unsharp masking and high-boost fitlering
*/

#include "image.h"
#include "mask.h"
#include <cstring>
#include <iostream>

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

void subtractImages(ImageType& input1, ImageType& input2, ImageType& output) {
    // assuming everything is the same dimensions
    int N, M, Q;
    input1.getImageInfo(N, M, Q);
    int tempVal1;
    int tempVal2;
    int difference;
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < M; col++) {
            input1.getPixelVal(row, col, tempVal1);
            input2.getPixelVal(row, col, tempVal2);
            difference = tempVal1 - tempVal2;
            if (difference < 0) difference = -1 * difference;
            output.setPixelVal(row, col, difference);
        }
    }
}

void highBoostFilter(ImageType& input, ImageType& mask, ImageType& output, float scale) {
    int N, M, Q;
    input.getImageInfo(N, M, Q);
    int inputPixel;
    int maskPixel;
    float scaled;
    for (int row = 0; row < N; row++) {

        for (int col = 0; col < M; col++) {
            input.getPixelVal(row, col, inputPixel);
            mask.getPixelVal(row, col, maskPixel);
            scaled = (float) inputPixel + (float) maskPixel * scale;
            if (scaled < 0.0) scaled = 0;
            if (scaled >= (float) Q) scaled = (float) Q - 1;
            output.setPixelVal(row, col, (int) scaled);
        }
    }
}

int main(int argc, char* argv[]) {
    // init
    char* lennaPath = "./lenna.pgm";
    char* f16Path = "./f_16.pgm";
    CorrelationHelper correlator;

    int N, M, Q;
    bool type;
    readImageHeader(lennaPath, N, M, Q, type);
    ImageType lenna(N, M, Q);
    readImage(lennaPath, lenna);

    // get mask
    Mask mask("./gaussian7.txt");

    // get smoothed image
    ImageType lennaSmooth(N, M, Q);
    mask.applyMask(lenna, lennaSmooth, correlator);

    // get difference
    ImageType lennaMask(N, M, Q);
    subtractImages(lenna, lennaSmooth, lennaMask);

    // high boost filter
    // k = 1
    ImageType lennaUnsharp(N, M, Q);
    highBoostFilter(lenna, lennaMask, lennaUnsharp, 1.0);
    // k = 1.5
    ImageType lenna1d5(N, M, Q);
    highBoostFilter(lenna, lennaMask, lenna1d5, 1.5);
    // k = 2
    ImageType lenna2(N, M, Q);
    highBoostFilter(lenna, lennaMask, lenna2, 2.0);
    // k = 3
    ImageType lenna3(N, M, Q);
    highBoostFilter(lenna, lennaMask, lenna3, 3.0);
    // k = 5
    ImageType lenna5(N, M, Q);
    highBoostFilter(lenna, lennaMask, lenna5, 5.0);

    // write image
    char* lennaMaskPath = "./sharp_lenna_mask.pgm";
    char* lenna1Path = "./sharp_lenna_1.pgm";
    char* lenna1d5Path = "./sharp_lenna_1_5.pgm";
    char* lenna2Path = "./sharp_lenna_2.pgm";
    char* lenna3Path = "./sharp_lenna_3.pgm";
    char* lenna5Path = "./sharp_lenna_5.pgm";
    writeImage(lennaMaskPath, lennaMask);
    writeImage(lenna1Path, lennaUnsharp);
    writeImage(lenna1d5Path, lenna1d5);
    writeImage(lenna2Path, lenna2);
    writeImage(lenna3Path, lenna3);
    writeImage(lenna5Path, lenna5);

    // sf
    char* sfPath = "./sf.pgm";
    readImageHeader(sfPath, N, M, Q, type);
    ImageType sf(N, M, Q);
    readImage(sfPath, sf);

    ImageType sfSmooth(N, M, Q);
    mask.applyMask(sf, sfSmooth, correlator);

    ImageType sfMask(N, M, Q);
    subtractImages(sf, sfSmooth, sfMask);

    // high boost filter
    // k = 1
    ImageType sfUnsharp(N, M, Q);
    highBoostFilter(sf, sfMask, sfUnsharp, 1.0);
    // k = 1.5
    ImageType sf1d5(N, M, Q);
    highBoostFilter(sf, sfMask, sf1d5, 1.5);
    // k = 2
    ImageType sf2(N, M, Q);
    highBoostFilter(sf, sfMask, sf2, 2.0);
    // k = 3
    ImageType sf3(N, M, Q);
    highBoostFilter(sf, sfMask, sf3, 3.0);
    // k = 5
    ImageType sf5(N, M, Q);
    highBoostFilter(sf, sfMask, sf5, 5.0);

    // write images
    char* sfMaskPath = "./sharp_sf_mask.pgm";
    char* sf1Path = "./sharp_sf_1.pgm";
    char* sf1d5Path = "./sharp_sf_1_5.pgm";
    char* sf2Path = "./sharp_sf_2.pgm";
    char* sf3Path = "./sharp_sf_3.pgm";
    char* sf5Path = "./sharp_sf_5.pgm";
    writeImage(sfMaskPath, sfMask);
    writeImage(sf1Path, sfUnsharp);
    writeImage(sf1d5Path, sf1d5);
    writeImage(sf2Path, sf2);
    writeImage(sf3Path, sf3);
    writeImage(sf5Path, sf5);
}