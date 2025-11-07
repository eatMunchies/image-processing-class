# https://www.cse.unr.edu/~bebis/CS474/Code/makefile
CFLAGS = -g

Experiment3: image.h image.o FT.h FT.o ReadImage.o ReadImageHeader.o WriteImage.o Experiment3.cpp
	g++ -o Experiment3 ${CFLAGS} image.o ReadImage.o FT.o ReadImageHeader.o WriteImage.o Experiment3.cpp
Experiment2: image.h image.o FT.h FT.o ReadImage.o ReadImageHeader.o WriteImage.o Experiment2.cpp
	g++ -o Experiment2 $(CFLAGS) image.o ReadImage.o FT.o ReadImageHeader.o WriteImage.o Experiment2.cpp
Experiment1: image.h image.o FT.h FT.o ReadImage.o ReadImageHeader.o WriteImage.o Experiment1.cpp
	g++ -o Experiment1 $(CFLAGS) image.o ReadImage.o FT.o ReadImageHeader.o WriteImage.o Experiment1.cpp
FT.o: image.h FT.h FT.cpp
	g++ -c $(CFLAGS) FT.cpp
ReadImage.o: image.h ReadImage.cpp
	g++ -c $(CFLAGS) ReadImage.cpp
ReadImageHeader.o: image.h ReadImageHeader.cpp
	g++ -c $(CFLAGS) ReadImageHeader.cpp
WriteImage.o: image.h WriteImage.cpp
	g++ -c $(CFLAGS) WriteImage.cpp
image.o: image.h image.cpp
	g++ -c $(CFLAGS) image.cpp

clean:
	rm -f *.o Experiment1 Experiment2 Experiment3
