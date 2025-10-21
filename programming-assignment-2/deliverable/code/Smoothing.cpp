/*
Author: Anthony Silva
Assignment: Programming Assignment 2
Class: Image Processing
Date: 10/20/25
File: Smoothing.cpp
Description:
This program uses input masks to perform smoothing. 7x7 and 15x15 averaging and gaussian too
*/

#include "image.h"
#include "mask.h"
#include "correlationHelper.h"
#include <cstring>
#include <iostream>

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

int main(int argc, char *argv[])
{
    // paths
    char* lennaPath = "./lenna.pgm";
    char* sfPath = "./sf.pgm";

    // correlator helper
    CorrelationHelper correlator;

    // lenna first!
    int N, M, Q;
    bool type;
    readImageHeader(lennaPath, N, M, Q, type);
    ImageType lenna(N, M, Q);
    readImage(lennaPath, lenna);

    // get masks
    Mask average7("average", 7);
    Mask average15("average", 15);
    Mask gaussian7("gaussian", 7);
    Mask gaussian15("gaussian", 15);

    // setup outputs
    ImageType lennaA7(N, M, Q);
    ImageType lennaA15(N, M, Q);
    ImageType lennaG7(N, M, Q);
    ImageType lennaG15(N, M, Q);

    average7.applyMask(lenna, lennaA7, correlator);
    average15.applyMask(lenna, lennaA15, correlator);
    gaussian7.applyMask(lenna, lennaG7, correlator);
    gaussian15.applyMask(lenna, lennaG15, correlator);

    // save files
    char* lennaA7Path = "./average7_lenna.pgm";
    char* lennaA15Path = "./average15_lenna.pgm";
    char* lennaG7Path = "./gaussian7_lenna.pgm";
    char* lennaG15Path = "./gaussian15_lenna.pgm";

    writeImage(lennaA7Path, lennaA7);
    writeImage(lennaA15Path, lennaA15);
    writeImage(lennaG7Path, lennaG7);
    writeImage(lennaG15Path, lennaG15);

    // now sf
    readImageHeader(sfPath, N, M, Q, type);
    ImageType sf(N, M, Q);
    readImage(sfPath, sf);

    // setup outputs
    ImageType sfA7(N, M, Q);
    ImageType sfA15(N, M, Q);
    ImageType sfG7(N, M, Q);
    ImageType sfG15(N, M, Q);

    average7.applyMask(sf, sfA7, correlator);
    average15.applyMask(sf, sfA15, correlator);
    gaussian7.applyMask(sf, sfG7, correlator);
    gaussian15.applyMask(sf, sfG15, correlator);

    // save files
    char* sfA7Path = "./average7_sf.pgm";
    char* sfA15Path = "./average15_sf.pgm";
    char* sfG7Path = "./gaussian7_sf.pgm";
    char* sfG15Path = "./gaussian15_sf.pgm";

    writeImage(sfA7Path, sfA7);
    writeImage(sfA15Path, sfA15);
    writeImage(sfG7Path, sfG7);
    writeImage(sfG15Path, sfG15);
}