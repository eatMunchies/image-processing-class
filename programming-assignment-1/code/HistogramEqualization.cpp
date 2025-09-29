/*
Author: Anthony Silva
Assignment: Programming Assignment 1
Class: Image Processing
Date: 9/25/25
File: HistogramEqualization.cpp
Description:
a program to change the gray level frequencies of a gray pgm image through histogram equalization
the steps of histogram equalization are to:
-
*/

#include "image.h"
#include "histogram.h"
#include <cstring>
#include <iostream>

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

void makeFilePaths(char** boatSource, char** f16Source, char** testSource, char** testEnd, char** boatEnd, char** f16End) {
    // base paths
    const char* basePath = "../photos/base/";
    const char* endPath = "../photos/transformed/equalization/";
    const char* file1 = "boat";
    const char* file2 = "f_16";
    const char* extension = ".pgm";
    char* boatPathTemp = new char[strlen(basePath) + strlen(file1) + strlen(extension) + 1];
    strcpy(boatPathTemp, basePath);
    strcat(boatPathTemp, file1);
    strcat(boatPathTemp, extension);
    char* fPathTemp = new char[strlen(basePath) + strlen(file2) + strlen(extension) + 1];
    strcpy(fPathTemp, basePath);
    strcat(fPathTemp, file2);
    strcat(fPathTemp, extension);
    char* testPath = new char[strlen(basePath) + strlen("test") + strlen(extension) + 1];
    strcpy(testPath, basePath);
    strcat(testPath, "test");
    strcat(testPath, extension);
    char* endBoatPath = new char[strlen(endPath) + strlen(file1) + strlen(extension) + 1];
    strcpy(endBoatPath, endPath);
    strcat(endBoatPath, file1);
    strcat(endBoatPath, extension);
    char* endFPath = new char[strlen(endPath) + strlen(file2) + strlen(extension) + 1];
    strcpy(endFPath, endPath);
    strcat(endFPath, file2);
    strcat(endFPath, extension);
    char* endTestPath = new char[strlen(endPath) + strlen("test") + strlen(extension) + 1];
    strcpy(endTestPath,  endPath);
    strcat(endTestPath, "test");
    strcat(endTestPath, extension);

    // copy out
    *boatSource = boatPathTemp;
    *f16Source = fPathTemp;
    *testSource = testPath;
    *testEnd = endTestPath;
    *boatEnd = endBoatPath;
    *f16End = endFPath;
}

int main(int argc, char *argv[]) {
    // file paths
    char* boatSource;
    char* f16Source;
    char* testSource;
    char* testEnd;
    char* boatEnd;
    char* f16End;
    makeFilePaths(&boatSource, &f16Source, &testSource, &testEnd, &boatEnd, &f16End);

    // make test image
    ImageType test(5, 5, 255);
    
    for (int j = 0; j < 5; j++) {
        test.setPixelVal(0, j, 255);
        test.setPixelVal(1, j, 25);
    }
    test.setPixelVal(3, 2, 255);
    test.setPixelVal(3, 3, 25);
    test.setPixelVal(3, 4, 255);

    writeImage(testSource, test);

    // process histogram
    Histogram testHistogram(test);
    ImageType testFrequencies(256, 256, 255);
    testHistogram.convertToImage(256, 256, testFrequencies);

    // equalize image
    testHistogram.applyCDF(test);
    
    // transformed histogram
    Histogram testTransformedHistogram(test);
    ImageType testTransformedFrequencies(256, 256, 255);
    testTransformedHistogram.convertToImage(256, 256, testTransformedFrequencies);
    
    // write test
    writeImage("../photos/base/testHistogram.pgm", testFrequencies);
    writeImage("../photos/transformed/equalization/testTransformedHistogram.pgm", testTransformedFrequencies);
    writeImage(testEnd, test);

    // boat image
    int N, M, Q;
    bool type;
    readImageHeader(boatSource, N, M, Q, type);
    ImageType boat(N, M, Q);
    readImage(boatSource, boat);

    // histogram
    Histogram boatHistogram(boat);
    ImageType boatHistogramImage(256, 256, Q);
    boatHistogram.convertToImage(256, 256, boatHistogramImage);

    // equalization
    boatHistogram.applyCDF(boat);

    // histogram
    Histogram boatTransformedHistogram(boat);
    ImageType boatTransformedHistogramImage(256, 256, Q);
    boatTransformedHistogram.convertToImage(256, 256, boatTransformedHistogramImage);

    // write
    writeImage("../photos/base/boatHistogram.pgm", boatHistogramImage);
    writeImage("../photos/transformed/equalization/boatTransformedHistogram.pgm", boatTransformedHistogramImage);
    writeImage(boatEnd, boat);

    // f16 
    readImageHeader(f16Source, N, M, Q, type);
    ImageType f16(N, M, Q);
    readImage(f16Source, f16);

    // histogram
    Histogram f16Histogram(f16);
    ImageType f16HistogramImage(256, 256, Q);
    f16Histogram.convertToImage(256, 256, f16HistogramImage);

    // equalization
    f16Histogram.applyCDF(f16);

    // histogram
    Histogram f16TransformedHistogram(f16);
    ImageType f16TransformedHistogramImage(256, 256, Q);
    f16TransformedHistogram.convertToImage(256, 256, f16TransformedHistogramImage);

    // write
    writeImage("../photos/base/f16Histogram.pgm", f16HistogramImage);
    writeImage("../photos/transformed/equalization/f16TransformedHistogram.pgm", f16TransformedHistogramImage);
    writeImage(f16End, f16);

    return 0;
}