CC=gcc
CFLAGS=-Wall -std=c99 -pedantic
PROG=cbmp
PROGARGS=
SOURCE=cbitmap/main.c cbitmap/bmp.c
OBJ=$(patsubst %.c, %.o, $(SOURCE))

.SILENT:
.PHONY: all test clean help

all: $(PROG)

.c.o:
	@echo "compile $<"
	$(CC) $(CFLAGS) -c $<

$(PROG): $(OBJ)
	@echo "linking $(PROG)"
	$(CC) $(CFLAGS) -O0 $(SOURCE) -o $(PROG)

clean:
	-rm -f $(PROG)
	-rm -rf *.o

test: $(PROG)
	echo "./$(PROG) $(PROGARGS)
	./$(PROG) $(PROGARGS)
	
help:
	@echo "make options are all, test, clean, and help"
