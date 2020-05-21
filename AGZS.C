
/*
     Main
*/

#include "ALL.h"

// значение давления в MVD без датчика давления

int F_GR2=0; // 0 - master

char sw_ver[20]="G1.47 05 Sep 2018";

unsigned long ComBaud[5]={9600,9600,9600,9600,115200};
int Com_lgth[5]={8,8,8,8,8};
int Com_parity[5]={0,0,0,0,0};//0-(None);1-(Even);2-(Odd);3-(MARK, always 1); 4-(SPACE,always 0)

int Com_stop[5]={1,1,1,1,1};


int  ComProt[5]={0,0,0,0,0};

long int baudrate_val[]={1200,1200,1200,1200,2400,4800,9600,19200,38400,57600,115200};

int  MMI_inp_err=0;
int  MMI_out_err=0;

//------------------------
int f_get_nb( long int baud)
{
int i;
 for(i=3;i<11;i++)
  if(baudrate_val[i]== baud)
    return i;
 return -1;
}
//------------------------
/*
float var[3000]={
 1.235,
 3.546,
 789000,
 345.678,
 45.678,
 5.678,
 0.678,
 0.0678,

};
*/
//--------------------------
void ShowPort(int port)
{
  Show5DigitLedWithDot(1,port);
}
void ShowChecksum(int checksum)
{
  Show5DigitLedWithDot(2,checksum);
}
void ShowBaudrate(long baud)
{ int data;

  if(baud>100000) data=baud/1000;
  else data=baud/100;

  Show5DigitLed(5,data%10);
  data/=10;
  Show5DigitLed(4,data%10);
  data/=10;
  Show5DigitLed(3,data%10);
}

void ShowCom( int port,int checksum,long int baud)
{
  ShowPort(port);
  ShowChecksum(checksum);
  ShowBaudrate(baud);

}
//-------------------
int i_ish;
void ShowNumber(long Num)
{ // выводит число в десятичной форме на 5DigitLed
int i,i1,dig;
char buf[10];

i_ish=Num;
return;
/*
  sprintf(buf,"%ld",Num);
  i=strlen(buf);
  if(i>0) i--;

  for(i1=0;i1<5;i1++,i--)
  {
    if(i>=0)
    {
     dig=buf[i]-'0';
    }
    else dig=0;
      Show5DigitLed(i1,dig);
  }
  printf("\n\r==%s==\n\r",buf);
*/

}
//-------------------
int getch_host()
{
while(!IsCom(ComPortHost));
 return(ReadCom(ComPortHost));
}
//-------------------
int puts_host(char *str)
{
int i;
 for(i=0;str[i];i++)
 putch(str[i]);
 return i;
}
/* ================================================== */
int f_cycle_cmn()
{
// Host
// MMI
        f_time_dd();

        if( WDOG[0].status)  f_WDOG(0);

        f_ctrl_v2();

        ServiceCOM(1);
        ServiceCOM(2);
        if(ComPortPmp1 != 3) ServiceCOM(3);


        f_ctrl_v2();

#if defined(ICP_7060)
        if(I7060[0].status)  f_7060(0);
        else I7060_fl[0]=0;
        if(I7060[1].status)  f_7060(1);
        else I7060_fl[1]=0;
#endif


#if defined(RTU_ComSK)
        if(ComSK[0].status)  f_ComSK(0);
        else ComSK_fl[0]=0;
#endif


#if defined(RTU_Delta)
        if(Delta[0].status)  f_Delta(0);
        else Delta_fl[0]=0;
#endif


       f_print_TSD(0);

#if defined(ICP_TSD)
        if(TSD[0].status)  f_TSD(0);
#endif

#if defined(TSD_GLB)
      if(TSD[0].status)  f_TSD_GLB(0);
      else TSD_GLB_fl[0]=0;
#endif

     f_ctrl_v2();

     if(ff_serv_N != NULL) (*ff_serv_N)(); // завершение налива

     if(flag_pool != 0)
     {

        if(F_GR2==0)
           f_pump_run();


        if(MVD[0].status != 0)
        {

          if(Flag_check == 0)
          {
            f_MVD(0);
          }
          else if(ICP_error[MVD1] == 0)
          {
                    f_MVD(0);
          }
        }
        else
        {
          MVD_fl[0]=0;
          f_sim_MVD0();
        }

        if(MVD[2].status != 0)
        {
          if(Flag_check == 0)
          {
            f_MVD(2);
          }
          else if(ICP_error[MVD3] == 0)
          {
                    f_MVD(2);
          }
        }
        else MVD_fl[2]=0;

        f_ctrl_v2();

#if defined(RTU_Lnk)
        if(LIn[0].status)  f_LIn(0);
#endif
        if(MVD[1].status)
        {
          if(Flag_check == 0)
          {
            if( MVD_fl[1] >= 0 )
                            f_MVD(1);
            else if(fl_GO == 0)
                            f_MVD(1);
          }
          else if(ICP_error[MVD2] == 0)
          {
            if( MVD_fl[1] >= 0 )
                            f_MVD(1);
            else
             {
              if( (sw_dlv_liq > 0) || (sw_dns > 0) )
                            f_MVD(1);
             }
          }
        }
        else MVD_fl[1]=0;

#if defined(ICP_7017C)
          if(I7017C[0].status)  f_7017C(0);
#endif
       f_one_sec();
       if(ff_serv != NULL) (*ff_serv)(); // сканирование магистрали
//       trace();
     }
     else
     {  // (flag_pool == 0)
     if(f_chk_flup() != 10  )
              if(LIn[0].status)  f_LIn(0);

      if(flag_rcv==0)
      {

        if(MVD[2].status != 0)
        {
          if(Flag_check == 0)
          {
            f_MVD(2);
          }
          else if(ICP_error[MVD3] == 0)
          {
                    f_MVD(2);
          }
        }
        else MVD_fl[2]=0;

        f_ctrl_v2();

      }
      else
      {
        if(MVD[0].status != 0)
        {
          f_pump_run();

          if(Flag_check == 0)
          {
            f_MVD(0);
          }
          else if(ICP_error[MVD1] == 0)
          {
                    f_MVD(0);
          }
        }
        else
        {
          MVD_fl[0]=0;
          f_sim_MVD0();
        }
        f_ctrl_v2();

      }

     }

     if(flag_Serv==1)
         f_pump1();

     f_gun();
     f_sens();
     f_dsp_out();
     INP_VAR &= 0x1FFF;
     if(res_ctr[0] != 0) INP_VAR |= 0x8000;
     if(res_ctr[1] != 0) INP_VAR |= 0x4000;
     if(res_ctr[2] != 0) INP_VAR |= 0x2000;
     INP_VARi = INP_VAR ^ INP_inv;
     return 0;
}
/* ================================================== */
int f_cycle()
{  // основной цикл

     f_cycle_cmn();

     if(flag_pool != 0)
     {
       if(MMI.status)
       {
         if(f_menu_MMI()) return -1;
         f_MMI();
       }
     }

     if((flag_Serv==0)||(ComPortHost != ComPortPmp1 ) )
     {
       if(f_host()) return -1;
     }

     return 0;
}
/* ================================================== */
int f_cycle0()
{   // без вызова f_host()
     f_cycle_cmn();
     if(flag_pool  != 0)
     {
       if(MMI.status)
       {
         if(f_menu_MMI()) return -1;
         f_MMI();
       }
     }
   //  trace();
     return 0;
}
//-------------------
int f_cycle1()
{   // без вызова f_MMI()

     f_cycle_cmn();

     if((flag_Serv==0)||(ComPortHost != ComPortPmp1 ) )
     {
       if(f_host()) return -1;
     }

   //  trace();
     return 0;
}
/* ================================================== */

