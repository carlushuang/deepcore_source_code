#ifndef __cuda_kernel_h__
#define __cuda_kernel_h__

#include<stdint.h>
#ifdef __HIPCC__
#include <hip/hip_runtime.h>
#include <hip/hip_vector_types.h>
#else
#include<cuda.h>
#include<vector_types.h>
#endif
#include"../idc_macro.h"
#include"../idc_argmask.h"
#include <stdio.h>
#ifdef DC_VERBOSE
#define CU_WARN(call)           \
    do{                         \
        CUresult re = call;     \
        if(re != CUDA_SUCCESS){ \
            printf("CU error(%d), "#call"\n\t\t%s:%d\n", re, __FILE__, __LINE__);\
        }                       \
    }while(0)
#endif
#define KER_NAME_LEN 48
typedef struct cuda_kernel{
#ifdef __HIPCC__
    hipFunction_t id;
#else
    CUfunction id;
#endif
    char       kernel_name[KER_NAME_LEN];
    uint32_t   gdx;
    uint32_t   gdy;
    uint32_t   gdz;
    ushort2    block;
    uint32_t   smemnb;
    uint32_t   arg_size;
    void*      extra[5];
    uint8_t    arg_ofs[16];
    uint8_t    args[128];
} cuda_kernel_t;

INLINE void cuda_kernel_sao( cuda_kernel_t* p, uint32_t mask )
{   
    uint32_t i=0, ofs=0, k=mask;
    do{
        p->arg_ofs[i++]=(uint8_t)ofs;
        if((k&0x3)==PA){
#ifdef __HIPCC__
            ofs=IDC_AFFIS(ofs,__alignof(hipDeviceptr_t)); ofs+=sizeof(hipDeviceptr_t); 
#else
            ofs=IDC_AFFIS(ofs,__alignof(CUdeviceptr)); ofs+=sizeof(CUdeviceptr); 
#endif
        } else {
            ofs=IDC_AFFIS(ofs,__alignof(int32_t)); ofs+=__alignof(int32_t);
        }
    }while((k>>=2)!=0);
    p->arg_size=ofs;
}
INLINE void cuda_kernel_set_smemnb( cuda_kernel_t* p_kernel, uint32_t nb )
{
    p_kernel->smemnb=nb;
}
INLINE void cuda_kernel_sgl( cuda_kernel_t* p_kernel, uint32_t gdx, uint32_t gdy, uint32_t gdz )
{
    p_kernel->gdx=gdx; p_kernel->gdy=gdy; p_kernel->gdz=gdz;
#ifdef DC_VERBOSE
    printf("  gri:%dx%dx%d\n",gdx,gdy,gdz);
#endif
}
INLINE void cuda_kernel_sbl( cuda_kernel_t* p_kernel, uint32_t bdx, uint32_t bdy )
{
    p_kernel->block.x=bdx; p_kernel->block.y=bdy;
#ifdef DC_VERBOSE
    printf("  blk:%dx%dx%d\n",bdx,bdy,1);
#endif
}
#ifdef __HIPCC__
INLINE void cuda_kernel_sep_ptr( cuda_kernel_t* p_kernel, int i, hipDeviceptr_t p )
{
    *((hipDeviceptr_t*)&p_kernel->args[p_kernel->arg_ofs[i]])=p;
}
#else
INLINE void cuda_kernel_sep_ptr( cuda_kernel_t* p_kernel, int i, CUdeviceptr p )
{
    *((CUdeviceptr*)&p_kernel->args[p_kernel->arg_ofs[i]])=p;
}
#endif
INLINE void cuda_kernel_sep_i32( cuda_kernel_t* p_kernel, int i, int p )
{
    *((int*)&p_kernel->args[p_kernel->arg_ofs[i]])=p;
#ifdef DC_VERBOSE
    printf("  arg, idx:%d, int:%d\n", i, p);
#endif
}
INLINE void cuda_kernel_sep_f32( cuda_kernel_t* p_kernel, int i, float p )
{
    *((float*)&p_kernel->args[p_kernel->arg_ofs[i]])=p;
#ifdef DC_VERBOSE
    printf("  arg, idx:%d, flt:%f\n", i, p);
#endif
}
#ifdef __HIPCC__
INLINE void cuda_kernel_launch( cuda_kernel_t* p, hipStream_t s )
{
    hipError_t rtn = hipModuleLaunchKernel( p->id, p->gdx, p->gdy, p->gdz, p->block.x, p->block.y, 1, p->smemnb, s, NULL, (void**)p->extra );
    if(rtn != hipSuccess){
        printf("fail to get kernel %s, err:%d\n",p->kernel_name,(int)rtn);
    }
}
#else
INLINE void cuda_kernel_launch( cuda_kernel_t* p, CUstream s )
{
    CUresult rtn = cuLaunchKernel( p->id, p->gdx, p->gdy, p->gdz, p->block.x, p->block.y, 1, p->smemnb, s, NULL, (void**)p->extra );
    if(rtn != CUDA_SUCCESS){
        printf("fail to launch kernel\n");
    }
}
#endif

#endif
