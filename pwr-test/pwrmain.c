//============================================================================
// Name        : pwrmain.c
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

#include <sys/time.h>

using namespace std;
using namespace cv;

/* Shared Output File Name Buffer */
/* Output file names are inserted into this buffer after the output path (one at a time). The buffer is then passed to imwrite to save the images. */
/* We can safely do this because imwrite will block until the writing is completed. */
/* For now files are written in order from shortest name to longest. */
/* If files must be written in a specific order which violates this rule, the buffer should be zeroed before being rewritten by a shorter name. */
char sofnb[MAX_PATH_LEN_UNIX] = "output";

int main(int argc, char **argv) {
    
    unsigned long now, last = 0, start = 0;
    struct timeval tp;
    memset(&sofnb[0], 0, MAX_PATH_LEN_UNIX);
    do {
        gettimeofday(&tp, NULL);
        now = tp.tv_sec * 1000 + tp.tv_usec / 1000;
        if(now - last >= 30000) {
            vector<Mat> chBGR;
            Mat nm;
            Mat dn;
            
            Mat ndvi;
            Mat cm2_ndvi;
            Mat cm2_ndvi_gray;
            
            Mat imageBGR;
            start = now;
            snprintf(&sofnb[0], MAX_PATH_LEN_UNIX, "raspistill -n -e png -w 1920 -h 1080 -o %ld", now);
            system(&sofnb[0]);
            usleep(6000000);
            memset(&sofnb[0], 0, MAX_PATH_LEN_UNIX);
            snprintf(&sofnb[0], MAX_PATH_LEN_UNIX, "output/mscap_%ld.png", now);
            imageBGR = imread(&sofnb[0]);
            
            memset(&sofnb[0], 0, MAX_PATH_LEN_UNIX);
            
            split(imageBGR, chBGR);
            nm = chBGR[2] - chBGR[0];
            dn = chBGR[2] + chBGR[0];
            
            cv::divide(dn, nm, ndvi, 1., 5);
            normalize(ndvi, ndvi, 0, 255, NORM_MINMAX, CV_8UC1);
            
            applyColorMap(ndvi, cm2_ndvi, COLORMAP_HSV);
            cvtColor(cm2_ndvi, cm2_ndvi_gray, CV_BGR2GRAY);
            
            snprintf(&sofnb[0], MAX_PATH_LEN_UNIX, "output/ndvi-calc3_1_%ld.png", now);
            imwrite(&sofnb[0], cm2_ndvi);
            memset(&sofnb[0], 0, MAX_PATH_LEN_UNIX);
            snprintf(&sofnb[0], MAX_PATH_LEN_UNIX, "output/ndvi-calc3_2_%ld.png", now);
            imwrite(&sofnb[0], cm2_ndvi_gray);
            memset(&sofnb[0], 0, MAX_PATH_LEN_UNIX);
            
            gettimeofday(&tp, NULL);
            now = tp.tv_sec * 1000 + tp.tv_usec / 1000;
            fprintf(stdout, "Processing took %ld ms\n", now - start);
            last = now;
        }
        else {
            usleep(200000);// sleep 200 ms
        }
    }while(1);
    
    
    return 0;
}
