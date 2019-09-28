#ifndef __fft_h__
#define __fft_h__

#define LOAD4(d,s,n,e){    \
    (d)[0]  e=*((s)+0*(n));\
    (d)[1]  e=*((s)+1*(n));\
    (d)[2]  e=*((s)+2*(n));\
    (d)[3]  e=*((s)+3*(n));\
}

#define LOAD8(d,s,n,e){    \
    (d)[0]  e=*((s)+0*(n));\
    (d)[1]  e=*((s)+1*(n));\
    (d)[2]  e=*((s)+2*(n));\
    (d)[3]  e=*((s)+3*(n));\
    (d)[4]  e=*((s)+4*(n));\
    (d)[5]  e=*((s)+5*(n));\
    (d)[6]  e=*((s)+6*(n));\
    (d)[7]  e=*((s)+7*(n));\
}

#define LOAD4x2(d,s,m,n,e){    \
    (d)[0]  e=*((s)  +0*(n));  \
    (d)[1]  e=*((s)  +1*(n));  \
    (d)[2]  e=*((s)  +2*(n));  \
    (d)[3]  e=*((s)  +3*(n));  \
    (d)[4]  e=*((s)+(m)+0*(n));\
    (d)[5]  e=*((s)+(m)+1*(n));\
    (d)[6]  e=*((s)+(m)+2*(n));\
    (d)[7]  e=*((s)+(m)+3*(n));\
}

#define LOAD16(d,s,n,e){     \
    (d)[ 0]  e=*((s)+ 0*(n));\
    (d)[ 1]  e=*((s)+ 1*(n));\
    (d)[ 2]  e=*((s)+ 2*(n));\
    (d)[ 3]  e=*((s)+ 3*(n));\
    (d)[ 4]  e=*((s)+ 4*(n));\
    (d)[ 5]  e=*((s)+ 5*(n));\
    (d)[ 6]  e=*((s)+ 6*(n));\
    (d)[ 7]  e=*((s)+ 7*(n));\
    (d)[ 8]  e=*((s)+ 8*(n));\
    (d)[ 9]  e=*((s)+ 9*(n));\
    (d)[10]  e=*((s)+10*(n));\
    (d)[11]  e=*((s)+11*(n));\
    (d)[12]  e=*((s)+12*(n));\
    (d)[13]  e=*((s)+13*(n));\
    (d)[14]  e=*((s)+14*(n));\
    (d)[15]  e=*((s)+15*(n));\
}

#define LOAD32(d,s,n,e){     \
    (d)[ 0]  e=*((s)+ 0*(n));\
    (d)[ 1]  e=*((s)+ 1*(n));\
    (d)[ 2]  e=*((s)+ 2*(n));\
    (d)[ 3]  e=*((s)+ 3*(n));\
    (d)[ 4]  e=*((s)+ 4*(n));\
    (d)[ 5]  e=*((s)+ 5*(n));\
    (d)[ 6]  e=*((s)+ 6*(n));\
    (d)[ 7]  e=*((s)+ 7*(n));\
    (d)[ 8]  e=*((s)+ 8*(n));\
    (d)[ 9]  e=*((s)+ 9*(n));\
    (d)[10]  e=*((s)+10*(n));\
    (d)[11]  e=*((s)+11*(n));\
    (d)[12]  e=*((s)+12*(n));\
    (d)[13]  e=*((s)+13*(n));\
    (d)[14]  e=*((s)+14*(n));\
    (d)[15]  e=*((s)+15*(n));\
    (d)[16]  e=*((s)+16*(n));\
    (d)[17]  e=*((s)+17*(n));\
    (d)[18]  e=*((s)+18*(n));\
    (d)[19]  e=*((s)+19*(n));\
    (d)[20]  e=*((s)+20*(n));\
    (d)[21]  e=*((s)+21*(n));\
    (d)[22]  e=*((s)+22*(n));\
    (d)[23]  e=*((s)+23*(n));\
    (d)[24]  e=*((s)+24*(n));\
    (d)[25]  e=*((s)+25*(n));\
    (d)[26]  e=*((s)+26*(n));\
    (d)[27]  e=*((s)+27*(n));\
    (d)[28]  e=*((s)+28*(n));\
    (d)[29]  e=*((s)+29*(n));\
    (d)[30]  e=*((s)+30*(n));\
    (d)[31]  e=*((s)+31*(n));\
}

