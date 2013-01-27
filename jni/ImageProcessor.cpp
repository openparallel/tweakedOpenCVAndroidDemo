//
//  ImageProcessor.cpp
//  FaceIt
//
//  Created by Beau Johnston on 1/06/12.
//  Copyright (c) 2012 OpenParallel.com all rights reserved.
//

#include "ImageProcessor.h"

#include <arm_neon.h>
#include <time.h>



double oldTime, newTime, theTime;
int clockTime;



#pragma mark Private Utility functions
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



void tick(){
	oldTime = (double)getTickCount();
	clockTime = clock();
}



void tock(){
	
	newTime = (double)getTickCount();
	theTime = (newTime-oldTime)/getTickFrequency();
}



#pragma mark start of NeonvsSSE tests
/*
 * Start of ARM NEON vs Intel SSE Benchmarks
 */

/*
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
*/



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



/*
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
*/



void cvtScaleTests(){
	
	//convert the image to opencv2 Mat format
	Mat image = Mat(m_sourceImage, true);
	
	//convert to suitable initial data type (float)
	image.convertTo(image,CV_32F);
	
	//on the clock
	tick();
	
	//trigger the SIMD optimisation 32bit float -> 16 bit short
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



#pragma mark JNI interfacing functions
JNIEXPORT
jdouble
JNICALL
Java_com_example_tweakedopencvandroiddemo_MainActivity_doBenchmark(JNIEnv* env,
																	jobject thiz,
																	jint method,
																	jboolean doOptimise) {
	
	cv::setUseOptimized(doOptimise);
	
	useNe10();
	CV_EXPORTS_W bool useNEON();
	
	if(cv::useOptimized()){
		if(!cv::useNEON()){
			setUseNEON(true);
			setUseNe10(false);
		}
		else{
			setUseNEON(false);
			setUseNe10(true);
		}
	}
	
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
		
		//binary 32-bit
		case 14:
			thresholdTests(2, 0);
			break;
		
		//binary inv 32-bit
		case 15:
			thresholdTests(2, 1);
			break;
		
		//trunc 32-bit
		case 16:
			thresholdTests(2, 2);
			break;
		
		//to-zero 32-bit
		case 17:
			thresholdTests(2, 3);
			break;
		
		//to-zero inv 32-bit
		case 18:
			thresholdTests(2, 4);
			break;
		
		default:
			break;
		}
	
	return theTime;
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
	
	//clean up the jni environment
	env->ReleaseIntArrayElements(array_data, pixels, 0);
	
	return pixelsImg;
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
	
	jbyteArray res_array = env->NewByteArray(imageSize);
	if (res_array == 0) {
		LOGE("Unable to allocate a new byte array for the source image.");
		return 0;
	}
	env->SetByteArrayRegion(res_array, 0, imageSize, (jbyte*)strm->GetByte());
	strm->Close();
	return res_array;
	
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



// Set the source image and return true if successful or false otherwise.
JNIEXPORT
jboolean
JNICALL
Java_com_example_tweakedopencvandroiddemo_MainActivity_setSourceImage(JNIEnv* env,
																		jobject thiz,
																		jintArray photo_data,
																		jint width,
																		jint height) {
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
