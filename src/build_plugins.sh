#!/bin/sh
echo ""
echo "Building JavaClass..."
javac JavaClass.java -classpath ../DolbySurround.jar -bootclasspath $ANDROID_SDK_ROOT/platforms/android-8/android.jar -d .

echo ""
echo "Signature dump of JavaClass..."

javap -s com.dolby.ddpexample.JavaClass

echo "Creating JavaClass.jar..."
jar cvfM ../JavaClass.jar com/

echo ""
echo "Compiling NativeJavaBridge.cpp..."
$ANDROID_NDK_ROOT/ndk-build NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk $*
mv libs/armeabi/libjavabridge.so ..

echo ""
echo "Cleaning up / removing build folders..."  #optional..
rm -rf libs
rm -rf obj
rm -rf org
rm -rf com

echo ""
echo "Done!"
