#!/bin/bash

#compile JNI C/C++ source
cd jni/; ndk-build; cd ..

#clean & build java interface and Android apk
ant clean
ant debug

#remove the old version
adb uninstall com.example.tweakedopencvandroiddemo

#install the new version and start it up
adb -d install bin/tweakedOpenCVAndroidDemo-debug.apk
adb shell am start -a android.intent.action.MAIN -n com.example.tweakedopencvandroiddemo/.MainActivity 