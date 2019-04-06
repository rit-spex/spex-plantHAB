CPPC=g++
CPPFLAGS=-Wall -pedantic -O1
LFLAGS=-lopencv_core -lopencv_imgproc -lopencv_legacy -lopencv_highgui -lopencv_features2d -lopencv_contrib
DFLAGS=-DNPI
PROG=habcv-test
PROGARGS=-f $(TESTPICTURE)
NOPV=-n
SOURCE=cv/main.cpp
OUTPUTPATH=output
OUTPUTFILE=$(PROG).log
BUILDLOG=$(PROG)-build.log

TESTPICTURE=ndvi-test.png

.SILENT:
.PHONY: all test clean help testfile

all: $(PROG)


$(PROG): $(SOURCE)
	@echo "Building $(PROG)"
	$(CPPC) $(CPPFLAGS) $(SOURCE) -o $(PROG) $(LFLAGS) $(DFLAGS) >$(BUILDLOG) 2>&1
	echo "Build log can be viewed in $(BUILDLOG)"

clean:
	-rm -f $(PROG)
	-rm -f $(OUTPUTFILE)
	-rm -f $(BUILDLOG)
	-rm -rf $(OUTPUTPATH)

test: $(PROG)
	-mkdir -p $(OUTPUTPATH)
	echo "./$(PROG) >$(OUTPUTFILE) 2>&1"
	-./$(PROG) >$(OUTPUTFILE) 2>&1
	echo "Test log can be viewed in $(OUTPUTFILE)"

testfile: $(PROG)
	-mkdir -p $(OUTPUTPATH)
	echo "./$(PROG) $(PROGARGS) $(NOPV) >$(OUTPUTFILE) 2>&1"
	-./$(PROG) $(PROGARGS) $(NOPV) >$(OUTPUTFILE) 2>&1
	echo "Test log can be viewed in $(OUTPUTFILE)"
	
help:
	@echo "make options are all, test, testfile, clean, and help\n"
	@echo "to specify picture for make testfile, use 'make testfile TESTPICTURE=<picture>'"
	@echo "to change output directory for make test(file), use 'make test(file) OUTPUTPATH=<output directory>'"
	@echo "to enable automatic preview of make testfile, use 'make testfile NOPV='"
	@echo "to compile version to run on the PI, use 'make all DFLAGS='"
