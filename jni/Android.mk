APP_PLATFORM := android-9

LOCAL_PATH:= $(call my-dir)

USING_NEON:= -mfloat-abi=softfp -mfpu=neon -march=armv7 -mthumb
#USING_NEON:= 

include $(CLEAR_VARS)

NAME := core

LOCAL_CPP_EXTENSION := .cpp .cc .cxx
LOCAL_MODULE := $(NAME)

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/$(NAME)/include
LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%) -fexceptions $(USING_NEON)
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -ldl $(USING_NEON) 


LOCAL_SRC_FILES := \
		$(NAME)/src/algorithm.cpp \
		$(NAME)/src/array.cpp \
		$(NAME)/src/copy.cpp \
		$(NAME)/src/dxt.cpp \
		$(NAME)/src/mathfuncs.cpp \
		$(NAME)/src/matrix.cpp \
		$(NAME)/src/parallel.cpp \
		$(NAME)/src/rand.cpp \
		$(NAME)/src/tables.cpp \
		$(NAME)/src/alloc.cpp \
		$(NAME)/src/cmdparser.cpp \
		$(NAME)/src/datastructs.cpp \
		$(NAME)/src/gpumat.cpp \
		$(NAME)/src/matmul.cpp \
		$(NAME)/src/opengl_interop.cpp \
		$(NAME)/src/persistence.cpp \
		$(NAME)/src/stat.cpp \
		$(NAME)/src/arithm.cpp \
		$(NAME)/src/convert.cpp \
		$(NAME)/src/drawing.cpp \
		$(NAME)/src/lapack.cpp \
		$(NAME)/src/matop.cpp \
		$(NAME)/src/out.cpp \
		$(NAME)/src/precomp.cpp \
		$(NAME)/src/system.cpp
#		$(NAME)/src/*.cpp



include $(BUILD_STATIC_LIBRARY)



include $(CLEAR_VARS)

NAME := imgproc

LOCAL_CPP_EXTENSION := .cpp .cc .cxx
LOCAL_MODULE := $(NAME)

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/$(NAME)/include \
		$(LOCAL_PATH)/core/src/ \
		$(LOCAL_PATH)/core/include
LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%) -fexceptions $(USING_NEON)
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -ldl\
$(USING_NEON)

LOCAL_SRC_FILES := \
		$(NAME)/src/accum.cpp \
		$(NAME)/src/approx.cpp \
		$(NAME)/src/canny.cpp \
		$(NAME)/src/color.cpp \
		$(NAME)/src/contours.cpp \
		$(NAME)/src/convhull.cpp \
		$(NAME)/src/corner.cpp \
		$(NAME)/src/cornersubpix.cpp \
		$(NAME)/src/deriv.cpp \
		$(NAME)/src/distransform.cpp \
		$(NAME)/src/emd.cpp \
		$(NAME)/src/featureselect.cpp \
		$(NAME)/src/filter.cpp \
		$(NAME)/src/floodfill.cpp \
		$(NAME)/src/gabor.cpp \
		$(NAME)/src/geometry.cpp \
		$(NAME)/src/grabcut.cpp \
		$(NAME)/src/histogram.cpp \
		$(NAME)/src/hough.cpp \
		$(NAME)/src/imgwarp.cpp \
		$(NAME)/src/linefit.cpp \
		$(NAME)/src/matchcontours.cpp \
		$(NAME)/src/moments.cpp \
		$(NAME)/src/morph.cpp \
		$(NAME)/src/phasecorr.cpp \
		$(NAME)/src/precomp.cpp \
		$(NAME)/src/pyramids.cpp \
		$(NAME)/src/rotcalipers.cpp \
		$(NAME)/src/samplers.cpp \
		$(NAME)/src/segmentation.cpp \
		$(NAME)/src/shapedescr.cpp \
		$(NAME)/src/smooth.cpp \
		$(NAME)/src/subdivision2d.cpp \
		$(NAME)/src/sumpixels.cpp \
		$(NAME)/src/tables.cpp \
		$(NAME)/src/templmatch.cpp \
		$(NAME)/src/thresh.cpp \
		$(NAME)/src/undistort.cpp \
		$(NAME)/src/utils.cpp
#		$(NAME)/src/*.cpp


include $(BUILD_STATIC_LIBRARY)







include $(CLEAR_VARS)

#LOCAL_CPP_EXTENSION := .cc
LOCAL_CFLAGS := -D_ARM_ASSEM_
LOCAL_ARM_MODE := arm


LOCAL_MODULE    := opencv
LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/core/src/ \
		$(LOCAL_PATH)/core/include/ \
		$(LOCAL_PATH)/imgproc/src/ \
		$(LOCAL_PATH)/imgproc/include
        
LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%) $(USING_NEON) -O3 -ftree-vectorize -lz -lm


LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib \
				-L$(TARGET_OUT) -lcore -limgproc\
				 -ldl -llog -lstdc++ -lz -lm\
				 -mfloat-abi=softfp -mfpu=neon



LOCAL_SRC_FILES := \
		Utils.cpp \
		Transform.cpp \
		Harness.cpp \
        WLNonFileByteStream.cpp \
        ImageProcessor.cpp

LOCAL_MODULE_TAGS := eng

LOCAL_STATIC_LIBRARIES := core imgproc

include $(BUILD_SHARED_LIBRARY)

