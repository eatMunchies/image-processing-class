/* 
Author: Anthony Silva
Assignment: Programming Assignment 1
Class: Image Processing
Date: 9/17/25
File: image_sampling.c
Description:
a program to change the special resolution of an image from 
256x256 to 128x128, 64x64, 32x32 using image sampling by factors of 2, 4, and 8
resize the images back to the original size 256 x 256 to see the differences
using images lemma.png and peppers.png
*/

#include <stdio.h>
#include <stdlib.h>
#include <png.h>

char PHOTO_PATH[] = "../../photos/base/";
char END_PATH[] = "../../photos/transformed/";

typedef struct {
    unsigned char *image;
    int width;
    int height;
} GrayscaleImage;

GrayscaleImage* read_image(const char* filepath) {
    FILE *fp = fopen(filepath, "rb");
    if (!fp) {
        printf("Error: File %s could not be read\n", filepath);
        return NULL;
    }

    
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        return NULL;
    }
    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        return NULL;
    }

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    
    GrayscaleImage *image = malloc(sizeof(GrayscaleImage));
    image->height = height;
    image->width = width;
    image->image = malloc(width * height);

    // actually read the image
    png_bytep *row_pointers = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = image->data + y * width;
    }

    png_read_image(png, row_pointers);

    // clean
    free(row_pointers);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);

    return image;
}

GrayscaleImage* sample_image(const GrayscaleImage* image, const int factor) {
    GrayscaleImage *sampled = malloc(sizeof(GrayscaleImage));


}

bool save_image(const char* filepath, GrayscaleImage* image) {
    FILE *fp = fopen(filepath, "wb");
    if (!fp) {
        return 0;
    }

    // write structure
    png_structp png png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        return 0;
    }

    // info structure
    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        return 0;
    }


    png_write_info(png, info);

    png_bytep *row_pointers = malloc(sizeof(png_bytep) * image->height);
    for (int y = 0; y < image->height; y++) {
        row_pointers[y] = image->image + y * image->width;
    }

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    // cleanup
    free(row_pointers);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    
    return true;
}

void free_grayscale_image(GrayscaleImage* image) {
    if (image) {
        free(image->image);
        free(image);
    }
}

int main() {

    // get lenna and peppers images
    GrayscaleImage *lenna = read_image(strcat(PHOTO_PATH, "lenna.png"));
    GrayscaleImage *peppers = read_image(strcat(PHOTO_PATH, "peppers.png"));
    
    // subsample


    // write pictures
    save_image(strcat(END_PATH, "lenna_test.png"));
    save_image(strcat(END_PATH, "peppers_test.png"))

    // free data
    free_grayscale_image(lenna);
    free_grayscale_image(peppers);

    return 0;
}