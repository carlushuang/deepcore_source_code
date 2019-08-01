
#include "hip/hip_runtime.h"
#include"cuintrin.h"
#include "half/half.h"
#include "vop.h"

#define FFTCONV_CONJ
#define FFTCONV_CONJ_OMEGA

extern "C"
{

#include"fft/fft.h"
#include"blas/sblas/scgemm.h"

}
