#include "correlationHelper.h"
#include <iostream>
#include <cstring>

using namespace std;

CorrelationHelper::CorrelationHelper() {
    
}

void CorrelationHelper::correlation(float** output, ImageType& image, ImageType& mask, int N, int M, int Nm, int Mm, int x, int y) {
    // x is col
    // y is row
    
    cout << "correlating at x, y: " << x << ", " << y << endl;
    cout << "N, M, Nm, Mm : " << N << ", " << M << ", " << Nm << ", " << Mm << endl;

    // calculate one mask
    float val = 0;

    // assuming mask is odd dimensions
    // assuming output is same dimensions as input
    int maskVal;
    int imageVal;

    int rowStart = y - Nm / 2;
    int colStart = x - Mm / 2;

    // loop over the mask
    // go from start row to end row
    for (int mRow = 0; mRow < Nm; mRow++) {
        int row = rowStart + mRow;

        // if (row < 0 || row >= N) {
        //     cout << "out of bounds row!: " << row << endl;
        //     continue;
        // }

        // go from startcol to end col
        for (int mCol = 0; mCol < Mm; mCol++) {
            int col = colStart + mCol;

            // if (col < 0 || col >= M) {
            //     cout << "out of bounds col!: " << col << endl;
            //     continue;
            // }
            
            mask.getPixelVal(mRow, mCol, maskVal);
            
            // 0 padding implementation
            if (row < 0 || row >= N || col < 0 || col >= M) {
                imageVal = 0;
            } else {
                image.getPixelVal(row, col, imageVal);
            }
            
            val += (float) maskVal * (float) imageVal;
            
            // if (val > 0) {
            //     cout << "row loop at: " << row << endl;
            //     cout << "maskRow loop at: " << mRow << endl;
            //     cout << "col loop at: " << col << endl;
            //     cout << "mask col loop at: " << mCol << endl;
            //     cout << "got mask val: " << maskVal << endl;
            //     cout << "got image val: " << imageVal << endl;
            //     cout << "calculated mask val: " << val << endl;
            // }
        }
    } 

    cout << "calculated mask val: " << val << endl;

    // set output
    output[y][x] = val;
}

void CorrelationHelper::correlateImage(float** output, ImageType& image, ImageType& mask) {
    // prep loop
    int N, M, Q; 
    image.getImageInfo(N, M, Q);
    int Nm, Mm, Qm;
    mask.getImageInfo(Nm, Mm, Qm);

    cout << "got image info for correlate image" << endl;

    // loop over dimensions
    for (int i = 0; i < N; i++) { // N is rows, i is row = y
        for (int j = 0; j < M; j++) { // M is cols, j is col = x
            // for each pixel
            correlation(output, image, mask, N, M, Nm, Mm, j, i);
        }
    }
}

void CorrelationHelper::scaleImage(float** rawOutput, ImageType& output) {
    int N, M, Q;
    output.getImageInfo(N, M, Q);
    float minVal = rawOutput[0][0];
    float maxVal = rawOutput[0][0];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (rawOutput[i][j] < minVal) minVal = rawOutput[i][j];
            if (rawOutput[i][j] > maxVal) maxVal = rawOutput[i][j];
        }
    }
    float range = maxVal - minVal;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            int scaled = (int)((rawOutput[i][j] - minVal) / range * (Q - 1));
            output.setPixelVal(i, j, scaled);
        }
    }
}

void CorrelationHelper::correlateImage(float** output, ImageType& image, float** mask, int Nm, int Mm) {
    // Overloaded version that takes float** mask
    int N, M, Q; 
    image.getImageInfo(N, M, Q);

    // loop over dimensions
    for (int i = 0; i < N; i++) { // N is rows, i is row = y
        for (int j = 0; j < M; j++) { // M is cols, j is col = x
            correlation(output, image, mask, N, M, Nm, Mm, j, i);
        }
    }
}

void CorrelationHelper::correlation(float** output, ImageType& image, float** mask, int N, int M, int Nm, int Mm, int x, int y) {
    // x is col, y is row
    // Overloaded version that takes float** mask instead of ImageType
    
    float val = 0;
    float maskVal;
    int imageVal;

    int rowStart = y - Nm / 2;
    int colStart = x - Mm / 2;

    // loop over the mask
    for (int mRow = 0; mRow < Nm; mRow++) {
        int row = rowStart + mRow;

        // go from startcol to end col
        for (int mCol = 0; mCol < Mm; mCol++) {
            int col = colStart + mCol;
            
            maskVal = mask[mRow][mCol];
            
            // 0 padding implementation
            if (row < 0 || row >= N || col < 0 || col >= M) {
                imageVal = 0;
            } else {
                image.getPixelVal(row, col, imageVal);
            }
            
            val += maskVal * (float) imageVal;
        }
    } 

    // set output
    output[y][x] = val;
}