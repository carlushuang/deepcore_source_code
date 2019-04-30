#!/bin/sh
PWD=`pwd`
HIPCC=/opt/rocm/hip/bin/hipcc
HIPCC_FLAGS=" --genco  --targets gfx900,gfx906  "
# NVCC_FLAGS="$NVCC_FLAGS -G "
WD=deepcore_device/deepcore_device/
SRC=deepcore_device.hip.cpp
BUILD_DIR=$PWD/build
TARGET=dc_gfx906.co

#rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR

if [ $B0 ] ;then
# 1, build device code
rm -rf $BUILD_DIR/$TARGET
pushd $WD
$HIPCC $HIPCC_FLAGS $SRC -o $BUILD_DIR/$TARGET || exit 1
popd

test -f $BUILD_DIR/$TARGET || exit 1


# 2, let fatbin be c string
hexdump  -v -e '1/8 "0x%016x," "\n"' $BUILD_DIR/$TARGET  | \
        paste -sd '   \n' -   > deepcore/include/dev/fftconv/kbin_gfx906.h
fi

# 3, build deepcore
if [ $B1 ] ;then
WD=deepcore
CC=g++
CFLAGS=`/opt/rocm/bin/hipconfig --cpp_config`" -Wall -O2 -shared -fPIC -x c++ "
#CFLAGS=" $CFLAGS -DDC_VERBOSE"
CFLAGS=" $CFLAGS -D__HIPCC__ -D__HIP_PLATFORM_HCC__ "
TARGET=libdeepcore.so
LDFLAGS=" -L/opt/rocm/hcc/lib -Wl,--rpath=/opt/rocm/hcc/lib -ldl -lm -lpthread -lhc_am -Wl,--whole-archive -lmcwamp -Wl,--no-whole-archive"


rm -rf $BUILD_DIR/$TARGET
pushd $WD

SRC=$(find ./  -type f -name "*.c")
echo $SRC

$CC $CFLAGS $SRC $LDFLAGS -o  $BUILD_DIR/$TARGET || exit 1

popd
fi

# 4. build example
WD=example
CXX=g++
CXXFLAGS=`/opt/rocm/bin/hipconfig --cpp_config`" -I$PWD/deepcore/include/ -Wall -O2 "
CXXFLAGS=" $CXXFLAGS -D__HIPCC__ -D__HIP_PLATFORM_HCC__ "
LDFLAGS=" -L/opt/rocm/hcc/lib -L/opt/rocm/lib -L/opt/rocm/lib64 -Wl,--rpath=/opt/rocm/hcc/lib -ldl -lm -lpthread -lhc_am -Wl,--whole-archive -lmcwamp -lhip_hcc -lhsa-runtime64 -lhsakmt -Wl,--no-whole-archive"
LDFLAGS="$LDFLAGS -Wl,-rpath,$BUILD_DIR -lm  -L$BUILD_DIR -ldeepcore "
SRC=fftconv.cpp
TARGET=fftconv
pushd $WD

$CXX $CXXFLAGS $SRC $LDFLAGS -o $BUILD_DIR/$TARGET

popd

