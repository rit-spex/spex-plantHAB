/*
 * bmp.c
 *
 *  Created on: Dec 13, 2018
 *      Author: Bernard
 */

#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Decodes the bitmap given by the filename
 * Takes 1 string parameter: the filename of the bitmap including path
 *
 * Returns a heap allocated pointer to the d_bmp structure containing the bitmap data
 */
struct d_bmp *bmp_decode(const char *filename) {
	FILE *bmpFile = NULL;
	struct d_bmp *decoded = NULL;
	uint8_t *fileBuffer = NULL;
	uint32_t temp = 0;

	bmpFile = fopen(filename, "rb");/* open bitmap file for reading */
	if(NULL == bmpFile) {/* if failed to open, print error and return null */
		fprintf(stderr, "%s %d Failed to open file %s for reading\n", __FILE__, __LINE__, filename);
		return (void *)0;
	}

	/* allocate memory to store bitmap */
	decoded = (struct d_bmp *)malloc(sizeof(struct d_bmp));

	if(NULL == decoded) {/* if memory failed to allocate, print error and exit */
		fprintf(stderr, "%s %d Failed to allocate memory for bitmap header.\n", __FILE__, __LINE__);
		exit(-99);// TODO handle gracefully
	}

	fseek(bmpFile, 0, SEEK_END);/* goto beginning of file */
	long jpglen = ftell(bmpFile);/* get length of the bitmap */
	printf("Read %ld bytes from file\n", jpglen);
	rewind(bmpFile);
	fileBuffer = (uint8_t *)malloc((jpglen + 1) * sizeof(uint8_t));/* allocate buffer to store file data */
	fread(fileBuffer, jpglen, 1, bmpFile);/* read file data into buffer */
	fclose(bmpFile);/* close the bitmap file */
	/* load these bytes in reverse order */
	temp = (uint32_t)(((uint32_t)fileBuffer[0] & INT8_T_MASK) << 8);
	temp += (uint32_t)fileBuffer[1] & INT8_T_MASK;
	decoded->header.BM = temp;
	temp = (uint32_t)((uint32_t)fileBuffer[2] << 24);
	temp += (fileBuffer[3] << 16);
	temp += (fileBuffer[4] << 8);
	temp += (fileBuffer[5]);
	decoded->header.size = temp;
	temp = (uint32_t)((uint32_t)fileBuffer[6] << 8);
	temp += (fileBuffer[7]);
	decoded->header.reserved0 = temp;
	temp = (uint32_t)((uint32_t)fileBuffer[8] << 8);
	temp += (fileBuffer[9]);
	decoded->header.reserved1 = temp;
	temp = (uint32_t)((uint32_t)fileBuffer[10] << 24);
	temp += (fileBuffer[11] << 16);
	temp += (fileBuffer[12] << 8);
	temp += (fileBuffer[13]);
	decoded->header.imgStart = temp;
	temp = (uint32_t)((uint32_t)fileBuffer[14] << 24);
	temp += (fileBuffer[15] << 16);
	temp += (fileBuffer[16] << 8);
	temp += (fileBuffer[17]);
	/*decoded->header.dibHeadSize = temp;
	temp = (uint32_t)((uint32_t)fileBuffer[18] << 24);
	temp += (fileBuffer[19] << 16);
	temp += (fileBuffer[20] << 8);
	temp += (fileBuffer[21]);
	decoded->header.imgw = temp;
	temp = (uint32_t)((uint32_t)fileBuffer[22] << 24);
	temp += (fileBuffer[23] << 16);
	temp += (fileBuffer[24] << 8);
	temp += (fileBuffer[25]);
	decoded->header.imgh = temp;*/
	//memcpy(&(decoded->header.BM), fileBuffer, 0x46);
	/* after the bitmap header is read, memcpy the rest of the bitmap from the file into the structure */
	memcpy(&(decoded->header.dibHeadSize), &(fileBuffer[14]), /*0x46*/40);
	return decoded;
}
