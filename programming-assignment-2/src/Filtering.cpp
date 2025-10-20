/*
Author: Anthony Silva
Assignment: Programming Assignment 2
Class: Image Processing
Date: 10/20/25
File: Filtering.coo
Description:
This program does median filtering on a given image. 
*/

#include "image.h"
#include "mask.h"
// #include "correlationHelper.h"
#include <cstring>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>

using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], ImageType&);
int writeImage(char[], ImageType&);

void saltPepper(ImageType& input, ImageType& output, float p) {
    int N, M, Q;
    input.getImageInfo(N, M, Q);
    random_device r;
    mt19937 gen(r());
    uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            float randomNum = dis(gen);
            int val = 0;
            if (randomNum < p) {
                float blackWhite = dis(gen);
                if (blackWhite > 0.5) {
                    val = 0;
                } else {
                    val = Q;
                }
                output.setPixelVal(i, j, val);
            } else {
                input.getPixelVal(i, j, val);
                output.setPixelVal(i, j, val);
            }
        }
    }
}

void medianFilteringHelper(ImageType& input, ImageType& output, int windowSize, int row, int col) {
    int N, M, Q;
    input.getImageInfo(N, M, Q);
    
    int rowStart = row - windowSize / 2;
    int colStart = col - windowSize / 2;
    vector<int> window;
    int tempVal;
    
    for (int wRow = 0; wRow < windowSize; wRow++) {
        int row1 = rowStart + wRow;    
        if (row1 < 0 || row1 >= N) continue;
        for (int wCol = 0; wCol < windowSize; wCol++) {
            int col1 = colStart + wCol;
            if (col1 < 0 || col1 >= M) continue;
            input.getPixelVal(row1, col1, tempVal);
            window.push_back(tempVal);
        }
    }

    sort(window.begin(), window.end());

    int n = window.size();
    output.setPixelVal(row, col, window[n / 2]);
}

void medianFiltering(ImageType& input, ImageType& output, int window) {
    int N, M, Q;
    input.getImageInfo(N, M, Q);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            medianFilteringHelper(input, output, window, j, i);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <mask_size>" << endl;
        return 1;
    }
    
    int maskSize = atoi(argv[1]);
    
    // filepaths
    char* lennaFilePath = "./lenna.pgm";
    char* boatFilePath = "./boat.pgm";
    CorrelationHelper correlator;

    cout << "Init" << endl;

    // read lenna
    int N, M, Q;
    bool type;
    readImageHeader(lennaFilePath, N, M, Q, type);
    ImageType lenna(N, M, Q);
    ImageType lennaSP30(N, M, Q);
    ImageType lennaSP50(N, M, Q);
    readImage(lennaFilePath, lenna);

    cout << "read lenna" << endl;

    // apply salt and pepper noise
    saltPepper(lenna, lennaSP30, 0.3);
    saltPepper(lenna, lennaSP50, 0.5);

    cout << "lenna sp" << endl;

    // apply filtering
    ImageType lennaFSP30(N, M, Q);
    ImageType lennaFSP50(N, M, Q);
    medianFiltering(lennaSP30, lennaFSP30, maskSize);
    medianFiltering(lennaSP50, lennaFSP50, maskSize);

    cout << "lenna filtering" << endl;

    // apply averaging
    ImageType lennaASP30(N, M, Q);
    ImageType lennaASP50(N, M, Q);
    Mask averageMask("average", maskSize);
    averageMask.applyMask(lennaSP30, lennaASP30, correlator);
    averageMask.applyMask(lennaSP50, lennaASP50, correlator);

    cout << "lenna averaging" << endl;

    // write out
    char* lennaSP30Path = "./sp30_lenna.pgm";
    char* lennaSP50Path = "./sp50_lenna.pgm";
    char* lennaASP30Path = "./sp30_a_lenna.pgm";
    char* lennaASP50Path = "./sp50_a_lenna.pgm";
    char* lennaFSP30Path = "./sp30_f_lenna.pgm";
    char* lennaFSP50Path = "./sp50_f_lenna.pgm";
    writeImage(lennaSP30Path, lennaSP30);
    writeImage(lennaSP50Path, lennaSP50);
    writeImage(lennaASP30Path, lennaASP30);
    writeImage(lennaASP50Path, lennaASP50);
    writeImage(lennaFSP30Path, lennaFSP30);
    writeImage(lennaFSP50Path, lennaFSP50);

    cout << "lenna write" << endl;

    // do the same for boat
    readImageHeader(boatFilePath, N, M, Q, type);
    ImageType boat(N, M, Q);
    ImageType boatSP30(N, M, Q);
    ImageType boatSP50(N, M, Q);
    readImage(boatFilePath, boat);

    // sp
    saltPepper(boat, boatSP30, 0.3);
    saltPepper(boat, boatSP50, 0.5);

    // apply filtering
    ImageType boatFSP30(N, M, Q);
    ImageType boatFSP50(N, M, Q);
    medianFiltering(boatSP30, boatFSP30, maskSize);
    medianFiltering(boatSP50, boatFSP50, maskSize);

    // apply averaging
    ImageType boatASP30(N, M, Q);
    ImageType boatASP50(N, M, Q);
    averageMask.applyMask(boatSP30, boatASP30, correlator);
    averageMask.applyMask(boatSP50, boatASP50, correlator);

    // write out
    char* boatSP30Path = "./sp30_boat.pgm";
    char* boatSP50Path = "./sp50_boat.pgm";
    char* boatASP30Path = "./sp30_a_boat.pgm";
    char* boatASP50Path = "./sp50_a_boat.pgm";
    char* boatFSP30Path = "./sp30_f_boat.pgm";
    char* boatFSP50Path = "./sp50_f_boat.pgm";
    writeImage(boatSP30Path, boatSP30);
    writeImage(boatSP50Path, boatSP50);
    writeImage(boatASP30Path, boatASP30);
    writeImage(boatASP50Path, boatASP50);
    writeImage(boatFSP30Path, boatFSP30);
    writeImage(boatFSP50Path, boatFSP50);
}