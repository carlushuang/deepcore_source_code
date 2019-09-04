#ifndef __cuda_ctx_h__
#define __cuda_ctx_h__

#include"../idc_status.h"
#include"../../include/fft/fft_rf.h"
#include"cuda_kernel.h"
#include"../idc_string.h"

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
#include <string>
#include <unordered_map>
INLINE void cuda_context_bind( const cuda_context_t* p ){ hipCtxSetCurrent(p->ctx); }
INLINE void cuda_context_unbind(){ hipCtxPopCurrent(NULL); }
INLINE void warp_function(hipFunction_t *function, const char * p_name){
    static hipModule_t module;
    static int mod_inited=0;
    static std::unordered_map<std::string, hipFunction_t> t_map;
    printf("[warp:%s]\n",p_name);

    hipError_t rtn;
    if(!mod_inited){
        rtn = hipModuleLoad(&module, "kbin_gfx9.co");
        if(rtn != hipSuccess){
            printf("fail to get module, err:%d\n",(int)rtn);
            return;
        }
        mod_inited = 1;
    }
    std::string kname(p_name);
    if(t_map.count(kname)){
        *function = t_map[kname];
    }
    else{
        hipFunction_t func;
        rtn = hipModuleGetFunction( &func, module, p_name );
        if(rtn != hipSuccess){
            printf("fail to get kernel %s, err:%d\n",p_name,(int)rtn);
            return ;
        }
        t_map[kname] = func;
        *function = func;
    }
}
INLINE void cuda_context_create_kernel( cuda_kernel_t* p_kernel, hipModule_t module, const char* p_name )
{
    hipError_t rtn;
    if(idc_strcmp(p_name, "dk_sfft32x32_r2c_split_perm_pad")==0){
        warp_function(&p_kernel->id_wrap, p_name);
    }
    else if (idc_strcmp(p_name, "dk_scgemm_64x64")==0){
        warp_function(&p_kernel->id_wrap, p_name);
    }
    else if(idc_strcmp(p_name, "dk_sfft32x32_r2c_perm_s5")==0){
        warp_function(&p_kernel->id_wrap, p_name);
    }
    else
        p_kernel->id_wrap = NULL;

    rtn = hipModuleGetFunction( &p_kernel->id, module, p_name );
    if(rtn != hipSuccess){
        printf("fail to get kernel %s, err:%d\n",p_name,(int)rtn);
    }


    idc_strncpy(p_kernel->kernel_name, p_name+3, KER_NAME_LEN);
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
    CUresult rtn;
    rtn = cuModuleGetFunction( &p_kernel->id, module, p_name );
    if(rtn != CUDA_SUCCESS){
        printf("fail to get kernel %s, err:%d\n",p_name,(int)rtn);
    }
    idc_strncpy(p_kernel->kernel_name, p_name+3, KER_NAME_LEN);
    p_kernel->smemnb=0;
    p_kernel->extra[0]=(void*)CU_LAUNCH_PARAM_BUFFER_POINTER;
    p_kernel->extra[1]=(void*)p_kernel->args;
    p_kernel->extra[2]=(void*)CU_LAUNCH_PARAM_BUFFER_SIZE;
    p_kernel->extra[3]=(void*)&p_kernel->arg_size;
    p_kernel->extra[4]=(void*)CU_LAUNCH_PARAM_END;
}
#endif

#endif
