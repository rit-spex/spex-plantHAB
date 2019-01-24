/*
 * main.c
 *
 *  Created on: Dec 13, 2018
 *      Author: Bernard
 */

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "bmp.h"

int main(int argc, char *argv[]) {
	struct d_bmp *myBmp = NULL;
	char *filename = NULL;
	int rc;
	int option_index = 0;
	int32_t expectedWidth = 0, expectedHeight = 0;

	char *getoptOptions = "f:x:y:";
	struct option long_options[] = {
		{"file", required_argument, 0, 'f'},
		{"expw", required_argument, 0, 'x'},
		{"exph", required_argument, 0, 'y'},
		{0, 0, 0, 0}
	};
	opterr = 1;

	while ((rc = getopt_long_only(argc, argv, getoptOptions, long_options, &option_index)) != -1) {
		switch (rc) {
			case 'f':
				filename = optarg;
				break;
			case 'x':
				expectedWidth = (int)strtol(optarg, &optarg, 10);
				break;
			case 'y':
				expectedHeight = (int)strtol(optarg, &optarg, 10);
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
		if(!(expectedWidth & expectedHeight)) {/* if either one is still 0 (they can't both be because they are not equal) */
			fprintf(stderr, "Either both or neither x (expw) and y (exph) must be specified\n");
			exit(1);
		}
	}

	myBmp = bmp_decode(filename);

	if(expectedWidth & expectedHeight) {/* both are non-zero */
		char *lgcStr[] = {ANSI_COLOR_RED"FALSE"ANSI_COLOR_RESET, ANSI_COLOR_GREEN"TRUE"ANSI_COLOR_RESET};
		fprintf(stdout, "%d == width == expw == %d:\t%s\n", expectedWidth, myBmp->header.imgw, lgcStr[myBmp->header.imgw == expectedWidth]);
		fprintf(stdout, "%d == height == exph == %d:\t%s\n", expectedHeight, myBmp->header.imgh, lgcStr[myBmp->header.imgh == expectedHeight]);
	}

	bmp_destroy(myBmp);

	return 0;
}
