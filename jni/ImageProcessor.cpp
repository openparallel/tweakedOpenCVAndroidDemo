//
//  ImageProcessor.cpp
//  FaceIt
//
//  Created by Beau Johnston on 1/06/12.
//  Copyright (c) 2012 OpenParallel.com all rights reserved.
//

#include "ImageProcessor.h"
#include "Harness.hpp"
#include <arm_neon.h>

/*
 * Private Utility functions
 */

char* stringCat(char*s, char*s1){
    char* target = new char[strlen(s) + strlen(s1) + 1];
    strcpy(target, s);
    strcat(target, s1);
    return target;
}

void Log(char* message, bool errorFlag){
    //append a newline to the end of the log string
    message = stringCat(message, (char*)"\n");
    
#ifdef ANDROID
    //android log
    if (errorFlag) {
        LOGE(message);
    }
    else{
        LOGV(message);
    }
#endif
    
#ifndef ANDROID
    //regular log
    if (errorFlag) {
        printf("%s", message);
        exit(EXIT_FAILURE);
    }
    else {
        printf("%s", message);
    }
#endif
    return;
}

/*
//Divide by 3 in NEON
uint8x8_t vdiv3_u8(uint8x8_t in){
    //widen in
    uint16x8_t tmp = vmovl_u8(in);
    
    //q = (n >> 2) + (n >> 4)   ~ q = n * 0.0101 (approx.)
    uint16x8_t quo = vshrq_n_u16(tmp, 2);
    quo = vaddq_u16(quo, vshrq_n_u16(tmp, 4));
    
    //q = q + (q >> 4)          ~ q = n * 0.01010101
    quo = vaddq_u16(quo, vshrq_n_u16(quo, 4));
    //q = q + (q >> 8)          ~ q = n * 0.0101010101010101
    quo = vaddq_u16(quo, vshrq_n_u16(quo, 8));
    
    // r = n - q*3
    uint16x8_t rem = vsubq_u16(tmp,vmulq_n_u16(quo,3));
    
    // return q + (6*r >> 4)
    tmp = vaddq_u16(quo, vshrq_n_u16(vmulq_n_u16(rem,6),4));
    
    //shorten
    in  = vmovn_u16(tmp);
    return in;
}
*/

