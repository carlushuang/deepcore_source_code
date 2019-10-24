
__device__ __forceinline__ static void s_preproc_128x128( float2* c, float* smem, 
    const float2* __restrict__ d_src, const float2* d_RF, unsigned int i, unsigned int tx, unsigned int ty )
{
    float2 o[16];
    unsigned int x=i&31;
    unsigned int y=i>>5;
    unsigned int u=x?( 64-x):x;
    unsigned int v=y?(128-y):y;
    unsigned int p=y;
    unsigned int q=v;
    float2 RF=d_RF[x];
#pragma unroll
    for( int k=0; k<16; k+=2 )
    {    
        if(!((x==0)&(y>64))){
            o[k+0]=d_src[y*65   +x];
            o[k+1]=d_src[v*65+64-x];
            s_preproc(o[k],o[k+1],RF);          
            smem[y*64+x]=o[k+0].x;
            smem[v*64+u]=o[k+1].x;
        }
        v=128-(y+=16);
    } __syncthreads();
    LOAD16(c,&smem[i],512,.x)
    __syncthreads();

#pragma unroll
    for( int k=0; k<16; k+=2 ){
        if(!((x==0)&(p>64))){
            smem[p*64+x]=o[k+0].y;
            smem[q*64+u]=o[k+1].y;
        }
        q=128-(p+=16);
    } __syncthreads();
    LOAD16(c,&smem[i],512,.y)

    if(i<=64){
        v=i?(128-i):i;
        o[0]=d_src[i*65+32];
        o[1]=d_src[v*65+32];
        s_preproc(o[0],o[1],make_float2(0.f,-1.f));         
        ((float2*)smem)[4096+i]=o[0];
        ((float2*)smem)[4096+v]=o[1];
    } __syncthreads();

    if(tx==32){
        for( int k=0; k<16; ++k ){ c[k]=((float2*)smem)[4096+k*8+ty]; }
    } __syncthreads();
}

