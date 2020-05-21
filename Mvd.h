
 // симуляция MVD,   0 - нормальная работа
#define SIM0 (0)

#define GR  (2)  // индекс расходомера ГРК


/*
#define MVD_DNS  (2)   // индекс расходомера для измерения плотности
                       // 0 - система с 3-мя расходомерами
                       // 2 - система с 1 расходомером

#define MVD_FLOW  (2)  // индекс расходомера используемого при прокачке
                       // для f_flow_up()
                       // 0 - система с 3-мя расходомерами
                       // 2 - система с 1 расходомером
*/

#define MVD_DNS (MVD_FLOW)


extern int MVD_FLOW,MVD_FLOW_t;


// Pressure in MPa for condition:
// flowmeter: 2 tubes in parallel, diameter=1mm
// flow 1 kg/hour
// Density = 1 kg/m3

#define   ConstBern  (0.015635985)
 // MPa
 //  s_MVD[0].PressB= FctBern*s_MVD[0].FlowM*s_MVD[0].FlowM/s_MVD[0].Dens;

#define max_fl_ch     64
#define max_fl_ch_m1  63

#define max_MVD 3

#define  cutoff_off_M  dens_cutoff
#define  cutoff_off_V  dens_cutoff1

#define f_MVD_tim 1000

extern float  TimeMVD_Init;
extern float  TimeMVD_evt ;

#define F_Vol       1
#define F_Vol_Dens  2
#define F_Mass      3


float f_get_float( char *buf, int cod);
void f_MVD_1();
void f_MVD_2();
void f_MVD_3();

char *f_get_unit(int code);

#define ToutAnsMVD_rtu 100

#define MVD_max_out_err 5
#define MVD_max_inp_err 5
#define MVD_max_t_err 3

void f_MVD(int ii);
void f_MVD_rd(int ii);
void f_init_MVD(int ii);
void f_MVD_cor();

int f_reset_VolT();
int f_reset_zer_MVD( int ii);

extern struct s_icp_dev MVD[];
extern struct COM_rqst MVD_Rqst[];

extern int MVD_fn[];     // регистр функций драйвера
extern int MVD_fl[];

#define MVD_RD_I   1
#define MVD_RD_L   2
#define MVD_RD_F   3
#define MVD_RD_S   4
#define MVD_RD_C   5
#define MVD_RD_P   8
#define MVD_WR_I   16
#define MVD_WR_L   17
#define MVD_WR_F   18
#define MVD_WR_C   19

#define F_PROGRESS  20
#define F_FAULT     21

#define n_coil_b 8
#define n_char_b 50

struct s_MVD_dev
{
unsigned int r_addr; // address of MODBUS register
int n;            // number of data - for read coils and ascii
int type;         // data type
int i;            // int,coil value for write
int exc;          // exception
long int  l;      // long
float f;          // float

float FlowM;          // float
float FlowV;          // float
float MassI;          // float
float MassT;          // float
float VolI;          // float
float VolT;          // float
float VolTd;        // float
float Temp;          // float
float Dens;          // float
float Press;         // float
float Press2;        // float
float Compos;       // float
float PressG;       // float
float PressB;       // float
float PressD;       // float
float DensG;        // float

char c[n_coil_b]; // coil(s) value(s) read from MVD
char a[n_char_b]; // ascii

int reg_err;
};

extern int sw_mvd_1;
extern long int tm_mvd;
extern int unit_codes[];
extern char *unit_lst[];
extern char *err_F425[];
extern char *err_F425R[];

extern int flag_mvd_fst[];
void  f_ctrl_v1();

extern float CL_val[];
extern float CL_valR[];

extern int t_ctrl;
void  f_prep_int_v2();
float  f_int_v2();
void  f_ctrl_v2();

extern float VolT_int  ;
extern float summ_v_int;

extern int fl_GO;
extern int fl_take_temp;

extern float CL_T1;
extern float CL_T2;
extern float CL_T3;
extern long int CL_T4;

extern float CL_T1R;
extern float CL_T2R;

void f_stop_cnt();
void f_enable_cnt();
int f_MVD_WR(int ii,int fn,int addr,int i_val,long int l_val,float f_val);
void f_MVD_rd_t(int ii);
void f_gun();

extern float vol_mom,vol_mom0,flow_mom;

extern long int t_mvd_cor;

extern float VolT_id;
extern float summ_v_id;

extern int flag_pool;
extern int flag_O1;


extern int num_out1,num_out2;
extern int num_out3,num_out4;

extern int num_in1,num_in2;
extern int num_in3,num_in4;

extern float vol2,k_vol;
extern long int tt_vol1;
extern long int tt_vol2;

extern float vol2r,k_volr;
extern long int tt_vol1r;
extern long int tt_vol2r;
extern float Mas2r,k_Masr;
extern long int tt_Mas1r;
extern long int tt_Mas2r;

#define mErr   0
#define mVolT  1
#define mDens  2
//#define mFlowM 3
#define mSlot  3
#define mMassT 4
#define mTemp  5
//#define mFlowV 5
#define mVolI  6
#define mMassI 7

extern int flag_mvd_ton[];
extern long int tm_mvd_p;
extern int flag_nal;
extern void (*ff_serv_N)();