/* ================================================== */

/* ---- main ------------------------------------------------------------- */
  int flag_Serv=0; // 0 - связь с Host PC для отладки

  int ttt1,ttt2,ttt3,ttt4;
  int itt1,itt2,itt3,itt4;
  long int rltt1,rltt2,rltt3,rltt4;
  long int ltt1,ltt2,ltt3,ltt4;
  int  flag_prn=0;

  unsigned long ttt1_l,ttt2_l,ttt3_l;

  unsigned int count=0xffff;

  int flag_H=0;
void main(void)
{
  struct tm  mdat;
  TIME_DATE tim_dat;
  double dtmp1,dtmp2,dtmp3,dtmp4;
  float ftmp,ftmp1,ftmp2,ftmp3,ftmp4;
  int iitmp1,iitmp2,iitmp3;
  int i,j;

  char cmd[40];
//  int checksum=1;
  int key;
  int iRet;
  int year,month,day,hour,min,sec;

  int itmp;
  long int ltmp=0;
/*
  if(!Is7188xa())
  {
    printf("MMICON.EXE must run on I-7188xa");
    exit (0);
  }
  else
    printf("7188xa is found. Flash size %d\n",itmp);
*/
  tzset();
  InitLib();
  f_ns_count();

  ComPortHost=4;


    if( ( bf_pml = (struct stor_buf *)malloc( (size_t)count )) == NULL )
    {
      exit (0);
    }

 GetTimeDate(&tim_dat);
 mdat.tm_year  = tim_dat.year -1900 ;
 mdat.tm_mon   = tim_dat.month-1 ;
 mdat.tm_mday  = tim_dat.day   ;
 mdat.tm_hour  = tim_dat.hour  ;
 mdat.tm_min   = tim_dat.minute;
 mdat.tm_sec   = tim_dat.sec   ;

    ltmp = mktime( &mdat );
    st_pml0.time=ltmp;
    for(i=0;i<pmln_b_lgth;i++)
    {
//       bf_pml[i].type_n = 0;
//       bf_pml[i].time = ltmp;
         bf_pml[i]=st_pml0;
    }

  *(long int *)&(TimeStamp)=0;
  InstallCom(ComPortHost,115200L,8,0,1);

  for(;TimeStamp < 600;)
  {
    if(!IsCom(ComPortHost)) continue;
    key=ReadCom(ComPortHost);
    if(key == '\b')
    {
      flag_H=1;
      printf("\n\r BS pushed  ");
      while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */

      break;
    }
    if(key == 0x1b )
    {
      flag_H=2;
      printf("\n\r ESC pushed  ");
      while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */

      break;
    }
  }

//  RestoreCom(4);
/*
  RestoreCom(1);
  RestoreCom(2);
  RestoreCom(3);
  RestoreCom(4);
*/
//  X607_Init();
  InitEEPROM();

  f_get_flash_p();

  f_wr_evt(evt_t_off); // проверить передана ли вся информация перед выключением
  f_wr_evt(evt_t_on);

  f_chk_break();

  if (p1_state & End_dlvr);
  else
  {
     VolT_id   = 0;
     MassT_id=0;
     summ_v_id=0;

     if(cost_v <=0)
     {
       cost_v=1;
     }

     p1_state |= End_dlvr;
     n_msg_t='1';
  }

  RestoreCom(4);


  flag_echo_host=1;

  f_queue_init();
  f_ee_num_init();
  Init5DigitLed();


   SetDo1(0);
   SetDo2(1);


  if( flag_H == 2)  goto m_n_ee;
  if(f_chk_EEE_CRC()<0)
  {
 m_n_ee:
    InstallCom(ComPortHost,ComBaud[4],8,0,1);
    ShowCom(ComPortHost,8,ComBaud[ComPortHost]);
    printf("CRC Error.Default loaded\n");
    f_icp_errS(EEE_CRC_error);
    sw_mmi=99;
    f_rd_cor();
  }
  else
  {
    f_rd_eee();
    f_rd_cor();
    if( flag_H != 0) flag_Serv=0;

    if((flag_Serv==0)||(ComPortHost != ComPortPmp1 ) )
    {
     if(f_get_nb(ComBaud[ComPortHost]) <0 ) ComBaud[ComPortHost]=115200L;
     InstallCom(ComPortHost,ComBaud[ComPortHost],Com_lgth[ComPortHost],Com_parity[ComPortHost],Com_stop[ComPortHost]);
     ShowCom(ComPortHost,Com_lgth[ComPortHost],ComBaud[ComPortHost]);
    }
    if(flag_Serv==1)
    {
     if(f_get_nb(ComBaudPmp1) <0 ) ComBaudPmp1=4800L;
     InstallCom(ComPortPmp1,ComBaudPmp1,Com_lgthPmp1,Com_parityPmp1,Com_stopPmp1);
     ShowCom(ComPortPmp1,Com_lgthPmp1,ComBaudPmp1);
    }
    if(FlagWinSum) init_win_sum(1);
  }

//  baud=ComBaud[4];
//ShowPort(ComPortHost);
//ShowChecksum(checksum);
//ShowBaudrate(ComBaud[ComPortHost]);
/*
    if(EepType==16)
     printf("EEPROM 24LC16 is found (2k bytes)\n");
    else if (EepType==128)
     printf("EEPROM 24WC128 is found (16k bytes)\n");
    else
     printf("Unknown EEPROM.\n");
*/
//  TimerOpen();

  f_clr_scr_MMI();

#if defined(ICP_TSD)
  f_clr_TSD(0);
//f_clr_TSD(1);
  f_rev_TSD(0);
//f_rev_TSD(1);
#endif

#if defined(TSD_GLB)
  f_clr_TSD(0);
//f_clr_TSD(1);
  f_rev_TSD(0);
//f_rev_TSD(1);
#endif


  if(sw_mmi==0 )
  {

   if((flag_Serv==0)||(ComPortHost != ComPortPmp1 ) )
#if( Test == 0)
     printf("\nFacom Ver.%s",sw_ver);
#else
     printf("\nFacom.Test version.MVD simulate. Ver. %s",sw_ver);
#endif
   f_prn_begin();
  }
  else f_prn_CRC_error();

//  SetDisplayPage(6);
  SetDisplayPage(ZeroPage);

 /* MMI use I_7188 COM1(RS-232,RS-485),9600,N,8,1 */

  for(i=1;i<4;i++)
  {
   if(i != ComPortPmp1)
   {
    if(f_get_nb(ComBaud[i]) <0)  ComBaud[i]=9600L;
    InstallCom(i,ComBaud[i],Com_lgth[i],Com_parity[i],Com_stop[i]);
   }
  }

#if defined(ICP_TSD)
   f_init_TSD();
   f_init_TSD_ICP();
#endif

#if defined(TSD_GLB)
   f_init_TSD();
#endif


 if((flag_Serv==0)||(ComPortHost != ComPortPmp1 ) )
 printf("\n\r>");

  cost_v_c=cost_v;

// if(I7017C[0].status==0)
 {
    s_MVD[0].Press=  analog_offset[0];
    Press1=  analog_offset[1];
    Press2=  analog_offset[2];
    Temp1=   analog_offset[3];
    Temp2=   analog_offset[4];
    Press3=  analog_offset[5];
    Temp3=   analog_offset[6];
 }

 /*
    if(MVD[0].status != 0)
      F_GR2=0;
    else
      F_GR2=1;
 */

      if(Tank_nn == 1)
      {
       res_ctr[1]=1;
       res_ctr[2]=1;
       res_ctr[3]=1;
      }
      else if(Tank_nn == 2)
      {
       res_ctr[2]=1;
       res_ctr[3]=1;
      }
      else if(Tank_nn == 3)
      {
       res_ctr[3]=1;
      }

  one_sec_time=TimeStamp;
  while(1)
  {
     if(f_cycle()<0) break;
  }
printf("\n\rExit\n\r");

  RestoreCom(1);
  RestoreCom(2);
  RestoreCom(3);
  while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */
  RestoreCom(4);


  free( bf_pml );

}

