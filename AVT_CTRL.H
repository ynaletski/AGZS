
    // индекс MVD в ICP_error[]
#define MVD1  2
#define MVD2  3
#define MVD3  4

#define LV85_O 0x100

#define ErrMVD1  ICP_error[MVD1]
#define ErrMVD2  ICP_error[MVD2]

#define NA_scale (65535.)
#define MaxADC 65536L

#define RCV_FLG      0x100
#define UZA_E    2
#define LVL_E    3
#define STOP_E   5
#define CONS_E   6
#define TRAP_E   7
#define ID_E     8

#define  T_Mot_Stop (15000L)
//==================================
#define OUT1 msk_int[num_out1]
#define OUT2 msk_int[num_out2]

#define OUT3 msk_int[num_out3]
#define PMP  msk_int[num_out3]

#define OUT4 msk_int[num_out4]
#define C_DNS  msk_int[num_out4]

#define OUT5 msk_int[num_out5]
//#define GAS    msk_int[num_in4]

#define OUT6 msk_int[num_out6]
#define LV10 msk_int[num_out6]

#define OUT7 msk_int[num_out7]
#define LV90 msk_int[num_out7]

#define OUT8 msk_int[num_out8]
#define RCVB msk_int[num_out8]

#define OUT9 msk_int[num_out9]
#define RCVL msk_int[num_out9]

#define OUT10 msk_int[num_out10]
#define PMP_RCV msk_int[num_out10]

#define OUT11 msk_int[num_out11]
#define PMP_GAS msk_int[num_out11]

#define OUT12 msk_int[num_out12]
//#define BYPASS msk_int[num_out12]

#define OUT12 msk_int[num_out12]
#define OUT13 msk_int[num_out13]
#define OUT14 msk_int[num_out14]

#define INP1 msk_int[num_in1]
#define INP2 msk_int[num_in2]
#define INP3 msk_int[num_in3]

#define GUN msk_int[num_in1]
#define UZA msk_int[num_in2]
#define E_S msk_int[num_in3]
#define L85 msk_int[num_in4]

#define  bit_10p  (0x2000)
#define  bit_90p  (0x4000)
#define  bit_85p  (0x8000)

extern unsigned int OUT_VAR;
extern unsigned int INP_VAR;
extern unsigned int INP_inv;
extern unsigned int INP_VAR_c;
extern unsigned int INP_VARi;
extern unsigned int INP_MSK_OK;
extern unsigned int OUT_VAR_c;
extern unsigned int Out_en;
extern unsigned int Out_en_r;

extern int num_out1,num_out2;
extern int num_out3,num_out4;
extern int num_out5,num_out6;
extern int num_out7,num_out8;
extern int num_out9,num_out10;
extern int num_out11,num_out12;
extern int num_out13,num_out14;
extern int num_out15,num_out16;

extern int num_in1,num_in2;
extern int num_in3,num_in4;

extern int mski_inp1;
extern int mski_inp2;
extern int mski_inp3;
extern int mski_inp4;

//==================================

extern int flag_O3r;
//==================================

extern int *DriveStatus;
extern int DriveSV[];
extern int DriveSA[];
extern int DriveSD[];
extern int DriveState[];
extern int DriveCurrent[];
extern int DriveError[];

extern int (*f_Drive_St_chk)();
extern int (*f_Drive_Rn_chk)();
extern void (*f_Drive_Stop)();
extern void (*f_Drive_Run)();

extern int flag_ext_t;
extern int Drive_SV_beg[];
extern int Drive_SA_beg[];
extern int Drive_SD_beg[];

extern int analog_num[];
extern float analog_scale[];
extern float analog_offset[];

void f_prb_plus();
void f_prb_minus();

extern float PressLvl_off0;
extern float PressLvl_on0 ;

extern float PressLvlR_off0;
extern float PressLvlR_on0 ;

extern long int time_antd;

extern int stage_max;

extern long int time_antd_p;
extern long int time_antd_m;

extern long int time_prb_plus;
extern long int time_prb_minus;

extern long int Tim_antd;
extern long int Tim_prb_plus;
extern long int Tim_prb_minus;

extern int flag_dlv_fst;
extern int flag_prn_mass;
extern float dose_dlvr;
extern int sw_dlv_liq;
extern int sw_dns;

int f_rcv_liq(int key);
int f_dens_m(int key);

extern int sw_dns;

extern int fl_GO_rcv;

extern int flagS_ES;
extern int flagS_UZA;
extern int flagE_UZA;
extern int flagS_DLT;

extern float vBIG_P;
extern int Flag_massR;

extern int flag_fill_ok_R;
extern int flag_nal_r;

extern int flag_rcv;
void f_stop_cnt_rcv();
void f_stop_cnt_rcvM();
void f_stop_cnt_rcvB();
void f_enable_cnt_rcv();

int f_ctrl_rcv();
void f_dens_r();


extern int flag_nstg;
extern long int time_stg_ctrl;
extern long int time_stg_ctrl0;
extern long int Tim_stg_ctrl0;
extern long int Tim_stg_ctrl;

void  f_chk_stg();

extern float P_gas, P_bypass;

void f_set_o67();
void f_sens();

extern float Rcv_Result1;
extern float Rcv_Result2;
extern float Dens_Result;
extern float Temp_Result;

extern int Flag_check;

extern float F_Dns_off;
extern float F_Dns_off_DLV;
extern float F_Dns_off_DLVs;
extern long int T_Dns_off;
extern long int T_Dns_NDB;

extern long int time_t_NDB;

void f_out_var_a();
extern int Socket_mode;

extern float dose_dlvr1;
extern long int t_dose;

