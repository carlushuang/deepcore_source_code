#ifndef __idc_bop_h__
#define __idc_bop_h__

#include"idc_macro.h"
#include<stdint.h>
#ifdef _MSC_VER
#include<intrin.h>
#endif

INLINE uint32_t idc_bhs( uint32_t n )
{
    unsigned long i;
#ifdef _MSC_VER
    _BitScanReverse( &i, n );
#else
    i=32-__builtin_clz(n);
#endif
    return i;
}
INLINE uint32_t idc_popc( uint32_t n )
{
    uint32_t i;
#ifdef _MSC_VER
    i=_mm_popcnt_u32(n);
#else
    i=__builtin_popcount(n);
#endif
    return i;
}
INLINE uint32_t idc_minlds( uint32_t n )
{
    uint32_t p=idc_bhs(n);
    return (1<<(p+(idc_popc(n)>1)));
}

#endif