__device__ __forceinline__ void sfft128x128_c2r_store( float* dst, const float* null, float2* c, int x, int y, int nx, int ny, float alpha )
{
#ifdef FFTCONV_CONJ
    int dx=x<<1;
    int x0=dx;
    int x1=dx+1;
    int p=y; 
    for( int k=0; k<16; ++k ){
        if(p<ny){
            if(x0<nx){ dst[p*nx+x0]=alpha*c[k].x; }
            if(x1<nx){ dst[p*nx+x1]=alpha*c[k].y; }
        }
        p+=8;
    }
#else
    int dx=x<<1;
    int x0=dx?(128-dx):dx;
    int x1=127-dx;
    int p=y?(128-y):y;
#pragma unroll
    for( int k=0; k<16; ++k ){
        if(p<ny){
            if(x0<nx){ dst[p*nx+x0]=alpha*c[k].x; }
            if(x1<nx){ dst[p*nx+x1]=alpha*c[k].y; }
        }
        p=128-(y+=8);
    }
#endif
}
__device__ __forceinline__ void sfft128x128_c2r_store_relu( float* dst, const float* null, const float2* c, int x, int y, int nx, int ny, float alpha )
{
    int dx=x<<1;
    int x0=dx?(128-dx):dx;
    int x1=127-dx;
    int p=y?(128-y):y;
#pragma unroll
    for( int k=0; k<16; ++k ){
        if(p<ny){
            if(x0<nx){ dst[p*nx+x0]=s_relu(alpha*c[k].x); }
            if(x1<nx){ dst[p*nx+x1]=s_relu(alpha*c[k].y); }
        }
        p=128-(y+=8);
    }
}
__device__ __forceinline__ void sfft128x128_c2r_store_bias( float* dst, const float* bias, const float2* c, int x, int y, int nx, int ny, float alpha )
{
    int dx=x<<1;
    int x0=dx?(128-dx):dx;
    int x1=127-dx;
    int p=y?(128-y):y;
    float b=*bias;
#pragma unroll
    for( int k=0; k<16; ++k ){
        if(p<ny){
            if(x0<nx){ dst[p*nx+x0]=alpha*c[k].x+b; }
            if(x1<nx){ dst[p*nx+x1]=alpha*c[k].y+b; }
        }
        p=128-(y+=8);
    }
}
__device__ __forceinline__ void sfft128x128_c2r_store_bias_relu( float* dst, const float* bias, const float2* c, int x, int y, int nx, int ny, float alpha )
{
    int dx=x<<1;
    int x0=dx?(128-dx):dx;
    int x1=127-dx;
    int p=y?(128-y):y;
    float b=*bias;
#pragma unroll
    for( int k=0; k<16; ++k ){
        if(p<ny){
            if(x0<nx){ dst[p*nx+x0]=s_relu(alpha*c[k].x+b); }
            if(x1<nx){ dst[p*nx+x1]=s_relu(alpha*c[k].y+b); }
        }
        p=128-(y+=8);
    }
}
__device__ __forceinline__ void sfft128x128_c2r_store_drelu( float* dst, const float* a, const float2* c, int x, int y, int nx, int ny, float alpha )
{
    int dx=x<<1;
    int x0=dx?(128-dx):dx;
    int x1=127-dx;
    int p=y?(128-y):y;
#pragma unroll
    for( int k=0; k<16; ++k ){
        if(p<ny){
            if(x0<nx){ dst[p*nx+x0]=alpha*c[k].x*s_drelu(a[p*nx+x0]); }
            if(x1<nx){ dst[p*nx+x1]=alpha*c[k].y*s_drelu(a[p*nx+x1]); }
        }
        p=128-(y+=8);
    }
}
__device__ __forceinline__ void sfft128x128_c2r_store_xdrv( float* dst, const float* da, const float2* c, int x, int y, int nx, int ny, float alpha )
{
    int dx=x<<1;
    int x0=dx?(128-dx):dx;
    int x1=127-dx;
    int p=y?(128-y):y;
#pragma unroll
    for( int k=0; k<16; ++k ){
        if(p<ny){
            if(x0<nx){ dst[p*nx+x0]=alpha*c[k].x*da[p*nx+x0]; }
            if(x1<nx){ dst[p*nx+x1]=alpha*c[k].y*da[p*nx+x1]; }
        }
        p=128-(y+=8);
    }
}

#define sfft128x128_c2r(dir,suffix)        \
__global__ void dk_sfft128x128_c2r##suffix(\
          float *              d_r  ,\
    const float2* __restrict__ d_c  ,\
    const float2* __restrict__ d_RF ,\
    const float * __restrict__ d_x  ,\
    float                      alpha,\
    unsigned int               nx   ,\
    unsigned int               ny   ,\
    unsigned int               ldr ){\
    __shared__ float smem[128*72];\
    float2 c[16], RF[15], temp;   \
    unsigned int bx=blockIdx.x;   \
    unsigned int by=blockIdx.y;   \
    unsigned int tid=threadIdx.x; \
    unsigned int x=tid&63;        \
    unsigned int y=tid>>6;        \
    d_c+=(by*gridDim.x+bx)*65*128;\
    s_preproc_128x128( c, smem, d_c, d_RF, tid, x, y );\
    d_r+=by*ldr+bx*ny*nx;\
    if(dir==0){ d_x+=by; } else\
    if(dir==1){ d_x+=by*ldr+bx*ny*nx; }\
    unsigned int u=tid&7;     \
    unsigned int v=tid>>3;    \
    float* spx=&smem[ 72*y+x];\
    float* spy=&smem[576*y+x];\
    float* spu=&smem[72*v+u]; \
    float* spv=&smem[576*y+65*u+((v&7)<<3)];\
    RF[0]=d_RF[y];   \
    iCALRF16(RF)     \
    FFT16(c,i)       \
    MRF16(c,RF)      \
    RF[0]=d_RF[u<<1];\
    iCALRF8(RF)      \
    PERMUTE_S16_L8x2(spx,spy,c,576,4608,72,0xf)\
    FFT8(&c[0],i)\
    FFT8(&c[8],i)\
    PERMUTE8x2(spx,spu,c,576,1152,4608,8,0xf)\
    FFT8(&c[0],i) \
    FFT8(&c[8],i) \
    MRF8(&c[0],RF)\
    MRF8(&c[8],RF)\
    PERMUTE8x2(spy,spv,c,4608,65,4608,1,0xf)\
    FFT8(&c[0],i)\
    FFT8(&c[8],i)\
    PERMUTE_S8x2_L16(spu,spx,c,4608,8,576,0x7)\
    sfft128x128_c2r_store##suffix( d_r, d_x, c, x, y, nx, ny, alpha );\
}