/*
const int NUMBER_OF_THREADS = 4;

struct thread_data_neon
{
    int	thread_id;
    
    int image_size;
    uint8_t *r;
    uint8_t *g;
    uint8_t *b;
};

struct thread_data_neon thread_data_array_neon[NUMBER_OF_THREADS];

void *doThreadGruntworkNeon(void*threadarg){
    
    struct thread_data_neon *my_data;
    
    my_data = (struct thread_data_neon *) threadarg;
    
    //but do work on your quarter of the image
    int segment = my_data->image_size/NUMBER_OF_THREADS;
    int startPoint = my_data->thread_id * segment;
    int stopPoint = (my_data->thread_id+1) * segment;
    
    int n = stopPoint;
    
    uint8x8_t rfac = vdup_n_u8 (40);
    uint8x8_t gfac = vdup_n_u8 (20);
    uint8x8_t bfac = vdup_n_u8 (20);
    
    uint8x8_t imin = vdup_n_u8 (0);
    uint8x8_t imax = vdup_n_u8 (255);
    
    uint8_t *rptr = my_data->r+startPoint;
    uint8_t *bptr = my_data->b+startPoint;
    uint8_t *gptr = my_data->g+startPoint;
    
    for (int j=startPoint; j<stopPoint; j+=8){
        //get values for this block
        uint8x8_t red = vld1_u8(rptr);
        uint8x8_t grn = vld1_u8(gptr);
        uint8x8_t blu = vld1_u8(bptr);
        //intensity vector
        uint8x8_t ins;
        
        //average the channel intensity
        red = vdiv3_u8(red);
        grn = vdiv3_u8(grn);
        blu = vdiv3_u8(blu);
        
        //add all channels together
        ins = vadd_u8(blu,vadd_u8(red,grn));
        
        //add sepia weights
        blu = vqsub_u8(ins, bfac);
        grn = vqadd_u8(ins, gfac);
        red = vqadd_u8(ins, rfac);
        
        //do boundary checks
        blu = vmax_u8(blu, imin);
        red = vmin_u8(red, imax);
        grn = vmin_u8(grn, imax);
        
        //set values for this block
        vst1_u8(rptr, red);
        vst1_u8(gptr, grn);
        vst1_u8(bptr, blu);
        
        rptr+=8;
        bptr+=8;
        gptr+=8;
    }
    
    
    pthread_exit(NULL);
}

void applySepiaToneWithDirectPixelManipulationsAndNeonSSEAndPthreadsForSMP(IplImage* target){
    
    //allocate vectors
    uint8_t *b = new uint8_t[target->height*target->width];
    uint8_t *g = new uint8_t[target->height*target->width];
    uint8_t *r = new uint8_t[target->height*target->width];
    
    
    //collect image pixels into vectors
    int i=0; //pixel Position
    for( int y=0; y<target->height; y++ ){
        uchar* ptr = (uchar*) (
                               target->imageData + y * target->widthStep
                               );
        
        for( int x=0; x<target->width; x++ ) {
            b[i] = ptr[3*x+0];
            g[i] = ptr[3*x+1];
            r[i] = ptr[3*x+2];
            
            i++;
        }
    }
    
    
#ifdef TIMEIT
    //on the clock
    clock_t begin, end;
    double time_spent;
    //gettimeofday()
    begin = clock();
#endif
    
    
    //partition the toning into 4 threads
    pthread_t threads[NUMBER_OF_THREADS];
    int rc;
    for (int t = 0; t < NUMBER_OF_THREADS; t ++) {
        //load up resources for this thread
        thread_data_array_neon[t].thread_id = t;
        thread_data_array_neon[t].r = r;
        thread_data_array_neon[t].g = g;
        thread_data_array_neon[t].b = b;
        thread_data_array_neon[t].image_size = target->width*target->height;
        
        rc = pthread_create(&threads[t], NULL, doThreadGruntworkNeon, (void *)
                            &thread_data_array_neon[t]);
        if (rc) {
            LOGE("ERROR -> pthread_create() the thread was not born");
        }
    }
    
    //syncronise
    if(pthread_join(*threads,NULL)){
        LOGE("ERROR -> pthread_join() the threads weren't stuck back together");
    }
    
    
    
#ifdef TIMEIT
    //off the clock
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    //print the time taken
    char my_string[22];
    sprintf(my_string,"%18.4f",time_spent);
    LOGE("****************************************");
    LOGE("Time taken to compute Sepia Tone values:");
    LOGE(my_string);
    LOGE("****************************************");
    //saving global timeStamp to return
    timeStamp = time_spent;
    
#endif
    
    //write image pixels back from vectors
    i=0; //pixel Position
    for( int y=0; y<target->height; y++ ){
        uchar* ptr = (uchar*) (
                               target->imageData + y * target->widthStep
                               );
        
        for( int x=0; x<target->width; x++ ) {
            ptr[3*x+0] = b[i];
            ptr[3*x+1] = g[i];
            ptr[3*x+2] = r[i];
            
            i++;
        }
    }
    
    delete b;
    delete g;
    delete r;
    
}


void applySepiaToneWithDirectPixelManipulationsAndNeonSSE(IplImage* target){
    
    //allocate vectors
    uint8_t *b = new uint8_t[target->height*target->width];
    uint8_t *g = new uint8_t[target->height*target->width];
    uint8_t *r = new uint8_t[target->height*target->width];
    
    uint8_t* rptr = r;
    uint8_t* bptr = b;
    uint8_t* gptr = g;
    
    //collect image pixels into vectors
    int i=0; //pixel Position
    for( int y=0; y<target->height; y++ ){
        uchar* ptr = (uchar*) (
                               target->imageData + y * target->widthStep
                               );
        
        for( int x=0; x<target->width; x++ ) {
            b[i] = ptr[3*x+0];
            g[i] = ptr[3*x+1];
            r[i] = ptr[3*x+2];
            
            i++;
        }
    }
    
    
#ifdef TIMEIT
    //on the clock
    clock_t begin, end;
    double time_spent;
    //gettimeofday()
    begin = clock();
#endif
    
    int n = target->width*target->height;
    
    uint8x8_t rfac = vdup_n_u8 (40);
    uint8x8_t gfac = vdup_n_u8 (20);
    uint8x8_t bfac = vdup_n_u8 (20);
    
    uint8x8_t imin = vdup_n_u8 (0);
    uint8x8_t imax = vdup_n_u8 (255);
    
    n/=8;
    
    for (int j=0; j<n; j++){
        //get values for this block
        uint8x8_t red = vld1_u8(rptr);
        uint8x8_t grn = vld1_u8(gptr);
        uint8x8_t blu = vld1_u8(bptr);
        //intensity vector
        uint8x8_t ins;
        
        //average the channel intensity
        red = vdiv3_u8(red);
        grn = vdiv3_u8(grn);
        blu = vdiv3_u8(blu);
        
        //add all channels together
        ins = vadd_u8(blu,vadd_u8(red,grn));
        
        //add sepia weights
        blu = vqsub_u8(ins, bfac);
        grn = vqadd_u8(ins, gfac);
        red = vqadd_u8(ins, rfac);
        
        //do boundary checks
        blu = vmax_u8(blu, imin);
        red = vmin_u8(red, imax);
        grn = vmin_u8(grn, imax);
        
        //set values for this block
        vst1_u8(rptr, red);
        vst1_u8(gptr, grn);
        vst1_u8(bptr, blu);
        
        rptr+=8;
        bptr+=8;
        gptr+=8;
    }
    
#ifdef TIMEIT
    //off the clock
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    //print the time taken
    char my_string[22];
    sprintf(my_string,"%18.4f",time_spent);
    LOGE("****************************************");
    LOGE("Time taken to compute Sepia Tone values:");
    LOGE(my_string);
    LOGE("****************************************");
    
    //saving global timeStamp to return
    timeStamp = time_spent;
    
#endif
    
    //write image pixels back from vectors
    i=0; //pixel Position
    for( int y=0; y<target->height; y++ ){
        uchar* ptr = (uchar*) (
                               target->imageData + y * target->widthStep
                               );
        
        for( int x=0; x<target->width; x++ ) {
            ptr[3*x+0] = b[i];
            ptr[3*x+1] = g[i];
            ptr[3*x+2] = r[i];
            
            i++;
        }
    }
    
    delete b;
    delete g;
    delete r;
    
}

struct thread_data
{
    int	thread_id;
    
    int image_size;
    int *r;
    int *g;
    int *b;
};

struct thread_data thread_data_array[NUMBER_OF_THREADS];



void *doThreadGruntwork(void*threadarg){
    
    struct thread_data *my_data;
    
    my_data = (struct thread_data *) threadarg;
    
    //but do work on your quarter of the image
    int segment = my_data->image_size/NUMBER_OF_THREADS;
    int startPoint = my_data->thread_id * segment;
    int stopPoint = (my_data->thread_id+1) * segment;
    */
	
    /*
     *before
     *
     //do sepia processing in
     for(int i = startPoint; i < stopPoint; i ++){
     //store the greyscale value into the blue vector
     my_data->b[i] = round((my_data->b[i] + my_data->g[i] + my_data->r[i])/3);
     //set the other 2 vectors with the same greyscale value... da-doi
     my_data->g[i] = my_data->b[i];
     my_data->r[i] = my_data->b[i];
     
     //scale to give it a reddish-brown (sepia) tinge
     my_data->b[i] -= 20;
     my_data->g[i] += 20;
     my_data->r[i] += 40;
     
     //ensure that everything is in bounds (this is done implicitly in OpenCV)
     if (my_data->b[i] < 0) {
     my_data->b[i] = 0;
     }
     
     if (my_data->g[i] > 255) {
     my_data->g[i] = 255;
     }
     
     if (my_data->r[i] > 255) {
     my_data->r[i] = 255;
     }
     }
     *
     *after
     */
    
	 /*
    for(int i = startPoint; i < stopPoint; i ++){
        //you would think it is faster to *0.3 but nope!
        my_data->b[i] = (int)((my_data->b[i] + my_data->g[i] + my_data->r[i])/3)-20;
        
        my_data->g[i] = my_data->b[i]+40;
        my_data->r[i] = my_data->b[i]+60;
        
        my_data->b[i] = MAX(my_data->b[i],0);
        my_data->g[i] = MIN(my_data->g[i],255);
        my_data->r[i] = MIN(my_data->r[i],255);
    }
    
    pthread_exit(NULL);
}




void applySepiaToneWithDirectPixelManipulationsAndPthreadsForSMP(IplImage* target){
    //allocate vectors
    int *b = new int[target->height*target->width];
    int *g = new int[target->height*target->width];
    int *r = new int[target->height*target->width];
    
    //collect image pixels into vectors
    int i=0; //pixel Position
    for( int y=0; y<target->height; y++ ){
        uchar* ptr = (uchar*) (
                               target->imageData + y * target->widthStep
                               );
        
        for( int x=0; x<target->width; x++ ) {
            b[i] = ptr[3*x+0];
            g[i] = ptr[3*x+1];
            r[i] = ptr[3*x+2];
            
            i++;
        }
    }
    
#ifdef TIMEIT
    //on the clock
    clock_t begin, end;
    double time_spent;
    
    begin = clock();
#endif
    
    //partition the toning into 4 threads
    pthread_t threads[NUMBER_OF_THREADS];
    int rc;
    for (int t = 0; t < NUMBER_OF_THREADS; t ++) {
        //load up resources for this thread
        thread_data_array[t].thread_id = t;
        thread_data_array[t].r = r;
        thread_data_array[t].g = g;
        thread_data_array[t].b = b;
        thread_data_array[t].image_size = target->width*target->height;
        
        rc = pthread_create(&threads[t], NULL, doThreadGruntwork, (void *)
                            &thread_data_array[t]);
        if (rc) {
            LOGE("ERROR -> pthread_create() the thread was not born");
        }
    }
    
    //syncronise
    if(pthread_join(*threads,NULL)){
        LOGE("ERROR -> pthread_join() the threads weren't stuck back together");
    }
    
#ifdef TIMEIT
    //off the clock
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    //print the time taken
    char my_string[22];
    sprintf(my_string,"%18.4f",time_spent);
    LOGE("****************************************");
    LOGE("Time taken to compute Sepia Tone values:");
    LOGE(my_string);
    LOGE("****************************************");
    
    //saving global timeStamp to return
    timeStamp = time_spent;
    
#endif
    
    //write image pixels back from vectors
    i=0; //pixel Position
    for( int y=0; y<target->height; y++ ){
        uchar* ptr = (uchar*) (
                               target->imageData + y * target->widthStep
                               );
        
        for( int x=0; x<target->width; x++ ) {
            ptr[3*x+0] = b[i];
            ptr[3*x+1] = g[i];
            ptr[3*x+2] = r[i];
            
            i++;
        }
    }
}

void applySepiaToneWithDirectPixelManipulations(IplImage* target){
    
    //allocate vectors
    int *b = new int[target->height*target->width];
    int *g = new int[target->height*target->width];
    int *r = new int[target->height*target->width];
    
    //collect image pixels into vectors
    int i=0; //pixel Position
    for( int y=0; y<target->height; y++ ){
        uchar* ptr = (uchar*) (
                               target->imageData + y * target->widthStep
                               );
        
        for( int x=0; x<target->width; x++ ) {
            b[i] = ptr[3*x+0];
            g[i] = ptr[3*x+1];
            r[i] = ptr[3*x+2];
            
            i++;
        }
    }
    
    
#ifdef TIMEIT
    //on the clock
    clock_t begin, end;
    double time_spent;
    //gettimeofday()
    begin = clock();
#endif
    
    //do sepia processing
    
    for(int i = 0; i < target->width*target->height; i ++){
	*/
		
        /*
         *before:
         //store the greyscale value into the blue vector
         b[i] = (int)((b[i] + g[i] + r[i])/3);
         //set the other 2 vectors with the same greyscale value... da-doi
         g[i] = b[i]
         r[i] = b[i]
         
         //scale to give it a reddish-brown (sepia) tinge
         b[i] -= 20;
         g[i] += 20;
         r[i] += 40;
         
         //ensure that everything is in bounds (this is done implicitly in OpenCV)
         if (b[i] < 0) {
         b[i] = 0;
         }
         
         if (g[i] > 255) {
         g[i] = 255;
         }
         
         if (r[i] > 255) {
         r[i] = 255;
         }
         
         *
         *after:
         */
        
        //you would think it is faster to *0.3 but nope!
		 /*
		b[i] = (int)((b[i] + g[i] + r[i])/3)-20;
        
        g[i] = b[i]+40;
        r[i] = b[i]+60;
        
        b[i] = MAX(b[i],0);
        g[i] = MIN(g[i],255);
        r[i] = MIN(r[i],255);
        
    }
    
#ifdef TIMEIT
    //off the clock
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    //print the time taken
    char my_string[22];
    sprintf(my_string,"%18.4f",time_spent);
    LOGE("****************************************");
    LOGE("Time taken to compute Sepia Tone values:");
    LOGE(my_string);
    LOGE("****************************************");
    
    //saving global timeStamp to return
    timeStamp = time_spent;
    
#endif
    
    //write image pixels back from vectors
    i=0; //pixel Position
    for( int y=0; y<target->height; y++ ){
        uchar* ptr = (uchar*) (
                               target->imageData + y * target->widthStep
                               );
        
        for( int x=0; x<target->width; x++ ) {
            ptr[3*x+0] = b[i];
            ptr[3*x+1] = g[i];
            ptr[3*x+2] = r[i];
            
            i++;
        }
    }
}


void applySepiaTone(IplImage* target){
    
#ifdef TIMEIT
    //on the clock
    clock_t begin, end;
    double time_spent;
    
    begin = clock();
#endif
    
    for (int ix=0; ix<target->width; ix++) {
        for (int iy=0; iy<target->height; iy++) {
            
            //extract each pixel
            int r = cvGet2D(target, iy, ix).val[2];
            int g = cvGet2D(target, iy, ix).val[1];
            int b = cvGet2D(target, iy, ix).val[0];
            
            //generate a grayscale pixel
            int p = round(((r+g+b)/3));
            
            //to generate sepia tone colouration, use the colourspace
            //rgb (+40,+20,-20)
            //CvScalar expects bgr colour so:
            CvScalar bgr = cvScalar(p-20, p+20, p+40);
            
            cvSet2D(target, iy, ix, bgr);
        }
    }
    
#ifdef TIMEIT
    //off the clock
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    //print the time taken
    char my_string[22];
    sprintf(my_string,"%18.4f",time_spent);
    LOGE("****************************************");
    LOGE("Time taken to compute Sepia Tone values:");
    LOGE(my_string);
    LOGE("****************************************");
    
    //saving global timeStamp to return
    timeStamp = time_spent;
#endif
}

void overlayImage(IplImage* target, IplImage* source, int x, int y) {
    
    for (int ix=0; ix<source->width; ix++) {
        for (int iy=0; iy<source->height; iy++) {
            int r = cvGet2D(source, iy, ix).val[2];
            int g = cvGet2D(source, iy, ix).val[1];
            int b = cvGet2D(source, iy, ix).val[0];
            CvScalar bgr = cvScalar(b, g, r);
            cvSet2D(target, iy+y, ix+x, bgr);
        }
    }
}

*/