/* ================================================== */
int last_out[5]={-1,-1,-1,-1,-1};
long int scom_tim[5]={6,6,6,6,6};
long int Tm_snd[5]= {0,0,0,0,0};


unsigned long int time_snd[5]={0,0,0,0,0};
unsigned long int time_rcv[5]={0,0,0,0,0};


void ServiceCOM( int ii)
/*
  Функция принимает ответ из SLAVE контроллера
  Функция принимает ответ на команду посланную в  COM ii.
  После завершения приема команды, принятая команда обрабатывается
  потребителем, пославшим команду в Slave
*/
{
int i,Addr_rcv,itmp;
struct COM_rqst *request;
struct s_icp_dev *ICPl_DEV;
int  tmp;

  if(  f_timer(time_rcv[ii],scom_tim[ii]) == 0)
  {
   if(IsCom(ii)!=0)
   {
     while(IsCom(ii)!=0)
     {
      tmp= ReadCom(ii);

    if(flag_Deb)
      printf("\n%02X",tmp&0xff);
      time_rcv[ii]= TimeStamp;
     }
   }
   return;
  }

  if(COM_que[ii].busy_ptr != COM_que[ii].empt_ptr)
  {
    if(last_out[ii] != COM_que[ii].empt_ptr)
    {
      request=COM_que[ii].request[COM_que[ii].empt_ptr];
      if(request==NULL)
         goto m1;

      last_out[ii]=COM_que[ii].empt_ptr;

      // подготовка к обслуживанию запроса и вывод в порт

      ff_gets_com[ii]=request->gets_com;
      ff_answ_com[ii]=request->answ_com;
      ff_answ_flt[ii]=request->answ_flt;
      ToutAns[ii]=request->timeout;
      time_snd[ii]= TimeStamp;
      Tm_snd[ii]= TimeStamp;

      ICPl_DEV=request->ICP_dd;
      ICPl_DEV->time_stamp_snd=TimeStamp-request->time_stamp;

      // вывод в порт
//  ltt1=TimeStamp;
//  tim1();

#if defined(I7188D)
  Set485DirToTransmit(ii); //qqq
#endif

      ToComBufn(ii,request->Cmd,request->cmd_lgth);

#if defined(I7188D)
  WaitTransmitOver(ii);
  Set485DirToReceive(ii);
#endif

//  tim2();
//  ltt2=TimeStamp;

//   itt1=ttt2-ttt1-10;  // 10 следует отнять - это время присутствует
//   if(itt1 < 0) itt1+=10000;
//   rltt1=ltt2-ltt1;

//   if(flag_prn)
//   {
//     flag_prn=0;
//     printf("\n %d bytes send, time=%ld ms, %.1f mks",request->cmd_lgth,rltt1,(float)(itt1)/10.);
//   }
      request->status=2;
    }
  }

m1:
  if( (*ff_gets_com[ii])(ii) )
 /* Ввод ответа на команду. Указатель на функцию ввода
    устанавливает функция, инициировавшая передачу команды
 */
  {
      time_rcv[ii]= TimeStamp;
      request=COM_que[ii].request[COM_que[ii].empt_ptr];
      ICPl_DEV=request->ICP_dd;
      ICPl_DEV->time_stamp_rcv=TimeStamp - request->time_stamp;

   /* в cb_COM[ii][] находится команда длиной  n_bcom[ii]  */

   /* Обработка команды. Указатель на функцию обработки устанавливает
      функция, инициирующая передачу
   */

     (*ff_answ_com[ii])(ii);

     ff_gets_com[ii]=f_empty_gets;
     ff_answ_com[ii]=f_empty;
     n_bcom[ii]=0;
  }
}
/* ================================================== */
int f_timer(unsigned long int time_stamp,unsigned long int delay)
{
// если время с момента записи временного штампа превышает
 // delay мс - возвращает 1, иначе 0.
// if((TimeStamp-time_stamp) > delay) return 1;
// else return 0;
 if((TimeStamp-time_stamp - delay) & 0x80000000 ) return 0;
 else return 1;
}
/* ================================================== */
 char cb_COM[5][n_bufcom];
 int ghj=0;
 int n_bcom[5]={0,0,0,0,0};

