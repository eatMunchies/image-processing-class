# https://www.cse.unr.edu/~bebis/CS474/Code/makefile
CFLAGS = -g

Experiment1: image.h image.o ReadImage.o ReadImageHeader.o WriteImage.o Experiment1.cpp
	g++ -o Experiment1 $(CFLAGS) image.o ReadImage.o ReadImageHeader.o WriteImage.o Experiment1.cpp
ReadImage.o: image.h ReadImage.cpp
	g++ -c $(CFLAGS) ReadImage.cpp
ReadImageHeader.o: image.h ReadImageHeader.cpp
	g++ -c $(CFLAGS) ReadImageHeader.cpp
WriteImage.o: image.h WriteImage.cpp
	g++ -c $(CFLAGS) WriteImage.cpp
image.o: image.h image.cpp
	g++ -c $(CFLAGS) image.cpp

clean:
	rm -f *.o Experiment1
