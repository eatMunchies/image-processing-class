#include "histogram.h"
#include <cmath>
#include <iostream>

using namespace std;

Histogram::Histogram()
{
    for (int i = 0; i < 256; i++) {
        frequencies[i] = 0;
    }
}

Histogram::Histogram(ImageType& image)
{
    for (int i = 0; i < 256; i++) {
        frequencies[i] = 0;
    }
    int N, M, Q;
    int val;
    image.getImageInfo(N, M, Q);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.getPixelVal(i, j, val);
            frequencies[val]++; // increment the frequency
        }
    }
}

float Histogram::getCDF(const int index) 
{
    return cdf[index];
}

void Histogram::getFrequency(const int index, int& value) 
{
    value = frequencies[index];
}

void Histogram::addFrequency(const int index) 
{
    frequencies[index]++;
}

void Histogram::setFrequency(int index, const int value) {
    frequencies[index] = value;
}

void Histogram::getMaxFrequency(int& value) 
{
    int max = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > max) {
            max = frequencies[i];
        } 
    }
    value = max;
}

void Histogram::convertToImage(const int N, const int M, ImageType& histogram) {
    // make an image to display the frequencies
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            histogram.setPixelVal(i, j, 255);
        }
    }

    int maxFreq;
    getMaxFrequency(maxFreq);

    if (maxFreq == 0) return;

    for (int grayLevel = 0; grayLevel < M && grayLevel < 256; grayLevel++) {
        int frequency;
        getFrequency(grayLevel, frequency);

        int scaledHeight = (frequency * N) / maxFreq;
        for (int row = N - scaledHeight; row < N; row++) {
            histogram.setPixelVal(row, grayLevel, 0);
        }
    }
}

void Histogram::processCDF(int n) {
    // based on the current frequency array
    // calculate the probability of each value
    // we are assuming the total number of values is 256x256 = 65536
    float probabilites[256];
    // cdf is the P(X <= x) of values of x
    for (int i = 0; i < 256; i++) {
        probabilites[i] = ((float) frequencies[i]) / n;
        if (i == 0) {
            cdf[i] = probabilites[i];
        }
        else {
            cdf[i] = probabilites[i] + cdf[i - 1];
        }
    }
}

int normalizeValue(float number, int range) {
    if (number < 0) {number = 0.0;}
    if (number > range) {number = range;}
    return (int) round(number * range);
};

void Histogram::applyCDF(ImageType& image) {
    int N, M, Q; // should be 256 all around
    int value;
    int eqValue;

    image.getImageInfo(N, M, Q);

    processCDF(N * M);
    
    /*
    multiply each original pixel value 
    with its cumulative distribution function probability 
    and multiply to bit space (denormalize) 
    and round to closest int within bit space
    */
    for (int i = 0; i < N; i++) { // rows
        for (int j = 0; j < M; j++) { // columns
            image.getPixelVal(i, j, value);
            eqValue = normalizeValue(cdf[value], Q);
            image.setPixelVal(i, j, eqValue);
        }
    }
}