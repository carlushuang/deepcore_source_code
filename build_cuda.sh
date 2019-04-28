#!/bin/sh
PWD=`pwd`
NVCC=/usr/local/cuda/bin/nvcc
NVCC_FLAGS="--fatbin -gencode arch=compute_70,code=sm_70 --ptxas-options=-v -Xptxas -disable-optimizer-consts "
# NVCC_FLAGS="$NVCC_FLAGS -G "
WD=deepcore_device/deepcore_device/
SRC=deepcore_device.cu
BUILD_DIR=$PWD/build
TARGET=dc_sm_70.fatbin

#rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR

if [ $B0 ] ;then
# 1, build device code
rm -rf $BUILD_DIR/$TARGET
pushd $WD
$NVCC $NVCC_FLAGS $SRC -o $BUILD_DIR/$TARGET || exit 1
popd

test -f $BUILD_DIR/$TARGET || exit 1

# 2, let fatbin be c string
hexdump  -v -e '1/8 "0x%016x," "\n"' $BUILD_DIR/$TARGET  | \
        paste -sd '   \n' -   > deepcore/include/dev/fftconv/kbin_sm70.h
fi

# 3, build deepcore
if [ $B1 ] ;then
WD=deepcore
CC=gcc
CFLAGS=" -I/usr/local/cuda/include -std=c99 -Wall -O2 -shared -fPIC "
#CFLAGS=" $CFLAGS -DDC_VERBOSE"
LDFLAGS=" -L/usr/local/cuda/lib64 -lcuda -lcudart "
TARGET=libdeepcore.so

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
CXXFLAGS=" -I$PWD/deepcore/include/ -I/usr/local/cuda/include -Wall -O2 "
LDFLAGS=" -Wl,-rpath,$BUILD_DIR -lm  -L$BUILD_DIR -ldeepcore -L/usr/local/cuda/lib64 -lcuda -lcudart "
SRC=fftconv.cpp
TARGET=fftconv
pushd $WD

$CXX $CXXFLAGS $SRC $LDFLAGS -o $BUILD_DIR/$TARGET

popd

