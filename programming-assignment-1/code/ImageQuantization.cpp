/* 
Author: Anthony Silva
Assignment: Programming Assignment 1
Class: Image Processing
Date: 9/25/25
File: ImageQuantization.cpp
Description:
a program to change the bit space of an image from 
256 to 128, 32, 8, 2 using image quantization by factors of 2, 8, 32, 128
expand bitspace back to the original size 256 to see the differences
using images lemma.png and peppers.png
*/

#include "image.h"
#include <cstring>
#include <iostream>

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

void quantizeImage(ImageType& image, const int factor, ImageType& quantized) {
    int N, M, Q;
    image.getImageInfo(N, M, Q);

    // int Np, Mp, Qp;
    // Np = N;
    // Mp = M;
    // Qp = (Q + factor - 1) / factor;

    // quantized.setImageInfo(Np, Mp, Qp);

    int val;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.getPixelVal(i, j, val);
            quantized.setPixelVal(i, j, (val + factor - 1) / factor);
        }
    }
}

void resampleQuantizedImage(ImageType& quantized, const int factor, ImageType& refactored) {
    // get image metrics
    int N, M, Q;
    quantized.getImageInfo(N, M, Q);
    cout << "got image info" << endl;

    // allocate new image
    // int Np = N;
    // int Mp = M;
    // int Qp = Q * factor;
    
    // refactored.setImageInfo(Np, Mp, Qp);
    // cout << "set new image size" << endl;
    
    int ip;
    int jp;
    int val;
    // resample
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            quantized.getPixelVal(i, j, val);
            refactored.setPixelVal(i, j, val * factor);
        }
    }
    cout << "resampled" << endl;
}