#define LOAD4x4(d,s,m,n,e){       \
    (d)[ 0]  e=*((s)+0*(m)+0*(n));\
    (d)[ 1]  e=*((s)+0*(m)+1*(n));\
    (d)[ 2]  e=*((s)+0*(m)+2*(n));\
    (d)[ 3]  e=*((s)+0*(m)+3*(n));\
    (d)[ 4]  e=*((s)+1*(m)+0*(n));\
    (d)[ 5]  e=*((s)+1*(m)+1*(n));\
    (d)[ 6]  e=*((s)+1*(m)+2*(n));\
    (d)[ 7]  e=*((s)+1*(m)+3*(n));\
    (d)[ 8]  e=*((s)+2*(m)+0*(n));\
    (d)[ 9]  e=*((s)+2*(m)+1*(n));\
    (d)[10]  e=*((s)+2*(m)+2*(n));\
    (d)[11]  e=*((s)+2*(m)+3*(n));\
    (d)[12]  e=*((s)+3*(m)+0*(n));\
    (d)[13]  e=*((s)+3*(m)+1*(n));\
    (d)[14]  e=*((s)+3*(m)+2*(n));\
    (d)[15]  e=*((s)+3*(m)+3*(n));\
}

#define LOAD8x2(d,s,m,n,e){     \
    (d)[ 0]  e=*((s)    +0*(n));\
    (d)[ 1]  e=*((s)    +1*(n));\
    (d)[ 2]  e=*((s)    +2*(n));\
    (d)[ 3]  e=*((s)    +3*(n));\
    (d)[ 4]  e=*((s)    +4*(n));\
    (d)[ 5]  e=*((s)    +5*(n));\
    (d)[ 6]  e=*((s)    +6*(n));\
    (d)[ 7]  e=*((s)    +7*(n));\
    (d)[ 8]  e=*((s)+(m)+0*(n));\
    (d)[ 9]  e=*((s)+(m)+1*(n));\
    (d)[10]  e=*((s)+(m)+2*(n));\
    (d)[11]  e=*((s)+(m)+3*(n));\
    (d)[12]  e=*((s)+(m)+4*(n));\
    (d)[13]  e=*((s)+(m)+5*(n));\
    (d)[14]  e=*((s)+(m)+6*(n));\
    (d)[15]  e=*((s)+(m)+7*(n));\
}

#define ISTORE4(d,s,n,e){  \
    *((d)+0*(n))=(s)[0]  e;\
    *((d)+1*(n))=(s)[2]  e;\
    *((d)+2*(n))=(s)[1]  e;\
    *((d)+3*(n))=(s)[3]  e;\
}

#define ISTORE8(d,s,n,e){  \
    *((d)+0*(n))=(s)[0]  e;\
    *((d)+1*(n))=(s)[4]  e;\
    *((d)+2*(n))=(s)[2]  e;\
    *((d)+3*(n))=(s)[6]  e;\
    *((d)+4*(n))=(s)[1]  e;\
    *((d)+5*(n))=(s)[5]  e;\
    *((d)+6*(n))=(s)[3]  e;\
    *((d)+7*(n))=(s)[7]  e;\
}

#define ISTORE16(d,s,n,e){   \
    *((d)+ 0*(n))=(s)[ 0]  e;\
    *((d)+ 1*(n))=(s)[ 8]  e;\
    *((d)+ 2*(n))=(s)[ 4]  e;\
    *((d)+ 3*(n))=(s)[12]  e;\
    *((d)+ 4*(n))=(s)[ 2]  e;\
    *((d)+ 5*(n))=(s)[10]  e;\
    *((d)+ 6*(n))=(s)[ 6]  e;\
    *((d)+ 7*(n))=(s)[14]  e;\
    *((d)+ 8*(n))=(s)[ 1]  e;\
    *((d)+ 9*(n))=(s)[ 9]  e;\
    *((d)+10*(n))=(s)[ 5]  e;\
    *((d)+11*(n))=(s)[13]  e;\
    *((d)+12*(n))=(s)[ 3]  e;\
    *((d)+13*(n))=(s)[11]  e;\
    *((d)+14*(n))=(s)[ 7]  e;\
    *((d)+15*(n))=(s)[15]  e;\
}