void (*ff_serv)() = NULL;

int  (*ff_gets_com[5])(int ii) = {f_empty_gets,f_empty_gets,f_empty_gets,f_empty_gets,f_empty_gets};
void (*ff_answ_com[5])(int ii) = {f_empty,f_empty,f_empty,f_empty,f_empty};
void (*ff_answ_flt[5])(int ii) = {f_empty,f_empty,f_empty,f_empty,f_empty};
int i_empty;
void f_empty(int ii)
{
 i_empty=ii;
}

/* ---------------------------------------- */


/*-----------------------------------------*/
/*================================================================*/
/*================================================================*/
float ftmp_naMMI[10];
struct dis_set_MMI ds_list1[]=
{
//------------
  list1_dsr,
  84,               // Статус PUMALAN
  &flag_Serv,
  0,
  1,
  T_INT,
//----- addr PLAN --
  list1_dsr,
  83,               // Адрес PUMALAN
  &P_addr,
  0,
  31,
  T_INT,
//-----------
  list1_dsr,
  12,            // P рсхдм.полн.шк,МПа
  &ftmp_naMMI[0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  0,             // P рсхдм.смещ.,МПа
  &analog_offset[0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  13,             // P рез1.полн.шк,МПа
  &ftmp_naMMI[1],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  1,             // P рез1.смещ.,МПа
  &analog_offset[1],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  14,              // P рез2.полн.шк,МПа
  &ftmp_naMMI[2],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  2,             // P рез2.смещ.,МПа
  &analog_offset[2],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  25,              // P рез3.полн.шк,МПа
  &ftmp_naMMI[5],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  26,             // P рез3.смещ.,МПа
  &analog_offset[5],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  15,              // T рез1.полн.шк,C
  &ftmp_naMMI[3],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  3,               // T рез1.смещ.,C
  &analog_offset[3],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  16,              // T рез2.полн.шк,C
  &ftmp_naMMI[4],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  4,               // T рез2.смещ.,C
  &analog_offset[4],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  27,              // T рез3.полн.шк,C
  &ftmp_naMMI[6],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  28,               // T рез3.смещ.,C
  &analog_offset[6],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  7,              // Плотность 1,кг/м3
  &s_DGM[0].DensReal,
  500.,
  1200,
  T_FLOAT,
//----------------
  list1_dsr,
  8,              // Температура 1,C
  &s_DGM[0].TempReal,
  -50.,
  100,
  T_FLOAT,
//----------------

  list1_dsr,
  9,              // Плотность 2,кг/м3
  &s_DGM[1].DensReal,
  500.,
  1200,
  T_FLOAT,
//----------------
  list1_dsr,
  10,              // Температура 2,C
  &s_DGM[1].TempReal,
  -50.,
  100,
  T_FLOAT,
//----------------
//-----------
  list1_dsr,
  136,              // WD пер.PUMALAN,мс
  &WD_PMP_Per,
  0,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr,
  155,              // WD пер.PUMALAN,мс
  &WD_PMP_Char,
  0,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr,
  134,           // MAX недолив
  &ex_m,
  0,
  12.100,
  T_FLOAT,
//----------------
  list1_dsr,
  135,           // MAX перелив
  &ex_p,
  0,
  12.100,
  T_FLOAT,
//----------------
  list1_dsr,
  130,           // MAX перелив
  &ex_p_d,           // MAX для трассировки
  -1,
  2000,
  T_FLOAT,
//----------------
  list1_dsr,
  150,           // Round Flag
  &flag_round,
  0,
  10000,
  T_INT,
//----------------
  list1_dsr,
  137,           // Объем опред.переп BIG
  &val_flch,
  0,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  148,           // Объем опред.переп LTL
  &val_flch2,
  0,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  143,           // Flow last,l/ms
  &flow_mom,
  0,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  149,           // Flow min,l/ms
  &flow_min_c,
  0,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  139,           // Объем начала опред.переп
  &val0_flch,
  0,
  BIG_P,
  T_FLOAT,
//-----------
  list1_dsr,
  138,           // Время переполн,тик
  &time_flch,
  0,
  max_fl_ch_m1,
  T_INT,
//-----------
  list1_dsr,
  140,           // Время запр.переп,тик
  &TimeBegFlch,
  0,
  BIG_P,
  T_INT,
//-----------
  list1_dsr,
  99,              // ост.объем кл.2
  &CL_val[1],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
 101,              //время срабат. кл.2
  &CL_T2,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  98,              // ост.объем кл.1
  &CL_val[0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  100,            //время срабат. кл.1
  &CL_T1,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
 102,              //нач.запр.оп.MMI,мс
  &CL_T3,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
 103,             //длит.запр.о.MMI,мс
  &CL_T4,
  -BIG_P,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr,
  123,           //T зад.вкл.кл1,мс
  &cl1_delay_off,
  0,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr,
  141,
  &cl2_delay_off,
  0,
  BIG_P,
  T_INT_L,
//---------------
  list1_dsr,
  142,
  &Out3_delay_off,
  0,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr,
  104,          // подстр.отп. 0...1
  &fct_offs,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  105,          // подстр.прм. 0...1
  &fct_offs_r,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  106,
  &t_mvd_cor,
  0,
  BIG_P,
  T_INT_L,
//----------------
//-----------
/*
//-- Номер входа --
  list1_dsr,
  78,               // Номер входа Gun
  &num_in1,
  0,
  4,
  T_INT,
//-----------
*/

//-- Номер выхода --
  list1_dsr,
  80,               // Клапан Блш N вых.
  &num_out1,
  0,
  4,
  T_INT,
//-----------
  list1_dsr,
  81,               // Клапан Мнш N вых.
  &num_out2,
  0,
  4,
  T_INT,
//-----------
  list1_dsr,
  146,               // Насос, N выхода
  &num_out3,
  0,
  4,
  T_INT,
//-----------
  list1_dsr,
  147,               // Клап.Запорн.N вых.
  &num_out4,
  0,
  4,
  T_INT,
//-----------
  list1_dsr,
  51,
  &MVD[0].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  52,
  &MVD[1].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  56,
  &TSD[0].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  18,
  &I7060[0].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  24,
  &I7060[1].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  6,
  &I7017C[0].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  53,
  &LIn[0].status,
  0.,
  1.,
  T_INT,
//-----------
/*=====
//-----------
  list1_dsr,
  34,
  &ToutLicpScan,
  0.,
  BIG_I,
  T_INT,
//-----------
  list1_dsr,
  35,
  &licp_amax,
  0.,
  255,
  T_INT,
//-----------
  list1_dsr,
  36,
  &LICP_Rqst.CRC_flag,
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  85,
  &ComProt[1],
  0,
  2,
  T_INT,
//-----------
  list1_dsr,
  86,
  &ComProt[2],
  0,
  2,
  T_INT,
//-----------
  list1_dsr,
  87,
  &ComProt[3],
  0,
  2,
  T_INT,
//-----------

 =========*/

//-----------
  list1_dsr,
  94,
  &TSD_vn[0][0] ,
  1,
  18,
  T_INT,
//-----------
  list1_dsr,
  95,
  &TSD_vn[0][1] ,
  1,
  18,
// (float)(sizeof(list_ns) / sizeof(&summ_v)-1),
  T_INT,
//-----------
  list1_dsr,
  96,
  &TSD_vn[0][2] ,
  1,
  18,
  T_INT,
//-----------
/*
  list1_dsr,
  91,
  &n_dot[0][0],
  1,
  6,
  T_INT,
//-----------
  list1_dsr,
  92,
  &n_dot[0][1],
  1,
  6,
  T_INT,
//-----------
  list1_dsr,
  93,
  &n_dot[0][2],
  1,
  6,
  T_INT,
//-----------
  list1_dsr,
  97,
  &TSD_period,
  1,
  BIG_P,
  T_INT_L,
//-----------
*/
/*
//----------------
  list1_dsr,
  113,
  &offs_cor,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  114,
  &dsc_tmp,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
*/
/*****************************
  list1_dsr,
  115,
  &PermPlus,
  1,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  116,
  &PermMinus,
  0,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  117,
  &t20c,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  118,
  &v_Proc,
  0,
  100,
  T_FLOAT,
//----------------
  list1_dsr,
  119,
  &flag_20C,
  0,
  3,
  T_INT,
//----------------
  list1_dsr,
  120,
  &ModeFill,
  1.,
  3.,
  T_INT,
//----------------
******************************/

/*****************************
// MMU
//----------------
  list1_dsr,
  131,
  &un_mmi1,
  1.,
  6.,
  T_INT,
//----------------
  list1_dsr,
  132,
  &un_mmi2,
  1.,
  6.,
  T_INT,
//----------------
  list1_dsr,
  133,
  &un_mmi3,
  1.,
  6.,
  T_INT,
//----------------



// DNC
//----------------
  list1_dsr,
  121,
  &k_Den_Off,
  0,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  122,
  &k_Den_On,
  0,
  BIG_P,
  T_FLOAT,
//----------------
******************************/
/******************************
  list1_dsr,
  124,
  &TimDensOff,
  0,
  BIG_P,
  T_INT_L,
//----------------
  list1_dsr,
  125,
  &TimDensOn,
  0,
  BIG_P,
  T_INT_L,
//----------------
//DTC
//----------------
  list1_dsr,
  126,
  &Density,
  0,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  127,
  &Temperature,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  128,
  &Composition,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  129,
  &Dens0,
  0,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  130,           // темпер.привед.,C
  &Temperature1,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
******************************/

//----- COMP N--
/*
  list1_dsr,
  77,               // COMP Номер порта
  &ComPortPmp1,
  3,
  4,
  T_INT,
*/
//-----------
  list1_dsr,
  151,
  &rnd_add,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  152,
  &rnd_lim,
  0,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list1_dsr,
  153,
  &round_summ,
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  list1_dsr,
  126,
  &rnd_add_m,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  list1_dsr,
  154,
  &round_summ_m,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
/*
//-- инверсия входа --
  list1_dsr,
  79,               // Инвер-я входа Gun
  &mski_inp1,
  0,
  15,
  T_INT,
*/
//-----------
  list1_dsr,
  30,           // Тип изм.плотн.
  &dnc_mode,
  0,
  1,
  T_INT,
/*----------------*/
  list1_dsr,
  33,           // Упр.розеткой
  &Socket_mode,
  0,
  1,
  T_INT,
/*----------------*/
  list1_dsr,
  31,          // Кол-во резер-в
  &Tank_nn,
  1,
  3,
  T_INT,
/*----------------*/
  list1_dsr,
  32,           // Тип колонки(0,1)
  &F_GR2,
  0,
  1,
  T_INT,
/*----------------*/
  list1_dsr,
  34,           // Колич.расхдм
  &MVD_FLOW_t,
  1,
  3,
  T_INT,
/*----------------*/
//-----------
  list1_dsr, // Переход летнее время
  89,
  &FlagWinSum,
  0,
  1,
  T_INT,
//-----------
//-----------
  NULL,
  0,
  NULL,
  0,
  1,
  T_INT,
//-----------
//-----------
};
/*-----------------------------------------*/
int max_list1=(sizeof(ds_list1)/sizeof(ds_list1[0])) -1;
/*-----------------------------------------*/
long int time1=0,dd_time,dd_max=-1,dd_min=10000000;
void f_time_dd()
{
long int ltmp;
ltmp=TimeStamp;
dd_time=ltmp-time1;
time1=ltmp;
if(dd_time > dd_max) dd_max=dd_time;
else if(dd_time < dd_min) dd_min=dd_time;
}
//---
void f_print_dd()
{
 printf(" %ld , min= %ld, max=%ld",dd_time,dd_min,dd_max);
 dd_min=10000000;
 dd_max=-1;
}
/*-----------------------------------------*/
float cl_val_c=-100000;
float cl_t_c=-100000;
void f_wr_cor()
{
float ftmp;
union  { float f; char c[4]; } o;

    ftmp=CL_val[1]+CL_valR[1];

    if( (cl_val_c != CL_val[1]) || (cl_t_c != CL_valR[1] ) )
    {
       o.f= CL_val[1] ;
       cl_val_c=o.f;
       WriteNVRAM((int) nr_dv0   ,(int)o.c[0] );
       WriteNVRAM((int) nr_dv1   ,(int)o.c[1] );
       WriteNVRAM((int) nr_dv2   ,(int)o.c[2] );
       WriteNVRAM((int) nr_dv3   ,(int)o.c[3] );

       o.f= CL_valR[1] ;
       cl_t_c=o.f;
       WriteNVRAM((int) nr_dv0_r ,(int)o.c[0] );
       WriteNVRAM((int) nr_dv1_r ,(int)o.c[1] );
       WriteNVRAM((int) nr_dv2_r ,(int)o.c[2] );
       WriteNVRAM((int) nr_dv3_r ,(int)o.c[3] );

       o.f= ftmp;
       WriteNVRAM((int) nr_sm0   ,(int)o.c[0] );
       WriteNVRAM((int) nr_sm1   ,(int)o.c[1] );
       WriteNVRAM((int) nr_sm2   ,(int)o.c[2] );
       WriteNVRAM((int) nr_sm3   ,(int)o.c[3] );
    }
}
/*-----------------------------------------*/
void f_rd_cor()
{
union  { float f; char c[4]; } o;
float ftmp,ftmp1,ftmp2,ftmp3;

 o.c[0]= ReadNVRAM( nr_dv0 );
 o.c[1]= ReadNVRAM( nr_dv1 );
 o.c[2]= ReadNVRAM( nr_dv2 );
 o.c[3]= ReadNVRAM( nr_dv3 );
 ftmp = o.f;
 o.c[0]= ReadNVRAM( nr_dv0_r );
 o.c[1]= ReadNVRAM( nr_dv1_r );
 o.c[2]= ReadNVRAM( nr_dv2_r );
 o.c[3]= ReadNVRAM( nr_dv3_r );
 ftmp1 = o.f;
 ftmp3=ftmp+ftmp1;
 o.c[0]= ReadNVRAM( nr_sm0 );
 o.c[1]= ReadNVRAM( nr_sm1 );
 o.c[2]= ReadNVRAM( nr_sm2 );
 o.c[3]= ReadNVRAM( nr_sm3 );
 ftmp2 = o.f;
 if(ftmp2== ftmp3)
 {
   CL_val[1]=ftmp;
   CL_valR[1]=ftmp1;

   if(CL_valR[1] < -40) CL_valR[1]= 0;
   if(CL_valR[1] >  40) CL_valR[1]= 0;
 }
}
/*-----------------------------------------*/
unsigned long int one_sec_time=0;
int day_c=-1;
void f_one_sec()
{
union  { float f; char c[4]; } o;
  int win_sum;
  TIME_DATE t;

 if(flag_pool  != 0 )
 {
  if( f_timer(one_sec_time,1000L) == 0)  return;

  one_sec_time+=1000;

  GetTimeDate(&t);

  if(day_c == -1)
  {
    day_c=t.day;
  }
  else if(t.day != day_c)
  {
    day_c=t.day;
    f_wr_evt(evt_dsp_grk);
    f_wr_evt(evt_ext_dt1);
    f_wr_evt(evt_ext_dt2);
  }
  if(FlagWinSum)
   {
      if((t.sec <= 2)&& (t.minute==0))
        if((t.day >= 25)&&(t.weekday==0))
        {
          if( (t.month==3)&& (t.hour==2) )
          // в 2 часа ,псл Вск, mart,ч+=1
          {  /*переход на летнее время */

           win_sum = ReadNVRAM( nr_win_sum )  ;
           if(win_sum==0)
           { // еще не переведено
            f_wr_evt(evt_none);
            t.hour++;
            win_sum=1;  // 1 - лето
            SetTime((int)t.hour,(int)t.minute,(int)t.sec);

            WriteNVRAM((int)nr_win_sum,(int)win_sum );
            f_wr_evt(evt_ch_sum);
           }
          }
          else if((t.month==10)&& (t.hour==3) )
           // в 3 часа ,псл Вск, oct,ч-=1
          { //переход на зимнее время

            win_sum  = ReadNVRAM( nr_win_sum )  ;
            if(win_sum)
            { // еще не переведено
              f_wr_evt(evt_none);
              t.hour--;
              win_sum=0; // зимнее
              SetTime((int)t.hour,(int)t.minute,(int)t.sec);
              WriteNVRAM((int)nr_win_sum,(int)win_sum );
              f_wr_evt(evt_ch_win);
            }
          }
        }
   }
    WriteNVRAM((int)nr_year   ,(int)(t.year-2000));
    WriteNVRAM((int)nr_month  ,(int)t.month  );
    WriteNVRAM((int)nr_day    ,(int)t.day    );
    WriteNVRAM((int)nr_hour   ,(int)t.hour   );
    WriteNVRAM((int)nr_min    ,(int)t.minute );
    WriteNVRAM((int)nr_sec    ,(int)t.sec    );

//  if(s_MVD[GR].VolI>=0)
  {
    o.f=  s_MVD[GR].VolI;
    WriteNVRAM((int) nr_v0   ,(int)o.c[0] );
    WriteNVRAM((int) nr_v1   ,(int)o.c[1] );
    WriteNVRAM((int) nr_v2   ,(int)o.c[2] );
    WriteNVRAM((int) nr_v3   ,(int)o.c[3] );
  }
//  if(s_MVD[GR].MassI>=0)
  {
    o.f=  s_MVD[GR].MassI;
    WriteNVRAM((int) nr_m0   ,(int)o.c[0] );
    WriteNVRAM((int) nr_m1   ,(int)o.c[1] );
    WriteNVRAM((int) nr_m2   ,(int)o.c[2] );
    WriteNVRAM((int) nr_m3   ,(int)o.c[3] );
  }
 }
}

/*-----------------------------------------*/
int init_win_sum(int cor)
{
//  возвращает признак зимнего или летнего времени
//  для текущего момента: 1 - летнее время , 0 - зимнее время
//  если cor != 0 корректирует текущее время при переходе зим./летн.

  int year,month,day,hour,min,sec;
  int win_sum;
  time_t ltime1,ltime2;
  int prev_ws;

  struct tm *tdat;

 int i,i1;
 TIME_DATE t;
struct arch_str_t tim_0;

 f_prepare_t(&tim_0);
 GetTimeDate(&t);
/*
 tim_0.year  = t.year-2000;
 tim_0.month = t.month;
 tim_0.day   = t.day;
 tim_0.hour  = t.hour;
 tim_0.min   = t.minute;
 tim_0.sec   = t.sec;
*/

// printf("\n\r %02d.%02d.%02d  " ,tim_0.day,tim_0.month,tim_0.year);
// printf("%02d.%02d.%02d -- ",(int)tim_0.hour,(int)tim_0.min,(int)tim_0.sec);

 ltime1=f_get_time_t(&tim_0); // текущее время

  switch((int)tim_0.month)
  {
   case 11:
   case 12:
   case 1:
   case 2: // зима
         win_sum=0;
         break;
   case 4:
   case 5:
   case 6:
   case 7:
   case 8:
   case 9: // лето
         win_sum=1;
         break;
   case 3:  // март
        if(t.day>= 10) i=t.day-t.weekday;
        else i=t.day+7-t.weekday;
        for(;i<=31;i+=7)
         {
          i1=i;
         }
//        printf("\n\r==Last Sunday - %d Mart ",i1);
      // i1 - число последнего воскресенья марта
         tim_0.day=i1;
         tim_0.hour=2;
         tim_0.min=0;
         tim_0.sec=0;

    ltime2=f_get_time_t(&tim_0); // время Ч

    if(ltime1<ltime2)
         win_sum=0;
    else
         win_sum=1;
    break;
   case 10:  // октябрь
        if(t.day>= 10) i=t.day-t.weekday;
        else i=t.day+7-t.weekday;
        for(;i<=31;i+=7)
         {
          i1=i;
         }
      // i1 - число последнего воскресенья октября
         tim_0.day=i1;
         tim_0.hour=3;
         tim_0.min=0;
         tim_0.sec=0;
 //       printf("\n\r==Last Sunday - %d Oct ",i1);

// printf("\n\r %02d.%02d.%02d  " ,tim_0.day,tim_0.month,tim_0.year);
// printf("%02d.%02d.%02d -- ",(int)tim_0.hour,(int)tim_0.min,(int)tim_0.sec);

    ltime2=f_get_time_t(&tim_0); // время Ч
//    printf("\n\rltime1=%ld,ltime2=%ld",ltime1,ltime2);

    if( (ltime2>ltime1)&& ((ltime2 - ltime1)< 3600L))
    {
      win_sum=ReadNVRAM(nr_win_sum);
      break;
    }
    if(ltime1<ltime2)
    {
         win_sum=1;
    }
    else
    {
         win_sum=0;
    }
    break;
  }

  if(cor)
  {
    prev_ws=ReadNVRAM(nr_win_sum);
//  printf("\n\r Win_sum_ee=%d ( 1 - summer),win_sum_real=%d",prev_ws,win_sum);

    if((prev_ws==0)&&(win_sum!=0))
    {// переход на летнее время

//    ltime1=ltime1+3600L-GM_COR;
//    tdat = gmtime( &ltime1 );

      ltime1+=3600L;
      tdat = localtime( &ltime1 );

      year   = tdat->tm_year+1900;
      month  = tdat->tm_mon+1;
      day    = tdat->tm_mday ;
      hour   = tdat->tm_hour;
      min    = tdat->tm_min;
      sec    = tdat->tm_sec;

      SetTime(hour, min,sec);
      SetDate(year, month, day);
      f_wr_evt(evt_ch_sum);
    }
    else if((prev_ws!=0)&&(win_sum==0))
    {// переход на зимнее время


      ltime1-=3600L;
      tdat = localtime( &ltime1 );

 //   ltime1=ltime1-3600L-GM_COR;
 //   tdat = gmtime( &ltime1 );

      year   = tdat->tm_year+1900;
      month  = tdat->tm_mon+1;
      day    = tdat->tm_mday ;
      hour   = tdat->tm_hour;
      min    = tdat->tm_min;
      sec    = tdat->tm_sec;
      SetTime(hour,min,sec);
      SetDate( year, month, day);
      f_wr_evt(evt_ch_win);
    }
  }
  WriteNVRAM((int)nr_win_sum,(int)win_sum );
  return win_sum;
}
/*-----------------------------------------*/
#if defined(MMI_ICP)
#include "MMI.c"
#endif

#if defined(MMI_NEW)
#include "MMI_new.c"
#endif


#include "7017c.c"
#include "i7060.c"
//#include "sk.c"

#if defined(RTU_Delta)
#include "delta.c"
#endif

#include "MVD.c"

// допустима только одна из двух строк
// необходимо объявить через #define требуемый флаг в device.h

#if defined(ICP_TSD)
// трехстрочный дисплей с протоколом ICP
  #include "tsd.c"
#endif

#if defined(TSD_GLB)
// трехстрочный дисплей Gilbarco
  #include "tsd_glb.c"
#endif

#include "avt_ctrw.c"

#include "icp.c"
#include "queue.c"

/*-----------------------------------------*/
