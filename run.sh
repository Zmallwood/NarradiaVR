export ANDROID_HOME=/home/andreas/Android/Sdk
./gradlew installOpenGLESDebug &&
adb shell am start -n org.khronos.openxr.hello_xr.opengles/android.app.NativeActivity &&
adb logcat -s Narradia:*
