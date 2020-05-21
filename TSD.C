
// Трех-строчный дисплей, 6 символов в строке

#define ICP_TSD

// Для перехода к центам  p4.CST=0.01;    (scale_cost = 0.01)
// p1.TSD = 2, p3.TSD = 2;

float scale_cost=0.01; // масштаб цены
//---------------
int TSD_vn[max_TSD][3]={
8,7,3,  // ╤єььр, юс·хь, Ўхэр
8,7,3,
};
//---------------

//---------------
void f_init_TSD_ICP()
{
 int i,ii;

    ii=0;
    if(TSD[0].status)
       if(TSD[ii].port == 3)
       {
        i=3;
        ComBaud[i]=9600;
        Com_lgth[i]=8;
        Com_parity[i]=0;
        Com_stop[i]=1;
        RestoreCom(i);
        InstallCom(i,ComBaud[i],Com_lgth[i],Com_parity[i],Com_stop[i]);

        TSD[ii].n_transaction    = 0;
        TSD[ii].n_success        = 0;
        TSD[ii].n_timeout_error  = 0;
        TSD[ii].n_CRC_error      = 0;

        TSD_Rqst[ii].status=0;

        TSD_val_c[ii][0]=-BIG_P;
        TSD_val_c[ii][1]=-BIG_P;
        TSD_val_c[ii][2]=-BIG_P;
       }
}
//---------------
//---------------

float fict_TSD;
float *TSD_value[max_TSD][3]={
//&TSD_tst[0],&TSD_tst[1],&TSD_tst[2],
//&s_MVD[0].VolT,&s_MVD[0].Dens,&s_MVD[0].FlowM,
&s_MVD[2].VolT,&summ_v,&cost_v,
&fict_TSD,&fict_TSD,&fict_TSD,
};
//---------------
int n_dot[max_TSD][3]={
 2,2,2,
 2,2,0
};

