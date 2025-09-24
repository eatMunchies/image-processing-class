CC=gcc
CFLAGS=-Wall
LIBS=-lpng

all: image_sampling histogram_equalization histogram_specification image_quantization

image_sampling: image_sampling/image_sampling.c
    $(CC) $(CFLAGS) -o image_sampling/image_sampling image_sampling/image_sampling.c $(LIBS)

histogram_equalization: histogram_equalization/histogram_equalization.c
    $(CC) $(CFLAGS) -o histogram_equalization/histogram_equalization histogram_equalization/histogram_equalization.c $(LIBS)

histogram_specification: histogram_specification/histogram_specification.c
    $(CC) $(CFLAGS) -o histogram_specification/histogram_specification histogram_specification/histogram_specification.c $(LIBS)

image_quantization: image_quantization/image_quantization.c
    $(CC) $(CFLAGS) -o image_quantization/image_quantization image_quantization/image_quantization.c $(LIBS)

clean:
    rm -f image_sampling/image_sampling histogram_equalization/histogram_equalization histogram_specification/histogram_specification image_quantization/image_quantization