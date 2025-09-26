/*
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
#include <cstring>
#include <iostream>

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

int main(int argc, char *argv[]) {
    // base paths
    const char* basePath = "../photos/base/";
    const char* endPath = "../photos/transformed/equalization/";
    const char* file1 = "boat";
    const char* file2 = "f_16";
    const char* extension = ".pgm";

    // filenames
    char* boatPath = new char[strlen(basePath) + strlen(file1) + strlen(extension) + 1];
    strcpy(boatPath, basePath);
    strcat(boatPath, file1);
    strcat(boatPath, extension);
    char* fPath = new char[strlen(basePath) + strlen(file2) + strlen(extension) + 1];
    strcpy(fPath, basePath);
    strcat(fPath, file2);
    strcat(fPath, extension);

    // end paths
    char* endBoatPath = new char[strlen(endPath) + strlen(file1) + strlen(extension) + 1];
    strcpy(endBoatPath, endPath);
    strcat(endBoatPath, file1);
    strcat(endBoatPath, extension);

    char* endFPath = new char[strlen(endPath) + strlen(file2) + strlen(extension) + 1];
    strcpy(endFPath, endPath);
    strcat(endFPath, file2);
    strcat(endFPath, extension);

    // logging
    cout << "Reading file 1: " << boatPath << endl;

    // read first file
    int i, j;
    int M, N, Q;
    bool type;
    
    // read image header
    readImageHeader(boatPath, N, M, Q, type);

    cout << "Read image header" << endl;

    // allocate image 
    ImageType boat(N, M, Q);

    cout << "Allocated image space" << endl;

    // read image
    readImage(boatPath, boat);

    cout << "Read boat image" << endl;

    // equalization

    // write image
    writeImage(endBoatPath, NULL);
    cout << "wrote boat image" << endl;

    // logging
    cout << "Reading file 2: " << fPath << endl;

    // read first file
    int i, j;
    int M, N, Q;
    bool type;
    
    // read image header
    readImageHeader(fPath, N, M, Q, type);

    cout << "Read image header" << endl;

    // allocate image 
    ImageType f16(N, M, Q);

    cout << "Allocated image space" << endl;

    // read image
    readImage(boatPath, f16);

    cout << "Read f16 image" << endl;

    // equalization

    // write image
    writeImage(endFPath, NULL);
    cout << "wrote f16 image" << endl;

    return 0;
}