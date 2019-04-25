#ifndef __activation_dummy__
#define __activation_dummy__

__device__ __forceinline__ float s_relu( float x ) {return 0;}
__device__ __forceinline__ float2 s_relu_x2( float2& a ) {return make_float2(0,0);}
__device__ __forceinline__ float s_drelu( float x ) {return 0;}



#endif
