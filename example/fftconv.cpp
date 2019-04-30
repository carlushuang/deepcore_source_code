#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"common.h"
#include"deepcore.h"
//#pragma comment( lib, "../deepcore/deepcore.lib" )
//#pragma comment( lib, "cuda.lib" )

#define DC_CALL(call) 							\
	do{											\
		dc_status_t dc_stat=call;				\
		if(dc_stat != dc_success){				\
			printf("error(%d) " #call ", line:%d\n", dc_stat,__LINE__ );	\
			dc_exit();							\
			exit(0);							\
		}										\
	}while(0)

#define AP "[fftconv]"
#define WARMUP 3
#define LOOP 8

int main()
{
	dc_status_t dc_stat;
	DC_CALL(dc_init());

	dc_set_device(0);

	//tensor_shape_t shapes[]={{48,3,32,64,8},{40,3,32,64,1},{24,3,32,64,32},{18,3,32,32,1},{10,3,32,32,32}};
	//tensor_shape_t shapes[]={{27,5,64,192,512,2} , {13,3,192,384,512,1}, {13,3,384,384,512,1}, {13,3,384,256,512,1}};
	tensor_shape_t shapes[]={
		{13,3,384,384,512,1},{27,3,384,384,512,1},{55,3,384,384,256,1},{96,3,384,384,64,1},
		{13,5,384,384,512,2},{27,5,64 ,192,512,2},{55,5,64 ,192,256,2},{96,5,64 ,192,64,2},
		{13,7,384,384,128,3},{27,7,384,384,128,3},{55,7,384,384,128,3},{96,7,384,384,64,3}};

	for( int e=0; e<sizeof(shapes)/sizeof(shapes[0]); e++  )
	{
		int dir=0;
		for(  dir=0; dir<1; ++dir )
		{
			printf(AP"start shape %d, dir:%d -----------------------------------\n",e,dir);
			int pn=shapes[e].ds;
			int fn=shapes[e].fs;
			int pad = shapes[e].pad;
			int qn=pn+2*pad-fn+1;
			int pnc=shapes[e].pnc;
			int qnc=shapes[e].qnc;
			int bat=shapes[e].bat;
			int inc=dir==0?pnc:qnc;
			int onc=dir==0?qnc:pnc;
			int in=dir==0?pn:qn;
			int on=dir==0?qn:pn;
			//int pad=dir==0?0:(fn-1);
			printf(AP"  input(cnhw):%dx%dx%dx%d, filter(kcrs):%dx%dx%dx%d, output(cnhw):%dx%dx%dx%d, pad:%d\n",
				pnc,bat,pn,pn,    qnc,pnc,fn,fn,    qnc,bat,qn,qn,   pad);
			
			uint64_t qshape=dc_create_tensor_shape( dcMaskPrecisionFloat, (qn<<16)|qn, (qnc<<16)|bat );
			uint64_t pshape=dc_create_tensor_shape( dcMaskPrecisionFloat, (pn<<16)|pn, (pnc<<16)|bat );
			uint64_t kshape=dc_create_tensor_shape_filter( dcMaskPrecisionFloat, (fn<<16)|fn, (qnc<<16)|pnc );
			uint64_t ishape=dir==0?pshape:qshape;
			uint64_t oshape=dir==0?qshape:pshape;

			dc_fftconvOp Op;
			size_t auxnb;
			size_t dc_pad = (pad | pad<<8);		// warp to feed dc
			DC_CALL(dc_create_fftconvOp( &Op, &auxnb, dcMaskPrecisionFloat|dir, 1, pshape, kshape, qshape, dc_pad ));


			void *d_a, *d_b, *d_c;
#ifdef __HIPCC__
			hipDeviceptr_t auxbuf;
#else
			CUdeviceptr auxbuf;
#endif
			dc_create_tensor( (void**)&d_a, ishape );
			dc_create_tensor( (void**)&d_b, kshape );
			dc_create_tensor( (void**)&d_c, oshape );
#ifdef __HIPCC__
			hipMalloc(&auxbuf, auxnb);
#else
			cuMemAlloc( &auxbuf, auxnb );
#endif
			char auxnb_str[20];
			b2s(auxnb, auxnb_str);
			printf(AP"  aux buffer size:%llu(%s)\n", auxnb, auxnb_str);

			float* a=new float[bat*inc*in*in];
			float* b=new float[qnc*pnc*fn*fn];
			float* c=new float[bat*onc*on*on];
			float* d=new float[bat*onc*on*on];

			for( int i=0; i<inc; ++i )
			{
				for( int z=0; z<bat; ++z ){
					for( int y=0; y<in; ++y ){
						for( int x=0; x<in; ++x ){
							a[((i*bat+z)*in+y)*in+x]=((float)rand())/RAND_MAX;
						}
					}
				}
			}
			for( int i=0; i<fn*fn*pnc*qnc; ++i ){
				b[i]=((float)rand())/RAND_MAX;
			}
#if 0
			printf(AP"  start cpu caculation...");
			for( int i=0; i<onc; ++i ){
				for( int s=0; s<bat; ++s ){
					conv( &c[(i*bat+s)*on*on], &a[s*in*in], &b[i*(dir==0?(pnc*fn*fn):(fn*fn))], dir, in, in, fn, fn, on, on, inc, bat, pad, pad, dir==0?(fn*fn):(pnc*fn*fn) );
				}
			}
			printf(" finish\n");
#endif
			dc_tensor_store( d_a, ishape, a, bat*in*in*sizeof(float), bat*in*in*sizeof(float), inc, NULL );
			dc_tensor_store( d_b, kshape, b, pnc*fn*fn*sizeof(float), pnc*fn*fn*sizeof(float), qnc, NULL );
			bool is_ok;
#ifdef __HIPCC__
			hipEvent_t evt_0, evt_1;
			hipEventCreate(&evt_0);
			hipEventCreate(&evt_1);
#else
			CUevent evt_0, evt_1;
			cuEventCreate(&evt_0, CU_EVENT_DEFAULT);
			cuEventCreate(&evt_1, CU_EVENT_DEFAULT);
#endif


			for(int i=0;i<WARMUP;i++){
				if(dc_fftconv( Op, (void*)auxbuf, d_c, d_a, d_b, NULL, 1.f, NULL )!=dc_success){
					printf( AP"error: conv exec failed!\n" );
					is_ok = false;
					goto __LAB0;
				}
			}
#ifdef __HIPCC__
			hipCtxSynchronize();
			hipEventRecord(evt_0, NULL);
#else
			cuCtxSynchronize();
			cuEventRecord(evt_0, NULL);
#endif

			for(int i=0;i<LOOP;i++){
				if(dc_fftconv( Op, (void*)auxbuf, d_c, d_a, d_b, NULL, 1.f, NULL )!=dc_success){
					printf( AP"error: conv exec failed!\n" );
					is_ok = false;
					goto __LAB0;
				}
			}
			float elapsed_ms;
#ifdef __HIPCC__
			hipEventRecord(evt_1, NULL);
			hipEventSynchronize(evt_1);
			hipCtxSynchronize();
			hipEventElapsedTime(&elapsed_ms, evt_0, evt_1);
#else
			cuEventRecord(evt_1, NULL);
			cuEventSynchronize(evt_1);
			cuCtxSynchronize();
			cuEventElapsedTime(&elapsed_ms, evt_0, evt_1);
#endif
			printf( AP"examples[%d][%d] cost:%f ms, ", dir, e ,elapsed_ms/LOOP);

			dc_tensor_load( d, bat*on*on*sizeof(float), d_c, oshape, bat*on*on*sizeof(float), onc, NULL );
#ifdef __HIPCC__
			hipCtxSynchronize();
#else
			cuCtxSynchronize();
#endif

			is_ok=check( c, d, bat*onc*on*on );
			printf("compute %s\n",is_ok?"ok":"fail");

		__LAB0:
#ifdef __HIPCC__
			hipEventDestroy(evt_0);
			hipEventDestroy(evt_1);
			hipFree(auxbuf);
#else
			cuEventDestroy(evt_0);
			cuEventDestroy(evt_1);
			cuMemFree(auxbuf);
#endif
			dc_release_tensor( d_a );
			dc_release_tensor( d_b );
			dc_release_tensor( d_c );
			dc_destroy_fftconvOp(Op);
			delete[] a;
			delete[] b;
			delete[] c;
			delete[] d;
			//if(!is_ok) break;
		}
	}
	dc_exit();
}
