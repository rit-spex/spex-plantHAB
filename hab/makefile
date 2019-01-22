CC=gcc
CFLAGS=-Wall -std=c99 -pedantic
PROG=cbitmap
PROGARGS=
SOURCE=cbitmap/main.c cbitmap/bmp.c
OBJ=$(patsubst %.c, %.o, $(SOURCE))

.SILENT:
.PHONY: all test clean help

all: $(PROG)

.c.o:
	@echo "compile $<"
        $(CC) $(CFLAGS) -c $<

$(PROG): $(SOURCE)
	@echo "linking $(PROG)"
	$(CC) $(CFLAGS) -O0 $(SOURCE) -o $(PROG)

clean:
	-rm -f $(PROG)
	

test: $(PROG)
	echo "./$(PROG) $(PROGARGS)
	./$(PROG) $(PROGARGS)
	
help:
	@echo "make options are all, test, clean, and help"
