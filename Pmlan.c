
//
//
//       Протокол связи pmp1
//  PUMALAN Protocol, Marconi
//                cable, DSUB 9F
//
//                   GND      RCV-      TX+    +12V
//
//   AGZS servis      5       1,3       2,4
//
//     BERLIO         2         3         4      5
//
//
//     Berlio   1<Enter>   PG_DN  <Enter>  22  <Enter>  <Enter>
//              |            |
//            N КОЛОНКИ    режим          доза          записать в колонку
//                     (технология)
//
//
//    1<Enter>  CTR_-  - остановить отпуск
//    1<Enter>  SHT_-  - выключить колонку
//    1<Enter>  SHT_+  - включить колонку
//
//
//    11 Dec 2012 'D' как 'F'
//    31 Jan 2013  if(ftmp > 0)  ...
//


int uk_beg_pm=0;
int uk_fin_pm=0;
int flag_repeat=0;

// 23.08.11
//  добавлено
//в 'E'
//  p1_state &= ~Dlvr_rqst;  // ZZZZZ
//
//


//struct stor_buf bf_pml[pmln_b_lgth];
struct stor_buf *bf_pml;
struct stor_buf st_pml0={
0,
0,
0,
         "                     "
};

int ComPortPmp1 = 4;
int P_addr=0;      // 0x30 ... 0x4f
int flag_spec = 0; // флаг расширенного отчета

/*
unsigned long ComBaudPmp1=4800;
int Com_lgthPmp1=7;
int Com_parityPmp1=2;// Odd   //0-(None);1-(Even);2-(Odd);3-(MARK, always 1); 4-(SPACE,always 0)
int Com_stopPmp1=1;
int flag_fst_pmp1=1;
*/

unsigned long ComBaudPmp1=4800;
int Com_lgthPmp1  =7;
int Com_parityPmp1=2;// Odd   //0-(None);1-(Even);2-(Odd);3-(MARK, always 1); 4-(SPACE,always 0)
int Com_stopPmp1  =1;


int lamp_c=0; // OUT3 - L1
              // OUT4 - L2

int flag_fst_pmp1=1;   // устанавливается в f_cl_error()
int p1_state=0;
int credit_type=0;
float dec_val_p[]={1.,0.1,0.01,0.001,0.0001,0.00001,0.000001};

//
// -- данные посылаемые по окончании налива
//
float *cost_p=&cost_v;       // see tsd.c for variants
float cost_vvc=-1;
/*
// --первичные данные из расходомера
float *vol_p=&s_MVD[0].VolT;
float *amount_p=&summ_v;
*/

// -- красивые данные
float *vol_p=&VolT_id;
float *amount_p=&summ_v_id;

int cp_pmp1=0x30;
int opz_pmp1=0;


int flag_StopPmp1=0;

int flag_EOT=0;
int flag_ETX=0;
int flag_SEND=0;    // устанавливается, если в ответ на команду послан буфер
int flag_ACK_NAK=0; // устанавливается, если в ответ на команду послан NAK,ACK
int n_msg_r='1';
int n_msg_t='0';    // 0 - признак включения питания

#define max_out_pmp1  50

char buf_out_pmp1[max_out_pmp1];
int n_out_pmp1=0;

int prod_code=0x30;
//int Modef=0x50;// D0 - gun picked up
int Modef=0x50;// D0 - gun picked up
              // D1 - delivering in automatic mode
              // D2 - lamp1 on
              // D3 - lamp2 on
              // D4 - pump in automatic mode
              // D5 - pump in error condition, изменяется в f_cl_error(),f_icp_errS(),f_icp_error()
              // D6 = 1

