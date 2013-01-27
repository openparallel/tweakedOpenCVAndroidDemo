APP_PLATFORM := android-9

LOCAL_PATH:= $(call my-dir)

USING_NEON:= -mfloat-abi=softfp -mfpu=neon -march=armv7 -mthumb


include $(CLEAR_VARS)


NAME := ne10

LOCAL_CPP_EXTENSION := .cpp .cc .cxx
LOCAL_MODULE := $(NAME)
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES := \
				$(LOCAL_PATH)/NE10/headers/ $(LOCAL_PATH)/NE10/inc/
				
LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%) -D_ARM_ASSEM_  -fexceptions $(USING_NEON)

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -ldl $(USING_NEON)

ne10_neon_source := \
	NE10/source/NE10_abs.neon.s \
	NE10/source/NE10_addc.neon.c \
	NE10/source/NE10_addmat.neon.c \
	NE10/source/NE10_add.neon.s \
	NE10/source/NE10_cross.neon.s \
	NE10/source/NE10_detmat.neon.s \
	NE10/source/NE10_divc.neon.c \
	NE10/source/NE10_div.neon.s \
	NE10/source/NE10_dot.neon.s \
	NE10/source/NE10_identitymat.neon.s \
	NE10/source/NE10_invmat.neon.s \
	NE10/source/NE10_len.neon.s \
	NE10/source/NE10_mla.neon.s \
	NE10/source/NE10_mlac.neon.c \
	NE10/source/NE10_mulcmatvec.neon.s \
	NE10/source/NE10_mulc.neon.c \
	NE10/source/NE10_mulmat.neon.s \
	NE10/source/NE10_mul.neon.c \
	NE10/source/NE10_normalize.neon.s \
	NE10/source/NE10_rsbc.neon.c \
	NE10/source/NE10_setc.neon.c \
	NE10/source/NE10_subc.neon.c \
	NE10/source/NE10_submat.neon.c \
	NE10/source/NE10_sub.neon.s \
	NE10/source/NE10_transmat.neon.s

ne10_source_files := \
	NE10/source/NE10_abs.asm.s \
	NE10/source/NE10_addc.asm.s \
	NE10/source/NE10_addmat.asm.s \
	NE10/source/NE10_add.asm.s \
	NE10/source/NE10_cross.asm.s \
	NE10/source/NE10_detmat.asm.s \
	NE10/source/NE10_divc.asm.s \
	NE10/source/NE10_div.asm.s \
	NE10/source/NE10_dot.asm.s \
	NE10/source/NE10_identitymat.asm.s \
	NE10/source/NE10_invmat.asm.s \
	NE10/source/NE10_len.asm.s \
	NE10/source/NE10_mla.asm.s \
	NE10/source/NE10_mlac.asm.s \
	NE10/source/NE10_mulcmatvec.asm.s \
	NE10/source/NE10_mulc.asm.s \
	NE10/source/NE10_mulmat.asm.s \
	NE10/source/NE10_mul.asm.s \
	NE10/source/NE10_normalize.asm.s \
	NE10/source/NE10_rsbc.asm.s \
	NE10/source/NE10_setc.asm.s \
	NE10/source/NE10_subc.asm.s \
	NE10/source/NE10_submat.asm.s \
	NE10/source/NE10_sub.asm.s \
	NE10/source/NE10_transmat.asm.s \
	NE10/source/NE10_abs.c \
	NE10/source/NE10_addc.c \
	NE10/source/NE10_addmat.c \
	NE10/source/NE10_add.c \
	NE10/source/NE10_cross.c \
	NE10/source/NE10_detmat.c \
	NE10/source/NE10_divc.c \
	NE10/source/NE10_div.c \
	NE10/source/NE10_dot.c \
	NE10/source/NE10_identitymat.c \
	NE10/source/NE10_invmat.c \
	NE10/source/NE10_len.c \
	NE10/source/NE10_mla.c \
	NE10/source/NE10_mlac.c \
	NE10/source/NE10_mulcmatvec.c \
	NE10/source/NE10_mulc.c \
	NE10/source/NE10_mulmat.c \
	NE10/source/NE10_mul.c \
	NE10/source/NE10_normalize.c \
	NE10/source/NE10_rsbc.c \
	NE10/source/NE10_setc.c \
	NE10/source/NE10_subc.c \
	NE10/source/NE10_submat.c \
	NE10/source/NE10_sub.c \
	NE10/source/NE10_transmat.c


include $(BUILD_STATIC_LIBRARY)



include $(CLEAR_VARS)

NAME := core

LOCAL_CPP_EXTENSION := .cpp .cc .cxx
LOCAL_MODULE := $(NAME)
LOCAL_ARM_MODE := arm

#build with NE10 includes
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/$(NAME)/include $(LOCAL_PATH)/NE10/headers/ $(LOCAL_PATH)/NE10/inc/
LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%) -D_ARM_ASSEM_ -fexceptions $(USING_NEON)
#link against the NE10 static lib
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -lne10 -ldl $(USING_NEON) 


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
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/NE10/headers/ \
		$(LOCAL_PATH)/NE10/inc/ \
		$(LOCAL_PATH)/$(NAME)/include \
		$(LOCAL_PATH)/core/src/ \
		$(LOCAL_PATH)/core/include
LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%)  -D_ARM_ASSEM_ -fexceptions $(USING_NEON)
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -lne10 -ldl\
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
		$(LOCAL_PATH)/NE10/headers/ \
		$(LOCAL_PATH)/NE10/inc/ \
		$(LOCAL_PATH)/core/src/ \
		$(LOCAL_PATH)/core/include/ \
		$(LOCAL_PATH)/imgproc/src/ \
		$(LOCAL_PATH)/imgproc/include
        
LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%) $(USING_NEON) -Wno-psabi -O3 -ftree-vectorize -lz -lm


LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib \
				-L$(TARGET_OUT) -lne10 -lcore -limgproc\
				 -ldl -llog -lstdc++ -lz -lm\
				 -mfloat-abi=softfp -mfpu=neon



LOCAL_SRC_FILES := \
        WLNonFileByteStream.cpp \
        ImageProcessor.cpp

LOCAL_MODULE_TAGS := eng

LOCAL_STATIC_LIBRARIES := core imgproc ne10

include $(BUILD_SHARED_LIBRARY)

