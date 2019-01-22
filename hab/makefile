CC=gcc
CFLAGS=-Wall -std=c99 -pedantic
PROG=cbmp
PROGARGS=
SOURCE=cbitmap/main.c cbitmap/bmp.c
OBJ=$(patsubst %.c, %.o, $(SOURCE))
OUTPUTFILE=$(PROG).log

TESTPICTURE=testbmp.bmp
CAPTUREARGS=-e bmp  -w 1280 -h 720 -o $(TESTPICTURE)

.SILENT:
.PHONY: all test clean help

all: $(PROG)

.c.o:
	@echo "compile $<"
	$(CC) $(CFLAGS) -c $<

$(PROG): $(OBJ)
	@echo "linking $(PROG)"
	$(CC) $(CFLAGS) -O0 $(OBJ) -o $(PROG)

clean:
	-rm -f $(PROG)
	-rm -f *.o
	-rm -f $(OUTPUTFILE)

test: $(PROG)
	echo "raspistill $(CAPTUREARGS) &> $(OUTPUTFILE)"
	raspistill $(CAPTUREARGS) &> $(OUTPUTFILE)
	echo "./$(PROG) $(PROGARGS) &>> $(OUTPUTFILE)"
	./$(PROG) $(PROGARGS) &>> $(OUTPUTFILE)
	cat $(OUTPUTFILE)
	
help:
	@echo "make options are all, test, clean, and help"
