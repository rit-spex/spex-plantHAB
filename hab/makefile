CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -O3
PROG=cbmp
PROGARGS=-f $(TESTPICTURE)
SOURCE=cbitmap/main.c cbitmap/bmp.c
OBJ=$(patsubst %.c, %.o, $(SOURCE))
OUTPUTFILE=$(PROG).log

TESTIMGW=1280
TESTIMGH=720
TESTPXSIZE=24
TESTDIBSIZE=40
PROGSIZETESTARGS=-x $(TESTIMGW) -y $(TESTIMGH) -z $(TESTPXSIZE) -b $(TESTDIBSIZE) -n
TESTPICTURE=testbmp.bmp
CAPTUREARGS=-t 1 -e bmp -w $(TESTIMGW) -h $(TESTIMGH) -o $(TESTPICTURE)

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
	raspistill $(CAPTUREARGS) &>$(OUTPUTFILE)
	sleep 5
	echo "./$(PROG) $(PROGSIZETESTARGS) $(PROGARGS) &>> $(OUTPUTFILE)"
	./$(PROG) $(PROGSIZETESTARGS) $(PROGARGS) &>>$(OUTPUTFILE)
	cat $(OUTPUTFILE)
	sleep 0.5
	
help:
	@echo "make options are all, test, clean, and help"
