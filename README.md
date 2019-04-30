# deepcore_source_code

  This iterm is subpart source of deepcore v0.7, include fftconv, cellconvand and gemm operation; remove the conv, reduce ,batch-normalization, pooling and activation operation.

  deepcore is based on CUDA and very fast, if want to use it please do with below:
      0 compile and build the device kernels in deepcore_device,copy the fatBinary to the deepcore/include/dev/*/kbin_sm*.h
      1 compile and build the deepcore to generate deepcore.dll or libdeepcore.so

Note:
    The data struct of deepcore is CNHW(Not NCHW), please see the deepcore doc for more info.# deepcore_source_code

# build from source

currently only the fftconv related kernel is built.
## nvidia/cuda

for nvidia platform, make sure installed cuda 9.0+, and use `build_cuda.sh` to build. the output will in `build/` directory

NOTE: tested under V100(sm70). you need to change to other sm targeting other nv arch.

```
# build device code, host code, example code
B0=1 B1=1 sh build_cuda.sh
# only need to build host code, example
B1=1 sh build_cuda.sh
# only need to build example
sh build_cuda.sh

# then run example code like:
./build/fftconv
```

## amd/rocm
for amd platform, make sure [rocm sw stack]((https://rocm.github.io/ROCmInstall.html)) is installed properly. Then use `build_rocm.sh` to build.

NOTE: tested on gfx906. change to other gfx*** to target other amd gpu arch.

```
# build device code, host code, example code
B0=1 B1=1 sh build_rocm.sh
# only need to build host code, example
B1=1 sh build_rocm.sh
# only need to build example
sh build_rocm.sh

# then run example code like:
./build/fftconv
```