/*
 * End of Private utility functions
 */








/*
 * Public feature detection functions
 */
	 
	 /*
IplImage* drawRectangleOnImage(CvRect featureRect, IplImage*inputImage){
    
    cvRectangle(inputImage, cvPoint(featureRect.x, featureRect.y), cvPoint(featureRect.x + featureRect.width, featureRect.y + featureRect.height), cvScalar(0, 255, 255, 255), 3, 1, 0);
    
    return inputImage;
}

IplImage* drawRectangleOnImageWithColour(CvRect featureRect, IplImage*inputImage,CvScalar colour){
    
    cvRectangle(inputImage, cvPoint(featureRect.x, featureRect.y), cvPoint(featureRect.x + featureRect.width, featureRect.y + featureRect.height), colour, 3, 1, 0);
    
    return inputImage;
}

IplImage* drawRectangleOnImageWithColourAndFilled(CvRect featureRect, IplImage*inputImage,CvScalar colour){
    
    cvRectangle(inputImage, cvPoint(featureRect.x, featureRect.y), cvPoint(featureRect.x + featureRect.width, featureRect.y + featureRect.height), colour, CV_FILLED, 1, 0);
    
    return inputImage;
}
	*/
#pragma mark start of NeonvsSSE tests
/*
    Start of ARM NEON vs Intel SSE Benchmarks
*/

