/*
 * main.c
 *
 *  Created on: Dec 13, 2018
 *      Author: Bernard
 */
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[]) {
	struct d_bmp *myBmp = NULL;
	myBmp = bmp_decode("C:/Users/Bernard/Documents/bmp_format1.bmp");
	return 0;
}
