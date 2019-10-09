#ifdef FFTCONV_CONJ
#   ifdef FFTCONV_CONJ_OMEGA
#define FLIP_X_sfft8x8_c2r_grad(x) (x)
#   else
#define FLIP_X_sfft8x8_c2r_grad(x) ((8-x)&7)
#   endif
#else
#define FLIP_X_sfft8x8_c2r_grad(x) ((8-x)&7)
#endif

__global__ void dk_sfft8x8_c2r_grad( 
          float *              d_r, 
    const float2* __restrict__ d_c, 
    const float * __restrict__ d_RF, 
    float scale, unsigned int nx, unsigned int ny )
{   
    __shared__ float smem[16*44];
    __shared__ float2 s_RF[4];
    float2 c[4], d[5];
    const int brev[]={0,2,1,3}; 
    unsigned int bid=blockIdx.x;
    unsigned int tid=threadIdx.x;   
    unsigned int x=tid&7;
    unsigned int y=tid>>3;
    unsigned int icell=(bid<<4)+y;
    unsigned int flip_x=FLIP_X_sfft8x8_c2r_grad(x);
    d_r+=icell*ny*nx+flip_x;
    d_c+=icell*48+x;
    if(y==0){ ((float*)s_RF)[x]=d_RF[x]; }
#pragma unroll
    for( int i=0; i<5; ++i ){ d[i]=d_c[i*8]; }
    __syncthreads();
    s_hifft8( c, d, &smem[y*44], s_RF, brev, x );
    s_vifft8( d, c, s_RF, brev, x );
    if(flip_x<nx){
    #pragma unroll
        for( int i=0; i<4; ++i ){
            if((2*i+0)<ny){ d_r[0]=scale*d[i].x; d_r+=nx; }
            if((2*i+1)<ny){ d_r[0]=scale*d[i].y; d_r+=nx; }
        }
    }
}
__global__ void dk_sfft8x8_c2r_grad_s3( 
          float *              d_r, 
    const float2* __restrict__ d_c, 
    const float * __restrict__ d_RF, float scale )
{   
    __shared__ float smem[16*44];
    __shared__ float2 s_RF[4];
    float2 c[4], d[5];
    const int brev[]={0,2,1,3}; 
    unsigned int bid=blockIdx.x;
    unsigned int tid=threadIdx.x;
    unsigned int x=tid&7;
    unsigned int y=tid>>3;
    d_r+=bid*144+tid;
    d_c+=((bid<<4)+y)*48+x;
    if(y==0){ ((float*)s_RF)[x]=d_RF[x]; }
#pragma unroll
    for( int i=0; i<5; ++i ){ d[i]=d_c[i*8]; }
    __syncthreads();
    s_hifft8( c, d, &smem[y*44], s_RF, brev, x );
    s_vifft8( d, c, s_RF, brev, x );
    unsigned int flip_x=FLIP_X_sfft8x8_c2r_grad(x);
    float* spr=&smem[y*9+flip_x];
    if(flip_x<3){
        spr[0]=d[0].x;
        spr[3]=d[0].y;
        spr[6]=d[1].x;
    } __syncthreads();
    d_r[0]=scale*smem[tid];
    if(tid<16){ d_r[128]=scale*smem[128+tid]; }
}