#ifndef __platform_h__
#define __platform_h__

#ifdef __HIPCC__
#include <hip/hip_runtime.h>
#else
#include<cuda.h>
#endif
#include"../idc_macro.h"
#include"../idc_string.h"

typedef struct cuda_platform{
#ifdef __HIPCC__
    hipDevice_t devices[IDC_MAX_DEVICES_PER_NODE];
#else
    CUdevice devices[IDC_MAX_DEVICES_PER_NODE];
#endif
    int      nSM    [IDC_MAX_DEVICES_PER_NODE];
    int      arch   [IDC_MAX_DEVICES_PER_NODE];
    int      n_devices;
} cuda_platform_t;

__local_func int cuda_platform_init( cuda_platform_t* );

#endif
