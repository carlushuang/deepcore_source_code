#!/bin/sh
NVCC=/usr/local/cuda/bin/nvcc
NVCC_FLAGS="--fatbin -arch=sm_70 --ptxas-options=-v -Xptxas -disable-optimizer-consts "
WD=deepcore_device/deepcore_device/
SRC=deepcore_device.cu
TARGET=dc_sm_70.fatbin

# 1, build device code
pushd $WD
rm -rf $TARGET
$NVCC $NVCC_FLAGS $SRC -o $TARGET
popd

test -f $WD/$TARGET || exit 1

# 2, let fatbin be c string
hexdump  -v -e '1/8 "0x%016x," "\n"' $WD/$TARGET  | \
        paste -sd '   \n' -   > deepcore/include/dev/fftconv/kbin_sm70.h