#define ISTORE32(d,s,n,e){   \
    *((d)+ 0*(n))=(s)[ 0]  e;\
    *((d)+ 1*(n))=(s)[16]  e;\
    *((d)+ 2*(n))=(s)[ 8]  e;\
    *((d)+ 3*(n))=(s)[24]  e;\
    *((d)+ 4*(n))=(s)[ 4]  e;\
    *((d)+ 5*(n))=(s)[20]  e;\
    *((d)+ 6*(n))=(s)[12]  e;\
    *((d)+ 7*(n))=(s)[28]  e;\
    *((d)+ 8*(n))=(s)[ 2]  e;\
    *((d)+ 9*(n))=(s)[18]  e;\
    *((d)+10*(n))=(s)[10]  e;\
    *((d)+11*(n))=(s)[26]  e;\
    *((d)+12*(n))=(s)[ 6]  e;\
    *((d)+13*(n))=(s)[22]  e;\
    *((d)+14*(n))=(s)[14]  e;\
    *((d)+15*(n))=(s)[30]  e;\
    *((d)+16*(n))=(s)[ 1]  e;\
    *((d)+17*(n))=(s)[17]  e;\
    *((d)+18*(n))=(s)[ 9]  e;\
    *((d)+19*(n))=(s)[25]  e;\
    *((d)+20*(n))=(s)[ 5]  e;\
    *((d)+21*(n))=(s)[21]  e;\
    *((d)+22*(n))=(s)[13]  e;\
    *((d)+23*(n))=(s)[29]  e;\
    *((d)+24*(n))=(s)[ 3]  e;\
    *((d)+25*(n))=(s)[19]  e;\
    *((d)+26*(n))=(s)[11]  e;\
    *((d)+27*(n))=(s)[27]  e;\
    *((d)+28*(n))=(s)[ 7]  e;\
    *((d)+29*(n))=(s)[23]  e;\
    *((d)+30*(n))=(s)[15]  e;\
    *((d)+31*(n))=(s)[31]  e;\
}

#define ISTORE4x4(d,s,m,n,e){   \
    *((d)+0*m+0*(n))=(s)[ 0]  e;\
    *((d)+0*m+1*(n))=(s)[ 2]  e;\
    *((d)+0*m+2*(n))=(s)[ 1]  e;\
    *((d)+0*m+3*(n))=(s)[ 3]  e;\
    *((d)+1*m+0*(n))=(s)[ 4]  e;\
    *((d)+1*m+1*(n))=(s)[ 6]  e;\
    *((d)+1*m+2*(n))=(s)[ 5]  e;\
    *((d)+1*m+3*(n))=(s)[ 7]  e;\
    *((d)+2*m+0*(n))=(s)[ 8]  e;\
    *((d)+2*m+1*(n))=(s)[10]  e;\
    *((d)+2*m+2*(n))=(s)[ 9]  e;\
    *((d)+2*m+3*(n))=(s)[11]  e;\
    *((d)+3*m+0*(n))=(s)[12]  e;\
    *((d)+3*m+1*(n))=(s)[14]  e;\
    *((d)+3*m+2*(n))=(s)[13]  e;\
    *((d)+3*m+3*(n))=(s)[15]  e;\
}

#define ISTORE4x2(d,s,m,n,e){  \
    *((d)    +0*(n))=(s)[0]  e;\
    *((d)    +1*(n))=(s)[2]  e;\
    *((d)    +2*(n))=(s)[1]  e;\
    *((d)    +3*(n))=(s)[3]  e;\
    *((d)+(m)+0*(n))=(s)[4]  e;\
    *((d)+(m)+1*(n))=(s)[6]  e;\
    *((d)+(m)+2*(n))=(s)[5]  e;\
    *((d)+(m)+3*(n))=(s)[7]  e;\
}

