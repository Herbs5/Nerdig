#export  ANDROID_NDK_ROOT="/mnt/home/android/milky2/MilkyTracker-1.02.00/platforms/android" 
export  ANDROID_NDK_ROOT="/mnt/sda3/MilkyTracker-095+lr/platforms/android" 
export MILKYTRACKER_ROOT="/mnt/sda3/MilkyTracker-095+lr"

if [ -z $ANDROID_NDK_ROOT ]; then
    echo "set the ANDROID_NDK_ROOT environment variable"
else
    echo "ANDROID_NDK_ROOT   already set";
fi
if [ -z $MILKYTRACKER_ROOT ]; then
	echo " set the MILKYTRACKER_ROOT environment variable"
 else
    echo "MILKYTRACKER_ROOT   already set";
fi



NDK_TOOLCHAIN_VERSION=$GCCVER
[ -z "$NDK_TOOLCHAIN_VERSION" ] && NDK_TOOLCHAIN_VERSION=4.9

export NDK_PROJECT_PATH="/mnt/home/MilkyTracker-095+lr/platforms/android/project"
cd "$NDK_PROJECT_PATH"
ndk-build APP=milkytracker V=1 APP_ABI=armeabi-v7a
# APP_ABI=x86  = arm64-v8a