extern int  fl_VI, fl_MI;

extern struct s_MVD_dev s_MVD[];
extern long int tim_MVD_delay;

extern int mode_mvd;

extern int sw_MMI_cur; // начальная строка на дисплее MMI
extern int sw_MMI_lim;
extern int sw_mvd_m1;
void f_MVD_M1();
void f_sim_MVD();
void  f_prep_int_dens();
void  f_prep_int_Mass();

extern int flag_fst_gun;

extern int i_dens;
void f_density();
float f_get_composition(float Dens,float Temp);
float f_get_Dns(float Compos,float Temp);
float f_get_dens();
//void f_get_dens_c();
unsigned int f_get_int( char *buf);
void  f_prep_int_MassF();

void  f_flow_chk();
extern int fl_rd_err;

extern long int TimDensOff;
extern long int TimDensOn;

extern long int cl1_delay_off;
extern long int cl2_delay_off;
extern long int Out3_delay_off;
extern long int cl1_delayR_off;
extern long int cl2_delayR_off;
extern long int Out3_delayR_off;
extern float k_Den_Off;
extern float k_Den_On;
extern float Den2,k_Den;


extern float Dens0;
extern float propan[];
extern float butan[];

extern float PermPlus;
extern float PermMinus;
extern int flag_20C;
extern float Temperature;
extern float Temperature1;
extern float Density;
extern float Composition;

extern float t20c;
extern float v_Proc;
extern float K_unit; // переводит из г/см3 в кг/м3
extern float HTemp,HDens,HCompos;

extern int ModeFill;
extern int igun_1;

extern int flag_flch;
extern int flag_gun;
extern long int beg_flch;

extern int   time_flch;
extern float val_flch;
extern long int time_beg_fill;
extern int TimeBegFlch;

extern float val0_flch;
extern int TimeBegFlch_R;
extern int time_flch_R;
extern float val_flch_R;
extern float val_flch2_R;
extern float val0_flch_R;

void f_start_MVD();

extern long int Out4_delay_on  ;
extern long int Out4_delay_off ;
extern long int Out4_dl_off_gun;
extern int io_1;

extern long int fl_zeroing;

extern int flag_0;
extern float val_flch2; // предельное  значение потока для останова

extern float VolT_id1;
extern float summ_v_id1;

extern float k_forv;
extern float VolT_id1_1;

extern long int t_end_dlvr;
extern int flag_round;

extern float round_summ;
extern float rnd_add;
extern float rnd_lim;

extern int st_MVD_sw;
extern int cl_MVD_sw;
extern long int t_prep_MVD;
extern float volm_cut_wrk;
extern float mass_cut_wrk;

float f_round(float value);
void f_prep_MVD();
void f_close_MVD();
void f_prep_bern();

extern float FctBern,R_MVD;
extern int  MVD_t_rslt[];

void f_dsp_out();
void f_prep_int_rcv();

extern long int Tim_Prep1   ;
extern long int Tim_Prep2   ;
extern long int Tim_Prep_ND ;
extern long int Tim_Prep3   ;

// Если при инициализации расходомера Fl_grk != Fl_grk_OK , значит во время
// приема было выключено питание.При этом в расходомере
// по адресам Addr_Mass_grk, Addr_Vol_grk содержится значение
// счетчиков колонки Vol_grk, Mass_grk на момент старта приема от газовоза.
//  Если при инициализации расходомера Fl_grk == Fl_grk_OK ,
// значит по адресам Addr_Mass_kd , Addr_Vol_kd содержится добавка к
// значениям Inventory Vol_kd, Mass_kd , приводящая к значениям счетчиков ГРК.

#define Addr_Fl_grk    139
#define Fl_grk_OK      1
#define Fl_grk_FLT     2

#define Addr_Mass_kd   241
#define Addr_Vol_kd    243

#define Addr_Mass_grk  611
#define Addr_Vol_grk   613

extern float Vol_kd ;
extern float Mass_kd;

extern float Vol_grk ;
extern float Mass_grk;

void f_set_float( char *buf,float value, int cod);
void f_set_long( char *buf,long int value);
void f_set_int( char *buf, int value);

extern int i_LVL;

extern int ii_MVD;
extern float flow_min_c;

extern unsigned int OUT_VAR_a;

void    f_wr_dat_st();

struct deb_str
{
long int time;
unsigned  int out_var;
unsigned  int out_var_c_r;
float VolT ;
float MassT;
float FlowM;
float P;
};
//------------------------------
extern int debs_ptr;

#define deb_s_lgth  (18)
extern struct deb_str deb_s[deb_s_lgth];
void  f_wr_deb();
extern int i_deb_s;
extern float ex_p_d;
extern float M_fct[];
extern float V_fct[];
extern float D_fct[];
extern float M_fct_d[];
extern float V_fct_d[];
extern float D_fct_d[];
extern float MassT_id;

extern float round_summ_m;
extern float rnd_add_m;
extern float rnd_lim_m;

extern int flag_end_dlvr;

extern int  PMP_sw;
extern int  st_PMP_sw;
extern int  RQ1,RQ2;
extern int  RQ2W;

extern int F_GR2;
void f_pump_run();

extern int dnc_mode;

void f_sim_MVD0();

