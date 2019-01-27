/*
 * main.c
 *
 *  Created on: Dec 13, 2018
 *      Author: Bernard
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <stdint.h>

#include "bmp.h"

#define BMP_IDENTIFIER 0x424d

static const char *lgcStr[] = {"\033[22;31mFALSE\033[0m", "\033[22;32mTRUE\033[0m"};

int main(int argc, char *argv[]) {
	struct d_bmp *myBmp = NULL;
	char *filename = NULL;
	int rc;
	int option_index = 0;
	uint8_t checkBitmapIntegrity = 0;
	uint32_t expectedDIBHeaderSize = 0;
	int32_t expectedWidth = 0, expectedHeight = 0;
	uint16_t expectedPxSize = 0;

	char *getoptOptions = "f:x:y:z:b:n";
	struct option long_options[] = {
		{"file", required_argument, 0, 'f'},
		{"expw", required_argument, 0, 'x'},
		{"exph", required_argument, 0, 'y'},
		{"pxsize", required_argument, 0, 'z'},
		{"dibsize", required_argument, 0, 'b'},
		{"chbmpint", no_argument, 0, 'n'},
		{0, 0, 0, 0}
	};
	opterr = 1;

	while ((rc = getopt_long_only(argc, argv, getoptOptions, long_options, &option_index)) != -1) {
		switch (rc) {
			case 'f':
				filename = optarg;
				break;
			case 'x':
				expectedWidth = (int32_t)strtol(optarg, &optarg, 10);
				break;
			case 'y':
				expectedHeight = (int32_t)strtol(optarg, &optarg, 10);
				break;
			case 'z':
				expectedPxSize = (uint16_t)strtol(optarg, &optarg, 10);
				break;
			case 'b':
				expectedDIBHeaderSize = (uint32_t)strtol(optarg, &optarg, 10);
				break;
			case 'n':
				checkBitmapIntegrity = 1;
				break;
			default:
				printf ("Internal error: undefined option %0xX\n", rc);
				exit(1);
		}
	}
	if(NULL == filename) {
		fprintf(stderr, "Usage: dbmp [OPTIONS...] -f <filename>\n");
		exit(1);
	}
	if(expectedWidth != expectedHeight) {
		if((expectedWidth == 0 || expectedHeight == 0)) {/* if either one is still 0 (they can't both be because they are not equal) */
			fprintf(stderr, "Either both or neither x (expw) and y (exph) must be specified\n");
			exit(1);
		}
	}
	
	myBmp = bmp_decode(filename);
	
	if((void *)0 == myBmp) {
		fprintf(stderr, "Could not open file for reading. Terminating...\n");
		exit(2);
	}

	if(checkBitmapIntegrity) {
		fprintf(stdout, "0x%x == BMP_ID == BM == 0x%x:\t%s\n", BMP_IDENTIFIER, myBmp->header.BM, lgcStr[myBmp->header.BM == BMP_IDENTIFIER]);
	}
	if(expectedDIBHeaderSize) {
		fprintf(stdout, "%u == dibsize  == dibHeadSize == %u:\t%s\n", expectedDIBHeaderSize, myBmp->header.dibHeadSize, lgcStr[myBmp->header.dibHeadSize == expectedDIBHeaderSize]);
	}
	if(expectedWidth && expectedHeight) {/* both are non-zero */
		fprintf(stdout, "%d == width == expw == %d:\t\t%s\n", expectedWidth, myBmp->header.imgw, lgcStr[myBmp->header.imgw == expectedWidth]);
		fprintf(stdout, "%d == height == exph == %d:\t\t%s\n", expectedHeight, myBmp->header.imgh, lgcStr[myBmp->header.imgh == expectedHeight]);
	}
	if(expectedPxSize) {
		fprintf(stdout, "%u == pxSize_bits == pxsize == %u:\t%s\n", expectedPxSize, myBmp->header.pxSize_bits, lgcStr[myBmp->header.pxSize_bits == expectedPxSize]);
	}
	
	bmp_destroy(myBmp);

	return 0;
}
