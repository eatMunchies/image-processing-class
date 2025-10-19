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
    ImageType lennaF7SP30(N, M, Q);
    ImageType lennaF7SP50(N, M, Q);
    ImageType lennaF15SP30(N, M, Q);
    ImageType lennaF15SP50(N, M, Q);
    medianFiltering(lennaSP30, lennaF7SP30, 7);
    medianFiltering(lennaSP30, lennaF15SP30, 15);
    medianFiltering(lennaSP50, lennaF7SP50, 7);
    medianFiltering(lennaSP50, lennaF15SP50, 15);

    cout << "lenna filtering" << endl;

    // apply averaging
    ImageType lennaA7SP30(N, M, Q);
    ImageType lennaA7SP50(N, M, Q);
    ImageType lennaA15SP30(N, M, Q);
    ImageType lennaA15SP50(N, M, Q);
    Mask average7("average", 7);
    Mask average15("average", 15);
    average7.applyMask(lennaSP30, lennaA7SP30, correlator);
    average7.applyMask(lennaSP50, lennaA7SP50, correlator);
    average15.applyMask(lennaSP30, lennaA15SP30, correlator);
    average15.applyMask(lennaSP50, lennaA15SP50, correlator);

    cout << "lenna averaging" << endl;

    // write out
    char* lennaSP30Path = "./sp30_lenna.pgm";
    char* lennaSP50Path = "./sp50_lenna.pgm";
    char* lennaSP30A7Path = "./sp30_a7_lenna.pgm";
    char* lennaSP30A15Path = "./sp30_a15_lenna.pgm";
    char* lennaSP30F7Path = "./sp30_f7_lenna.pgm";
    char* lennaSP30F15Path = "./sp30_f15_lenna.pgm";
    char* lennaSP50A7Path = "./sp50_a7_lenna.pgm";
    char* lennaSP50A15Path = "./sp50_a15_lenna.pgm";
    char* lennaSP50F7Path = "./sp50_f7_lenna.pgm";
    char* lennaSP50F15Path = "./sp50_f15_lenna.pgm";
    writeImage(lennaSP30Path, lennaSP30);
    writeImage(lennaSP50Path, lennaSP50);
    writeImage(lennaSP30A7Path, lennaA7SP30);
    writeImage(lennaSP30A15Path, lennaA15SP30);
    writeImage(lennaSP50A7Path, lennaA7SP50);
    writeImage(lennaSP50A15Path, lennaA15SP50);
    writeImage(lennaSP30F7Path, lennaF7SP30);
    writeImage(lennaSP30F15Path, lennaF15SP30);
    writeImage(lennaSP50F7Path, lennaF7SP50);
    writeImage(lennaSP50F15Path, lennaF15SP50);

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
    ImageType boatF7SP30(N, M, Q);
    ImageType boatF7SP50(N, M, Q);
    ImageType boatF15SP30(N, N, Q);
    ImageType boatF15SP50(N, M, Q);
    medianFiltering(boatSP30, boatF7SP30, 7);
    medianFiltering(boatSP30, boatF15SP30, 15);
    medianFiltering(boatSP50, boatF7SP50, 7);
    medianFiltering(boatSP50, boatF15SP50, 15);

    // apply averaging
    ImageType boatA7SP30(N, M, Q);
    ImageType boatA7SP50(N, M, Q);
    ImageType boatA15SP30(N, N, Q);
    ImageType boatA15SP50(N, M, Q);
    average7.applyMask(boatSP30, boatA7SP30, correlator);
    average7.applyMask(boatSP50, boatA7SP50, correlator);
    average15.applyMask(boatSP30, boatA15SP30, correlator);
    average15.applyMask(boatSP50, boatA15SP50, correlator);

    // write out
    char* boatSP30Path = "./sp30_boat.pgm";
    char* boatSP50Path = "./sp50_boat.pgm";
    char* boatSP30A7Path = "./sp30_a7_boat.pgm";
    char* boatSP30A15Path = "./sp30_a15_boat.pgm";
    char* boatSP30F7Path = "./sp30_f7_boat.pgm";
    char* boatSP30F15Path = "./sp30_f15_boat.pgm";
    char* boatSP50A7Path = "./sp50_a7_boat.pgm";
    char* boatSP50A15Path = "./sp50_a15_boat.pgm";
    char* boatSP50F7Path = "./sp50_f7_boat.pgm";
    char* boatSP50F15Path = "./sp50_f15_boat.pgm";
    writeImage(boatSP30Path, boatSP30);
    writeImage(boatSP50Path, boatSP50);
    writeImage(boatSP30A7Path, boatA7SP30);
    writeImage(boatSP30A15Path, boatA15SP30);
    writeImage(boatSP50A7Path, boatA7SP50);
    writeImage(boatSP50A15Path, boatA15SP50);
    writeImage(boatSP30F7Path, boatF7SP30);
    writeImage(boatSP30F15Path, boatF15SP30);
    writeImage(boatSP50F7Path, boatF7SP50);
    writeImage(boatSP50F15Path, boatF15SP50);
}