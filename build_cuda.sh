#!/bin/sh
PWD=`pwd`
NVCC=/usr/local/cuda/bin/nvcc
NVCC_FLAGS="--fatbin -arch=sm_70 --ptxas-options=-v -Xptxas -disable-optimizer-consts "
WD=deepcore_device/deepcore_device/
SRC=deepcore_device.cu
BUILD_DIR=$PWD/build
TARGET=dc_sm_70.fatbin

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR

#if [ ] ;then
# 1, build device code
pushd $WD
$NVCC $NVCC_FLAGS $SRC -o $BUILD_DIR/$TARGET
popd

test -f $BUILD_DIR/$TARGET || exit 1

# 2, let fatbin be c string
hexdump  -v -e '1/8 "0x%016x," "\n"' $BUILD_DIR/$TARGET  | \
        paste -sd '   \n' -   > deepcore/include/dev/fftconv/kbin_sm70.h
#fi

# 3, build deepcore
WD=deepcore
CC=gcc
CFLAGS=" -I/usr/local/cuda/include -std=c99 -Wall -O2 -shared -fPIC "
LDFLAGS=" -L/usr/local/cuda/lib64 -lcuda -lcudart "
TARGET=libdeepcore.so
pushd $WD

SRC=$(find ./  -type f -name "*.c")
echo $SRC

$CC $CFLAGS $SRC $LDFLAGS -o  $BUILD_DIR/$TARGET


popd