double oldTime, newTime, theTime;
int clockTime;

#include <time.h>


void tick(){
    oldTime = (double)getTickCount();
    clockTime = clock();
}

void tock(){
    newTime = (double)getTickCount();
    
    theTime = (newTime-oldTime)/getTickFrequency();
    
    //the execution time in seconds
    /*char my_string[22];
    sprintf(my_string,"%18.4f",theTime);
    LOGE(my_string);
    
    
    sprintf(my_string,"%18.4f",((float)(clock()-clockTime))/CLOCKS_PER_SEC);
    LOGE(my_string);
    */
    
    //difference in clock ticks
//    sprintf(my_string,"%18.4f",(newTime-oldTime));
//    LOGE(my_string);

}

void resizeTests(){
    Mat image = Mat(m_sourceImage, true);
    
    Mat resizedImage;
    
    tick();
    
    resize(image, resizedImage,Size(image.cols/3,image.rows/3));
    resize(resizedImage, image, Size(resizedImage.cols*3,resizedImage.rows*3));

    tock();
    
    //write it back to IplImage
    for (int y = 0; y < image.rows; y++) {
        uchar* imagedata = image.ptr<uchar>(y);
        uchar* ptr = (uchar*) (m_sourceImage->imageData + y * m_sourceImage->widthStep);
        
        for (int x = 0; x < image.cols * 3; x+=3) {
            uchar b = imagedata[x];
            uchar g = imagedata[x+1];
            uchar r = imagedata[x+2];
            
            
            
            ptr[x]   = b;
            ptr[x+1] = g;
            ptr[x+2] = r;
        }
        
    }

    
}

