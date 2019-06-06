#pragma warning( disable:4996 )
#include"../../include/cuda/cuda_context.h"

#ifdef __HIPCC__
static const unsigned int kbin_fftconv_gfx906[]=
{
#include"../../include/dev/fftconv/kbin_gfx906.h"
};
static const unsigned int* p_devbin[][4]=
{
    { kbin_fftconv_gfx906 },
};
#else
static const unsigned int kbin_fftconv_sm50[]=
{
#include"../../include/dev/fftconv/kbin_sm50.h"
};
static const unsigned int kbin_fftconv_sm52[]=
{
#include"../../include/dev/fftconv/kbin_sm52.h"
};
static const unsigned int kbin_fftconv_sm60[]=
{
#include"../../include/dev/fftconv/kbin_sm60.h"
};
static const unsigned int kbin_fftconv_sm61[]=
{
#include"../../include/dev/fftconv/kbin_sm61.h"
};
static const unsigned int kbin_fftconv_sm70[]=
{
#include"../../include/dev/fftconv/kbin_sm70.h"
};

static const unsigned int kbin_blas_sm50[]=
{
#include"../../include/dev/blas/kbin_sm50.h"
};
static const unsigned int kbin_blas_sm52[]=
{
#include"../../include/dev/blas/kbin_sm52.h"
};
static const unsigned int kbin_blas_sm60[]=
{
#include"../../include/dev/blas/kbin_sm60.h"
};
static const unsigned int kbin_blas_sm61[]=
{
#include"../../include/dev/blas/kbin_sm61.h"
};
static const unsigned int kbin_blas_sm70[]=
{
#include"../../include/dev/blas/kbin_sm70.h"
};

static const unsigned int* p_devbin[][4]=
{
    { kbin_fftconv_sm50, kbin_blas_sm50 },
    { kbin_fftconv_sm52, kbin_blas_sm52 },
    { kbin_fftconv_sm61, kbin_blas_sm61 },
    { kbin_fftconv_sm60, kbin_blas_sm60 },
    { kbin_fftconv_sm70, kbin_blas_sm70 }
};
#endif

/****************************************************************************************************************************************************************
=================================================================================================================================================================
****************************************************************************************************************************************************************/

