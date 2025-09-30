# https://www.cse.unr.edu/~bebis/CS474/Code/makefile
CFLAGS = -g

HistogramSpecification: image.h image.o histogram.h histogram.o ReadImage.o ReadImageHeader.o WriteImage.o HistogramSpecification.cpp ImageSampling ImageQuantization HistogramEqualization 
	g++ -o HistogramSpecification image.o histogram.o ReadImage.o ReadImageHeader.o WriteImage.o HistogramSpecification.cpp
HistogramEqualization: image.h image.o histogram.h histogram.o ReadImage.o ReadImageHeader.o WriteImage.o HistogramEqualization.cpp
	g++ -o HistogramEqualization image.o histogram.o ReadImage.o ReadImageHeader.o WriteImage.o HistogramEqualization.cpp
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
histogram.o: histogram.h histogram.cpp
	g++ -c $(CFLAGS) histogram.cpp
image.o: image.h image.cpp
	g++ -c $(CFLAGS) image.cpp