#include <sched.h>
#include "types.h"
#include "options.h"
#include <omp.h>
#include "altivec.h"
//#include "veclib.1.0.4/include/veclib_types.h"
//#include "veclib.1.0.4/include/vec256dp.h"
#define load4(x) vec_xld2(0,x)
#define store4(x,y) vec_xstd2(y,0,x)
#define make4(x) (vector double)(x)

#define loadV(x,y,z) vec_xld2( 2*z + 2*Lkk*y + 2*LkkLjj*x , baseV )
#define loadS(x,y,z,dir) vec_xld2(2*z + 2*(Lkk-2)*y + 2*Lkk_2Ljj_2*x + 2*Lkk_2Ljj_2Lii_1 * dir , baseS)
#define storeV(x,y,z,v) vec_xstd2(v,2*z + 2*Lkk*y + 2*LkkLjj*x,baseST)

typedef vector double vector4;

void simd_diff_cpu_thr(Real* __restrict__ d_psi, Real* __restrict__ d_npsi, Real* __restrict__ d_sigmaX, Real* __restrict__ d_sigmaY, Real* __restrict__ d_sigmaZ,int Lii, int Ljj, int Lkk,int numtimes)
{


  //rough sizing may be needed
  //map z dir to SIMD
  //z is the fastest varying direction

  //2d decomposition
  //15x15 in y z direction

  #define sigmaX(x,y,z,dir) d_sigmaX[2*( z + (Lkk-2) * ( y + (Ljj-2) * ( x + (Lii-1) * dir ) ) )]
  #define sigmaY(x,y,z,dir) d_sigmaY[2*( z + (Lkk-2) * ( y + (Ljj-1) * ( x + (Lii-2) * dir ) ) )]
  #define sigmaZ(x,y,z,dir) d_sigmaZ[2*( z + (Lkk-1) * ( y + (Ljj-2) * ( x + (Lii-2) * dir ) ) )]

  #define psi(x,y,z) d_psi[2*( z + Lkk * ( y + Ljj * x ) )]
  #define npsi(x,y,z) d_npsi[2*( z + Lkk * ( y + Ljj * x ) )]
  #define V(x,y,z) psi(x,y,z)


  if ((Ljj-2)%15 != 0 ) printf("Ljj-2 must be multiple of 15. This will be fixed, later\n");
  if ((Lkk-2)%15 != 0 ) printf("Lkk-2 must be multiple of 15. This will be fixed, later\n");

  Lii = Lii/2;

  const int LkkLjj = Lkk * Ljj;
  const int Lkk_2Ljj_2 = (Lkk-2)*(Ljj-2);
  const int Lkk_2Ljj_2Lii_1 = (Lkk-2)*(Ljj-2)*(Lii-1);
 
  int kblocks = (Lkk-2)/15;
  int t_tile = (Ljj-2)*kblocks/15;
     
  //printf("totla number of threads : %d\n", omp_get_num_threads());

  #pragma omp parallel 
  {
    vector4 xm[15][16]; //temporary array
    vector4 ym[16]; //temporary array


    for (int ii=0; ii<numtimes; ii++)
    {
      //  for(int tid=0;tid<t_tile;tid++)
      int tid = omp_get_thread_num();
      {
        int cjj = tid / kblocks;
        int ckk = tid % kblocks;
    
        cjj = cjj*15 + 1;
        ckk = ckk*15 + 1;
      
        //todo: when Ljj is not multiple of 15
      //  for(int cjj=1;cjj<Ljj-1;cjj+=15)
      //  for(int ckk=1;ckk<Lkk-1;ckk+=15)
        {
      
          __memset(xm,0,sizeof(xm));
      //    for(int tjj=0;tjj<15;tjj++)
      //    for(int tkk=0;tkk<15;tkk++)
      //      xm[tjj][tkk] = make4(0);
      
          for(int x=1;x<Lii-1;x++)
          {
              //z direction: all loads are un-aligned load
              //
      
              for(int tjj=0;tjj<15;tjj++)       
              {                                 
                vector4 last = make4(0);
      
                double *baseV =(double*)(&V(x,cjj+tjj,ckk));
                double *baseS = (double*)(&sigmaZ(x,cjj+tjj,ckk,0));
                double *baseST = (double*)(&npsi(x,cjj+tjj,ckk));
      
                for(int tkk=0;tkk<16;tkk++)     
                {                               
                  int z = tkk;       
      
                  vector4 v00 = loadV(0,0,z-1)   ;
                  vector4 v0p = loadV(0,0+1,z-1) ;
                  vector4 v0m = loadV(0,0-1,z-1) ;
                  vector4 vp0 = loadV(0+1,0,z-1) ;
                  vector4 vm0 = loadV(0-1,0,z-1) ;
                 
                  vector4 w00 = loadV(0,0,z)   ;
                  vector4 w0p = loadV(0,0+1,z) ;
                  vector4 w0m = loadV(0,0-1,z) ;
                  vector4 wp0 = loadV(0+1,0,z) ;
                  vector4 wm0 = loadV(0-1,0,z) ;
                  
                  vector4 sX = loadS( 0,0,z-1,0) ;
                  vector4 sY = loadS( 0,0,z-1,1) ;
                  vector4 sZ = loadS( 0,0,z-1,2) ;
                 
                  vector4 zzm = sZ * (w00 - v00);
                  vector4 zxm = sX * ( vp0 - vm0 + wp0 - wm0 ) * make4(0.5);
                  vector4 zym = sY * ( v0p - v0m + w0p - w0m ) * make4(0.5);
                
                  vector4 tmp = zzm + zzm + zym;
                  storeV(0,0,z, tmp + last);
                  last = tmp;
                } 
              } 
      
              //x direction: all loads are un-aligned load
              for(int tjj=0;tjj<15;tjj++)       
              {                                 
      
                double *baseV =(double*)(&V(x,cjj+tjj,ckk));
                double *baseS = (double*)(&sigmaX(x,cjj+tjj,ckk,0));
                double *baseST = (double*)(&npsi(x,cjj+tjj,ckk));
      
                for(int tkk=0;tkk<16;tkk++)     
                {                               
                  int z = tkk;       
      
                  vector4 v0m = loadV( 0,0,z-1) ;
                  vector4 v0p = loadV( 0,0,z+1) ;
                  vector4 v00 = loadV( 0,0,z) ;
      
                  vector4 vp0 = loadV( 0,0+1,z) ;
                  vector4 vm0 = loadV( 0,0-1,z) ;
      
                  vector4 w0m = loadV( 0+1,0,z-1) ;
                  vector4 w0p = loadV( 0+1,0,z+1) ;
                  vector4 w00 = loadV( 0+1,0,z) ;
      
                  vector4 wp0 = loadV( 0+1,0+1,z) ;
                  vector4 wm0 = loadV( 0+1,0-1,z) ;
                 
                  vector4 sX = loadS( 0,0,z,0) ;
                  vector4 sY = loadS( 0,0,z,1) ;
                  vector4 sZ = loadS( 0,0,z,2) ;
                 
                  vector4 xxm = sX * (w00 - v00);
                  vector4 xym = sY * (vp0 - vm0 + wp0 - wm0 ) * make4(0.5);
                  vector4 xzm = sZ * (v0p - v0m + w0p - w0m ) * make4(0.5);
                
                  vector4 tmp = xxm + xym + xzm;
                  storeV(0,0,z, xm[tjj][tkk] + tmp);
      
                  xm[tjj][tkk] = tmp;
                } 
              } 
      
              //y direction:         
              __memset(ym,0,sizeof(ym));
      //        for(int tkk=0;tkk<16;tkk++) ym[tkk]=make4(0);
           
              for(int tjj=0;tjj<15;tjj++)       
              {                                 
      
                double *baseV =(double*)(&V(x,cjj+tjj,ckk));
                double *baseS = (double*)(&sigmaY(x,cjj+tjj,ckk,0));
                double *baseST = (double*)(&npsi(x,cjj+tjj,ckk));
      
                for(int tkk=0;tkk<16;tkk++)     
                {                               
                  int z = tkk;       
      
                  vector4 v0m = loadV( 0,0,z-1) ;
                  vector4 v0p = loadV( 0,0,z+1) ;
                  vector4 v00 = loadV( 0,0,z) ;
      
                  vector4 vp0 = loadV( 0+1,0,z) ;
                  vector4 vm0 = loadV( 0-1,0,z) ;
      
                  vector4 w0m = loadV( 0,0+1,z-1) ;
                  vector4 w0p = loadV( 0,0+1,z+1) ;
                  vector4 w00 = loadV( 0,0+1,z) ;
      
                  vector4 wp0 = loadV( 0+1,0+1,z) ;
                  vector4 wm0 = loadV( 0-1,0+1,z) ;
                 
                  vector4 sX = loadS (0,0,z,0) ;
                  vector4 sY = loadS (0,0,z,1) ;
                  vector4 sZ = loadS (0,0,z,2) ;
                 
                  vector4 yxm = sX * (vp0 - vm0 + wp0 - wm0 ) * make4(0.5);
                  vector4 yym = sY * (w00 - v00);
                  vector4 yzm = sZ * (v0p - v0m + w0p - w0m ) * make4(0.5);
                
                  vector4 tmp = yxm + yym + yzm;
                  storeV(0,0,z, ym[tkk] + tmp);
                  ym[tkk] = tmp;
                } 
              } 
          } //x loop
        } //cjj,ckk
      } //end of block
    }
  }
}
