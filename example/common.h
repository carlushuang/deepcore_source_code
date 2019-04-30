#ifndef __common_h__
#define __common_h__

typedef struct tensor_shape{
    int ds;
	int fs;
    int pnc;
    int qnc;
    int bat;
	int pad;
} tensor_shape_t;

void conv( float* c, const float* __restrict a, const float* __restrict b, int is_flip, int anx, int any, int bnx, int bny, int cnx, int cny, int nc, int bat, int pad_x, int pad_y, int ofs )
{
	for( int y=0; y<cny; ++y )
	{
		int sy=y-pad_y;
		for( int x=0; x<cnx; ++x ){
			int sx=x-pad_x;
			float s=0.f;
			for( int chan=0; chan<nc; ++chan ){
				const float* Ac=&a[chan*bat*anx*any];
				const float* Bc=&b[chan*ofs];
				for( int v=0; v<bny; ++v ){
					int sv=is_flip?(bny-v-1):v;
					for( int u=0; u<bnx; ++u ){
						int su=is_flip?(bnx-u-1):u;
						if(((sy+v)>=0)&&((sy+v)<any)&&((sx+u)>=0)&&((sx+u)<anx)){
							s+=Ac[(sy+v)*anx+sx+u]*Bc[sv*bnx+su];
						}
					}
				}
			}
			c[y*cnx+x]=s;
		}
	}
}

bool check( const float* __restrict a, const float* __restrict b, int n )
{
    double s0=0;
    double s1=0;
    double ref=0;
    for( int i=0; i<n; ++i ){
       float  ai=a[i];
       float  bi=b[i];
	   double d=(double)ai-(double)bi;
       double dd=d*d;
       double aa=2.0*ai*ai;
	   double x=dd/aa;
       if(x>ref){ ref=x; }
       s0+=dd;
       s1+=aa;
	}
    return (sqrt(s0/s1)<1e-6);
}

void inline b2s(size_t bytes, char * str){
	if(bytes<1024){
		sprintf(str, "%lluB", bytes);
	}else if(bytes<(1024*1024)){
		double b= (double)bytes/1024.0;
		sprintf(str, "%.2fKB", b);
	}else if(bytes<(1024*1024*1024)){
		double b= (double)bytes/(1024.0*1024);
		sprintf(str, "%.2fMB", b);
	}else{
		double b= (double)bytes/(1024.0*1024*1024);
		sprintf(str, "%.2fGB", b);
	}
}

#endif
