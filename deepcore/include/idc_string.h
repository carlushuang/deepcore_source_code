#ifndef __idc_string_h__
#define __idc_string_h__

#include"idc_macro.h"

INLINE int idc_strcat( char* __restrict p_dst, const char* __restrict p_src )
{
    char* p_start=p_dst;
    while( *p_dst++=*p_src++ );
    return ((int)(p_dst-p_start-1));
}
INLINE int idc_strcmp( const char * src, const char * dst )
{
    int c=0;
    while(!(c=*(unsigned char*)src-*(unsigned char *)dst)&&*dst){ ++src, ++dst; }
    c=c<0?-1:(c>0?1:c);
    return c;
}

INLINE char * idc_strncpy(char *dest, const char *src, int n)
{
    int i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';
    return dest;
}


#endif