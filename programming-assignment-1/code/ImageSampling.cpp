/* 
Author: Anthony Silva
Assignment: Programming Assignment 1
Class: Image Processing
Date: 9/17/25
File: ImageSampling.cpp
Description:
a program to change the special resolution of an image from 
256x256 to 128x128, 64x64, 32x32 using image sampling by factors of 2, 4, and 8
resize the images back to the original size 256 x 256 to see the differences
using images lemma.png and peppers.png
*/

#include "image.h"
#include <cstring>
#include <iostream>

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

void sampleImage(ImageType& image, const int factor, ImageType& sampled) {

    // image is N x M with pixel depth of Q at each
    // a factor n will reduce the dimensions by n. 
    // N* = N / n; 
    // M* = M / n;
    // 16x16 image reduce by 2 -> 8x8

    int N, M, Q;
    image.getImageInfo(N, M, Q);
    cout << "got image info loaded" << endl;

    // allocate image info (p for prime)
    int Np, Mp, Qp;
    Np = (N + factor - 1) / factor; // it should round up i think if not even...
    Mp = (M + factor - 1) / factor;
    Qp = Q; // same bit depth

    sampled.setImageInfo(Np, Mp, Qp);
    cout << "set sampled image" << endl;

    // now time to sample!
    int row = 0;
    int val;
    cout << "starting loop" << endl;
    for (int i = 0; i < N; i += factor) { 
        int col = 0;
        for (int j = 0; j < M; j += factor) {
            cout << "i: " << i << endl;
            cout << "j: " << j << endl;
            cout << "row: " << row << endl;
            cout << "col: " << col << endl;
            // basically, we loop over and get each top left spot of the source image and assign it to the next image of the next one
            image.getPixelVal(i, j, val);
            cout << "got val: " << val << endl;
            sampled.setPixelVal(row, col, val);
            col++;
        }
        row++;
    }
    cout << "sampled image" << endl;
}

void resampleImage(ImageType& image, const int factor, ImageType& resampled) {
    // get image metrics
    int N, M, Q;
    image.getImageInfo(N, M, Q);
    cout << "got image info" << endl;

    // allocate new image
    int Np = N * factor;
    int Mp = M * factor;
    int Qp = Q;
    
    resampled.setImageInfo(Np, Mp, Qp);
    cout << "set new image size" << endl;
    
    int ip;
    int jp;
    int val;
    // resample
    for (int i = 0; i < Np; i++) {
        for (int j = 0; j < Mp; j++) {
            ip = i / factor;
            jp = j / factor;
            image.getPixelVal(ip, jp, val);
            resampled.setPixelVal(i, j, val);
        }
    }
    cout << "resampled" << endl;
}

int main(int argc, char *argv[]) 
{
    // base paths
    const char* basePath = "../photos/base/";
    const char* endPath = "../photos/transformed/sampling/";
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
    char* lennaEndPath4 = new char[strlen(endPath) + strlen(file1) + strlen("4") + strlen(extension) + 1];
    char* lennaEndPath8 = new char[strlen(endPath) + strlen(file1) + strlen("8") + strlen(extension) + 1];
    strcpy(lennaEndPath2, endPath);
    strcat(lennaEndPath2, file1);
    strcat(lennaEndPath2, "2");
    strcat(lennaEndPath2, extension);
    strcpy(lennaEndPath4, endPath);
    strcat(lennaEndPath4, file1);
    strcat(lennaEndPath4, "4");
    strcat(lennaEndPath4, extension);
    strcpy(lennaEndPath8, endPath);
    strcat(lennaEndPath8, file1);
    strcat(lennaEndPath8, "8");
    strcat(lennaEndPath8, extension);

    char* peppersEndPath2 = new char[strlen(endPath) + strlen(file2) + strlen("2") + strlen(extension) + 1];
    char* peppersEndPath4 = new char[strlen(endPath) + strlen(file2) + strlen("4") + strlen(extension) + 1];
    char* peppersEndPath8 = new char[strlen(endPath) + strlen(file2) + strlen("8") + strlen(extension) + 1];
    strcpy(peppersEndPath2, endPath);
    strcat(peppersEndPath2, file2);
    strcat(peppersEndPath2, "2");
    strcat(peppersEndPath2, extension);
    strcpy(peppersEndPath4, endPath);
    strcat(peppersEndPath4, file2);
    strcat(peppersEndPath4, "4");
    strcat(peppersEndPath4, extension);
    strcpy(peppersEndPath8, endPath);
    strcat(peppersEndPath8, file2);
    strcat(peppersEndPath8, "8");
    strcat(peppersEndPath8, extension);

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

    // now we sample...
    ImageType sampled2l(128, 128, Q);
    sampleImage(lenna, 2, sampled2l);
    cout << "sampled by factor of 2" << endl;
    ImageType sampled4l(64, 64, Q);
    sampleImage(lenna, 4, sampled4l);
    cout << "sampled by factor of 4" << endl;
    ImageType sampled8l(32, 32, Q);
    sampleImage(lenna, 8, sampled8l);
    cout << "sampled by factor of 8" << endl;

    // now we resample...
    ImageType resampled2l(N, M, Q);
    resampleImage(sampled2l, 2, resampled2l);
    cout << "resampled by factor of 2" << endl;
    ImageType resampled4l(N, M, Q);
    resampleImage(sampled4l, 4, resampled4l);
    cout << "resampled by factor of 4" << endl;
    ImageType resampled8l(N, M, Q);
    resampleImage(sampled8l, 8, resampled8l);
    cout << "resampled by factor of 8" << endl;

    // write image
    writeImage(lennaEndPath2, resampled2l);
    cout << "wrote first image" << endl;
    writeImage(lennaEndPath4, resampled4l);
    cout << "wrote second image" << endl;
    writeImage(lennaEndPath8, resampled8l);
    cout << "wrote third image" << endl;

    cout << "Reading second file..." << endl;

    // read second file
    readImageHeader(peppersPath, N, M, Q, type);
    cout << "read image header" << endl;

    // allocate image
    ImageType peppers(N, M, Q);
    cout << "allocated peppers image space" << endl;

    // read image
    readImage(peppersPath, peppers);
    cout << "read peppers image" << endl;

    // now we sample...
    ImageType sampled2p(128, 128, Q);
    sampleImage(peppers, 2, sampled2p);
    cout << "sample peppers by 2" << endl;
    ImageType sampled4p(64, 64, Q);
    sampleImage(peppers, 4, sampled4p);
    cout << "sample peppers by 4" << endl;
    ImageType sampled8p(32, 32, Q);
    sampleImage(peppers, 8, sampled8p);
    cout << "sample peppers by 8" << endl;

    // now we resample...
    ImageType resampled2p(N, M, Q);
    resampleImage(sampled2p, 2, resampled2p);
    cout << "resampled peppers by 2" << endl;
    ImageType resampled4p(N, M, Q);
    resampleImage(sampled4p, 4, resampled4p);
    cout << "resampled peppers by 4" << endl;
    ImageType resampled8p(N, M, Q);
    resampleImage(sampled8p, 8, resampled8p);
    cout << "resampled peppers by 8" << endl;

    // write image
    writeImage(peppersEndPath2, resampled2p);
    cout << "wrote first image" << endl;
    writeImage(peppersEndPath4, resampled4p);
    cout << "wrote second image" << endl;
    writeImage(peppersEndPath8, resampled8p);
    cout << "wrote third image" << endl;

    return 0;
}