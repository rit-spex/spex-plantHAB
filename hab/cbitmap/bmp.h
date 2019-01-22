/*
 * bmp.h
 *
 *  Created on: Dec 13, 2018
 *      Author: Bernard
 */

#ifndef BMP_H_
#define BMP_H_

#include <stdint.h>

#define INT8_T_MASK 0xFF
#define INT16_T_MASK 0xFFFF
#define INT32_t_MASK 0xFFFFFFFF

struct __d_bmp_header{
	uint16_t BM;/* ID */
	uint32_t size;/* size of bmp */
	uint16_t reserved0;/* reserved */
	uint16_t reserved1;/* reserved */
	uint32_t imgStart;/* index of start of pixel array */
	/* BEGIN DIB HEADER */
	uint32_t dibHeadSize;/* size of header */
	int32_t imgw;/* image width in px */
	int32_t imgh;/* image height in px */
	uint16_t numplanes;
	uint16_t pxSize_bits;/* size of pixels in bits */
	uint32_t BI_RGB;
	uint32_t rawImgSize;
	uint32_t imgPrintResH;
	uint32_t imgPrintResV;
	uint32_t paletteColorCt;
	uint32_t importantColors;
	/* END DIB HEADER */
};

/* bitmap pixel array iterator structure */
struct __d_bmp_px_iter{
	uint32_t idx;/* x index of the iterator */
	uint32_t idy;/* y index of the iterator */
	uint8_t step;/* step size of the iterator */
	uint8_t dirx;/* determines the direction of increasing x index values */
	uint8_t diry;/* determines the direction of increasing y index values */
};

/* bitmap data structure */
struct d_bmp{
	struct __d_bmp_header header;/* bitmap header structure */
	struct __d_bmp_px_iter _iter_;/* bitmap pixel iterator */
	uint8_t *pxData;/* byte array storing the bitmap pixel data */
};

struct d_bmp *bmp_decode(const char *filename);
void bmp_destroy(struct d_bmp *bitmap);

#endif /* BMP_H_ */