int err_pmp1=0x00;
int mess_type=0;
/*----------------------------*/
int fl_pr_pmln=0;
int f_pump1()
{
int itmp;
//  int  hour,min,sec;
     // вводит команду из Host (PC) и интерпретирует ее по протоколу pmp1

         if(ffgets_pmp1(ComPortPmp1))
         {
          f_stor_pmp(M_RD,cb_COM[ComPortPmp1],n_bcom[ComPortPmp1]);

          if(IsCom(ComPortPmp1))
          {
           flag_SEND=0;
           mess_type=0;
           n_bcom[ComPortPmp1]=0;
           return 0;
          }

          if(IsTxBufEmpty(ComPortPmp1) == 0)
          {
            flag_SEND=0;
            mess_type=0;
            n_bcom[ComPortPmp1]=0;
            return 0;
          }

          _fmemcpy(intrpr.bfcom,&cb_COM[ComPortPmp1][4],(long int)(n_bcom[ComPortPmp1]-3) );
          n_bcom[ComPortPmp1]=0;
          f_intr_pmp1();
         }
 //        f_gun();
         return 0;
}
/*-------------------------------------------*/
int  WD_PMP_flag=1;
int  WD_PMP_Fl=0;
long int WD_PMP_time=0;
long int PMP_last_cht=0;
long int WD_PMP_Char=35;   // величина таймаута по приему символа
long int WD_PMP_Per=20000; // величина таймаута по приему сообщения
/*-------------------------------------------*/
int ffgets_pmp1(int port)
{
//  Функция принимает команду от Host контроллера (PC)
// EOT  IG   ad  STX  nmsgr <mess> ETX bcc
//     '0'  'x'        'x'   ....
//       0    1    2    3
//  выходная строка  cb_COM[port];
//  длина строки   n_bcom[port];
//  EOT не помещается в буфер и не обсчитывается контрольной суммой

 int tmp,tmp1;
 int i;
m1:

    if( f_timer(WD_PMP_time,WD_PMP_Per ))
    {// нет успешного приема
     if(WD_PMP_flag )
     {
       WD_PMP_flag=0;
       WD_PMP_Fl=1;
       f_icp_errS(WD_pmlan);
     }
    }

   if(!IsCom(port))
   {
/*
    if(IsTxBufEmpty(ComPortPmp1) == 0)
         PMP_last_cht=TimeStamp;
*/
    return 0;
   }

   tmp=ReadCom(port);

   if( f_timer(PMP_last_cht,WD_PMP_Char))
    {
     flag_ETX=0;
     flag_EOT=0;
     flag_SEND=0;
     mess_type=0;
     n_bcom[port]=0;
    }
   PMP_last_cht=TimeStamp;

// putch(tmp);

   if(flag_ETX)
   { // принят байт контрольной суммы
     // проверка телеграммы
     flag_ETX=0;
     flag_EOT=0;
     flag_SEND=0;
     if(cb_COM[port][0] != IG)
     {
       goto m0;
     }
     if(cb_COM[port][1] != (P_addr+'0'))
     {
       goto m0;
     }
     if(cb_COM[port][2] != STX )
     {
       goto m0;
     }
     if( (cb_COM[port][3] < '0' )|| (cb_COM[port][3] > '9' ) )
     {
       goto m0;
     }

//   tmp1=f_calc_csum_pmp1(cb_COM[port],n_bcom[port]);
//   printf("\n\r ksumm = %04x ,%c",tmp1,tmp1);
//   printf("\n\r ksumm_rcv = %04x",tmp);
     if( f_calc_csum_pmp1(cb_COM[port],n_bcom[port]) != tmp )
     {
       goto m_nak;
     }

     if (cb_COM[port][3] == '0' )
     {
       n_msg_r='0';
       n_msg_t='1';
       flag_SEND=0;
     }
     n_msg_r++;
     if(n_msg_r>='9')n_msg_r='1';
     n_bcom[port]--;
     cb_COM[port][n_bcom[port]]=0;

     WD_PMP_flag=1;
     WD_PMP_time=TimeStamp;
     if((ICP_error[icp_lst_max] | flag_wdp ) & WD_pmlan)
     {
      flag_wdp=0;
      ICP_error[icp_lst_max] &= ~(unsigned int)WD_pmlan;
      ICP_error_c[icp_lst_max]=ICP_error[icp_lst_max];
      for(i=0,tmp1=0;i<=icp_lst_max;i++)
      {
        tmp1 |=ICP_error[i];
      }
//     printf("\r\n tmp1= %04x",tmp1);

      if(tmp1==0)
      {
       FL_err=0;
       if(sw_mmi==0) f_prn_begin();
      }
     }
     return 1;
  m0:
     flag_ETX=0;
     flag_EOT=0;
     flag_SEND=0;
     return 0;
  m_nak:
     flag_ACK_NAK=NAK;
     flag_SEND=0;
     f_stor_pmp(M_WR,(char *)&flag_ACK_NAK,1);
     (*ff_putch[ComPortPmp1])(flag_ACK_NAK);
     goto m0;
   }

   if(tmp == ACK)
   { // мастер успешно принял ответное сообщение

     f_stor_pmp(M_RD,(char *)&tmp,1);

     if(IsTxBufEmpty(ComPortPmp1) == 0)
     {
       flag_SEND=0;
       mess_type=0;
       n_bcom[ComPortPmp1]=0;
       goto m2;
     }

     if(flag_SEND)
     {
       switch( mess_type)
       {
        case No_say:
        break;

        case End_dlvr:

 // сбросить флаг налива в NVRAM
 WriteNVRAM((int) nr_flg_n ,(int)0);

        p1_state &= ~mess_type;
        break;

        case Dlvr_rqst:
        p1_state &= ~mess_type;   // ZZZ
        break;

        case Status:
           flag_fst_pmp1=0;
        break;

        default: break;
       }
//     mess_type=0;
       n_msg_t++;
       if(n_msg_t>'9') n_msg_t='1';
  m2:
       flag_ACK_NAK=0;
       flag_SEND=0;
       mess_type=0;
  m3:
       flag_ETX=0;
       flag_EOT=0;
       return 0;
     }
     else goto m2;
   }

   if(tmp == NAK)
   {  // мастер запросил повторную передачу буфера
     f_stor_pmp(M_RD,(char *)&tmp,1);
     if(flag_SEND)
     {
       ToComBufn(port,buf_out_pmp1,n_out_pmp1);
       goto m3;
     }
     else goto m2;
   }

   if(tmp == ENQ)
   { // мастер не принял ACK/NAK и запрашивает повтор

    f_stor_pmp(M_RD,(char *)&tmp,1);
    flag_SEND=0;
    if(flag_ACK_NAK)
     (*ff_putch[port])(flag_ACK_NAK);
    goto m3;
   }

   if( tmp == EOT)
    { // начало приема телеграммы
     n_bcom[port]=0;
     flag_ETX=0;
     flag_SEND=0;
     mess_type=0;
     flag_ACK_NAK=0;
     flag_EOT=1;
     goto m1;
    }

    flag_SEND=0;
    mess_type=0;
    flag_ACK_NAK=0;

    if(flag_EOT==0) goto m1;
 // прием тела телеграммы
    cb_COM[port][n_bcom[port]]=tmp;

    if( (++n_bcom[port]) > (n_bufcom-1) )
     {
       n_bcom[port]=n_bufcom-1;
     }
    if(tmp==ETX) flag_ETX=1; // конец телеграммы, далее байт контрольной суммы

    goto m1;

  }