void edgeDetectionTests(){
    //convert the image to opencv2 Mat format
    Mat image = Mat(m_sourceImage, true);
    Mat sobel, sobelX, sobelY;
    
    tick();
    
    // Compute norm of Sobel
    Sobel(image,sobelX,CV_16S,1,0);
    threshold(sobelX,sobelX,200,255,THRESH_BINARY);
    
    Sobel(image,sobelY,CV_16S,0,1);
    threshold(sobelY,sobelY,200,255,THRESH_BINARY);
    
    tock();
    //compute the L1 norm
    sobel= abs(sobelX)+abs(sobelY);
    
    // Find Sobel max value
    double sobmin, sobmax;
    minMaxLoc(sobel,&sobmin,&sobmax);
    // Conversion to 8-bit image
    // sobelImage = -alpha*sobel + 255
    
    sobel.convertTo(image,CV_8U,-255./sobmax,255);
    
    //write it back to IplImage
    for (int y = 0; y < image.rows; y++) {
        uchar* imagedata = image.ptr<uchar>(y);
        uchar* ptr = (uchar*) (m_sourceImage->imageData + y * m_sourceImage->widthStep);
        
        for (int x = 0; x < image.cols * 3; x+=3) {
            uchar b = imagedata[x];
            uchar g = imagedata[x+1];
            uchar r = imagedata[x+2];
            
            
            
            ptr[x]   = b;
            ptr[x+1] = g;
            ptr[x+2] = r;
        }
        
    }

}

void sobelTests(){
    
    //convert the image to opencv2 Mat format
    Mat image = Mat(m_sourceImage, true);
    Mat sobel, sobelX, sobelY;
    
    tick();
    
    // Compute norm of Sobel
    Sobel(image,sobelX,CV_16S,1,0);
    Sobel(image,sobelY,CV_16S,0,1);
    
    tock();
    //compute the L1 norm
    sobel= abs(sobelX)+abs(sobelY);

    // Find Sobel max value
    double sobmin, sobmax;
    minMaxLoc(sobel,&sobmin,&sobmax);
    // Conversion to 8-bit image
    // sobelImage = -alpha*sobel + 255
    
    sobel.convertTo(image,CV_8U,-255./sobmax,255);
    
    //write it back to IplImage
    for (int y = 0; y < image.rows; y++) {
        uchar* imagedata = image.ptr<uchar>(y);
        uchar* ptr = (uchar*) (m_sourceImage->imageData + y * m_sourceImage->widthStep);
        
        for (int x = 0; x < image.cols * 3; x+=3) {
            uchar b = imagedata[x];
            uchar g = imagedata[x+1];
            uchar r = imagedata[x+2];
            
            
            
            ptr[x]   = b;
            ptr[x+1] = g;
            ptr[x+2] = r;
        }
        
    }
    
}

void gaussianBlurTests(){
    //convert the image to opencv2 Mat format
    Mat image = Mat(m_sourceImage, true);
    Mat filtered;
    
    tick();
    
    GaussianBlur(image, filtered, Size(3,3), 1.5f);
    
    tock();
    
    image = filtered.clone();
    //image.convertTo(image,CV_8U);
    
    
    
    //write it back to IplImage
    for (int y = 0; y < image.rows; y++) {
        uchar* imagedata = image.ptr<uchar>(y);
        uchar* ptr = (uchar*) (m_sourceImage->imageData + y * m_sourceImage->widthStep);
        
        for (int x = 0; x < image.cols * 3; x+=3) {
            uchar b = imagedata[x];
            uchar g = imagedata[x+1];
            uchar r = imagedata[x+2];
            
            
            
            ptr[x]   = b;
            ptr[x+1] = g;
            ptr[x+2] = r;
        }
        
    }
    
}



void thresholdTests(int runType, int operation){
    //convert the image to opencv2 Mat format
    Mat image = Mat(m_sourceImage, true);
    Mat thresholded;
    
    
    const int EIGHT_BIT_UNSIGNED = 0;
    const int SIXTEEN_BIT_SIGNED = 1;
    const int THIRTY_TWO_BIT_FLOAT = 2;
    
    //int runType = EIGHT_BIT_UNSIGNED;
    
    switch (runType) {
        case EIGHT_BIT_UNSIGNED:
            image.convertTo(image,CV_8U);
            
            switch (operation) {
                case 0:
                    tick();
                    threshold(image,thresholded,60,255,THRESH_BINARY);
                    tock();
                    break;
                case 1:
                    tick();
                    threshold(image,thresholded,60,255,THRESH_BINARY_INV);
                    tock();
                    break;
                case 2:
                    tick();
                    threshold(image,thresholded,60,255,THRESH_TRUNC);
                    tock();
                    break;
                case 3:
                    tick();
                    threshold(image,thresholded,60,255,THRESH_TOZERO);
                    tock();
                    break;
                case 4:
                    tick();
                    threshold(image,thresholded,60,255,THRESH_TOZERO_INV);
                    tock();
                    break;
                default:
                    break;
            }
            
            
            //inverse binary
            //threshold(image,thresholded,60,255,THRESH_BINARY_INV);
            
            //truncate
            //threshold(image,thresholded,60,255,THRESH_TRUNC);
            
            //to zero
            //threshold(image,thresholded,60,255,THRESH_TOZERO);
            
            //to zero inverse
            //threshold(image,thresholded,60,255,THRESH_TOZERO_INV);
            
            //off the clock
            //tock();
            
            break;
            
        case SIXTEEN_BIT_SIGNED:
            image.convertTo(image,CV_16S);
            switch (operation) {
                case 0:
                    tick();
                    threshold(image,thresholded,60,255,THRESH_BINARY);
                    tock();
                    break;
                case 1:
                    tick();
                    threshold(image,thresholded,60,255,THRESH_BINARY_INV);
                    tock();
                    break;
                case 2:
                    tick();
                    threshold(image,thresholded,60,255,THRESH_TRUNC);
                    tock();
                    break;
                case 3:
                    tick();
                    threshold(image,thresholded,60,255,THRESH_TOZERO);
                    tock();
                    break;
                case 4:
                    tick();
                    threshold(image,thresholded,60,255,THRESH_TOZERO_INV);
                    tock();
                    break;
                default:
                    break;
            }
            
            break;
            
        case THIRTY_TWO_BIT_FLOAT:
            image.convertTo(image,CV_32F);
            tick();
            
            //binary
            threshold(image,thresholded,60,255,THRESH_BINARY);
            
            //inverse binary
            //threshold(image,thresholded,60,255,THRESH_BINARY_INV);
            
            //truncate
            //threshold(image,thresholded,60,255,THRESH_TRUNC);
            
            //to zero
            //threshold(image,thresholded,60,255,THRESH_TOZERO);
            
            //to zero inverse
            //threshold(image,thresholded,60,255,THRESH_TOZERO_INV);
            
            tock();
            break;
    }
    
    image = thresholded.clone();
    image.convertTo(image,CV_8U);
    
    //write it back to IplImage
    for (int y = 0; y < image.rows; y++) {
        uchar* imagedata = image.ptr<uchar>(y);
        uchar* ptr = (uchar*) (m_sourceImage->imageData + y * m_sourceImage->widthStep);
        
        for (int x = 0; x < image.cols * 3; x+=3) {
            uchar b = imagedata[x];
            uchar g = imagedata[x+1];
            uchar r = imagedata[x+2];
            
            ptr[x]   = b;
            ptr[x+1] = g;
            ptr[x+2] = r;
        }
        
    }
	
}

