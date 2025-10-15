# https://www.cse.unr.edu/~bebis/CS474/Code/makefile
CFLAGS = -g


# ExtraCredit: image.h image.o histogram.h histogram.o ReadImage.o ReadImageHeader.o WriteImage.o ExtraCredit.cpp Correlation Smoothing Filtering Derivatives 
# 	g++ -o Derivatives image.o histogram.o ReadImage.o ReadImageHeader.o WriteImage.o ExtraCredit.cpp
# Derivatives: image.h image.o histogram.h histogram.o ReadImage.o ReadImageHeader.o WriteImage.o Derivatives.cpp Correlation Smoothing Filtering 
# 	g++ -o Derivatives image.o histogram.o ReadImage.o ReadImageHeader.o WriteImage.o Derivatives.cpp
# Filtering: image.h image.o histogram.h histogram.o ReadImage.o ReadImageHeader.o WriteImage.o Filtering.cpp Correlation Smoothing
# 	g++ -o Filtering image.o histogram.o ReadImage.o ReadImageHeader.o WriteImage.o Filtering.cpp
# Smoothing: image.h image.o ReadImage.o ReadImageHeader.o WriteImage.o Smoothing.cpp Correlation
# 	g++ -o Smoothing image.o ReadImage.o ReadImageHeader.o WriteImage.o Smoothing.cpp
Correlation: image.h image.o ReadImage.o ReadImageHeader.o WriteImage.o Correlation.cpp
	g++ -o Correlation $(CFLAGS) image.o ReadImage.o ReadImageHeader.o WriteImage.o Correlation.cpp
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


clean:
	rm -f *.o ExtraCredit Derivatives Filtering Smoothing Correlation