/* ================================================== */
int f_calc_csum_pmp1(char *buf, int nn)
{
int i,itmp;
  itmp=0;
  for(i=0;i<nn;i++)
   itmp ^= (int)buf[i] & 0xff;
  return itmp;
}
/*-------------------------------------------*/
// счетчик посылок с одинаковой ценой

int cost_old=-1;
int n_cnt_cst=0;

int tmp_cst=-1;

void f_cst()
{
         if(tmp_cst > 0)
         {
            if(tmp_cst != cost_old)
            {
             cost_old=tmp_cst;
             n_cnt_cst=0;
             return;
            }
            else if((++n_cnt_cst)< N_cst)
             {
               return;
             }
       //     else
            n_cnt_cst= N_cst;

            cost_v=tmp_cst;
            cost_vvc=tmp_cst;

            if(flag_fst_cst)
            {
             flag_fst_cst=0;
             cost_v_c=cost_v;
             flag_cst_chg=0;
             cnt_ltch=0;
            }
         }
         else if(tmp_cst < 0)
         {
          if(n_cnt_cst > 0 )
             n_cnt_cst--;
         }

}
/*-------------------------------------------*/
int flag_pmp_init=0;
int fl_ok_pmp=1;

#define t_wt_e_dlvr   1000

long int vol_deliv=0;
long int mass_deliv=0;
int cost_deliv=0;