void invertTests(){
    
    //convert the image to opencv2 Mat format
    Mat image = Mat(m_sourceImage, true);
    
    cvtColor(image, image, CV_RGB2GRAY);
    
    int origCols = image.cols;
    int origRows = image.rows;
    
    //needs to be a square matrix
    if(image.cols > image.rows){
            resize(image, image, Size(image.rows,image.rows));
    }else{
            resize(image, image, Size(image.cols,image.cols));
    }
    
    
    
    image.convertTo(image,CV_32F);
    
    //time the mission critical matrix invert
    tick();
    
    //sharpening operation:
    // x = (A^-1 A^T) A^T y
    // were x is the sharpened image
    // A is the smoothing kernel image
    // y is the dodgy image
    
    
    //if negative default sigma = 0.3*(kSize/2 - 1) + 0.8
    Mat A = getGaussianKernel(image.rows, -1, CV_32F);
    
    //A = [0] x  [0, 1]  =  [0, 0]
    //    [1]               [0, 1]
    
    Mat origA = A.clone();
    Mat TA;
    
    transpose(A, TA);
    
    A = A*TA;

    //createGaussianFilter(int type, Size ksize, double sigmaX, double sigmaY=0, int borderType=BORDER_DEFAULT)
    //Mat A = createGaussianFilter(CV_32F, Size(image.cols,image.rows),-1,-1,BORDER_DEFAULT);
    
//    char my_string[40];
//    sprintf(my_string,"origA -> %i", (int)A.at<uchar>(1,1));
//    LOGE(my_string);
    
    
    Mat invA = Mat(Size(A.cols,A.rows),CV_32F);
    invert(A,invA,CV_LU); //should also try with DECOMP_CHOLESKY
    
    Mat traA;
    transpose(A,traA);
    
    //invA is the problem (check dimensions)
    //multiply(invA,image,image);
    
    //image = traA*image;
    A = ((invA*traA)*traA*image);
    
    image = A.clone();
    //invert(image,image,DECOMP_SVD);
    //invert(image,image,DECOMP_SVD);
    
    //bitwise_not(image,image);
    
    tock();
    
    image.convertTo(image,CV_8U);
    
    resize(image, image, Size(origCols,origRows));
    
    
    //write it back to IplImage
    for (int y = 0; y < image.rows; y++) {
        uchar* imagedata = image.ptr<uchar>(y);
        uchar* ptr = (uchar*) (m_sourceImage->imageData + y * m_sourceImage->widthStep);
        int i = 0;
        for (int x = 0; x < image.cols * 3; x+=3) {
            uchar b = imagedata[i];
            uchar g = imagedata[i];
            uchar r = imagedata[i];
            
            
            
            ptr[x]   = b;
            ptr[x+1] = g;
            ptr[x+2] = r;
            
            i++;
        }
        
    }

    
}


void cvtScaleTests(){
    
    //convert the image to opencv2 Mat format
    Mat image = Mat(m_sourceImage, true);
    

    //cvConvertScale(&image, &image, 1, 0);

    //laplacian transform
//    cv::Mat laplaceImage;
//    image.convertTo(laplaceImage,CV_8U,-1,128);
//    
//    image = laplaceImage;
//
  
    //inversion
//    bitwise_not(image,image);
    
    //cvScale test
    //should call cvtScale_<short,int,float>

    //normalise the data to change low level function called to use one with SSE2 optimisations
    //image *= (1./255);
    
    //trigger the SIMD optimisation 32bit float -> 16 bit short
    image.convertTo(image,CV_32F);
    
    //on the clock
    tick();
    
    image.convertTo(image,CV_16S);
    
    //off the clock
    tock();
    
    //and back again for writing
    image.convertTo(image,CV_8U);
    
    
    
    //write it back to IplImage 
    for (int y = 0; y < image.rows; y++) {
        uchar* imagedata = image.ptr<uchar>(y);
        uchar* ptr = (uchar*) (m_sourceImage->imageData + y * m_sourceImage->widthStep);

        for (int x = 0; x < image.cols * 3; x+=3) {
            uchar b = imagedata[x];
            uchar g = imagedata[x+1];
            uchar r = imagedata[x+2];
            
            
            
            ptr[x]   = b;
            ptr[x+1] = g;
            ptr[x+2] = r;
        }
        
    }
        
}

/*
 end of ARM NEON vs Intel SSE Benchmarks
 */
#pragma mark end of NeonvsSSE tests