//---------------
struct s_icp_dev TSD[max_TSD]=
{
// device 1
0,                // status
3,                // port
1,                // addr
"TSD/1",          // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
0,                // protocol
1,                // CRC_flag
200,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&TSD_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
1,                // port
3,                // addr
"TSD/2",        // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
0,                // protocol
1,                // CRC_flag
200,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&TSD_Rqst[1],   // *request
};
/*----------------------------*/
struct COM_rqst TSD_Rqst[max_TSD]={
//----------------------
// device 1
//--------------------------
0,                   //  состояние запроса
ffgets_com_ICP,      //  gets_com
f_TSD_rd,            //  answ_com
f_TSD_flt,        //  answ_flt
"TSD/1",             //  name
0,                   //  n_dev номер устройства в группе однотипных устройств
&TSD[0],             //  *ICP_dd
0,                   //  текущая функция
ToutAnsTSD,           //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                   //  состояние запроса
ffgets_com_ICP,      //  gets_com
f_TSD_rd,            //  answ_com
f_TSD_flt,        //  answ_flt
"TSD/2",             //  name
1,                   //  n_dev номер устройства в группе однотипных устройств
&TSD[1],             //  *ICP_dd
0,                   //  текущая функция
ToutAnsTSD,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/

int TSD_fn[max_TSD]={0,0};    // регистр функций драйвера

//  Если записать значение функции в регистр функции , например:
//  ComSK_fn[0]= RES_SK_ERR;  драйвер выполнит заданную функцию - в данном случае
// пошлет в устройство ComSK команду сброса привода.
//  После постановки запроса с установленной функцией в очередь на вывод в COM,
// регистр функции очищается драйвером.

int TSD_sw[max_TSD]={0,0};
int TSD_seq[16]={0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0};  // последовательность функций вывода

unsigned int TSD_inp_err[max_TSD]={0,0};  // счетчик ошибок ввода

char TsdBuf[max_TSD][3][10];
char TsdBuf_c[max_TSD][3][10];
char TsdBuf1[3][10];

char TsdStr[max_TSD][10];
int tsd_sw[max_TSD]={0,0};
int TsdStrN[max_TSD];
int fl_tsd_new[max_TSD]={1,1};
int fl_tsd_new1[max_TSD]={0,0};

//----------------------

void f_TSD(int ii)
{
 // ii - номер устройства
 //
int fn,sw;
unsigned char buf_tmp[20];
unsigned char *buf1_tmp;
int n_str;

 if(TSD[ii].status == 0) return;

//  f_print_TSD(ii);


  if((TSD_Rqst[ii].status == Req_Flt) ||
     (TSD_Rqst[ii].status == Req_OK));
  else return;

 if(f_queue_chk(TSD[ii].port)== 0) return;// очередь переполнена

 f_chk_new_TSD(ii);
 if((TSD_fn[ii] != 0) && (TSD_fn[ii] < F_PROGRESS))
      fn=TSD_fn[ii];
 else if( f_timer(TSD[ii].time_stamp_pool,TSD[ii].pool_time ))
      fn=TSD_WR_P;
 else return;

      switch(fn)
      {
          case TSD_WR_S:  // вывод строки
              buf1_tmp=TsdStr[ii];
              n_str=TsdStrN[ii];

              TSD_fn[ii]=F_PROGRESS;
              break;

          case TSD_WR_P:  // циклическая запись
          // формирует команды записи
             TSD_sw[ii]=(TSD_sw[ii]+1) & 0xf;
             sw=TSD_seq[TSD_sw[ii]];

             TSD_fn[ii]=0;

             switch (sw)
             {
              case 0:
              case 1:
              case 2:
               buf1_tmp=TsdBuf[ii][sw];
               n_str=sw+1;
               break;
              default:
               return;
             }
             fn+=sw;
             break;

          default:
          TSD_fn[ii]=0;
          return;
      }
// n_str - номер строки вывода
// buf1_tmp[] - содержание строки

    sprintf(buf_tmp,"~%02XT%01X%s",TSD[ii].addr,n_str,buf1_tmp);

           TSD_Rqst[ii].function = fn;
           f_prepare7000(buf_tmp,TSD[ii].CRC_flag);
//   printf("\n\r==%s==",buf_tmp);
           strcpy(TSD_Rqst[ii].Cmd,buf_tmp);
           TSD_Rqst[ii].cmd_lgth=strlen(buf_tmp);
           TSD_Rqst[ii].time_stamp=TimeStamp;
           TSD[ii].time_stamp_pool=TimeStamp;
           TSD[ii].n_transaction++;
           f_queue_put(TSD[ii].port, &TSD_Rqst[ii]) ;
}
/*----------------------------*/
void f_TSD_rd(int ii)
{
// ii - номер порта

 struct COM_rqst *request;
 int i,nn,fn;

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  nn=request->n_dev; // номер устройства  (0,1..)

  fn=request->function;
  if((fn==TSD_WR_S) || ((fn & 0xf8)== TSD_WR_P ) )
  { // ответ на команду записи
       if(cb_COM[ii][0]=='!')
       {
           TSD[nn].n_success++;
           TSD_inp_err[nn]=0;
           f_queue_free(ii,&TSD_Rqst[nn]);
           TSD_fn[nn]=0;
           return;
       }
  }

m_err:
         TSD[nn].n_CRC_error++;
         TSD_inp_err[nn]++;
         if(TSD_inp_err[nn]>=TSD_max_inp_err)
         {
           TSD_inp_err[nn]=TSD_max_inp_err+1;
           f_icp_error(&TSD[nn],RD_ERR );
         }
/*
         if(fn==TSD_WR_S)
         {
             TSD_fn[nn]=fn;
         }
         else
*/
  if(flag_prn)
        printf("\n\r CRC==%s===",TSD_Rqst[nn].Cmd  ); // QQQ

             TSD_fn[nn]=0;
         f_queue_free(ii,&TSD_Rqst[nn]);
}
/*-------------------------------------------*/
void f_TSD_flt(int ii)
{
 struct COM_rqst *request;
 int nn;

 request=COM_que[ii].request[COM_que[ii].empt_ptr];

 nn=request->n_dev; // номер устройства (0,1..)

 TSD[nn].time_stamp_rcv=0;
 TSD[nn].n_timeout_error++;

 TSD_inp_err[nn]++;
 if(TSD_inp_err[nn]>=TSD_max_inp_err)
 {
   TSD_inp_err[nn]=TSD_max_inp_err;
   f_icp_error(&TSD[nn],RD_ERR );
 }
 f_queue_Flt(ii);
}
/* ---------------------------------------- */
void f_clr_TSD(int ii)
{  // очистка экранного буфера
  _fmemset( TsdBuf[ii][0],' ',sizeof(TsdBuf1)); // очистка буфера экрана
   fl_tsd_new[ii]=1;
}
//----------------------
#define TSD_unused_char  0xff
void f_rev_TSD(int ii)
{
   // вызывает передачу всего экранного буфера в TSD
  _fmemset( TsdBuf_c[ii][0],TSD_unused_char,sizeof(TsdBuf1));  // заполнение буфера копии неиспоьзуемым значением
   fl_tsd_new[ii]=1;
}
//----------------------
long int TSD_time_pool=0;
long int TSD_period=50;  // ms

int  cur_str=0;

void f_chk_new_TSD(int ii)
{  // отображает содержание TsdBuf на дисплей
 char *buf,*buf_c;
 int i;

 if(TSD_fn[ii] != 0) return;

 if( fl_tsd_new1[ii] <= 0)
 {
   if( f_timer(TSD_time_pool,TSD_period) == 0) return;
 }
// else fl_tsd_new1[ii] --;

 TSD_time_pool=TimeStamp;

 /*-------------------------*/
 // Сравнение буфера экрана и вывод в TSD при наличии расхождений

  for(i=0;i<3;i++)
  {

   buf=TsdBuf[ii][cur_str];
   buf_c=TsdBuf_c[ii][cur_str];

   if( _fmemcmp( buf, buf_c , 7) != 0)
   { //  найдены различия

    _fmemcpy(buf_c,buf,(long int)7);
    _fmemcpy(TsdStr[ii],buf,(long int)7);
    TSD_fn[ii]=TSD_WR_S;
    TsdStrN[ii]=cur_str+1;
    fl_tsd_new1[ii]=3;
    cur_str++;
    if(cur_str>2) cur_str=0;
    return;
   }


   cur_str++;
   if(cur_str>2) cur_str=0;

  }

  cur_str=0;
  fl_tsd_new1[ii]=0;

}
//----------------------
float fict_TSD=0;

float TSD_val_c[max_TSD][3]={
 -1,-1,-1,
 -1,-1,-1
};

int  tsd_swf[max_TSD]={0,0};
//375
#define Blink_time  500
long int Time_err_blink=0;
int flag_blink=0;
int fl_mtsd=-1,fl_mtsd_c=-1;

//----------------------------
void f_print_TSD(int ii)
{ // отображает значения float на дисплее

 int nn;
 int mm;
 char buf_tmp[20];
 float ftmp;

 if( FL_err  == 0)
 {

 if(
    ( flag_wdp == 0) ||
    ( (sw_mmi >= 209) && (sw_mmi <= 212) ) ||

    ((sw_mmi == 200) || (sw_mmi == 203))

   ) ;
 else goto m_err;


 if( f_timer(TSD_time_pool,TSD_period) == 0) return;
    TSD_time_pool=TimeStamp;

 if(TSD[ii].status != 0)
   if(TSD_fn[ii] != 0) return;

 //-------------------------
 // Сравнение буфера экрана и вывод в TSD при наличии расхождений

   if(fl_mtsd_c != fl_mtsd)
   {
     TSD_val_c[0][0]=BIG_P;
     TSD_val_c[0][1]=BIG_P;
     TSD_val_c[0][2]=BIG_P;
   }
   fl_mtsd_c = fl_mtsd;

   if ( (sw_mmi >= 209) && (sw_mmi <= 212) )
   {  // Измерение плотности
     fl_mtsd=2;


   if( Reg_State & ( DEN_fin | RCV_fin) )
       ftmp=Dens_Result;
   else
       ftmp=s_MVD[MVD_DNS].Dens;
     if( ftmp != TSD_val_c[ii][0])
     {
      TSD_val_c[ii][0]=ftmp;
      f_prep_TSD(2,TsdBuf[ii][0],ftmp);
//    f_prep_StrG(ii,0);
//    TSD_fn[ii]=TSD_WR_S;
     }

   if( Reg_State & ( DEN_fin | RCV_fin) )
       ftmp=Temp_Result;
   else
       ftmp=s_MVD[MVD_DNS].Temp; // QQQ

     if( ftmp != TSD_val_c[ii][1])
     {
      TSD_val_c[ii][1]=ftmp;
      flag_minus=1;
      f_prep_TSD(2,TsdBuf[ii][1],ftmp);
//    f_prep_StrG(ii,1);
//    TSD_fn[ii]=TSD_WR_S;
     }

     if(sw_mmi == 209)
     {
      ftmp=-1;
     }
     else
      ftmp=Tank_n+1;

     if( ftmp != TSD_val_c[ii][2])
     {
      TSD_val_c[ii][2]=ftmp;
      f_prep_TSD(0,TsdBuf[ii][2],ftmp);
//    f_prep_StrG(ii,2);
//    TSD_fn[ii]=TSD_WR_S;
     }
   }
   else if ((sw_mmi == 200) || (sw_mmi == 203))
   {  // Прием от газовоза
     fl_mtsd=1;
     ftmp=fabs(MassT);
     if( ftmp != TSD_val_c[ii][0])
     {
      TSD_val_c[ii][0]=ftmp;
//    f_prep_TSD(2,TsdBuf[ii][0],ftmp);
      f_prep_TSD(4,TsdBuf[ii][0],ftmp);
//    f_prep_StrG(ii,0);
//    TSD_fn[ii]=TSD_WR_S;
     }

     ftmp=fabs(VolT);
     if( ftmp != TSD_val_c[ii][1])
     {
      TSD_val_c[ii][1]=ftmp;
//    f_prep_TSD(2,TsdBuf[ii][1],ftmp);
      f_prep_TSD(4,TsdBuf[ii][1],ftmp);
//    f_prep_StrG(ii,1);
//    TSD_fn[ii]=TSD_WR_S;
     }

   if( Reg_State & ( DEN_fin | RCV_fin) )
       ftmp=Dens_Result;
   else
       ftmp=s_MVD[MVD_DNS].Dens;

     if( ftmp != TSD_val_c[ii][2])
     {
      TSD_val_c[ii][2]=ftmp;
      f_prep_TSD(1,TsdBuf[ii][2],ftmp);
//    f_prep_StrG(ii,2);
//    TSD_fn[ii]=TSD_WR_S;
     }
   }
   else
//    if((sw_dns<=0) && (sw_dlv_liq <=0) && (flag_rcv == 0) )
    {
     // Режим ГРК

     fl_mtsd=0;

     ftmp=fabs(*TSD_value[ii][0]) * scale_cost ;
     if( ftmp != TSD_val_c[ii][0])
     {
      TSD_val_c[ii][0]=ftmp;
      f_prep_TSD(n_dot[ii][0],TsdBuf[ii][0],ftmp);
//    f_prep_StrG(ii,0);
//    TSD_fn[ii]=TSD_WR_S;
     }

     ftmp=fabs(*TSD_value[ii][1]);
     if( ftmp != TSD_val_c[ii][1])
     {
      TSD_val_c[ii][1]=ftmp;
      f_prep_TSD(n_dot[ii][1],TsdBuf[ii][1],ftmp);
//    f_prep_StrG(ii,1);
//    TSD_fn[ii]=TSD_WR_S;
     }

     ftmp=fabs(*TSD_value[ii][2]) * scale_cost;
     if( ftmp != TSD_val_c[ii][2])
     {
      TSD_val_c[ii][2]=ftmp;
      f_prep_TSD(n_dot[ii][2],TsdBuf[ii][2],ftmp);
//    f_prep_StrG(ii,2);
//    TSD_fn[ii]=TSD_WR_S;
     }
    }


 }
  else   // (FL_err != 0)
  {
//    f_chk_new_TSD(0);
m_err:

    if( f_timer(Time_err_blink,Blink_time) == 0) return;
    Time_err_blink=TimeStamp;
//  fl_tsd_new[ii]=1;

    if(flag_blink==0)
    { // отображение ошибок
/*
     sprintf(TsdBuf[ii][0],"E %1x%1x%1x%1x",ICP_error[2],ICP_error[3],ICP_error[4],ICP_error[5]);
     sprintf(TsdBuf[ii][1],"E %1x%1x%1x%1x",ICP_error[6],ICP_error[7],ICP_error[8],ICP_error[9]);
*/

     sprintf(TsdBuf[ii][0],"  %1x%1x%1x%1x",ICP_error[2],ICP_error[3],ICP_error[4],ICP_error[5]);
     sprintf(TsdBuf[ii][1],"  %1x%1x%1x%1x",ICP_error[6],ICP_error[7],ICP_error[8],ICP_error[9]);
     sprintf(TsdBuf[ii][2],"  %04x",ICP_error[10] | flag_wdp);
 //    sprintf(TsdBuf[ii][2],"   ERR");
    }
    else
    { // гашение экрана
     sprintf(TsdBuf[ii][0],"      ");
     sprintf(TsdBuf[ii][1],"      ");
     sprintf(TsdBuf[ii][2],"      ");
    }
    flag_blink ^=1;
//   f_prep_StrG(ii,0);
//   f_prep_StrG(ii,1);
//   f_prep_StrG(ii,2);
  }

}
//----------------------------
//----------------------------
char buf_space[]="      ";

int flag_minus=0;

void  f_prep_TSD(int dot,char *buf,float f_val)
{
float f_value;

int itmp;
itmp=dot;

  fl_tsd_new1[0]=1;
  if( f_val < 0 )
  {

   if(flag_minus==0)
   {
m_err:
    strcpy(buf,buf_space);
    goto m_end;
   }
   else
     f_value=fabs(f_val);

  }
  else
     f_value = f_val;

flag_minus=0;
  if(itmp>3)
  {
/*
    if(f_value <= 9.99999)
    {
     itmp=5;
     goto m_sw;
    }
    if(f_value <= 99.9999)
    {
     itmp=4;
     goto m_sw;
    }
*/
    if(f_value <= 999.999)
    {
     itmp=3;
     goto m_sw;
    }
    if(f_value <= 9999.99)
    {
     itmp=2;
     goto m_sw;
    }

    if(f_value <= 99999.9)
    {
     itmp=1;
     goto m_sw;
    }
    if(f_value <= 999999)
    {
     itmp=0;
     goto m_sw;
    }
    goto m_err;
  }

m_sw:
  switch( itmp)
  {
  case 0:
     if(f_value > 999999) goto m_err;
     sprintf(buf,"%6.0f",f_value);
     break;

  case 1:
     if(f_value > 99999.9) goto m_err;
     sprintf(buf,"%7.1f",f_value);
     break;
  case 2:
     if(f_value > 9999.99) goto m_err;
     sprintf(buf,"%7.2f",f_value);
     break;
  case 3:
     if(f_value > 999.999) goto m_err;
     sprintf(buf,"%7.3f",f_value);
     break;
/*
  case 4:
     if(f_value > 99.9999) goto m_err;
     sprintf(buf,"%7.4f",f_value);
     break;

  case 5:
     if(f_value > 9.99999) goto m_err;
     sprintf(buf,"%7.5f",f_value);
     break;
*/
   default: goto m_err;

  }
m_end:
//printf("\n\r---%s---",buf);

}
//----------------------------
//----------------------------

