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

#include "bmp.h"

int main(int argc, char *argv[]) {
	struct d_bmp *myBmp = NULL;
	char *filename = NULL;
	int rc;
	int option_index = 0;

	char *getoptOptions = "f:";
	struct option long_options[] = {
		{"file", required_argument, 0, 'f'},
		{0, 0, 0, 0}
	};
	opterr = 1;

	while ((rc = getopt_long_only(argc, argv, getoptOptions, long_options, &option_index)) != -1) {
		switch (rc) {
			case 'f':
				filename = optarg;
				break;
			default:
				printf ("Internal error: undefined option %0xX\n", rc);
				exit(1);
		}
	}
	if(NULL == filename) {
		fprintf(stderr, "Usage: dbmp -f <filename>\n");
		exit(1);
	}

	myBmp = bmp_decode(filename);
	bmp_destroy(myBmp);
	return 0;
}
