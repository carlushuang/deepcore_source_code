#ifndef __cuda_ctx_h__
#define __cuda_ctx_h__

#include"../idc_status.h"
#include"../../include/fft/fft_rf.h"
#include"cuda_kernel.h"

static const uint16_t g_fftco[]={0,4,12,28,60,124};

typedef struct cuda_context{
#ifdef __HIPCC__
    hipCtx_t        ctx;
    hipModule_t     module_fftconv;
    hipModule_t     module_blas;
    hipDeviceptr_t  d_RF;
    hipDevice_t     dev;
    int             dev_id;
#else
    CUcontext   ctx;
    CUmodule    module_fftconv;
    CUmodule    module_blas;
    CUdeviceptr d_RF;
    CUdevice    dev;
#endif
    int         status;
    int         arch;
    int         n_sm;
    int         cmemnb;
    int         max_smemnb_per_block;
} cuda_context_t;

__local_func int  cuda_context_create( cuda_context_t*, char* );
__local_func int  cuda_context_get_current( const cuda_context_t*, int );
__local_func void cuda_context_release( cuda_context_t* );
#ifdef __HIPCC__
INLINE void cuda_context_bind( const cuda_context_t* p ){ hipCtxSetCurrent(p->ctx); }
INLINE void cuda_context_unbind(){ hipCtxPopCurrent(NULL); }
INLINE void cuda_context_create_kernel( cuda_kernel_t* p_kernel, hipModule_t module, const char* p_name )
{
    hipModuleGetFunction( &p_kernel->id, module, p_name );
    p_kernel->smemnb=0;
    p_kernel->extra[0]=(void*)HIP_LAUNCH_PARAM_BUFFER_POINTER;
    p_kernel->extra[1]=(void*)p_kernel->args;
    p_kernel->extra[2]=(void*)HIP_LAUNCH_PARAM_BUFFER_SIZE;
    p_kernel->extra[3]=(void*)&p_kernel->arg_size;
    p_kernel->extra[4]=(void*)HIP_LAUNCH_PARAM_END;
}
#else
INLINE void cuda_context_bind( const cuda_context_t* p ){ cuCtxSetCurrent(p->ctx); }
INLINE void cuda_context_unbind(){ cuCtxPopCurrent(NULL); }
INLINE void cuda_context_create_kernel( cuda_kernel_t* p_kernel, CUmodule module, const char* p_name )
{
    cuModuleGetFunction( &p_kernel->id, module, p_name );
    p_kernel->smemnb=0;
    p_kernel->extra[0]=(void*)CU_LAUNCH_PARAM_BUFFER_POINTER;
    p_kernel->extra[1]=(void*)p_kernel->args;
    p_kernel->extra[2]=(void*)CU_LAUNCH_PARAM_BUFFER_SIZE;
    p_kernel->extra[3]=(void*)&p_kernel->arg_size;
    p_kernel->extra[4]=(void*)CU_LAUNCH_PARAM_END;
}
#endif

#endif
