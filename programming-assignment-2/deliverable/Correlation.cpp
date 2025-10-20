/*
Author: Anthony Silva
Assignment: Programming Assignment 2
Class: Image Processing
Date: 10/20/25
File: Correlation.cpp
Description:
This program produces a correlation image of a source image and a given mask
Use an input file to describe the mask
*/

#include "image.h"
#include "correlationHelper.h"
#include <cstring>
#include <iostream>

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

int main(int argc, char *argv[]) 
{
    // arg check
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <mask_filepath> [image_filepath]" << endl;
        return 1;
    }

    // filepaths
    char* imageFilePath = (argc > 2) ? const_cast<char*>(argv[2]) : const_cast<char*>("./Image.pgm");
    char* maskFilePath = (argc > 1) ? const_cast<char*>(argv[1]) : const_cast<char*>("./Pattern.pgm");

    // create correlation object
    CorrelationHelper correlator;

    // read images
    int N, M, Q; 
    bool type;
    readImageHeader(imageFilePath, N, M, Q, type);
    int Nm, Mm, Qm;
    bool typem;
    readImageHeader(maskFilePath, Nm, Mm, Q, typem);

    cout << "Read Image Headers" << endl;

    ImageType image(N, M, Q);
    readImage(imageFilePath, image);
    ImageType mask(Nm, Mm, Qm);
    readImage(maskFilePath, mask);
    ImageType output(N, M, Q);
    float** rawOutput = new float*[N];
    for (int i = 0; i < N; i++) {
        rawOutput[i] = new float[M];
    }

    cout << "Initialized image objects" << endl;

    // apply correlation
    correlator.correlateImage(rawOutput, image, mask);

    cout << "Correlated image" << endl;

    // scale output
    correlator.scaleImage(rawOutput, output);
    for(int i = 0; i < N; i++) delete[] rawOutput[i];
    delete[] rawOutput;

    // save results
    char* outputFilePath = "./Correlation.pgm";
    writeImage(outputFilePath, output);
    cout << "Wrote image" << endl;
    cout << "Success!" << endl;
}