int f_intr_pmp1()
{

int itmp00,itmp,itmp1,itmp2;
char buf_tmp[10];
float ftmp,ftmp1,ftmp2;
long int ltmp0,ltmp1;

        intrpr.i3=0;
        if(f_inw()==0)
        {
          intrpr.n_bcom=0;
          goto fin;
        }
   //   printf("\n\r--%s--",intrpr.wrd);
//--------------------------------------------------------
       if (!strcmp(intrpr.wrd,"P" ))
        {   //'P' General Polling
           if (p1_state & End_dlvr)
           { // отпуск завершен

             if( f_timer(t_end_dlvr,t_wt_e_dlvr )==0 )
                 goto m_no_say;
mmp:
      //       if(flag_spec== 0)
             {  // стандартный отчет

                 flag_cst_chg=1;
                 cnt_ltch=0;

                 buf_out_pmp1[0]=STX;
                 buf_out_pmp1[1]=n_msg_t;
                 buf_out_pmp1[2]='F';
                 buf_out_pmp1[3]=P_addr+'0';

//                 sprintf(buf_tmp,"%05ld",(long int)(*vol_p * 100.+0.5) );
  //             vol_deliv =fabs( *vol_p) * 100.+0.5;
                 vol_deliv = *vol_p * 100.+0.5;
                 if(vol_deliv < 0) vol_deliv=0;
                 sprintf(buf_tmp,"%05ld",vol_deliv);
                 buf_out_pmp1[4]=buf_tmp[4];
                 buf_out_pmp1[5]=buf_tmp[3];
                 buf_out_pmp1[6]=buf_tmp[2];
                 buf_out_pmp1[7]=buf_tmp[1];
                 buf_out_pmp1[8]=buf_tmp[0];
     //          sprintf(buf_tmp,"%06ld",(long int)(*amount_p+0.5) );
//                 sprintf(buf_tmp,"%06ld",(long int)(MassT_id * 1000.+0.5) );
//               mass_deliv = fabs(MassT_id) * 1000.+0.5;
                 mass_deliv = (MassT_id) * 1000.+0.5;
                 if(mass_deliv < 0) mass_deliv=0;
                 sprintf(buf_tmp,"%06ld",mass_deliv);
                 buf_out_pmp1[9]=buf_tmp[5];
                 buf_out_pmp1[10]=buf_tmp[4];
                 buf_out_pmp1[11]=buf_tmp[3];
                 buf_out_pmp1[12]=buf_tmp[2];
                 buf_out_pmp1[13]=buf_tmp[1];
                 buf_out_pmp1[14]=buf_tmp[0];

//                 sprintf(buf_tmp,"%04d",(int)(*cost_p+0.5) );
//                 sprintf(buf_tmp,"%04d",(int)(cost_v+0.5) );
                 cost_deliv =  cost_v+0.5;
                 sprintf(buf_tmp,"%04d", cost_deliv );
                 buf_out_pmp1[15]=buf_tmp[3];
                 buf_out_pmp1[16]=buf_tmp[2];
                 buf_out_pmp1[17]=buf_tmp[1];
                 buf_out_pmp1[18]=buf_tmp[0];


                 f_wr_evt(evt_deliv_rep);
                 buf_out_pmp1[19]= (cp_pmp1 & 0xef ) | ((opz_pmp1<<4) & 0x10);

                 buf_out_pmp1[20]=ETX;
                 n_out_pmp1=21;
                 mess_type=End_dlvr;

                 PMP_last_cht=TimeStamp+47;

             }

/*============
             else
             { // специальный отчет
                 buf_out_pmp1[0]=STX;
                 buf_out_pmp1[1]=n_msg_t;
                 buf_out_pmp1[2]='f';
                 buf_out_pmp1[3]=P_addr+'0';

                 sprintf(buf_tmp,"%08ld",(long int)(*vol_p * 1000.+0.5) );
                 buf_out_pmp1[4]=buf_tmp[7];
                 buf_out_pmp1[5]=buf_tmp[6];
                 buf_out_pmp1[6]=buf_tmp[5];
                 buf_out_pmp1[7]=buf_tmp[4];
                 buf_out_pmp1[8]=buf_tmp[3];
                 buf_out_pmp1[9]=buf_tmp[2];
                 buf_out_pmp1[10]=buf_tmp[1];
                 buf_out_pmp1[11]=buf_tmp[0];

                 sprintf(buf_tmp,"%08ld",(long int)(*amount_p+0.5) );
                 buf_out_pmp1[12]=buf_tmp[7];
                 buf_out_pmp1[13]=buf_tmp[6];
                 buf_out_pmp1[14]=buf_tmp[5];
                 buf_out_pmp1[15]=buf_tmp[4];
                 buf_out_pmp1[16]=buf_tmp[3];
                 buf_out_pmp1[17]=buf_tmp[2];
                 buf_out_pmp1[18]=buf_tmp[1];
                 buf_out_pmp1[19]=buf_tmp[0];

//                 sprintf(buf_tmp,"%06ld",(long int)(*cost_p+0.5) );
                 sprintf(buf_tmp,"%06ld",(long int)(cost_v+0.5) );
                 buf_out_pmp1[20]=buf_tmp[5];
                 buf_out_pmp1[21]=buf_tmp[4];
                 buf_out_pmp1[22]=buf_tmp[3];
                 buf_out_pmp1[23]=buf_tmp[2];
                 buf_out_pmp1[24]=buf_tmp[1];
                 buf_out_pmp1[25]=buf_tmp[0];

                 buf_out_pmp1[26]= (cp_pmp1 & 0xef ) | ((opz_pmp1<<4) & 0x10);

                 if(s_MVD[0].VolI> 0)
                   sprintf(buf_tmp,"%09ld",(long int)(s_MVD[0].VolI+0.5) );
                 else
                   sprintf(buf_tmp,"000000000");

                 buf_out_pmp1[27]=buf_tmp[0];
                 buf_out_pmp1[28]=buf_tmp[1];
                 buf_out_pmp1[29]=buf_tmp[2];
                 buf_out_pmp1[30]=buf_tmp[3];
                 buf_out_pmp1[31]=buf_tmp[4];
                 buf_out_pmp1[32]=buf_tmp[5];
                 buf_out_pmp1[33]=buf_tmp[6];
                 buf_out_pmp1[34]=buf_tmp[7];
                 buf_out_pmp1[35]=buf_tmp[8];

                 buf_out_pmp1[36]='0';
                 buf_out_pmp1[37]=ETX;

                 n_out_pmp1=38;
                 mess_type=End_dlvr;
                 PMP_last_cht=TimeStamp+80;
             }
=============*/
           }
           else if(flag_fst_pmp1)
           {
            goto m_stat;
           }
           else if (p1_state & Dlvr_rqst)
           {  // запрос на отпуск
m_dlvr_rq:
                 buf_out_pmp1[0]=STX;
                 buf_out_pmp1[1]=(char)n_msg_t;
                 buf_out_pmp1[2]='A';
                 buf_out_pmp1[3]=P_addr+'0';
                 buf_out_pmp1[4]=ETX;
                 n_out_pmp1=5;
                 mess_type=Dlvr_rqst;
                 PMP_last_cht=TimeStamp+14;
           }
           else//( p1_state==0)
           {     // No say
     m_no_say:
                 buf_out_pmp1[0]=STX;
                 buf_out_pmp1[1]=(char)n_msg_t;
                 buf_out_pmp1[2]='C';
                 buf_out_pmp1[3]=P_addr+'0';
                 buf_out_pmp1[4]=ETX;
                 n_out_pmp1=5;
                 mess_type=No_say;
                 PMP_last_cht=TimeStamp+14;
           }
           goto out_b;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='S')
        {   //'S ' "S" Статус Status
            prod_code=intrpr.wrd[1];

m_stat:
            buf_out_pmp1[0]=STX;
            buf_out_pmp1[1]=(char)n_msg_t;
            buf_out_pmp1[2]='S';
            buf_out_pmp1[3]=PK;
            buf_out_pmp1[4]=P_addr+'0';
            buf_out_pmp1[5]='F';  //'F' - авт, 'G' - ручн.
            buf_out_pmp1[6]=Modef;
//            sprintf(&buf_out_pmp1[7],"%02d%04d",err_pmp1,(int)(*cost_p+0.5));
            sprintf(&buf_out_pmp1[7],"%02d%04d",err_pmp1,(int)(cost_v+0.5));
/*
            sprintf(buf_tmp,"%02d",err_pmp1);
            buf_out_pmp1[7]=buf_tmp[0];
            buf_out_pmp1[8]=buf_tmp[1];

//            sprintf(buf_tmp,"%04d",(int)(*cost_p+0.5));
            sprintf(buf_tmp,"%04d",(int)(cost_v+0.5));

            buf_out_pmp1[9] =buf_tmp[0];
            buf_out_pmp1[10]=buf_tmp[1];
            buf_out_pmp1[11]=buf_tmp[2];
            buf_out_pmp1[12]=buf_tmp[3];
*/
            buf_out_pmp1[13]=ETX;
            n_out_pmp1=14;
            mess_type=Status;
            goto out_b;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='C')
        {   //'C '"C"  Counts request

           if (p1_state & End_dlvr)
           { // отпуск завершен

             if( f_timer(t_end_dlvr,t_wt_e_dlvr )!=0 )
                 goto mmp;
           }
           else if(flag_fst_pmp1)
           {
            goto m_stat;
           }
           else if (p1_state & Dlvr_rqst)
           goto m_dlvr_rq;

            buf_out_pmp1[0]=STX;
            buf_out_pmp1[1]=(char)n_msg_t;
            buf_out_pmp1[2]='D';
            buf_out_pmp1[3]=P_addr+'0';
/*
            if(Modef & pmp1_dlvr_bit)
            {
              ftmp=*vol_p - rnd_lim-rnd_lim;
              ftmp1=-s_MVD[0].VolT - rnd_lim-rnd_lim;
              if(ftmp > ftmp1)
              {
               ftmp=ftmp1;
              }
              if(ftmp < 0) ftmp=0;
              ftmp2=ftmp * cost_v;
            }
            else
            {
             ftmp=*vol_p;
             ftmp2=*amount_p;
            }
*/

            if( flag_end_dlvr != 0 )
            {
             ftmp=*vol_p;
  //         ftmp2=*amount_p;

             if(ftmp > 0)
             {
               ftmp2=-s_MVD[GR].MassT+rnd_add_m;
               ftmp2=((long int)(ftmp2 * 1000.+0.5));
             }
             else ftmp2=0.;

            }
            else
            {
              ftmp=*vol_p - ex_p - rnd_lim_m;
              ftmp1=-s_MVD[GR].VolT - ex_p - rnd_lim_m;
              if(ftmp > ftmp1)
              {
               ftmp=ftmp1;
              }
              if(ftmp < 0) ftmp=0;
//            ftmp2=ftmp * cost_v;


             if(ftmp > 0)
              {
               ftmp2=-s_MVD[GR].MassT+rnd_add_m;
               ftmp2=((long int)(ftmp2 * 1000.+0.5));
              }
              else ftmp2=0.;

            }

       /*
            if(flag_fault == 8)
            {
             if(ftmp < 10.)
             {
               ftmp= ftmp * 1.2;
             }

            }
      */

//          sprintf(buf_tmp,"%05ld",(long int)(*vol_p * 100.+0.5) );
            sprintf(buf_tmp,"%05ld",(long int)(ftmp * 100.+0.5) );
            buf_out_pmp1[4]=buf_tmp[4];
            buf_out_pmp1[5]=buf_tmp[3];
            buf_out_pmp1[6]=buf_tmp[2];
            buf_out_pmp1[7]=buf_tmp[1];
            buf_out_pmp1[8]=buf_tmp[0];
//          sprintf(buf_tmp,"%06ld",(long int)(*amount_p+0.5) );
            sprintf(buf_tmp,"%06ld",(long int)(ftmp2+0.5) );
            buf_out_pmp1[9]=buf_tmp[5];
            buf_out_pmp1[10]=buf_tmp[4];
            buf_out_pmp1[11]=buf_tmp[3];
            buf_out_pmp1[12]=buf_tmp[2];
            buf_out_pmp1[13]=buf_tmp[1];
            buf_out_pmp1[14]=buf_tmp[0];
            buf_out_pmp1[15]= (cp_pmp1 & 0xef ) | ((opz_pmp1<<4) & 0x10);
            buf_out_pmp1[16]=ETX;
            n_out_pmp1=17;
            mess_type=No_say;
            goto out_b;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='v')
        {   //'v '"v"  program version request

            buf_out_pmp1[0]=STX;
            buf_out_pmp1[1]=(char)n_msg_t;
            buf_out_pmp1[2]='v';
            buf_out_pmp1[3]=P_addr+'0';

            buf_out_pmp1[4]='H'; // nn
            buf_out_pmp1[5]='T'; // nn

            buf_out_pmp1[6]='0'; // t
            buf_out_pmp1[7]='1'; // t
            buf_out_pmp1[8]='0'; // t

            buf_out_pmp1[9] ='0'; // e
            buf_out_pmp1[10]='1'; // e

            buf_out_pmp1[11]=ETX;
            n_out_pmp1=12;
            mess_type=No_say;
            goto out_b;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='E')
        {   //'E ' Credit sending message

         _fmemcpy( buf_tmp,&intrpr.wrd[2],12L);
         intrpr.wrd[12]=0;

         if( (Modef & pmp1_dlvr_bit) || ( p1_state & End_dlvr))
         {
//         goto m_wrg;
           flag_repeat=1;
         }
         else
           flag_repeat=0;

         tmp_cst=-1;
//       if( sscanf(buf_tmp,"%4d%4d%4d",&itmp,&itmp1,&itmp2) <3 ) goto out_nak;
         if( sscanf(buf_tmp,"%4d%4d%4d",&itmp,&tmp_cst,&itmp2) <3 ) goto out_nak;
         amount_f=(float)(itmp) * 1000;

         f_cst();

         volume_f=((float)itmp2)/10.;
         if( flag_repeat )
         {
       //   if(volume_f !=  vol_f)
       //       goto m_wrg;
       //   else

//        if((volume_f !=  vol_f) && (volume_f != 0))
          if(volume_f !=  vol_f)
              goto m_wrg;
          else
          {
             flag_ACK_NAK=ACK;
             flag_SEND=0;

             f_stor_pmp(M_WR,(char *)&flag_ACK_NAK,1);

             (*ff_putch[ComPortPmp1])(flag_ACK_NAK);

        //    f_wr_evt(evt_wrong_start);
              f_wr_evt(evt_start_f);
              fl_ok_pmp=1;
              return;
          }
         }

//         cp_pmp1=intrpr.wrd[14] & 0xff; // cp
         cp_pmp1=(intrpr.wrd[14] & 0x3f) | 0x20;  // cp
         opz_pmp1=intrpr.wrd[15] & 0xff; // opz

         vol_f=0;
         if((amount_f != 0) && (cost_v > 0) )
             vol_f=amount_f/cost_v;
         else if(volume_f != 0)
             vol_f=volume_f;

 m_start:
         if(cost_v <= 0)
                goto m_wrg;

         if(cost_v != cost_vvc )
                goto m_wrg;

         if(n_cnt_cst < N_cst)
                goto m_wrg;

         if(flag_0 != 0)
                goto m_wrg;


         if(CMD_IN & CMD_DIS_PMP)
                   goto m_wrg;

         if( (Modef & pmp1_dlvr_bit) || ( p1_state & End_dlvr))
                   goto m_wrg;

//       if( (i7060_inp[0] ^ mski_inp1 ) & INP1);
         if( INP_VARi & INP1);
         else  goto m_wrg;

        if((sw_dns<=0) && (sw_dlv_liq <=0) && (flag_rcv == 0) ) ;
        else  goto m_wrg;

        if( (OUT_VAR & LV10) != 0 )
        {
          ICP_error[icp_lst_max] |= LVL_min;
          f_set_pmp_err();
          Modef |= pmp1_err_bit;
          FL_err=1;
          goto m_wrg;
        }

         if(vol_f != 0)
         {
          if(fl_zeroing != 0) goto m_wrg;
          if(FL_err)
          {
     m_wrg:
           flag_ACK_NAK=NAK;
//         flag_ACK_NAK=ACK;

           flag_SEND=0;

           f_stor_pmp(M_WR,(char *)&flag_ACK_NAK,1);

           (*ff_putch[ComPortPmp1])(flag_ACK_NAK);

           if( fl_ok_pmp != 0)
                  f_wr_evt(evt_wrong_start);
            fl_ok_pmp=0;
            return;
          }
          if( ff_serv_N != NULL) goto m_wrg;
          if( fl_GO != 0) goto m_wrg;
/*
          if(MVD[0].status & I7060[0].status);
          else  if(t_ctrl==0)
          {
            flag_ACK_NAK=ACK;
            flag_SEND=0;
            f_stor_pmp(M_WR,(char *)&flag_ACK_NAK,1);
            (*ff_putch[ComPortPmp1])(flag_ACK_NAK);
                f_wr_evt(evt_no_dev);
             return;
          }
*/
          if( flag_cst_chg != 0 )
          {
           flag_cst_chg=0;
           cnt_ltch=0;
           uk_fin_pm=(uk_fin_pm+1)&pmln_b_lgth_1;
          }

          f_start_MVD();

          fl_ok_pmp=1;

          ftmp=f_get_dens();

          Dens0=ftmp;
          Modef |= pmp1_dlvr_bit;

          p1_state &= ~Dlvr_rqst;  // ZZZZZ

          VolT_id=0;
          summ_v_id=0;

          VolT_id1=VolT_id;
          VolT_id1_1=VolT_id;
          summ_v_id1=summ_v_id;

         }
         else
         {  // (vol_f == 0)
           flag_ACK_NAK=ACK;
           flag_SEND=0;
           f_stor_pmp(M_WR,(char *)&flag_ACK_NAK,1);
           (*ff_putch[ComPortPmp1])(flag_ACK_NAK);
     //    f_wr_evt(evt_start_f);
           summ_v=0;
           return;
         }
/*
         if(flag_fault == 9)
         {
           flag_fault=0;
           return;
         }

*/
         goto out_ack;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='O')
        {   //'O ' Fractional credit sending message

         _fmemcpy( buf_tmp,&intrpr.wrd[2],11L);
         intrpr.wrd[11]=0;

         if( (Modef & pmp1_dlvr_bit) || ( p1_state & End_dlvr))
           goto m_wrg;

         tmp_cst=-1;
         if( sscanf(buf_tmp,"%1d%6d%4d",&itmp,&itmp1,&tmp_cst) <3 ) goto out_nak;

         credit_type=intrpr.wrd[13]&0xff;
         cp_pmp1=(intrpr.wrd[14] & 0x3f) | 0x20;  // cp
         opz_pmp1=intrpr.wrd[15] & 0xff; // opz

         if(itmp> 6) goto out_nak;

         f_cst();

         if(credit_type== AMNT)
         { // Amount
          amount_f=(float)(itmp1) * dec_val_p[itmp];
          volume_f=0;
          if(cost_v <= 0.) goto out_nak;
          vol_f=amount_f/cost_v;
         }
         else if(credit_type== VOLM)
         { // Volume
          volume_f=(float)(itmp1) * dec_val_p[itmp];
          amount_f=0;
          vol_f=volume_f;
         }
         else goto out_nak;

         goto m_start;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='M')
        {   //'M ' "M" Unit price
          goto m_u;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='N')
        {   //'N ' "N" Unit price
          goto m_u;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='U')
        {   //'U ' "U" Unit price
m_u:
         if( ff_serv_N != NULL ) goto out_nak;
         if( fl_GO != 0) goto out_nak;
         if( (Modef & pmp1_dlvr_bit) || ( p1_state & End_dlvr))
           goto out_nak;

         _fmemcpy( buf_tmp,&intrpr.wrd[1],4L);
         intrpr.wrd[4]=0;

         tmp_cst=-1;
         if( sscanf(buf_tmp,"%d",&tmp_cst) <1 ) goto out_nak;

         f_cst();

         goto out_ack;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='X')
        {   //'X ' "X" Unit price

         if( ff_serv_N != NULL) goto out_nak;
         if( fl_GO != 0) goto out_nak;
         if( (Modef & pmp1_dlvr_bit) || ( p1_state & End_dlvr))
           goto out_nak;

         _fmemcpy( buf_tmp,&intrpr.wrd[1],4L);
         intrpr.wrd[4]=0;

         if (!strcmp(buf_tmp,"CRED" )) goto out_ack;
         if (!strcmp(buf_tmp,"CASH" )) goto out_ack;

         tmp_cst=-1;
         if( sscanf(buf_tmp,"%d",&tmp_cst) <1 ) goto out_nak;
         f_cst();
         goto out_ack;
        }
//---------------------------------
        if(intrpr.wrd[0]=='T')
        {   //'T '"T" Stop delivery
           flag_StopPmp1=1;
           f_stop_cnt();
           goto out_ack;
        }
//---------------------------------
        if(intrpr.wrd[0]=='H')
        {   //'H '"H" Init
    //     flag_StopPmp1=1;
    //     f_stop_cnt();
           flag_pmp_init=1;
           f_cl_error();
           goto out_ack;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='L')
        {   //'L ' Lamp1 ON
//         lamp_c |= OUT3;
           Modef |= pmp1_L1_bit;
           goto out_ack;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='A')
        {   //'A ' Lamp2 ON
   //      lamp_c |= OUT4;
           Modef |= pmp1_L2_bit;
           goto out_ack;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='D')
        {   //'D ' Lamp1 OFF
 //        lamp_c &= ~OUT3;
           Modef &= ~pmp1_L1_bit;
           goto out_ack;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='B')
        {   //'B ' Lamp2 OFF
//         lamp_c &= ~OUT4;
           Modef  &= ~pmp1_L2_bit;
           goto out_ack;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='H')
        {
           goto out_ack;
        }
//---------------------------------
        if  (intrpr.wrd[0]=='w')
        {
           goto out_ack;
        }
//---------------------------------

out_nak:
// flag_ACK_NAK=NAK;
 flag_ACK_NAK=ACK;
 goto out_n_a;

fin:
    return;
out_b:
 // сообщение в buf_out_pmp1[], длина n_out_pmp1

 f_stor_pmp(M_WR,buf_out_pmp1,n_out_pmp1);

 f_preparePmp1(buf_out_pmp1,&n_out_pmp1);

 ToComBufn(ComPortPmp1,buf_out_pmp1,n_out_pmp1);

 flag_SEND=1;
 flag_ACK_NAK=0;
 return;

out_ack:
 flag_ACK_NAK=ACK;
out_n_a:
 f_stor_pmp(M_WR,(char *)&flag_ACK_NAK,1);
 flag_SEND=0;
 (*ff_putch[ComPortPmp1])(flag_ACK_NAK);
 return;

}

