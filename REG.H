
#define K_F_INT (10000L)
#define k_div   (128L)
#define n_shft  7

#define  YiN  LIM_P_c
#define I_real  Delta_current[0]

#define Dlt_P_Dem  (0.05 * K_F_INT)

extern long int P_dem,YN,XN,XN_1,XN_2;

extern long int P_dem_c;

extern long int f_Beg;
extern long int maxP_errP, maxP_errM;
extern long int LIM_P,LIM_M;
extern long int K0,K1,K2,K3;
extern long int k_p,k_i,k_d;

extern float P_Dif_d;
extern long int kd_p,kd_i,kd_d;
extern long int maxPd_errP, maxPd_errM;
extern long int LIMd_P,LIMd_M;
extern long int Kd0,Kd1,Kd2,Kd3;

extern long int YdN,XdN,XdN_1,XdN_2;

extern int mode_r;

extern float Flow_on[] ;
extern float Flow_off[];
extern int flag_flow;

extern long int I_dem;
extern long int ki_p,ki_i,ki_d;
extern long int maxPi_errP, maxPi_errM;
extern long int LIMi_P,LIMi_M ;
extern long int Ki0,Ki1,Ki2,Ki3;
extern long int XiN,XiN_1,XiN_2;

extern long int LIM_P_c;
extern int mode_i;
extern int flg_prn_reg;

void f_init_reg_P();
void f_reg_P();
void f_reg_D();
void f_count_cf();
void f_reg_I();

