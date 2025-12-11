# https://www.cse.unr.edu/~bebis/CS474/Code/makefile
CFLAGS = -g

Experiment4: image.h image.o FT.h FT.o frequencyHelper.h frequencyHelper.o ReadImage.o FT.o ReadImageHeader.o WriteImage.o Experiment4.cpp Experiment3
	g++ -o Experiment4 ${CFLAGS} image.o ReadImage.o FT.o frequencyHelper.o ReadImageHeader.o WriteImage.o Experiment4.cpp
Experiment3: image.h image.o FT.h FT.o frequencyHelper.h frequencyHelper.o ReadImage.o ReadImageHeader.o WriteImage.o Experiment3.cpp Experiment2
	g++ -o Experiment3 ${CFLAGS} image.o ReadImage.o FT.o frequencyHelper.o ReadImageHeader.o WriteImage.o Experiment3.cpp
Experiment2: image.h image.o correlationHelper.h correlationHelper.o mask.h mask.o frequencyHelper.h frequencyHelper.o FT.h FT.o ReadImage.o ReadImageHeader.o WriteImage.o Experiment2.cpp Experiment1
	g++ -o Experiment2 $(CFLAGS) mask.o image.o correlationHelper.o ReadImage.o FT.o frequencyHelper.o ReadImageHeader.o WriteImage.o Experiment2.cpp 
Experiment1: image.h image.o frequencyHelper.h frequencyHelper.o FT.h FT.o ReadImage.o ReadImageHeader.o WriteImage.o Experiment1.cpp
	g++ -o Experiment1 $(CFLAGS) image.o ReadImage.o FT.o frequencyHelper.o ReadImageHeader.o WriteImage.o Experiment1.cpp
frequencyHelper.o: FT.h frequencyHelper.h frequencyHelper.cpp
	g++ -c ${CFLAGS} frequencyHelper.cpp
correlationHelper.o: image.h correlationHelper.h correlationHelper.cpp
	g++ -c ${CFLAGS} correlationHelper.cpp
mask.o: image.h mask.h mask.cpp
	g++ -c $(CFLAGS) mask.cpp
FT.o: image.h mask.h FT.h FT.cpp
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
	rm -f *.o Experiment1 Experiment2 Experiment3 Experiment4
