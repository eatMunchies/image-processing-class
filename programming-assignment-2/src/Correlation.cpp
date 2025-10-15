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
#include <cstring>
#include <iostream>

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

void readMaskDetails(char filepath[], int& N, int& M) {
    // basically see the size of the mask for use before we read it
}

void readMask(char filepath[], ImageType& mask, int, int) {
    // read the mask
}

void padImage() {
    // pad an image so the mask works on edges
}

void correlation() {
    // calculate one mask
}

void correlateImage(ImageType& image, ImageType& mask) {
    // prep loop
    int N, M, Q; 
    image.getImageInfo(N, M, Q);
    int Nm, Mm, Qm;
    mask.getImageInfo(Nm, Mm, Qm);

    // pad image

    // loop over dimensions
    for (int i = 0; i < N; i ++) {
        for (int j = 0; j < M; j++) {
            continue
        }
    }
}

int main(int argc, char *argv[]) 
{
    // filepaths

    // read images

    // apply correlation

    // save results
}