#ifndef ANDROID
void setWorkingDir(char* wd){
    pwd = wd;
}
#endif


JNIEXPORT
jdouble
JNICALL
Java_com_example_tweakedopencvandroiddemo_MainActivity_doBenchmark(JNIEnv* env, jobject thiz, jint method, jboolean doOptimise){
    
    cv::setUseOptimized(doOptimise);
    
    switch (method) {
            //convert
        case 0:
            cvtScaleTests();
            break;
            //binary 8-bit
        case 1:
            thresholdTests(0, 0);
            break;
        
            //binary inv 8-bit
        case 2:
            thresholdTests(0, 1);
            break;
            
            //trunc 8-bit
        case 3:
            thresholdTests(0, 2);
            break;
            
            //to-zero 8-bit
        case 4:
            thresholdTests(0, 3);
            break;
            
            //to-zero inv 8-bit
        case 5:
            thresholdTests(0, 4);
            break;
        
            //binary 16-bit
        case 6:
            thresholdTests(1, 0);
            break;
        
            //binary inv 16-bit
        case 7:
            thresholdTests(1, 1);
            break;
            
            //trunc 16-bit
        case 8:
            thresholdTests(1, 2);
            break;
            
            //to-zero 16-bit
        case 9:
            thresholdTests(1, 3);
            break;
            
            //to-zero inv 16-bit
        case 10:
            thresholdTests(1, 4);
            break;
            
            //gaussian blur
        case 11:
            gaussianBlurTests();
            break;
            
            //sobel test
        case 12:
            sobelTests();
            break;
            
            //edge detection tests
        case 13:
            edgeDetectionTests();
            break;
            
        default:
            break;
    }
    
    
        
    return theTime;
}

/*
JNIEXPORT
void 
JNICALL
Java_com_example_tweakedopencvandroiddemo_MainActivity_setWorkingDir(JNIEnv* env, jobject thiz, jstring javaString){
    
    const char *nativeString = env->GetStringUTFChars(javaString, 0);
    
    pwd = (char*)nativeString;
    
    //env->ReleaseStringUTFChars(javaString, nativeString);
    
    return;
}
*/

JNIEXPORT
void
JNICALL
Java_com_example_tweakedopencvandroiddemo_MainActivity_doGrayscaleTransform(JNIEnv* env,
                                                                           jobject thiz){

    IplImage* r = cvCreateImage( cvGetSize(m_sourceImage), IPL_DEPTH_8U, 1 ); 
    IplImage* g = cvCreateImage( cvGetSize(m_sourceImage), IPL_DEPTH_8U, 1 ); 
    IplImage* b = cvCreateImage( cvGetSize(m_sourceImage), IPL_DEPTH_8U, 1 );
    
    
    // Split image onto the color planes. 
    cvSplit( m_sourceImage, b, g, r, NULL );
    
    // Temporary storage
    IplImage* s = cvCreateImage(cvGetSize(m_sourceImage), IPL_DEPTH_8U, 1 );
    
    // Release the source image
    if (m_sourceImage) {
		cvReleaseImage(&m_sourceImage);
		m_sourceImage = 0;
	}
    
    // Add equally weighted rgb values. 
    cvAddWeighted( r, 1./3., g, 1./3., 0.0, s ); 
    cvAddWeighted( s, 2./3., b, 1./3., 0.0, s );
    
    // Merge the 4 channel to an BGRA image
    m_sourceImage = cvCreateImage(cvGetSize(s), 8, 3);
    
    cvMerge(s, s, s, NULL, m_sourceImage);
    
    cvReleaseImage(&r); 
    cvReleaseImage(&g); 
    cvReleaseImage(&b); 
    cvReleaseImage(&s);
    
    return;
}

// Given an integer array of image data, load a float array.
// It is the responsibility of the caller to release the float image.
float* getFloatImageFromIntArray(JNIEnv* env, jintArray array_data, 
                                 jint width, jint height){
    // Load Image
    
    int *pixels = env->GetIntArrayElements(array_data, 0);
    
    float *pixelsImg = new float [width*height];
    
    for (int y = 0; y < height; y ++) {
        
        for (int x = 0; x < width; x++) {
            pixelsImg[x+y*width] = (float)((char)pixels[x+y*width] & 0xFF);
            //pixelsImg[x+y*width*3+1] = (float)(pixels[origX+y*width] >> 8 & 0xFF);
            //pixelsImg[x+y*width*3+2] = (float)(pixels[origX+y*width] >> 16 & 0xFF);
        }
    }
    
    for (int i = 0; i < width; i ++) {
        char buffer[32];
        sprintf(buffer, "@ pixel no. %i -> %f", i, pixelsImg[i]);
        LOGV((char*)buffer,false);
    }
	
    
    
//    LOGV((char*)"checking source int pixels",false);
    
    
    
    
//    float * pixelsAsFloats= new float[width*height];
//
//    for (int i = 0; i < width*height; i++) {
//        pixelsAsFloats[i] = (float)pixels[i];
//    }
    
//    for (int i = 0; i < width; i++) {
//        for (int j = 0; j < height; j++) {
//            pixelsAsFloats[(j*width) + i] = (float)pixels[(j*width) + i];
//        }
//    }
    
    //clean up the jni environment
    env->ReleaseIntArrayElements(array_data, pixels, 0);
    
    return pixelsImg;
}


// Given an integer array of image data, load an IplImage.
// It is the responsibility of the caller to release the IplImage.
IplImage* getIplImageFromIntArray(JNIEnv* env, jintArray array_data, 
								  jint width, jint height) {
	// Load Image
	int *pixels = env->GetIntArrayElements(array_data, 0);
	if (pixels == 0) {
		LOGE("Error getting int array of pixels.");
		return 0;
	}
	
	IplImage *image = loadPixels(pixels, width, height);
	env->ReleaseIntArrayElements(array_data, pixels, 0);
	if(image == 0) {
		LOGE("Error loading pixel array.");
		return 0;
	}
	
	return image;
}


