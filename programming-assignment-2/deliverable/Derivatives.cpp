/*
Author: Anthony Silva
Assignment: Programming Assignment 2
Class: Image Processing
Date: 10/20/25
File: Derivates.cpp
Description:
Apply first and second derivative masks to the source images
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

int main(int argc, char* argv[])
{
    // lenna and sf
    CorrelationHelper correlator;
    char* lennaPath = "./lenna.pgm";
    char* sfPath = "./sf.pgm";

    cout << "init" << endl;

    // lenna
    int N, M, Q;
    bool type;
    readImageHeader(lennaPath, N, M, Q, type);
    ImageType lenna(N, M, Q);
    readImage(lennaPath, lenna);

    cout << "read lenna" << endl;

    // get masks
    Mask prewittx("./prewittX.txt");
    Mask prewitty("./prewitty.txt");
    Mask sobelx("./sobelX.txt");
    Mask sobely("./sobelY.txt");
    Mask laplacian("./laplacian.txt");

    cout << "initialized masks" << endl;

    ImageType lennaPX(N, M, Q);
    ImageType lennaPY(N, M, Q);
    ImageType lennaSX(N, M, Q);
    ImageType lennaSY(N, M, Q);
    ImageType lennaL(N, M, Q);
    prewittx.applyMask(lenna, lennaPX, correlator);
    prewitty.applyMask(lenna, lennaPY, correlator);
    sobelx.applyMask(lenna, lennaSX, correlator);
    sobely.applyMask(lenna, lennaSY, correlator);
    laplacian.applyMask(lenna, lennaL, correlator);

    cout << "applied masks" << endl;

    char* lennaPXPath = "./lenna_px.pgm";
    char* lennaPYPath = "./lenna_py.pgm";
    char* lennaSXPath = "./lenna_sx.pgm";
    char* lennaSYPath = "./lenna_sy.pgm";
    char* lennaLPath = "./lenna_l.pgm";
    writeImage(lennaPXPath, lennaPX);
    writeImage(lennaPYPath, lennaPY);
    writeImage(lennaSXPath, lennaSX);
    writeImage(lennaSYPath, lennaSY);
    writeImage(lennaLPath, lennaL);

    cout << "wrote images" << endl;

    // sf
    readImageHeader(sfPath, N, M, Q, type);
    ImageType sf(N, M, Q);
    readImage(sfPath, sf);

    // get masks
    ImageType sfPX(N, M, Q);
    ImageType sfPY(N, M, Q);
    ImageType sfSX(N, M, Q);
    ImageType sfSY(N, M, Q);
    ImageType sfL(N, M, Q);
    prewittx.applyMask(sf, sfPX, correlator);
    prewitty.applyMask(sf, sfPY, correlator);
    sobelx.applyMask(sf, sfSX, correlator);
    sobely.applyMask(sf, sfSY, correlator);
    laplacian.applyMask(sf, sfL, correlator);

    char* sfPXPath = "./sf_px.pgm";
    char* sfPYPath = "./sf_py.pgm";
    char* sfSXPath = "./sf_sx.pgm";
    char* sfSYPath = "./sf_sy.pgm";
    char* sfLPath = "./sf_l.pgm";
    writeImage(sfPXPath, sfPX);
    writeImage(sfPYPath, sfPY);
    writeImage(sfSXPath, sfSX);
    writeImage(sfSYPath, sfSY);
    writeImage(sfLPath, sfL);
}