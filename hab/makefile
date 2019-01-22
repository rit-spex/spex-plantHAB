CC=gcc
CFLAGS=-Wall -std=c99 -pedantic
PROG=cbmp
PROGARGS=-f $(TESTPICTURE)
SOURCE=cbitmap/main.c cbitmap/bmp.c
OBJ=$(patsubst %.c, %.o, $(SOURCE))
OUTPUTFILE=$(PROG).log

TESTPICTURE=testbmp.bmp
CAPTUREARGS=-t 1 -e bmp -w 1280 -h 720 -o $(TESTPICTURE)

.SILENT:
.PHONY: all test clean help

all: $(PROG)

.c.o:
	@echo "compile $<"
	$(CC) $(CFLAGS) -c $< -o $(patsubst %.c, %.o, $<)

$(PROG): $(OBJ)
	@echo "linking $(PROG)"
	$(CC) $(CFLAGS) -O0 $(OBJ) -o $(PROG)

clean:
	-rm -f $(PROG)
	-rm -f cbitmap/*.o
	-rm -f $(OUTPUTFILE)
	-rm -f $(TESTPICTURE)

test: $(PROG)
	echo "raspistill $(CAPTUREARGS) &> $(OUTPUTFILE)"
	raspistill $(CAPTUREARGS) &> $(OUTPUTFILE)
	sleep 5
	echo "./$(PROG) $(PROGARGS) &>> $(OUTPUTFILE)"
	./$(PROG) $(PROGARGS) &>> $(OUTPUTFILE)
	cat $(OUTPUTFILE)
	
help:
	@echo "make options are all, test, clean, and help"