int main(int argc, char *argv[]) {
    // base paths
    const char* basePath = "../photos/base/";
    const char* endPath = "../photos/transformed/quantized/";
    const char* file1 = "lenna";
    const char* file2 = "peppers";
    const char* extension = ".pgm";

    // filenames
    char* lennaPath = new char[strlen(basePath) + strlen(file1) + strlen(extension) + 1];
    strcpy(lennaPath, basePath);
    strcat(lennaPath, file1);
    strcat(lennaPath, extension);
    char* peppersPath = new char[strlen(basePath) + strlen(file2) + strlen(extension) + 1];
    strcpy(peppersPath, basePath);
    strcat(peppersPath, file2);
    strcat(peppersPath, extension);
    // output filenames
    char* lennaEndPath2 = new char[strlen(endPath) + strlen(file1) + strlen("2") + strlen(extension) + 1];
    char* lennaEndPath8 = new char[strlen(endPath) + strlen(file1) + strlen("8") + strlen(extension) + 1];
    char* lennaEndPath32 = new char[strlen(endPath) + strlen(file1) + strlen("32") + strlen(extension) + 1];
    char* lennaEndPath128 = new char[strlen(endPath) + strlen(file1) + strlen("128") + strlen(extension) + 1];
    strcpy(lennaEndPath2, endPath);
    strcat(lennaEndPath2, file1);
    strcat(lennaEndPath2, "2");
    strcat(lennaEndPath2, extension);
    strcpy(lennaEndPath8, endPath);
    strcat(lennaEndPath8, file1);
    strcat(lennaEndPath8, "8");
    strcat(lennaEndPath8, extension);
    strcpy(lennaEndPath32, endPath);
    strcat(lennaEndPath32, file1);
    strcat(lennaEndPath32, "32");
    strcat(lennaEndPath32, extension);
    strcpy(lennaEndPath128, endPath);
    strcat(lennaEndPath128, file1);
    strcat(lennaEndPath128, "128");
    strcat(lennaEndPath128, extension);

    char* peppersEndPath2 = new char[strlen(endPath) + strlen(file2) + strlen("2") + strlen(extension) + 1];
    char* peppersEndPath8 = new char[strlen(endPath) + strlen(file2) + strlen("8") + strlen(extension) + 1];
    char* peppersEndPath32 = new char[strlen(endPath) + strlen(file2) + strlen("32") + strlen(extension) + 1];
    char* peppersEndPath128 = new char[strlen(endPath) + strlen(file2) + strlen("128") + strlen(extension) + 1];
    strcpy(peppersEndPath2, endPath);
    strcat(peppersEndPath2, file2);
    strcat(peppersEndPath2, "2");
    strcat(peppersEndPath2, extension);
    strcpy(peppersEndPath8, endPath);
    strcat(peppersEndPath8, file2);
    strcat(peppersEndPath8, "8");
    strcat(peppersEndPath8, extension);
    strcpy(peppersEndPath32, endPath);
    strcat(peppersEndPath32, file2);
    strcat(peppersEndPath32, "32");
    strcat(peppersEndPath32, extension);
    strcpy(peppersEndPath128, endPath);
    strcat(peppersEndPath128, file2);
    strcat(peppersEndPath128, "128");
    strcat(peppersEndPath128, extension);

    // logging
    cout << "Reading file 1: " << lennaPath << endl;

    // read first file
    int i, j;
    int M, N, Q;
    bool type;
    
    // read image header
    readImageHeader(lennaPath, N, M, Q, type);

    cout << "Read image header" << endl;

    // allocate image 
    ImageType lenna(N, M, Q);

    cout << "Allocated image space" << endl;

    // read image
    readImage(lennaPath, lenna);

    cout << "Read lenna image" << endl;

    // quantize
    ImageType quantized2l(N, M, 128);
    quantizeImage(lenna, 2, quantized2l);
    cout << "sample lenna by 2" << endl;
    ImageType quantized8l(N, M, 32);
    quantizeImage(lenna, 8, quantized8l);
    cout << "sample lenna by 8" << endl;
    ImageType quantized32l(N, M, 8);
    quantizeImage(lenna, 32, quantized32l);
    cout << "sample lenna by 32" << endl;
    ImageType quantized128l(N, M, 4);
    quantizeImage(lenna, 128, quantized128l);
    cout << "sample lenna by 128" << endl;

    // resample
    ImageType resampled2l(N, M, Q);
    resampleQuantizedImage(quantized2l, 2, resampled2l);
    cout << "resampled lenna by 2" << endl;
    ImageType resampled8l(N, M, Q);
    resampleQuantizedImage(quantized8l, 8, resampled8l);
    cout << "resampled lenna by 8" << endl;
    ImageType resampled32l(N, M, Q);
    resampleQuantizedImage(quantized32l, 32, resampled32l);
    cout << "resampled lenna by 32" << endl;
    ImageType resampled128l(N, M, Q);
    resampleQuantizedImage(quantized128l, 128, resampled128l);
    cout << "resampled lenna by 128" << endl;


    // write
    writeImage(lennaEndPath2, resampled2l);
    cout << "wrote first image" << endl;
    writeImage(lennaEndPath8, resampled8l);
    cout << "wrote second image" << endl;
    writeImage(lennaEndPath32, resampled32l);
    cout << "wrote third image" << endl;
    writeImage(lennaEndPath128, resampled128l);
    cout << "wrote fourth image" << endl;

    cout << "Reading second file..." << endl;

    readImageHeader(peppersPath, N, M, Q, type);
    cout << "read image header" << endl;

    // allocate image
    ImageType peppers(N, M, Q);
    cout << "allocated peppers image space" << endl;

    // read image
    readImage(peppersPath, peppers);
    cout << "read peppers image" << endl;

    // quantize
    ImageType quantized2p(N, M, 128);
    quantizeImage(peppers, 2, quantized2p);
    cout << "sample peppers by 2" << endl;
    ImageType quantized8p(N, M, 32);
    quantizeImage(peppers, 8, quantized8p);
    cout << "sample peppers by 8" << endl;
    ImageType quantized32p(N, M, 8);
    quantizeImage(peppers, 32, quantized32p);
    cout << "sample peppers by 8" << endl;
    ImageType quantized128p(N, M, 2);
    quantizeImage(peppers, 128, quantized128p);
    cout << "sample peppers by 128" << endl;

    // resample
    ImageType resampled2p(N, M, Q);
    resampleQuantizedImage(quantized2p, 2, resampled2p);
    cout << "resampled peppers by 2" << endl;
    ImageType resampled8p(N, M, Q);
    resampleQuantizedImage(quantized8p, 8, resampled8p);
    cout << "resampled peppers by 8" << endl;
    ImageType resampled32p(N, M, Q);
    resampleQuantizedImage(quantized32p, 32, resampled32p);
    cout << "resampled peppers by 32" << endl;
    ImageType resampled128p(N, M, Q);
    resampleQuantizedImage(quantized128p, 128, resampled128p);
    cout << "resampled peppers by 128" << endl;

    // write
    writeImage(peppersEndPath2, resampled2p);
    cout << "wrote first image" << endl;
    writeImage(peppersEndPath8, resampled8p);
    cout << "wrote second image" << endl;
    writeImage(peppersEndPath32, resampled32p);
    cout << "wrote third image" << endl;
    writeImage(peppersEndPath128, resampled128p);
    cout << "wrote fourth image" << endl;

    return 0;
}
