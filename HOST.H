

#define ICP 0
#define RTU 1


#define dim_xyz_c 100

struct eee_dat {
void *ptr;
int l;
};

#define ToutAnsHost 100
#define ToutAnsHost_rtu 100

// типы данных для dis_set,dis_set_MMI
#define T_FLOAT 1
#define T_INT   2
#define T_INT_L  3
#define T_HEX  4
#define T_HX2  5
#define T_HX4  6
#define T_STR  7
#define T_DBL  8
#define T_HEX_L 9

#define RD_INT    1
#define RD_LONG   2
#define RD_FLOAT  3
#define RD_STRING 4
#define RD_COIL   5

#define WR_INT    6
#define WR_LONG   7
#define WR_FLOAT  8
#define WR_STRING 9
#define WR_COIL   10

int f_host();
void f_WDOG(int ii);
int ffgets_host( int port);
int f_inw();
int f_innum(char *strg);
int f_inhex(char *strg);
float f_lim(float vvr,float lim_low,float lim_high);
int f_dis_set( struct dis_set *ds_mas,int ib,int nn);
void f_send_to_Host(int ii);
void f_empty_ans_echo(int ii);

void f_result_Licp(int ii);
void f_empty_ans_Licp(int ii);
void f_ans_Licp_cfg(int ii);
void f_ans_Licp_Flt(int ii);
void  f_icp_hlp();
void   f_menu();

void f_drv_list_MMI(int *nn);
void f_mod_list_MMI(int *nn);
void  f_WD_end (int ii);

int ffgets_no_com_ICP( int ii);

void f_send_to_Host_rtu(int ii);

struct icp_mod *f_get_mod_str(int num);

#define wrd_w 15
#define HEX_MAX       0xffffff
#define BIG_P (10e30)
#define BIG_I (32767)

#define icp_mod_max 20

struct icp_mod
{
int port;
int addr;
char name[10];
int TT;
int CC;
int FF;
int addr0;
};

extern int ICP_mod_fnd[5];
extern struct icp_mod ICP_MOD[5][icp_mod_max];

struct intr_r    /* состояние интерпретатора */
{
    int sw;                 /* переключатель состояния */
    int n_bcom;             /* указывает на  последний введенный символ в bfcom      */
                            /* если intrpr.bfcom[intrpr.n_bcom]==0 - команда введена */
    int i1;                 /* индекс к bfcom начала слова         */
    int i2;                 /* индекс к bfcom конца слова          */
    int i3;                 /* индекс к bfcom начала след-го слова */
    int wdl;                /* длина введенного слова              */
    float num_var;          /* текущий номер символьной переменной */
    int  num_var_i;

    char bfcom[n_bufcom];   /* буфер команды интерпретатора     */
    char wrd[wrd_w];        /* буфер слова                   */
    char com[15];           /* выполняемая команда              */

};
/*------------------------------*/
struct dis_set   /* структура для отображения-ввода значений */
{
 void  *op;
 float low_l;
 float high_l;
 int   type;    // 1 - float, 2 - int
                //T_FLOAT , T_INT
};
/*------------------------------*/
/*------------------------------*/
extern struct intr_r intrpr;
extern int ComPortHost;
extern int flag_echo_host;
extern int (*ff_putch[5])(int ch);
extern int AddrHost;

void f_look_ICP(int port,int CRC_flag);
void f_result_Licp(int ii);
void f_empty_ans_Licp(int ii);
extern int licp_sw;
extern int flag_print_licp;
extern int licp_port,licp_crc_flag;
extern int licp_addr;
extern int licp_fnd;

extern struct COM_rqst LICP_Rqst;
extern int ToutLicpScan;
extern int licp_amax;

void   f_print_que();
void f_drv_list();
void f_mod_list();

extern struct s_icp_dev WDOG[];
extern struct COM_rqst WDOG_Rqst[];
extern struct s_icp_dev Host;

extern struct COM_rqst Host_Rqst;
void f_look_RTU();
void f_result_Licp_RTU(int ii);
void f_send_to_Host_sk(int ii);
void f_prompt();
extern struct COM_rqst LICP_Rqst_RTU;

extern float cost_v;
extern float summ_v;

extern float vol_f;
extern float amount_f;
extern float volume_f;

extern float offs_cor;
extern float fct_offs,fct_delay;

extern float fct_offs_r;
extern float dsc_tmp;

extern int msk_int[];

extern unsigned long int *Flash_ptr;
extern float dens_cutoff;
extern float dens_cutoff1;

extern int flag_Deb;

  /* для trace()  */

#define Val_after (100)

extern long int i_nn_d;
extern int fl_trace;
extern long int i_nn;
extern int dim_xyz;
extern int fl_trace_loop;

extern int uk_buf;
//extern float *buf_xyz;
//extern float buf_xyz[];
extern long int buf_xyz[];
extern long int *uk_ns[];
extern int f_ns[5];
extern long int Password;

extern int cnt_after;
void f_ns_count();
void trace();
void putch2(long int i1 );
void putch4(long int i1 );
void f_rep();

extern float ex_p,ex_m;

void f_rd_cor();
void f_wr_cor();
void f_init_mki();

extern int en_mski;

extern float fl_id,fl_id1;
extern char chr_id[20];

extern int  ZeroPage;
extern int  EmptPage;

extern long int t_kd_wr;
extern int kd_sw;
void f_kd_wr();

void f_before_MMI();
void f_after_MMI();

extern float ftmp_naMMI[];

extern int test_int;
extern long int test_lng;
extern long int test_lng1;
extern float test_float;

extern int DenMeasSrc;

extern float VolT;
extern float MassT;

extern int Tank_nn;
