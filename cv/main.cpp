//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   :
// Description :
//============================================================================

#define MAX_PATH_LEN_UNIX (4096)

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;
using namespace cv;

/* Shared Output File Name Buffer */
/* Output file names are inserted into this buffer after the output path (one at a time). The buffer is then passed to imwrite to save the images. */
/* We can safely do this because imwrite will block until the writing is completed. */
/* For now files are written in order from shortest name to longest. */
/* If files must be written in a specific order which violates this rule, the buffer should be zeroed before being rewritten by a shorter name. */
char sofnb[MAX_PATH_LEN_UNIX];

int main(int argc, char **argv) {
    /* length of output path. this is used to determine where to start writing the output filename into the buffer. */
    size_t outputPathLen = 0;
#ifdef NPI
    /* This never gets built on the HAB. */
    /* ifdef NPI blocks compile on a computer being used for testing specific data samples. */
    
    /* Data is provided to the program through the '-f' flag specifying an image filename. */
    char *filename = NULL;
    /* An alternative directory for storing output can optionally be specified (see option parsing) */
    char *outputPath = "./output";
    int rc;
    
    /* Image formats required signed dimensions to determine direction of image rendering */
    /* I think that's stupid */
    int32_t imgw = 1920, imgh = 1080; /* Temporarily unused */
    uint8_t nopv = 0;

    opterr = 1;

    /* Options are -f, -w, -h, -n, -o */
    /* These options do not affect operation of the program on the HAB. */
    /* Option parsing only compiles on test builds. */
    while ((rc = getopt(argc, argv, "f:w:h:no:")) != EOF) {
        switch (rc) {
            /* input filename (required) (required string arg) */
            case 'f':
                filename = optarg;
                break;
            /* image capture width (unused) */
            case 'w':
                imgw = (int32_t)strtol(optarg, &optarg, 10);
                break;
            /* image capture height (unused) */
            case 'h':
                imgh = (int32_t)strtol(optarg, &optarg, 10);
                break;
            /* no-preview - specify this flag to save the output silently and exit immediately */
            case 'n':
                nopv = 1;
                break;
            /* specify a non-default path to save output images (optional) (required string arg) */
            case 'o':
                outputPath = optarg;
                break;
            default:
                printf ("Internal error: undefined option 0x%0x\n", rc);
                exit(1);
        }
    }
    if(NULL == filename) {
        fprintf(stderr, "Usage: habcv-test [OPTIONS...] -f <filename>\n");
        exit(1);
    }
    if(imgw != imgh) {
        if((imgw == 0 || imgh == 0)) {/* if either one is still 0 (they can't both be because they are not equal) */
            fprintf(stderr, "Either both or neither w (width) and h (height) must be specified\n");
            exit(1);
        }
    }
    Mat imageBGR = imread(filename, CV_LOAD_IMAGE_COLOR);
#else
    /* This gets built on the HAB. To test from the HAB we want to capture an image and then process it */
    VideoCapture cap;
    if(!cap.open(0)) {
        fprintf(stderr, "Error opening camera\n");
        exit(-1);
    }
    Mat imageBGR;
    cap >> imageBGR;
    if(imageBGR.empty()) {
        fprintf(stderr, "Reached unexpected end of video stream\n");
        exit(-2);
    }
#endif
    memset(&sofnb[0], 0, MAX_PATH_LEN_UNIX);/* Clear sofnb */
    outputPathLen = strlen(outputPath);/* Calculate length of path to determine insertion index */
    memcpy(&sofnb[0], outputPath, outputPathLen);/* Copy the output path into the output filename buffer */
    vector<Mat> chBGR;
    split(imageBGR, chBGR);
    Mat nm = chBGR[2] - chBGR[0];
    Mat dn = chBGR[2] + chBGR[0];
    Mat ndvi;
    cv::divide(dn, nm, ndvi, 1., 5);
    normalize(ndvi, ndvi, 0, 255, NORM_MINMAX, CV_8UC1);
    Mat cm2_ndvi;
    applyColorMap(ndvi, cm2_ndvi, COLORMAP_HSV);
    Mat cm2_ndvi_gray;
    cvtColor(cm2_ndvi, cm2_ndvi_gray, CV_BGR2GRAY);
    memcpy(&sofnb[outputPathLen], "/mscap.png", strlen("/mscap.png"));
    fprintf(stdout, "%s\n", &sofnb[0]);
    imwrite(&sofnb[0], imageBGR);
    memcpy(&sofnb[outputPathLen], "/ndvi-calc3_1.png", strlen("/ndvi-calc3_1.png"));
    fprintf(stdout, "%s\n", &sofnb[0]);
    imwrite(&sofnb[0], cm2_ndvi);
    memcpy(&sofnb[outputPathLen], "/ndvi-calc3_2.png", strlen("/ndvi-calc3_2.png"));
    fprintf(stdout, "%s\n", &sofnb[0]);
    imwrite(&sofnb[0], cm2_ndvi_gray);
#ifdef NPI
    /* If we aren't on the HAB, display images before exiting (unless forbidden by the '-n' flag) */
    if(!nopv) {
        imshow("MSCAP", imageBGR);
        imshow("NDVI", cm2_ndvi);
        imshow("NDVI_imap", cm2_ndvi_gray);
        waitKey(0);
    }
#endif
    return 0;
}
