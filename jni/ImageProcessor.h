//
//  FaceDetection.h
//  FaceIt
//
//  Created by Beau Johnston on 1/06/12.
//  Copyright (c) 2012 OpenParallel.com all rights reserved.
//

#define USINGNEON

#ifndef FaceIt_FaceDetection_h
#define FaceIt_FaceDetection_h
 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <arm_neon.h>

#ifndef ANDROID
	
	#include <opencv2/core/core.hpp>
	#include <opencv2/imgproc/imgproc.hpp>
	#include <opencv2/objdetect/objdetect.hpp>
	
#endif

#ifdef ANDROID
	
	#include <jni.h>
	#include <android/log.h>
	#include "WLNonFileByteStream.h"
	#include "highgui/src/grfmt_bmp.hpp"
	
	
	using namespace std;
	using namespace cv;
	
	#define LOGV(...) __android_log_print(ANDROID_LOG_SILENT, LOG_TAG, __VA_ARGS__)
	#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
	
	#define ANDROID_LOG_VERBOSE ANDROID_LOG_DEBUG
	#define LOG_TAG "Captain's Log"
	#define INVALID_ARGUMENT -18456
	
	#define		SAFE_DELETE(p)			{ if(p){ delete (p); (p)=0; } }
	#define		SAFE_DELETE_ARRAY(p)	{ if(p){ delete [](p); (p)=0; } }
	
	
	#define IMAGE( i, x, y, n )   *(( unsigned char * )(( i )->imageData      \
	+ ( x ) * sizeof( unsigned char ) * 3 \
	+ ( y ) * ( i )->widthStep ) + ( n ))
	
	// CV Objects
	static const char* fmtSignBmp = "BM";
	
	IplImage *m_sourceImage = 0;
	
	
	
	
	#ifdef __cplusplus
	extern "C" {
	#endif
	
		JNIEXPORT
		jdouble
		JNICALL
		Java_com_example_tweakedopencvandroiddemo_MainActivity_doBenchmark(JNIEnv* env, jobject thiz, jint method, jboolean doOptimise);
	
	
		JNIEXPORT
		jbyteArray
		JNICALL
		Java_com_example_tweakedopencvandroiddemo_MainActivity_getSourceImage(JNIEnv* env,
		jobject thiz);
	
		JNIEXPORT
		jboolean
		JNICALL
		Java_com_example_tweakedopencvandroiddemo_MainActivity_setSourceImage(JNIEnv* env,
		jobject thiz,
		jintArray photo_data,
		jint width,
		jint height);
	
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
		jint height);
	
	#ifdef __cplusplus
	}
	#endif
	
	
	IplImage* loadPixels(int* pixels, int width, int height) {
	
	int x, y;
	IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	
	for ( y = 0; y < height; y++ ) {
	for ( x = 0; x < width; x++ ) {
	// blue
	IMAGE( img, x, y, 0 ) = pixels[x+y*width] & 0xFF;
	// green
	IMAGE( img, x, y, 1 ) = pixels[x+y*width] >> 8 & 0xFF;
	// red
	IMAGE( img, x, y, 2 ) = pixels[x+y*width] >> 16 & 0xFF;
	}
	}
	
	return img;
	}
	
	void  FillGrayPalette( PaletteEntry* palette, int bpp, bool negative )
	{
	int i, length = 1 << bpp;
	int xor_mask = negative ? 255 : 0;
	
	for( i = 0; i < length; i++ )
	{
	int val = (i * 255/(length - 1)) ^ xor_mask;
	palette[i].b = palette[i].g = palette[i].r = (uchar)val;
	palette[i].a = 0;
	}
	}
	
	void loadImageBytes(const uchar* data,
	int step,
	int width,
	int height,
	int depth,
	int channels,
	WLNonFileByteStream* m_strm) {
	
	int fileStep = (width*channels + 3) & -4;
	uchar zeropad[] = "\0\0\0\0";
	char log_str[100];
	
	assert( data && width > 0 && height > 0 && step >= fileStep );
	
	int  bitmapHeaderSize = 40;
	int  paletteSize = channels > 1 ? 0 : 1024;
	int  headerSize = 14 /* fileheader */ + bitmapHeaderSize + paletteSize;
	PaletteEntry palette[256];
	
	int testSize = fileStep*height + headerSize;
	m_strm->Open(testSize);
	sprintf(log_str, "fileStep*height + headerSize=%i", testSize);
	LOGV(log_str);
	
	// write signature 'BM'
	m_strm->PutBytes( fmtSignBmp, (int)strlen(fmtSignBmp) );
	
	// write file header
	m_strm->PutDWord( fileStep*height + headerSize ); // file size
	m_strm->PutDWord( 0 );
	m_strm->PutDWord( headerSize );
	
	// write bitmap header
	m_strm->PutDWord( bitmapHeaderSize );
	m_strm->PutDWord( width );
	m_strm->PutDWord( height );
	m_strm->PutWord( 1 );
	m_strm->PutWord( channels << 3 );
	m_strm->PutDWord( BMP_RGB );
	m_strm->PutDWord( 0 );
	m_strm->PutDWord( 0 );
	m_strm->PutDWord( 0 );
	m_strm->PutDWord( 0 );
	m_strm->PutDWord( 0 );
	
	if( channels == 1 )
	{
	FillGrayPalette( palette, 8 );
	m_strm->PutBytes( palette, sizeof(palette));
	}
	
	width *= channels;
	data += step*(height - 1);
	for( ; height--; data -= step )
	{
	m_strm->PutBytes( data, width );
	if( fileStep > width )
	m_strm->PutBytes( zeropad, fileStep - width );
	}
	}
	
	
	bool is_NULL_field_JavaObj(JNIEnv* env, jobject java_obj, const char* field_name, const char* field_type) {
	
	
	LOGV("in is_NULL_field_JavaObj!");
	jclass clazz = env->GetObjectClass(java_obj);
	
	// get field
	jfieldID fid = env->GetFieldID(clazz, field_name, field_type);
	
	jobject obj = env->GetObjectField(java_obj, fid);
	if(obj == 0) {
	LOGV("Object is NULL!");
	return true;
	}
	return false;
	}
	
	bool is_NULL_vec_field_JavaObj(JNIEnv* env, jobject java_obj, const char* field_name) {
	return is_NULL_field_JavaObj(env, java_obj, field_name, "Lorg/siprop/opencv/util/Vector3;");
	}
	
	bool is_NULL_point_field_JavaObj(JNIEnv* env, jobject java_obj, const char* field_name) {
	return is_NULL_field_JavaObj(env, java_obj, field_name, "Lorg/siprop/opencv/util/Point3;");
	}
	
	bool is_NULL_axis_field_JavaObj(JNIEnv* env, jobject java_obj, const char* field_name) {
	return is_NULL_field_JavaObj(env, java_obj, field_name, "Lorg/siprop/opencv/util/Axis;");
	}
	
	bool is_NULL_pivot_field_JavaObj(JNIEnv* env, jobject java_obj, const char* field_name) {
	return is_NULL_field_JavaObj(env, java_obj, field_name, "Lorg/siprop/opencv/util/Pivot3;");
	}
	
	bool is_NULL_quat_field_JavaObj(JNIEnv* env, jobject java_obj, const char* field_name) {
	return is_NULL_field_JavaObj(env, java_obj, field_name, "Lorg/siprop/opencv/util/Quaternion;");
	}
	
	bool is_NULL_mat3x3_field_JavaObj(JNIEnv* env, jobject java_obj, const char* field_name) {
	return is_NULL_field_JavaObj(env, java_obj, field_name, "Lorg/siprop/opencv/util/Matrix3x3;");
	}
	bool is_NULL_mat3x1_field_JavaObj(JNIEnv* env, jobject java_obj, const char* field_name) {
	return is_NULL_field_JavaObj(env, java_obj, field_name, "Lorg/siprop/opencv/util/Matrix3x1;");
	}
	
	
	void set_JavaObj_int(JNIEnv* env, jobject java_obj, const char* field_name, jint val) {
	
	LOGV("in set_JavaObj_int!");
	
	jclass clazz = env->GetObjectClass(java_obj);
	jfieldID fid = env->GetFieldID(clazz, field_name, "I");
	
	env->SetIntField(java_obj, fid, val);
	
	}
	
	int get_id_by_JavaObj(JNIEnv* env, jobject java_obj) {
	
	LOGV("in get_id_by_JavaObj!");
	
	jclass method_clazz = env->GetObjectClass(java_obj);
	jmethodID get_type_mid = env->GetMethodID(method_clazz, "getID", "()I");
	return env->CallIntMethod(java_obj, get_type_mid);
	
	}
	
	int get_type_by_JavaObj(JNIEnv* env, jobject java_obj) {
	
	LOGV("in get_type_by_JavaObj!");
	
	jclass method_clazz = env->GetObjectClass(java_obj);
	jmethodID get_type_mid = env->GetMethodID(method_clazz, "getType", "()I");
	return env->CallIntMethod(java_obj, get_type_mid);
	
	}
	
	
	int get_int_by_JavaObj(JNIEnv* env, jobject java_obj, const char* field_name) {
	
	LOGV("in get_int_by_JavaObj!");
	
	jclass clazz = env->GetObjectClass(java_obj);
	jfieldID int_fid = env->GetFieldID(clazz, field_name, "I");
	return env->GetIntField(java_obj, int_fid);
	
	}
	
	
	float get_float_by_JavaObj(JNIEnv* env, jobject java_obj, const char* field_name) {
	
	LOGV("in get_float_by_JavaObj!");
	
	jclass clazz = env->GetObjectClass(java_obj);
	jfieldID float_fid = env->GetFieldID(clazz, field_name, "F");
	return env->GetFloatField(java_obj, float_fid);
	
	}
	
	
	jobject get_obj_by_JavaObj(JNIEnv* env, jobject java_obj, const char* field_name, const char* obj_type) {
	
	LOGV("in get_obj_by_JavaObj!");
	
	jclass clazz = env->GetObjectClass(java_obj);
	jfieldID obj_fid = env->GetFieldID(clazz, field_name, obj_type);
	return env->GetObjectField(java_obj, obj_fid);
	
	}
	
#endif

/*
 * Private functions
 */
void Log(char* message, bool errorFlag);

#endif
