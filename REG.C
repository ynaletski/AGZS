
#define P_real   s_MVD[0].Press
#define YN_OUT   DriveSV[0]

long int P_dem=1.5*K_F_INT;
long int P_dem_c=1.5*K_F_INT;
long int f_Beg=200; // Гц

long int k_p=10,k_i=15,k_d=0; // коэффициенты ПИД для регулятора давления
long int maxP_errP= 0.1 * K_F_INT, maxP_errM = 0.1 * K_F_INT;
long int LIM_P=(long int)(5000L*k_div) ,LIM_M = (long int)(150L*k_div); // предельные частоты привода
long int LIM_P_c=0;

long int K0,K1,K2,K3;
long int YN=0,XN=0,XN_1=0,XN_2=0;

//long int YiN=0;
//---------------------------------
void f_init_reg_P()
{
 YN= f_Beg * k_div;
 XN=0;
 XN_1=0;
 XN_2=0;
 YN_OUT= YN >> n_shft;
 LIM_P_c=LIM_P;
 P_dem_c=P_dem;

}
//---------------------------------
long int tl_reg_b=0;
long int tl_reg_e=0;
long int tl_reg_res=0;
int t_reg_res=0;
int flg_prn_reg=0;

void f_reg_P()
{
// Регулятор давления - изменяет частоту вращения
// двигателя насоса для поддержания
// заданного давления P_dem_c на выходе насоса.
  tl_reg_b=TimeStamp;
  tim1();

   P_dem_c=P_dem;
   XN = P_dem_c - (long int)(P_real * K_F_INT);

   if( XN  >  maxP_errP)
   {
      XN =  maxP_errP;
   }
   else if( XN  <  (-maxP_errM) )
   {
      XN = -maxP_errM;
   }

   YN = YN *K0 +K1 *XN-K2 *XN_1 +K3 *XN_2 ;

   if(YN >  LIM_P_c )
      YN = LIM_P_c;
   else  if(YN <  LIM_M )
      YN = LIM_M;

   XN_2  = XN_1 ;
   XN_1  = XN;

   YN_OUT= YN >> n_shft;


  tl_reg_e=TimeStamp;
  tim2();

  if(flg_prn_reg)
  {
    flg_prn_reg=0;
    tl_reg_res=tl_reg_e-tl_reg_b;
    t_reg_res=ttt2-ttt1-10;  // 10 следует отнять - это время присутствует
    if(t_reg_res < 0) t_reg_res+=10000;
    printf("\n\r f_reg_P() time: %ld ms +  %f mks\n\r>",tl_reg_res,(float)(t_reg_res)/10.);
  }
}
//-----------------------

void f_count_cf()
{ /* Пересчет ПИД коэфф-в в коэфф-ты ур-ий в конечных разностях */
 /*
    Y = Kp*x + Ki*Sxdt + Kd*dx/dt
    K0 =  1.0
    K1 =  Kp+Ki/2+Kd
    K2 =  Kp-Ki/2+2*Kd
    K3 =  Kd
    Y(n) =  K0 * Y(n-1) + K1*x(n) - K2*x(n-1) + K3*x(n-2)
 */
        if(k_i ==0.)
         {
           K0  =  0.;
           K1  =  (k_p +k_d );
           K2  =  k_d ;
           K3  =  0.;
         }
        else
         {
           K0  =  1.0;
           K1  =  (k_p  + k_i /2. +    k_d ) ;
           K2  =  (k_p  - k_i /2. + 2.*k_d ) ;
           K3  =  k_d ;
         }

}