__local_func int cuda_context_create( cuda_context_t* p_ctx, char* p_temp )
{
#ifdef __HIPCC__
    int i, n, p, q;
    hipCtxGetCurrent(&p_ctx->ctx);
    p_ctx->status=0;
    if(p_ctx!= NULL){ p_ctx->status=1; }
    hipDevicePrimaryCtxRetain(&p_ctx->ctx, p_ctx->dev);
    //hipDevicePrimaryCtxGetState();
    hipCtxPushCurrent(p_ctx->ctx );

    if(hipModuleLoadData( &p_ctx->module_fftconv, p_devbin[0][0] ) != hipSuccess){
        printf("fail to load fftconv module\n");
        return idc_error_invalid_value;
    }
    n=sizeof(g_fftco)/sizeof(g_fftco[0])-1;
    if(hipMalloc(&p_ctx->d_RF, g_fftco[n]*8)!=hipSuccess){
        hipModuleUnload( p_ctx->module_fftconv );
        hipDevicePrimaryCtxRelease(p_ctx->dev);
        p_ctx->ctx=NULL;
        return idc_error_out_of_memory;
    }
    for( p=g_fftco[(i=0)]; i<n; p=q ){
        q=g_fftco[++i];
        idc_fft_calcRF( ((float2*)p_temp)+p, q-p, 1.0/(q-p) );
    }
    hipMemcpyHtoD( p_ctx->d_RF, p_temp, g_fftco[n]*8 );
    hipDeviceGetAttribute( &p_ctx->n_sm                , hipDeviceAttributeMultiprocessorCount      , p_ctx->dev_id );
    hipDeviceGetAttribute( &p_ctx->cmemnb              , hipDeviceAttributeTotalConstantMemory      , p_ctx->dev_id );
    hipDeviceGetAttribute( &p_ctx->max_smemnb_per_block, hipDeviceAttributeMaxSharedMemoryPerBlock  , p_ctx->dev_id );
    hipCtxPopCurrent(&p_ctx->ctx);
    return idc_success;
#else
    int i, n, p, q; 
    cuDriverGetVersion(&i);
    if(i<9010)
        return idc_error_invalid_driver;
    cuCtxGetCurrent( &p_ctx->ctx );
    p_ctx->status=0;
    if(p_ctx!= NULL){ p_ctx->status=1; }
    cuDevicePrimaryCtxRetain( &p_ctx->ctx, p_ctx->dev );
#if CUDA_ARCH >= 70
    cuCtxSetCacheConfig (CU_FUNC_CACHE_PREFER_SHARED);
#endif
    cuDevicePrimaryCtxSetFlags( p_ctx->dev, CU_CTX_SCHED_AUTO|CU_CTX_MAP_HOST|CU_CTX_LMEM_RESIZE_TO_MAX );
    cuCtxPushCurrent( p_ctx->ctx );
    switch(p_ctx->arch)
    {
    case 50: i=0; break;
    case 52: i=1; break;
    case 61: i=2; break;
    case 60: i=3; break;
    case 70: i=4; break;
    }    
    cuModuleLoadFatBinary( &p_ctx->module_fftconv, p_devbin[i][0] );
    cuModuleLoadFatBinary( &p_ctx->module_blas   , p_devbin[i][1] );
    n=sizeof(g_fftco)/sizeof(g_fftco[0])-1;
    if(cuMemAlloc(&p_ctx->d_RF, g_fftco[n]*8)!=CUDA_SUCCESS){
        cuModuleUnload( p_ctx->module_fftconv );
        cuModuleUnload( p_ctx->module_blas    );
        cuDevicePrimaryCtxRelease(p_ctx->dev);
        p_ctx->ctx=NULL;
        return idc_error_out_of_memory;
    }
    for( p=g_fftco[(i=0)]; i<n; p=q ){
        q=g_fftco[++i];
        idc_fft_calcRF( ((float2*)p_temp)+p, q-p, 1.0/(q-p) );
    }
    cuMemcpyHtoD( p_ctx->d_RF, p_temp, g_fftco[n]*8 );
    cuDeviceGetAttribute( &p_ctx->n_sm                , CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT       , p_ctx->dev );
    cuDeviceGetAttribute( &p_ctx->cmemnb              , CU_DEVICE_ATTRIBUTE_TOTAL_CONSTANT_MEMORY      , p_ctx->dev );
    cuDeviceGetAttribute( &p_ctx->max_smemnb_per_block, CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK, p_ctx->dev );
    cuCtxPopCurrent(NULL);
    return idc_success;
#endif
}
__local_func int cuda_context_get_current( const cuda_context_t* p_ctx, int n_devices )
{
#ifdef __HIPCC__
    hipCtx_t  ctx;
    int i=n_devices-1;
    do{
        hipCtxGetCurrent( &ctx );
        if(p_ctx[i].ctx==ctx) break;
    }while((--i)>=0);
    return (i<n_devices?i:-1);
#else
    CUcontext ctx;
    int i=n_devices-1;
    do{
        cuCtxGetCurrent( &ctx );
        if(p_ctx[i].ctx==ctx) break;
    }while((--i)>=0);
    return (i<n_devices?i:-1);
#endif
}
__local_func void cuda_context_release( cuda_context_t* p_ctx )
{
#ifdef __HIPCC__
    if(p_ctx->ctx!=NULL){
        cuda_context_bind( p_ctx );
        hipFree( p_ctx->d_RF ); 
        hipModuleUnload( p_ctx->module_fftconv );
        hipModuleUnload( p_ctx->module_blas    );
        if(p_ctx->status==0){
            hipDevicePrimaryCtxRelease( p_ctx->dev );
        }
        p_ctx->ctx=NULL;
    }
#else
    if(p_ctx->ctx!=NULL){
        cuda_context_bind( p_ctx );
        cuMemFree( p_ctx->d_RF ); 
        cuModuleUnload( p_ctx->module_fftconv );
        cuModuleUnload( p_ctx->module_blas    );
        if(p_ctx->status==0){
            cuDevicePrimaryCtxRelease( p_ctx->dev );
        }
        p_ctx->ctx=NULL;
    }
#endif
}