// Generate and return a boolean array from the source image.
// Return 0 if a failure occurs or if the source image is undefined.
JNIEXPORT
jbyteArray
JNICALL
Java_com_example_tweakedopencvandroiddemo_MainActivity_getSourceImage(JNIEnv* env,
                                                                     jobject thiz)
{
    
	if (m_sourceImage == 0) {
		LOGE("Error source image was not set.");
		return 0;
	}
	
	CvMat stub;
    CvMat *mat_image = cvGetMat(m_sourceImage, &stub);
    int channels = CV_MAT_CN( mat_image->type );
    int ipl_depth = 4;
    
	WLNonFileByteStream *strm = new WLNonFileByteStream();
    loadImageBytes(mat_image->data.ptr, mat_image->step, mat_image->width,
                   mat_image->height, ipl_depth, channels, strm);
	
	int imageSize = strm->GetSize();
    
    //if you wanted to return an array of 1's and 0's ()
    /*
     jbooleanArray res_array = env->NewBooleanArray(imageSize);
     if (res_array == 0) {
     LOGE("Unable to allocate a new boolean array for the source image.");
     return 0;
     }
     env->SetBooleanArrayRegion(res_array, 0, imageSize, (jboolean*)strm->GetByte());
     */
    
    jbyteArray res_array = env->NewByteArray(imageSize);
    if (res_array == 0) {
        LOGE("Unable to allocate a new byte array for the source image.");
        return 0;
    }
    env->SetByteArrayRegion(res_array, 0, imageSize, (jbyte*)strm->GetByte());
    
	strm->Close();
	//SAFE_DELETE(strm);
	
	return res_array;
    
}

// Set the source image and return true if successful or false otherwise.
JNIEXPORT
jboolean
JNICALL
Java_com_example_tweakedopencvandroiddemo_MainActivity_fiveATime_MainActivity_setSourceImage(JNIEnv* env,
                                                                     jobject thiz,
                                                                     jintArray photo_data,
                                                                     jint width,
                                                                     jint height)
{	
	// Release the image if it hasn't already been released.
	if (m_sourceImage) {
		cvReleaseImage(&m_sourceImage);
		m_sourceImage = 0;
	}
	
	m_sourceImage = getIplImageFromIntArray(env, photo_data, width, height);
	if (m_sourceImage == 0) {
		LOGE("Error source image could not be created.");
		return false;
	}
	
	return true;
}

JNIEXPORT
jboolean
JNICALL
Java_com_example_tweakedopencvandroiddemo_MainActivity_fiveATime(JNIEnv* env,
                                                              jobject thiz,
                                                              jintArray photo_data1,
                                                              jintArray photo_data2,
                                                              jintArray photo_data3,
                                                              jintArray photo_data4,
                                                              jintArray photo_data5,
                                                              jint width,
                                                              jint height){
    
    IplImage* img1 = getIplImageFromIntArray(env, photo_data1, width, height);
	if (img1 == 0) {
		LOGE("Error source image could not be created.");
		return false;
	}
    
    IplImage* img2 = getIplImageFromIntArray(env, photo_data2, width, height);
	if (img2 == 0) {
		LOGE("Error source image could not be created.");
		return false;
	}
    
    IplImage* img3 = getIplImageFromIntArray(env, photo_data3, width, height);
	if (img3 == 0) {
		LOGE("Error source image could not be created.");
		return false;
	}
    
    IplImage* img4 = getIplImageFromIntArray(env, photo_data4, width, height);
	if (img4 == 0) {
		LOGE("Error source image could not be created.");
		return false;
	}
    
    IplImage* img5 = getIplImageFromIntArray(env, photo_data5, width, height);
	if (img5 == 0) {
		LOGE("Error source image could not be created.");
		return false;
	}
    
    Mat mat1 = Mat(img1, true);
    Mat mat2 = Mat(img2, true);
    Mat mat3 = Mat(img3, true);
    Mat mat4 = Mat(img4, true);
    Mat mat5 = Mat(img5, true);
    
    //call magic harness function!
    Harness(mat1, mat2, mat3, mat4, mat5, 5, false);

    
    //write mat1 back, why not?
    if (!m_sourceImage) {
        m_sourceImage = cvCreateImage(mat1.size(), 8, 3);
    }
    Mat image = mat1;
    
    for (int y = 0; y < image.rows; y++) {
        uchar* imagedata = image.ptr<uchar>(y);
        uchar* ptr = (uchar*) (m_sourceImage->imageData + y * m_sourceImage->widthStep);
        
        for (int x = 0; x < image.cols * 3; x+=3) {
            uchar b = imagedata[x];
            uchar g = imagedata[x+1];
            uchar r = imagedata[x+2];
            
            
            
            ptr[x]   = b;
            ptr[x+1] = g;
            ptr[x+2] = r;
        }
        
    }
    
    
    return true;
    
}
/*
JNIEXPORT
jboolean
JNICALL
Java_com_example_tweakedopencvandroiddemo_MainActivity_imageProcessingHasFinished(JNIEnv* env,
                                                                                 jobject thiz){
    return processingFinished;
}


JNIEXPORT jstring JNICALL
Java_com_example_videostreamprocessor_VideoActivity_stringFromJNI(JNIEnv* env, jobject thiz){
    //initialise the random seed for neonise functions (used to pick NEON colours)
    srand(time(NULL)); 
    
    //do a little bit of simple float arithmetric (vector by scalar)
    //if it runs, and computes the correct result... we know it works!
    
    return env->NewStringUTF("Hello from JNI!");
    
}
*/

/*
 * End of android specific stuff
 */


