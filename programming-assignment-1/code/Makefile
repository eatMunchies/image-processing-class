# https://www.cse.unr.edu/~bebis/CS474/Code/makefile
CFLAGS = -g

ImageQuantization: image.h image.o ReadImage.o ReadImageHeader.o WriteImage.o ImageQuantization.cpp
	g++ -o ImageQuantization image.o ReadImage.o ReadImageHeader.o WriteImage.o ImageQuantization.cpp
ImageSampling: image.h image.o ReadImage.o ReadImageHeader.o WriteImage.o ImageSampling.cpp
	g++ -o ImageSampling $(CFLAGS) image.o ReadImage.o ReadImageHeader.o WriteImage.o ImageSampling.cpp
ReadImage.o: image.h ReadImage.cpp
	g++ -c $(CFLAGS) ReadImage.cpp
ReadImageHeader.o: image.h ReadImageHeader.cpp
	g++ -c $(CFLAGS) ReadImageHeader.cpp
WriteImage.o: image.h WriteImage.cpp
	g++ -c $(CFLAGS) WriteImage.cpp
image.o: image.h image.cpp
	g++ -c $(CFLAGS) image.cpp