#define ISTORE8x2(d,s,m,n,e){   \
    *((d)    +0*(n))=(s)[ 0]  e;\
    *((d)    +1*(n))=(s)[ 4]  e;\
    *((d)    +2*(n))=(s)[ 2]  e;\
    *((d)    +3*(n))=(s)[ 6]  e;\
    *((d)    +4*(n))=(s)[ 1]  e;\
    *((d)    +5*(n))=(s)[ 5]  e;\
    *((d)    +6*(n))=(s)[ 3]  e;\
    *((d)    +7*(n))=(s)[ 7]  e;\
    *((d)+(m)+0*(n))=(s)[ 8]  e;\
    *((d)+(m)+1*(n))=(s)[12]  e;\
    *((d)+(m)+2*(n))=(s)[10]  e;\
    *((d)+(m)+3*(n))=(s)[14]  e;\
    *((d)+(m)+4*(n))=(s)[ 9]  e;\
    *((d)+(m)+5*(n))=(s)[13]  e;\
    *((d)+(m)+6*(n))=(s)[11]  e;\
    *((d)+(m)+7*(n))=(s)[15]  e;\
}

#define STORE4(d,s,n,e){   \
    *((d)+0*(n))=(s)[0]  e;\
    *((d)+1*(n))=(s)[1]  e;\
    *((d)+2*(n))=(s)[2]  e;\
    *((d)+3*(n))=(s)[3]  e;\
}

#define STORE4x2(d,s,m,n){  \
    *((d)    +0*(n))=(s)[0];\
    *((d)    +1*(n))=(s)[1];\
    *((d)    +2*(n))=(s)[2];\
    *((d)    +3*(n))=(s)[3];\
    *((d)+(m)+0*(n))=(s)[4];\
    *((d)+(m)+1*(n))=(s)[5];\
    *((d)+(m)+2*(n))=(s)[6];\
    *((d)+(m)+3*(n))=(s)[7];\
}

#define STORE4x4(d,s,m,n,e){    \
    *((d)+0*m+0*(n))=(s)[ 0]  e;\
    *((d)+0*m+1*(n))=(s)[ 1]  e;\
    *((d)+0*m+2*(n))=(s)[ 2]  e;\
    *((d)+0*m+3*(n))=(s)[ 3]  e;\
    *((d)+1*m+0*(n))=(s)[ 4]  e;\
    *((d)+1*m+1*(n))=(s)[ 5]  e;\
    *((d)+1*m+2*(n))=(s)[ 6]  e;\
    *((d)+1*m+3*(n))=(s)[ 7]  e;\
    *((d)+2*m+0*(n))=(s)[ 8]  e;\
    *((d)+2*m+1*(n))=(s)[ 9]  e;\
    *((d)+2*m+2*(n))=(s)[10]  e;\
    *((d)+2*m+3*(n))=(s)[11]  e;\
    *((d)+3*m+0*(n))=(s)[12]  e;\
    *((d)+3*m+1*(n))=(s)[13]  e;\
    *((d)+3*m+2*(n))=(s)[14]  e;\
    *((d)+3*m+3*(n))=(s)[15]  e;\
}

#define STORE8(d,s,n,e){   \
    *((d)+0*(n))=(s)[0]  e;\
    *((d)+1*(n))=(s)[1]  e;\
    *((d)+2*(n))=(s)[2]  e;\
    *((d)+3*(n))=(s)[3]  e;\
    *((d)+4*(n))=(s)[4]  e;\
    *((d)+5*(n))=(s)[5]  e;\
    *((d)+6*(n))=(s)[6]  e;\
    *((d)+7*(n))=(s)[7]  e;\
}

#define STORE8x2(d,s,m,n,e){    \
    *((d)    +0*(n))=(s)[ 0]  e;\
    *((d)    +1*(n))=(s)[ 1]  e;\
    *((d)    +2*(n))=(s)[ 2]  e;\
    *((d)    +3*(n))=(s)[ 3]  e;\
    *((d)    +4*(n))=(s)[ 4]  e;\
    *((d)    +5*(n))=(s)[ 5]  e;\
    *((d)    +6*(n))=(s)[ 6]  e;\
    *((d)    +7*(n))=(s)[ 7]  e;\
    *((d)+(m)+0*(n))=(s)[ 8]  e;\
    *((d)+(m)+1*(n))=(s)[ 9]  e;\
    *((d)+(m)+2*(n))=(s)[10]  e;\
    *((d)+(m)+3*(n))=(s)[11]  e;\
    *((d)+(m)+4*(n))=(s)[12]  e;\
    *((d)+(m)+5*(n))=(s)[13]  e;\
    *((d)+(m)+6*(n))=(s)[14]  e;\
    *((d)+(m)+7*(n))=(s)[15]  e;\
}

