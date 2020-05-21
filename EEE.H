
#define max_buf_eep 2050

void f_ee_num_init();
int f_wrt_eee();
int f_chk_EEE_CRC();
int f_rd_eee();

int f_rd_EEE_kd( struct eee_dat *EE_D_ptr );
int f_wrt_eee_kd( struct eee_dat *EE_D_ptr );

extern struct eee_dat EE_Dat[];
extern unsigned long int Eeprom_summ;
extern char buf_eep[2050];
extern unsigned int ee_lgth_byte;
extern int eee_num;
extern int ee_members;

extern int Fl_grk;

extern struct eee_dat EE_kd[];
extern struct eee_dat EE_grk[];
extern struct eee_dat EE_flg[];


