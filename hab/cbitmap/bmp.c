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

/*
 * Destroys and frees memory used by the given bitmap
 * Takes 1 parameter: a pointer to the bitmap to destroy
 *
 */
void bmp_destroy(struct d_bmp *bitmap) {
	free(bitmap->pxData);
	free(bitmap);
}

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
	uint32_t pxDataLen = 0;

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
	/* printf("Read %ld bytes from file\n", jpglen); */
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
	//memcpy(&(decoded->header.BM), fileBuffer, 58);

	memcpy(&(decoded->header.imgStart), &(fileBuffer[10]), 4);

	/*  Compute the size of the byte buffer needed to hold the pixel data
	    This is a bit easier said than done because pixel rows are always padded to be an integer multiple of 32 bits.

		Take the product of the height and width of the image to find the number of pixels in the image and multiply by the size of a pixel in bytes.
		This result gives us the size needed to hold just the image pixels. We need to add space for the padding too.
		To calculate the number of bytes needed for padding, divide the size of pixels in bits to get the size of pixels in bytes.
		Take the size of pixels in bytes and multiply by the width in pixels of the image to get the length of one pixel row in bytes.
		Take the remainder of the row size in bytes when dividing by 4. This tells us the number of extra bytes over the last multiple of 32 bits we have in the row.
		Invert that scale over the range 0-4 by subtracting the result from 4 bytes (32 bits).
		This result will either be 4, 3, 2, or 1. If the result if 3, 2, or 1, that is the number of bytes needed for padding the row. If the result if 4, 0 bytes are needed.
		We therefore take the remainder when dividing this  result by 4.
		If the result was 4, this evaluates to 0 bytes of padding needed. If the result was 3, 2, or 1, the modulo is an identity operation. */
	pxDataLen = decoded->header.imgh * decoded->header.imgw * (decoded->header.pxSize_bits / 8) + ((4 - (((decoded->header.pxSize_bits / 8) * decoded->header.imgw) % 4)) % 4);

	decoded->pxData = (uint8_t *)malloc(pxDataLen);
	if(NULL == decoded->pxData) {
		fprintf(stderr, "%s %d, Failed to allocate memory for pixel array\n", __FILE__, __LINE__);
	}
	
	memcpy(decoded->pxData, &(fileBuffer[decoded->header.imgStart]), pxDataLen);

	return decoded;
}