#define STORE16(d,s,n,e){    \
    *((d)+ 0*(n))=(s)[ 0]  e;\
    *((d)+ 1*(n))=(s)[ 1]  e;\
    *((d)+ 2*(n))=(s)[ 2]  e;\
    *((d)+ 3*(n))=(s)[ 3]  e;\
    *((d)+ 4*(n))=(s)[ 4]  e;\
    *((d)+ 5*(n))=(s)[ 5]  e;\
    *((d)+ 6*(n))=(s)[ 6]  e;\
    *((d)+ 7*(n))=(s)[ 7]  e;\
    *((d)+ 8*(n))=(s)[ 8]  e;\
    *((d)+ 9*(n))=(s)[ 9]  e;\
    *((d)+10*(n))=(s)[10]  e;\
    *((d)+11*(n))=(s)[11]  e;\
    *((d)+12*(n))=(s)[12]  e;\
    *((d)+13*(n))=(s)[13]  e;\
    *((d)+14*(n))=(s)[14]  e;\
    *((d)+15*(n))=(s)[15]  e;\
}

#define STORE32(d,s,n,e){    \
    *((d)+ 0*(n))=(s)[ 0]  e;\
    *((d)+ 1*(n))=(s)[ 1]  e;\
    *((d)+ 2*(n))=(s)[ 2]  e;\
    *((d)+ 3*(n))=(s)[ 3]  e;\
    *((d)+ 4*(n))=(s)[ 4]  e;\
    *((d)+ 5*(n))=(s)[ 5]  e;\
    *((d)+ 6*(n))=(s)[ 6]  e;\
    *((d)+ 7*(n))=(s)[ 7]  e;\
    *((d)+ 8*(n))=(s)[ 8]  e;\
    *((d)+ 9*(n))=(s)[ 9]  e;\
    *((d)+10*(n))=(s)[10]  e;\
    *((d)+11*(n))=(s)[11]  e;\
    *((d)+12*(n))=(s)[12]  e;\
    *((d)+13*(n))=(s)[13]  e;\
    *((d)+14*(n))=(s)[14]  e;\
    *((d)+15*(n))=(s)[15]  e;\
    *((d)+16*(n))=(s)[16]  e;\
    *((d)+17*(n))=(s)[17]  e;\
    *((d)+18*(n))=(s)[18]  e;\
    *((d)+19*(n))=(s)[19]  e;\
    *((d)+20*(n))=(s)[20]  e;\
    *((d)+21*(n))=(s)[21]  e;\
    *((d)+22*(n))=(s)[22]  e;\
    *((d)+23*(n))=(s)[23]  e;\
    *((d)+24*(n))=(s)[24]  e;\
    *((d)+25*(n))=(s)[25]  e;\
    *((d)+26*(n))=(s)[26]  e;\
    *((d)+27*(n))=(s)[27]  e;\
    *((d)+28*(n))=(s)[28]  e;\
    *((d)+29*(n))=(s)[29]  e;\
    *((d)+30*(n))=(s)[30]  e;\
    *((d)+31*(n))=(s)[31]  e;\
}

#define PERMUTE(R,sst,sld,c,m,n,mask){                      \
    ISTORE##R(sst,c,m,.x)   if(mask&0x1){ __syncthreads(); }\
    LOAD##R(c,sld,n,.x)     if(mask&0x2){ __syncthreads(); }\
    ISTORE##R(sst,c,m,.y)   if(mask&0x4){ __syncthreads(); }\
    LOAD##R(c,sld,n,.y)     if(mask&0x8){ __syncthreads(); }\
}

#define PERMUTE_S16_L4x4(sst,sld,c,k,m,n,mask){             \
    ISTORE16(sst,c,k,.x)    if(mask&0x1){ __syncthreads(); }\
    LOAD4x4(c,sld,m,n,.x)   if(mask&0x2){ __syncthreads(); }\
    ISTORE16(sst,c,k,.y)    if(mask&0x4){ __syncthreads(); }\
    LOAD4x4(c,sld,m,n,.y)   if(mask&0x8){ __syncthreads(); }\
}

#define PERMUTE_S4x4_L16(sst,sld,c,m,n,k,mask){             \
    ISTORE4x4(sst,c,m,n,.x) if(mask&0x1){ __syncthreads(); }\
    LOAD16(c,sld,k,.x)      if(mask&0x2){ __syncthreads(); }\
    ISTORE4x4(sst,c,m,n,.y) if(mask&0x4){ __syncthreads(); }\
    LOAD16(c,sld,k,.y)      if(mask&0x8){ __syncthreads(); }\
}

