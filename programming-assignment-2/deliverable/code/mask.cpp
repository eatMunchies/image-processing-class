#include "mask.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

Mask::Mask() {}

Mask::Mask(char* type, int maskSizeParam) {
    maskSize = maskSizeParam;
    mask = new float*[maskSizeParam];
    for (int i = 0; i < maskSizeParam; i++) {
        mask[i] = new float[maskSizeParam];
    }
    if (strcmp(type, "average") == 0) {
        for (int i = 0; i < maskSizeParam; i++) {
            for (int j = 0; j < maskSizeParam; j++) {
                mask[i][j] = 1.0;
            }
        }
    }
    if (strcmp(type, "gaussian") == 0) {
        if (maskSizeParam == 7) loadFromFile("./gaussian7.txt");
        if (maskSizeParam == 15) loadFromFile("./gaussian15.txt");
    }
    normalizeMask();
}


Mask::Mask(char* filepath) {
    // get mask size
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return;
    }
    
    std::string firstLine;
    std::getline(file, firstLine);
    std::istringstream iss(firstLine);
    float value;
    int colCount = 0;
    
    while (iss >> value) {
        colCount++;
    }
    
    maskSize = colCount; 
    file.close();
    
    // allocate mask array
    mask = new float*[maskSize];
    for (int i = 0; i < maskSize; i++) {
        mask[i] = new float[maskSize];
    }
    
    loadFromFile(filepath);
    
    normalizeMask();
}

void Mask::loadFromFile(char* filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    
    std::string line;
    int row = 0;
    
    while (std::getline(file, line) && row < maskSize) {
        std::istringstream iss(line);
        float value;
        int col = 0;
        
        while (iss >> value && col < maskSize) {
            mask[row][col] = value;
            col++;
        }
        row++;
    }
    
    file.close();
}

void Mask::normalizeMask() {
    float sum = 0.0;
    
    for (int i = 0; i < maskSize; i++) {
        for (int j = 0; j < maskSize; j++) {
            sum += mask[i][j];
        }
    }
    
    if (sum != 0.0) {
        for (int i = 0; i < maskSize; i++) {
            for (int j = 0; j < maskSize; j++) {
                mask[i][j] /= sum;
            }
        }
    }
}

void Mask::applyMask(ImageType& input, ImageType& output, CorrelationHelper& helper) {
    int N, M, Q;
    input.getImageInfo(N, M, Q);
    
    float** rawOutput = new float*[N];
    for (int i = 0; i < N; i++) {
        rawOutput[i] = new float[M];
    }
    
    helper.correlateImage(rawOutput, input, mask, maskSize, maskSize);
    
    helper.scaleImage(rawOutput, output);
    
    // clean
    for (int i = 0; i < N; i++) {
        delete[] rawOutput[i];
    }
    delete[] rawOutput;
}