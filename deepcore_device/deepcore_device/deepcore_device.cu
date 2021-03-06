#define M_CONV    0
#define M_BLAS    0
#define M_FFTCONV 1
#define CUDA_ARCH 70

#include"half/half.h"
#include"cuintrin.h"
#include"vop.h"

extern "C"
{
#if M_BLAS
#include"blas/blas.h"
#elif M_FFTCONV
#include"fft/fft.h"
#include"blas/sblas/scgemm.h"
#endif
}