/*-------------------------------------------*/
int f_preparePmp1(char *msg,int *msg_length)
{ //  функция вычисляет конторольную сумму команды в буфере msg
  //  и записывает  ее после команды.
  //  Длина сообщения увеличивается на 1 байт
  //  Возвращает 0 - OK,
  //             1 - нет места в буфере

unsigned int crc_calc;
int i,i1;

  i1=*msg_length;
  if( i1 >= (max_out_pmp1-2)) return 1;
  crc_calc = f_calc_csum_pmp1(msg,i1);

  msg[i1++]=crc_calc & 0xff;
  *msg_length +=1;
  return 0;
}
/*----------------------------*/

int flag_cst_chg=0;
int cnt_ltch=0;
int flag_fst_cst=1;
float cost_v_c=0;

#define  n_cnt_ltch 10

void f_stor_pmp( char type,char *buf,int buf_n)
{
  struct tm  mdat;
  TIME_DATE tim_dat;
  long int nn;
  int j,ii;
  char ch_tmp;

/* 11.02.11
  if(cost_v != cost_v_c)
  {
    cost_v_c=cost_v;
    flag_cst_chg=1;
  }
*/
/*
 GetTimeDate(&tim_dat);
 mdat.tm_year  = tim_dat.year -1900 ;
 mdat.tm_mon   = tim_dat.month-1 ;
 mdat.tm_mday  = tim_dat.day   ;
 mdat.tm_hour  = tim_dat.hour  ;
 mdat.tm_min   = tim_dat.minute;
 mdat.tm_sec   = tim_dat.sec   ;

 bf_pml[uk_fin_pm].time = mktime( &mdat );
*/

 bf_pml[uk_fin_pm].time = time_last_evt;
 bf_pml[uk_fin_pm].time_m = TimeStamp;

 if(buf_n >= mess_lgth ) nn=mess_lgth;
 else nn=buf_n;

 bf_pml[uk_fin_pm].type_n=type | (char)nn;

 _fmemset( bf_pml[uk_fin_pm].messg,' ',22);
 _fmemcpy(bf_pml[uk_fin_pm].messg,buf,nn);

 if(flag_cst_chg)
  if(cnt_ltch >= n_cnt_ltch) return;
  else cnt_ltch++;
 else cnt_ltch=0;

 uk_fin_pm=(uk_fin_pm+1)&pmln_b_lgth_1;

}
/*----------------------------*/
void  f_set_pmp_err()
{
 if(ICP_error[icp_lst_max] & LVL_min )
   err_pmp1=43; // low level

 else if(ICP_error[icp_lst_max] == WD_pmlan )
   err_pmp1=80; // polling timeout

 else if(ICP_error[icp_lst_max] == No_liq_err )
   err_pmp1=75; // motor timer error

 else  // other errors

// display error: transmission data error to main display
// or control error of volume electromechanical meter

   err_pmp1= 70;
}
/*----------------------------*/