sfft128x128_c2r(-1,)
sfft128x128_c2r( 0,_relu)
sfft128x128_c2r( 0,_bias)
sfft128x128_c2r( 0,_bias_relu)
sfft128x128_c2r( 1,_drelu)
sfft128x128_c2r( 1,_xdrv)

__global__ void dk_sfft128x128_c2r_grad( float* d_r, const float2* __restrict__ d_c, 
    const float2* __restrict__ d_RF, float scale, unsigned int nx, unsigned int ny )
{
    __shared__ float smem[128*72];
    float2 c[16], RF[15], temp; 
    unsigned int bid=blockIdx.x;
    unsigned int tid=threadIdx.x;
    unsigned int x=tid&63;
    unsigned int y=tid>>6;  
    d_c+=bid*65*128;
    s_preproc_128x128( c, smem, d_c, d_RF, tid, x, y );
    d_r+=bid*ny*nx;
    unsigned int u=tid&7;
    unsigned int v=tid>>3;
    float* spx=&smem[ 72*y+x];
    float* spy=&smem[576*y+x];
    float* spu=&smem[72*v+u];
    float* spv=&smem[576*y+65*u+((v&7)<<3)];
    RF[0]=d_RF[y];
    iCALRF16(RF)
    FFT16(c,i)
    MRF16(c,RF)
    RF[0]=d_RF[u<<1];
    iCALRF8(RF)
    PERMUTE_S16_L8x2(spx,spy,c,576,4608,72,0xf)
    FFT8(&c[0],i)
    FFT8(&c[8],i)
    PERMUTE8x2(spx,spu,c,576,1152,4608,8,0xf)
    FFT8(&c[0],i)
    FFT8(&c[8],i)
    MRF8(&c[0],RF)
    MRF8(&c[8],RF)
    PERMUTE8x2(spy,spv,c,4608,65,4608,1,0xf)
    FFT8(&c[0],i)
    FFT8(&c[8],i)
    PERMUTE_S8x2_L16(spu,spx,c,4608,8,576,0x7)
#ifdef FFTCONV_CONJ
    unsigned int dx=x<<1;
    unsigned int x0=dx;
    unsigned int x1=dx+1;
    unsigned int p=y;
#pragma unroll
    for( int k=0; k<16; ++k ){
        if(p<ny){
            if(x0<nx){ d_r[p*nx+x0]=scale*c[k].x; }
            if(x1<nx){ d_r[p*nx+x1]=scale*c[k].y; }
        }
        p+=8;
    }
#else
    unsigned int dx=x<<1;
    unsigned int x0=dx?(128-dx):dx;
    unsigned int x1=127-dx;
    unsigned int p=y?(128-y):y;
#pragma unroll
    for( int k=0; k<16; ++k ){
        if(p<ny){
            if(x0<nx){ d_r[p*nx+x0]=scale*c[k].x; }
            if(x1<nx){ d_r[p*nx+x1]=scale*c[k].y; }
        }
        p=128-(y+=8);
    }
#endif
}