APP_ABI := armeabi armeabi-v7a
APP_PLATFORM := android-9
APP_STL := gnustl_static
STLPORT_FORCE_REBUILD := true
ANDROID=1
DEBUG=1
__NEON__=1
APP_BUILD_SCRIPT := $(call my-dir)/Android.mk
APP_PROJECT_PATH := $(call my-dir)/..
APP_MODULES      := core imgproc opencv