#define PERMUTE_S8_L4x2(sst,sld,c,k,m,n,mask){              \
    ISTORE8(sst,c,k,.x)     if(mask&0x1){ __syncthreads(); }\
    LOAD4x2(c,sld,m,n,.x)   if(mask&0x2){ __syncthreads(); }\
    ISTORE8(sst,c,k,.y)     if(mask&0x4){ __syncthreads(); }\
    LOAD4x2(c,sld,m,n,.y)   if(mask&0x8){ __syncthreads(); }\
}

#define PERMUTE_S4x2_L8(sst,sld,c,m,n,k,mask){              \
    ISTORE4x2(sst,c,m,n,.x) if(mask&0x1){ __syncthreads(); }\
    LOAD8(c,sld,k,.x)       if(mask&0x2){ __syncthreads(); }\
    ISTORE4x2(sst,c,m,n,.y) if(mask&0x4){ __syncthreads(); }\
    LOAD8(c,sld,k,.y)       if(mask&0x8){ __syncthreads(); }\
}

#define PERMUTE4x2(sst,sld,c,a,b,m,n,mask){                 \
    ISTORE4x2(sst,c,a,b,.x) if(mask&0x1){ __syncthreads(); }\
    LOAD4x2(c,sld,m,n,.x)   if(mask&0x2){ __syncthreads(); }\
    ISTORE4x2(sst,c,a,b,.y) if(mask&0x4){ __syncthreads(); }\
    LOAD4x2(c,sld,m,n,.y)   if(mask&0x8){ __syncthreads(); }\
}

#define PERMUTE_S16_L8x2(sst,sld,c,k,m,n,mask){             \
    ISTORE16(sst,c,k,.x)    if(mask&0x1){ __syncthreads(); }\
    LOAD8x2(c,sld,m,n,.x)   if(mask&0x2){ __syncthreads(); }\
    ISTORE16(sst,c,k,.y)    if(mask&0x4){ __syncthreads(); }\
    LOAD8x2(c,sld,m,n,.y)   if(mask&0x8){ __syncthreads(); }\
}

#define PERMUTE_S8x2_L16(sst,sld,c,m,n,k,mask){             \
    ISTORE8x2(sst,c,m,n,.x) if(mask&0x1){ __syncthreads(); }\
    LOAD16(c,sld,k,.x)      if(mask&0x2){ __syncthreads(); }\
    ISTORE8x2(sst,c,m,n,.y) if(mask&0x4){ __syncthreads(); }\
    LOAD16(c,sld,k,.y)      if(mask&0x8){ __syncthreads(); }\
}

#define PERMUTE8x2(sst,sld,c,a,b,m,n,mask){                 \
    ISTORE8x2(sst,c,a,b,.x) if(mask&0x1){ __syncthreads(); }\
    LOAD8x2(c,sld,m,n,.x)   if(mask&0x2){ __syncthreads(); }\
    ISTORE8x2(sst,c,a,b,.y) if(mask&0x4){ __syncthreads(); }\
    LOAD8x2(c,sld,m,n,.y)   if(mask&0x8){ __syncthreads(); }\
}


#ifdef __HIPCC__

#define perm_mask 0x7
#define sync

#else

#if CUDA_ARCH>=70
#define perm_mask 0x7
#define sync      __syncwarp();
#else
#define perm_mask 0x0
#define sync
#endif
#endif //__HIPCC__

// sync issue walk aound
#define FFT_WA_SM70_SYNC
#if CUDA_ARCH>=70
#define LB_16x16_256 __launch_bounds__(256,8)
#define LB_16x16_128 __launch_bounds__(128,16)
#define LB_32x32_512 __launch_bounds__(512,2)
#define LB_32x32_256 __launch_bounds__(256,4)
#else
#define LB_16x16_256 __launch_bounds__(256,2)
#define LB_16x16_128 __launch_bounds__(128,4)
#define LB_32x32_512 __launch_bounds__(512,1)
#define LB_32x32_256 __launch_bounds__(256,2)
#endif


#include"../activation/activation.h"
#include"../fft/sfft/sfft.h"

#endif
