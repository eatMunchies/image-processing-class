/*
Author: Anthony Silva
Assignment: Programming Assignment 1
Class: Image Processing
Date: 9/25/25
File: HistogramSpecification.cpp
Description:
a program to transform an input image based on the histogram specification method
*/

#include "histogram.h"
#include "image.h"
#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);


// debug with test image

// make boat have sf frequency
// make f16 have peppers frequency

// the method
// s = original image
// r = equalized original image 
// z = target image
// v = equalized target image
// s = T(r)
// v = G(z)
// z = G^-1(s)
// z = G^-1(T(r))
// basically, assume that equalized images are the same so we can turn image to equalized, then unequalize to the z distribution with inverse G
// generate histogram apply cdf method 

void buildInverseMapping(int**, Histogram*);
void applyInverseMapping(int**, ImageType&);
void inverseMap(ImageType&, Histogram*);

int main(int argc, char *argv[]) {

    // get images
    char* boatSource = "./boat.pgm";
    char* sfSource = "./sf.pgm";
    char* f16Source = "./f_16.pgm";
    char* peppersSource = "./peppers.pgm";
    char* testSource = "./test.pgm";
    
    char* boatEnd = "./specBoat.pgm";
    char* f16End = "./specf16.pgm";
    char* testEnd = "./specTest.pgm";

    // genearte test image
    ImageType test(5, 5, 255);
    
    for (int j = 0; j < 5; j++) {
        test.setPixelVal(0, j, 255);
        test.setPixelVal(1, j, 25);
    }
    test.setPixelVal(3, 2, 255);
    test.setPixelVal(3, 3, 25);
    test.setPixelVal(3, 4, 255);

    writeImage(testSource, test);

    // load images
    int N, M, Q;
    bool type;
    readImageHeader(peppersSource, N, M, Q, type);
    ImageType peppers(N, M, Q);
    readImage(peppersSource, peppers);
    readImageHeader(boatSource, N, M, Q, type);
    ImageType boat(N, M, Q);
    readImage(boatSource, boat);
    readImageHeader(f16Source, N, M, Q, type);
    ImageType f16(N, M, Q);
    readImage(f16Source, f16);
    readImageHeader(sfSource, N, M, Q, type);
    ImageType sf(N, M, Q);
    readImage(sfSource, sf);


    // generate histograms for images
    Histogram testHistogram(test);
    Histogram peppersHistogram(peppers);
    Histogram f16Histogram(f16);
    Histogram boatHistogram(boat);
    Histogram sfHistogram(sf);

    // histogram images
    ImageType testHistogramImage(N, M, Q);
    ImageType peppersHistogramImage(N, M, Q);
    ImageType f16HistogramImage(N, M, Q);
    ImageType boatHistogramImage(N, M, Q);
    ImageType sfHistogramImage(N, M, Q);
    testHistogram.convertToImage(N, M, testHistogramImage);
    peppersHistogram.convertToImage(N, M, peppersHistogramImage);
    f16Histogram.convertToImage(N, M, f16HistogramImage);
    boatHistogram.convertToImage(N, M, boatHistogramImage);
    sfHistogram.convertToImage(N, M, sfHistogramImage);
    writeImage("./testHistogram.pgm", testHistogramImage);
    writeImage("./peppersHistogram.pgm", peppersHistogramImage);
    writeImage("./f16Histogram.pgm", f16HistogramImage);
    writeImage("./boatHistogram.pgm", boatHistogramImage);
    writeImage("./sfHistogram.pgm", sfHistogramImage);

    // s = T(r) is represented by rHistogram.applyCDF(image)
    testHistogram.applyCDF(test);
    boatHistogram.applyCDF(boat);
    f16Histogram.applyCDF(f16);

    // v = G(z) is basically the cdf of peppers, sf images
    // we don't need to build G(z) but we can still infer the inverse in the next step 

    // apply inverse G to s 
    inverseMap(test, &peppersHistogram);
    inverseMap(boat, &sfHistogram);
    inverseMap(f16, &peppersHistogram);

    Histogram testEndHistogram(test);
    Histogram boatEndHistogram(boat);
    Histogram f16EndHistogram(f16);
    testEndHistogram.convertToImage(N, M, testHistogramImage);
    boatEndHistogram.convertToImage(N, M, boatHistogramImage);
    f16EndHistogram.convertToImage(N, M, f16HistogramImage);
    writeImage("./testEndHistogram.pgm", testHistogramImage);
    writeImage("./boatEndHistogram.pgm", boatHistogramImage);
    writeImage("./f16EndHistogram.pgm", f16HistogramImage);
    
    // write image
    writeImage(testEnd, test);
    writeImage(boatEnd, boat);
    writeImage(f16End, f16);

    return 0;
}

void buildInverseMapping(int* inverseMapping, Histogram* targetHistogram) {
    targetHistogram->processCDF(256 * 256); // assuming resolution
    // for each gray value possible...
    for (int s = 0; s < 256; s++) {
        float normalized = s / 255.0f;

        // find z where G(z) is closest to normalized s
        int likelyZ = 0;
        float minDiff = fabs(targetHistogram->getCDF(0) - normalized);

        for (int z = 1; z < 256; z++) {
            float diff = fabs(targetHistogram->getCDF(z) - normalized);
            if (diff < minDiff) {
                minDiff = diff;
                likelyZ = z;
            }
        }
        inverseMapping[s] = likelyZ;
    }
}

void applyInverseMapping(int* inverseMapping, ImageType& outputImage) {
    int N, M, Q;
    outputImage.getImageInfo(N, M, Q);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int s;
            outputImage.getPixelVal(i, j, s);
            int z = inverseMapping[s];
            outputImage.setPixelVal(i, j, z);
        }
    }
}

void inverseMap(ImageType& outputImage, Histogram* targetHistogram) {
    int inverseMapping[256]; // like a reverse cdf
    buildInverseMapping(inverseMapping, targetHistogram);
    applyInverseMapping(inverseMapping, outputImage);
}