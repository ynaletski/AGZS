
// �ࠩ��� ����� (������) ��� ����� ������/������ �१ Modbus
// �ࠩ��� ����� (������) ��� �뢮�� ������ �१ Modbus


int Tank_n=0; // ����� १���� �� ����७�� ���⭮��

#define RTU_Lnk
#define max_Lnk  2

struct s_DGM_dev s_DGM[max_DGM];
/*
{
 0,       //  float Prod_lvl; // 0.1 mm
 0,       //  float offset;
 0.1,     //  float scale;
 0,       // float Wat_lvl;  // 0.1mm
 24.4,       // float Temp;     // 0.1 C
 5432,       // float VolLiq;
 23,       // float VolGas;
 600.,       // float MassLiq;
 0,       // float MassGas;
 0,       // float Mass;
 0,       // float DensLiq;
 0,       // float DensGas;
 0,       // float DensReal;
 0,       // float TempReal;
//==========================
 0,       //  float Prod_lvl; // 0.1 mm
 0,       //  float offset;
 0.1,     //  float scale;
 0,       // float Wat_lvl;  // 0.1mm
 0,       // float Temp;     // 0.1 C
 6543,    // float VolLiq;
 0,       // float VolGas;
 0,       // float MassLiq;
 0,       // float MassGas;
 0,       // float Mass;
 0,       // float DensLiq;
 0,       // float DensGas;
 0,       // float DensReal;
 0,       // float TempReal;
//==========
};
=================*/
// ��᫥����⥫쭮��� �㭪権 ����-�����
// mDat1   - �⥭�� ������ ᮣ��᭮ LIn_Dat1[]
// mDat1w  - ������ ������ ᮣ��᭮ LIn_Dat1w[]
// mDat2w  - ������ ������ ᮣ��᭮ LIn_Dat2w[]
// mDat3w  - ������ ������ ᮣ��᭮ LIn_Dat3w[]

// ��� ��।�� ������  � १���� ���⭮�� � ������襩 ���뫪�, �� ��।�� 䫠��
#define ind_mDat2w (2)

int LIn_seq[]={
mDat1, mDat1w,  mDat1, mDat2w,
mDat1, mDat3w,  mDat1, mDat4w,
};

int LIn_seqGR[]={
mDat1, mDat5w, mDat1, mDat5w,
mDat1, mDat5w, mDat1, mDat5w,
};

/*
int LIn_seq[16]={
mDat1, mDat1w, mDat2w, mDat3w,
mDat1, mDat1w, mDat2w, mDat3w,
mDat1, mDat1w, mDat2w, mDat3w,
mDat1, mDat1w, mDat2w, mDat3w,
};

int LIn_seq[16]={
mDat1, mDat1w, mDat1 , mDat2w,
mDat1, mDat1w, mDat1 , mDat3w,
mDat1, mDat1w, mDat1 , mDat2w,
mDat1, mDat1w, mDat1 , mDat3w,
};
*/
//---------------
struct eee_dat L1D;

// ����� ���뢠��� �� �吝��� ����஫���
struct eee_dat LIn_Dat1[]=
{
&CMD_IN        , T_INT   ,        //1
&i_LVL         , T_INT   ,        //1
&LIn_val       , T_FLOAT ,        //2
&RQ2           , T_INT   ,        //1
&PMP_sw        , T_INT   ,        //1
&s_DGM[0].Prod_lvl    , T_FLOAT , //2
&s_DGM[0].Temp        , T_FLOAT , //2
&s_DGM[0].VolLiq      , T_FLOAT , //2
&s_DGM[0].MassLiq     , T_FLOAT , //2
&s_DGM[0].MassGas     , T_FLOAT , //2
&s_DGM[1].Prod_lvl    , T_FLOAT , //2
&s_DGM[1].Temp        , T_FLOAT , //2
&s_DGM[1].VolLiq      , T_FLOAT , //2
&s_DGM[1].MassLiq     , T_FLOAT , //2
&s_DGM[1].MassGas     , T_FLOAT , //2
&s_DGM[2].Prod_lvl    , T_FLOAT , //2
&s_DGM[2].Temp        , T_FLOAT , //2
&s_DGM[2].VolLiq      , T_FLOAT , //2
&s_DGM[2].MassLiq     , T_FLOAT , //2
&s_DGM[2].MassGas     , T_FLOAT , //2
};                        // �㬬�=36

#define LIn_Lgth1 ((int) 36)    // 16 bit words

int LInd_max1=sizeof(LIn_Dat1) / sizeof(L1D) ;

#define LIn_Addr1 ((int) 2000)

#define LIn2_Addr1 ((int) 2200)

//unsigned char Lin_Rd_Dat1[]={1,3,  0x01 , 0x00 ,  0x00   ,  0x02 }; // R257 (A=256)  2 x 16 bit registers
//unsigned char Lin_Rd_Dat1[]={1,3,  ((LIn_Addr1-1)>>8)&0xff , (LIn_Addr1-1)&0xff ,  0x00   ,  LIn_Lgth1 };
unsigned char Lin_Rd_Dat1[]={1,3,  ((LIn_Addr1-1)>>8)&0xff , (LIn_Addr1)&0xff ,  0x00   ,  LIn_Lgth1 };
unsigned char Lin_Rd_Dat1r[]={1,3,  ((LIn_Addr1-1)>>8)&0xff , (LIn_Addr1)&0xff ,  0x00   ,  LIn_Lgth1-10  };

unsigned char Lin2_Rd_Dat1[]={1,3,  ((LIn2_Addr1-1)>>8)&0xff , (LIn2_Addr1)&0xff ,  0x00   ,  LIn_Lgth1 };
unsigned char Lin2_Rd_Dat1r[]={1,3,  ((LIn2_Addr1-1)>>8)&0xff , (LIn2_Addr1)&0xff ,  0x00   ,  LIn_Lgth1-10  };
//---------------
int   Tank1F      =0;
float New_Dens1   =0.;
float New_Temp1   =0.;
float New_Dens2   =0.;
float New_Temp2   =0.;
float New_Dens3   =0.;
float New_Temp3   =0.;

struct eee_dat LIn_Dat2[]=   // ��������� ���祭�� ����७��� ���⭮�� � १����
{
&Tank1F               , T_INT   , //1
&New_Dens1            , T_FLOAT , //2
&New_Temp1            , T_FLOAT , //2
&New_Dens2            , T_FLOAT , //2
&New_Temp2            , T_FLOAT , //2
&New_Dens3            , T_FLOAT , //2
&New_Temp3            , T_FLOAT , //2
};                        // �㬬�=13

#define LIn_Lgth2 ((int) 13)    // 16 bit words

int LInd_max2=sizeof(LIn_Dat2) / sizeof(L1D) ;
//---------------
float Den_tmp=0;
float Temp_tmp=0;
float MassT1_tmp=0;
float MassT2_tmp=0;

// ����� �����뢠��� � 横��, SLOT1

#define LIn_Addr1w ((int)3000)
struct eee_dat LIn_Dat1w[]=
{  //T_INT   T_INT_L T_FLOAT
&LInd_max1w        , LIn_Addr1w,

&CMD_OUT            , T_INT   ,
&Reg_State          , T_INT   ,
&Reg_err            , T_INT   ,

&OUT_VAR            , T_INT   ,
&INP_VARi           , T_INT   ,
&st_PMP_sw          , T_INT   ,
&Press1             , T_FLOAT ,
&Press2             , T_FLOAT ,
&Temp1              , T_FLOAT ,
&Temp2              , T_FLOAT ,
&Press3             , T_FLOAT ,
&Temp3              , T_FLOAT ,

};

int LInd_max1w=sizeof(LIn_Dat1w) / sizeof(L1D) ;

//---------------
// ����� �����뢠��� � 横��, SLOT2

#define LIn_Addr2w ((int)4000)
struct eee_dat LIn_Dat2w[]=
{  //T_INT   T_INT_L T_FLOAT
&LInd_max2w         , LIn_Addr2w ,
&s_MVD[0].MassI       , T_FLOAT ,
&MassT1_tmp         , T_FLOAT ,  // MVD[0].MassT
&s_MVD[0].VolI        , T_FLOAT ,
&s_MVD[0].VolT        , T_FLOAT ,
&s_MVD[0].FlowM       , T_FLOAT ,
&Den_tmp            , T_FLOAT ,  // MVD[0].Dens
&Temp_tmp           , T_FLOAT ,  // MVD[0].Temp
&s_MVD[0].Press       , T_FLOAT ,
&s_DGM[0].DensReal    , T_FLOAT ,
&s_DGM[0].TempReal    , T_FLOAT ,
&st_PMP_sw          , T_INT   ,
};
int LInd_max2w=sizeof(LIn_Dat2w) / sizeof(L1D) ;

//---------------
// ����� �����뢠��� � 横��, SLOT3

#define LIn_Addr3w ((int)5000)
struct eee_dat LIn_Dat3w[]=
{  //T_INT   T_INT_L T_FLOAT
&LInd_max3w         , LIn_Addr3w,
&s_MVD[1].MassI       , T_FLOAT ,
&MassT2_tmp         , T_FLOAT ,  // MVD[1].MassT
&s_MVD[1].VolI        , T_FLOAT ,
&s_MVD[1].VolT        , T_FLOAT ,
&s_MVD[1].FlowM       , T_FLOAT ,
&s_MVD[1].Dens        , T_FLOAT ,
&s_MVD[1].Temp        , T_FLOAT ,
&s_DGM[1].DensReal    , T_FLOAT ,
&s_DGM[1].TempReal    , T_FLOAT ,
&st_PMP_sw          , T_INT   ,
};
int LInd_max3w=sizeof(LIn_Dat3w) / sizeof(L1D) ;
//---------------
//---------------
// ����� �����뢠��� � 横��, SLOT4

#define LIn_Addr4w ((int)6000)
struct eee_dat LIn_Dat4w[]=
{  //T_INT   T_INT_L T_FLOAT
&LInd_max4w         , LIn_Addr4w,
&s_MVD[GR].MassI       , T_FLOAT ,
&s_MVD[GR].MassT       , T_FLOAT ,
&s_MVD[GR].VolI        , T_FLOAT ,
&s_MVD[GR].VolT        , T_FLOAT ,
&s_MVD[GR].FlowM       , T_FLOAT ,
&s_MVD[GR].Dens        , T_FLOAT ,
&s_MVD[GR].Temp        , T_FLOAT ,
&s_DGM[2].DensReal    , T_FLOAT ,
&s_DGM[2].TempReal    , T_FLOAT ,
&st_PMP_sw             , T_INT   ,
};
int LInd_max4w=sizeof(LIn_Dat4w) / sizeof(L1D) ;
//---------------
// ����� �����뢠��� � 横��, SLOT5
// ⮫쪮 ��� ०��� ��� N2

#define LIn_Addr5w ((int)7000)
struct eee_dat LIn_Dat5w[]=
{  //T_INT   T_INT_L T_FLOAT
&LInd_max5w        , LIn_Addr5w,

&CMD_OUT            , T_INT   ,
&Reg_State          , T_INT   ,
&Reg_err            , T_INT   ,

&OUT_VAR            , T_INT   ,
&INP_VARi           , T_INT   ,
&s_MVD[GR].MassI       , T_FLOAT ,
&s_MVD[GR].MassT       , T_FLOAT ,
&s_MVD[GR].VolI        , T_FLOAT ,
&s_MVD[GR].VolT        , T_FLOAT ,
&s_MVD[GR].FlowM       , T_FLOAT ,
&s_MVD[GR].Dens        , T_FLOAT ,
&s_MVD[GR].Temp        , T_FLOAT ,
&RQ2W                  , T_INT   ,

};



int LInd_max5w=sizeof(LIn_Dat5w) / sizeof(L1D) ;

//---------------
//---------------
struct s_icp_dev LIn[max_Lnk]=
{
// device 1
0,                // status
2,                // port
5,                // addr
"LIn/1",          // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
1,                // protocol
1,                // CRC_flag
500,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&LIn_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
2,                // port
10,               // addr
"LIn/2",          // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
1,                // protocol
1,                // CRC_flag
500,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&LIn_Rqst[1],   // *request
};
/*----------------------------*/
struct COM_rqst LIn_Rqst[max_Lnk]={
//----------------------
// device 1
//--------------------------
0,                     //  ���ﭨ� �����
ffgets_com_rtu,        //  gets_com
f_LIn_rd,            //  answ_com
f_LIn_rd,            //  answ_flt
"LIn/1",              //  name
0,                     //  n_dev ����� ���ன�⢠ � ��㯯� ����⨯��� ���ன��
&LIn[0],             //  *ICP_dd
0,                     //  ⥪��� �㭪��
ToutAnsLIn_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  ���ﭨ� �����
ffgets_com_rtu,        //  gets_com
f_LIn_rd,            //  answ_com
f_LIn_rd,            //  answ_flt
"LIn/2",              //  name
1,                     //  n_dev ����� ���ன�⢠ � ��㯯� ����⨯��� ���ன��
&LIn[1],             //  *ICP_dd
0,                     //  ⥪��� �㭪��
ToutAnsLIn_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
};
//---------------------------------
/*----------------------------*/
struct s_LIn_dev s_LIn[max_Lnk]=
{
0 , //unsigned int r_addr; address of MODBUS register
0,  //int n;               number of data - for read coils and ascii
0,  //int type;            data type
0,  //int exc;             int,coil value for write
0,  //long int l;          exception
0,  //int i1;              long
0,  //int i2;              float
0.,  //float f1;            float
0.,  //float f2;            float
0.,  //float f3;            float

//===========================
0 , //unsigned int r_addr; address of MODBUS register
0,  //int n;               number of data - for read coils and ascii
0,  //int type;            data type
0,  //int exc;             int,coil value for write
0,  //long int l;          exception
0,  //int i1;              long
0,  //int i2;              float
0.,  //float f1;            float
0.,  //float f2;            float
0.,  //float f3;            float
//===========================
};

//---------------------------------
unsigned int LIn_inp_err[max_Lnk]={0,0};  // ���稪 �訡�� �����
int LIn_sw[max_Lnk]={0,0};  // 横� ����


//                                     A_high                 A_low              n_high    n_low
void f_LIn(int ii)
{
 // ii - ����� ���ன�⢠
 //  �㭪�� 横���᪨ 諥� ����� �� ����祭�� ������  ��
 // ���ன�⢠ LIn  '01 03 01A2 0001' - R419 - ॣ���� �訡��,
 //
 //   ��ਮ� ���뫪� LIn[ii].pool_time ��.
 //

int i,fn,sw,i1,i2,itmp;
union  { float f; unsigned long l; } o;
unsigned char buf_tmp[200];
struct eee_dat *LIn_D;
int lin_max=0;

 if(LIn[ii].status == 0) return;

  if((LIn_Rqst[ii].status == Req_Flt) ||
     (LIn_Rqst[ii].status == Req_OK));
  else return;

 if(f_queue_chk(LIn[ii].port)== 0) return;  // ��।� ��९������

  if( f_timer(LIn[ii].time_stamp_pool,LIn[ii].pool_time ))
      fn=LIn_RD_P;
  else return;

          // 横���᪮�  �⥭��/������ ���ﭨ�
          // �ନ��� ������� �⥭�� / �����

             LIn_sw[ii]=(LIn_sw[ii]+1) & 0x7;
           if(F_GR2)
             sw=LIn_seqGR[LIn_sw[ii]];
           else
             sw=LIn_seq[LIn_sw[ii]];

             f_prep_state();
             f_prep_tmp_out();

             switch (sw)
             {
              case mDat1: // �⥭�� ������
              i1=6;

              if(F_GR2==0)
              {
              if(Tank_nn > 2)
                _fmemcpy(buf_tmp,Lin_Rd_Dat1,(long int)i1);
              else
                _fmemcpy(buf_tmp,Lin_Rd_Dat1r,(long int)i1);
              }
              else
              {
              if(Tank_nn > 2)
                _fmemcpy(buf_tmp,Lin2_Rd_Dat1,(long int)i1);
              else
                _fmemcpy(buf_tmp,Lin2_Rd_Dat1r,(long int)i1);
              }

// printf("\n\r WR mDat1 %ld ms",TimeStamp-time_beg_fill);
              break;

              case mDat2w: // ������ ������
                LIn_D=&LIn_Dat2w[0];

        m1:
//            buf_tmp[0]=addr;
              buf_tmp[1]=16; // ��� �㪭�樨

            //buf_tmp[2]= 0;  // ���� ॣ����
            //buf_tmp[3]= 2;  // --|
              f_set_int( &buf_tmp[2], LIn_D[0].l);

            //buf_tmp[4]= 0;  // ������⢮ ॣ���஢
            //buf_tmp[5]= 2;  // --|
            //buf_tmp[6]= 4;  // ������⢮ ����

              lin_max=*((int *) LIn_D[0].ptr);
              for(i1=7,i=1;i<lin_max;i++)
              {
                switch( LIn_D[i].l )
                {
                 case T_INT:
                   f_set_int( &buf_tmp[i1], *((int *) LIn_D[i].ptr)  );
                   i1+=2;
                   break;

                 case T_INT_L:
                   f_set_long( &buf_tmp[i1], *((long int *) LIn_D[i].ptr) );
                   i1+=4;
                   break;

                 case T_FLOAT:
                   f_set_float( &buf_tmp[i1], *((float *) LIn_D[i].ptr),1 );
                   i1+=4;
                   break;
                 default:
                   return;
                }
              }

              itmp =i1-7;
              buf_tmp[6]= itmp;  // ������⢮ ����
              f_set_int( &buf_tmp[4], itmp>>1 );
              break;

              case mDat1w: // ������ ������
                       LIn_D=&LIn_Dat1w[0];
                       goto m1;

              case mDat3w: // ������ ������
                       LIn_D=&LIn_Dat3w[0];
                       goto m1;
              case mDat4w: // ������ ������
                       LIn_D=&LIn_Dat4w[0];
                       goto m1;
              case mDat5w: // ������ ������
                       LIn_D=&LIn_Dat5w[0];
                       goto m1;

              default:
                return;
             }
             buf_tmp[0]=LIn[ii].addr;
             fn+=sw;

           LIn_Rqst[ii].function = fn;
           i2=i1;
           f_prepareRTU(buf_tmp,&i2);

/*===========
          if(sw != mDat1)
          {
            if(flag_prn)
            {
             printf("\r\n lin_max=%d,L1M=%d\r\n",lin_max,*((int *) LIn_D[0].ptr));
             printf("\r\n sw=%d,i2=%d\r\n",sw,i2);
             for(i=0;i<i2;i++)
             {
               printf("%2x ", ((int)buf_tmp[i])& 0xff  );
               if( (i%16)== 7) printf("  ");
               if( (i%16)== 15) printf("\r\n");
             }
             printf("\r\n>");
//             flag_prn=0;
            }

          }
       //   else

================*/

          {
           _fmemcpy(LIn_Rqst[ii].Cmd,buf_tmp,(long)i2);
           LIn_Rqst[ii].cmd_lgth=i2;
           LIn_Rqst[ii].time_stamp=TimeStamp;
           LIn[ii].time_stamp_pool=TimeStamp;
           LIn[ii].n_transaction++;
           f_queue_put(LIn[ii].port, &LIn_Rqst[ii]) ;
          }
}
/*----------------------------*/
long int tim_snd_LIn;
void f_LIn_rd(int ii)
{
float ftmp;
// ii - ����� ����
// ������ �ਭ��� ����� � ��६����
//
 struct COM_rqst *request;
 int i,i1,i2,nn,fn;
 int count;
 struct eee_dat *LIn_D;
 int Lin_N;
 int SizeL;


 request=COM_que[ii].request[COM_que[ii].empt_ptr];
 nn=request->n_dev; // ����� ���ன�⢠ LIn (0,1..)
 fn=request->function;

 count=n_bcom[ii];  // ����� �������

 if(count < 3)
   {
      goto m_tout_err;
   }
 if(f_check_crc_RTU(cb_COM[ii],count)==0)
   {
      goto m_crc_err;
   }
 if(cb_COM[ii][1] & 0x80)
   {
     s_LIn[nn].exc=cb_COM[ii][2];
     goto m_end;
   }
 if(( fn & 0xf8)==LIn_RD_P) // �⢥� �� ������� 横���᪮�� ����
  {
      fn &= 0x7;

      switch(  fn )
      {
      case mDat1:
      // ���� ��᫥����⥫쭮�� ������

    //� �⢥� �� 01 03 01 a2 00 01 ��室��:
    //  01 03 02   00 05    78 47
    //         |            |  |
    //    �����.����        CRC

//       if(count < (LIn_Lgth1+LIn_Lgth1+5) )

       if(count == (LIn_Lgth1+LIn_Lgth1+5) )     // 3 १����
       {
         SizeL=LInd_max1;
         goto m_rd_Slot1;
       }
       if(count == (LIn_Lgth1+LIn_Lgth1+5-20) )  // 2 १����
       {
         SizeL=LInd_max1-5;
         goto m_rd_Slot1;
       }
       if(count == (LIn_Lgth2+LIn_Lgth2+5) )
       {
         goto m_rd_Slot2;
       }
       else goto m_err;

      // Slot1
   m_rd_Slot1:

        f_inp_Lin(ii,&LIn_Dat1[0],SizeL);

        f_LIn_interp();
        break;
   m_rd_Slot2:
        f_inp_Lin(ii,&LIn_Dat2[0],LInd_max2);
        if(Tank1F & 0x1)
         {
           s_DGM[0].DensReal =New_Dens1 ;
           s_DGM[0].TempReal =New_Temp1 ;
         }
        if(Tank1F & 0x2)
         {
           s_DGM[1].DensReal = New_Dens2 ;
           s_DGM[1].TempReal = New_Temp2 ;
         }
        if(Tank1F & 0x4)
         {
           s_DGM[2].DensReal = New_Dens3 ;
           s_DGM[2].TempReal = New_Temp3 ;
         }
        if(Tank1F & 0x7)
         {
           Tank_n=Tank1F-1.;
           f_wr_evt(evt_dn_get);
         }

         CMD_OUT=Short_OK+(Tank1F & 0x7);
        break;

      case mDat1w:    //�⢥� �� ������� �����
      case mDat2w:    //�⢥� �� ������� �����
      case mDat3w:    //�⢥� �� ������� �����
      case mDat4w:    //�⢥� �� ������� �����
      case mDat5w:    //�⢥� �� ������� �����
       if ((cb_COM[ii][1] == 6)  || (cb_COM[ii][1] == 16) ||(cb_COM[ii][1] == 5));
       else
       {
         goto m_crc_err;
       }
       break;

      default:
               goto m_crc_err;
      }
  }

 m_end:

  LIn[nn].n_success++;
  LIn_inp_err[nn]=0;
  f_queue_free(ii,&LIn_Rqst[nn]);
  return;

m_crc_err:
 LIn[nn].time_stamp_rcv=0;
 LIn[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 LIn[nn].time_stamp_rcv=0;
 LIn[nn].n_timeout_error++;

m_err:
 LIn_inp_err[nn]++;

 if(LIn_inp_err[nn]>=LIn_max_inp_err)
 {
   LIn_inp_err[nn]=LIn_max_inp_err;
   f_icp_error(&LIn[nn],RD_ERR );
 }

 f_queue_free(ii,&LIn_Rqst[nn]);
}
/* ---------------------------------------- */
int Reg_State=0;
int CMD_IN=0;
int CMD_OUT=0;
float LIn_val=0;

void  f_LIn_interp()
{ // �������� ������� �� Link
//
//CMD_IN
//  01 - ����� �ਥ�� ���
//  02 - ���⢥ত���� �⥭�� ������ १���� �ਥ��
//  03 - ����� ����७�� ���⭮�� � १���� 1
//  13 - ����� ����७�� ���⭮�� � १���� 2
//  04 - ���⢥ত���� �⥭�� ������ १���� ���.���⭮�� � १���� 1
//  14 - ���⢥ত���� �⥭�� ������ १���� ���.���⭮�� � १���� 2
//  05 - ��⠭�� ����� �ਥ�� � ����७�� ���⭮��
//  06  - ��⠭�� ���᪠ �१ ������� ,
//  07  - ���⪠ �訡�� ,
//  08  - �⬥�� ����� ������ �� १���⠬ ����७�� ���⭮��
//  0x100(CMD_DIS_PMP) - ����� ���� ���᪠ �१ �������
//  00 - ࠧ�襭�� ���᪠ �१ ������� � ����祭�� �ਥ��,���.����. �� ���
//  0x1000(CMD_DIS_RCV) - ࠧ�襭�� ���᪠ �१ �������,����� ����祭��
//    �ਥ��,���.����. �� ���

// �᫨ ������� ����� ���� �믮����� - ��� ��९��뢠���� � ��室���
// ॣ���� �������   CMD_OUT.
// �᫨ ������� �� ����� ���� �믮����� - �������⥫쭮 ��⠭����������
// ��� 0x8000 (CMD_ERR).

// ����� ����訢����� � ��।����� � 横��
// ���⠢ ������ �� �ਥ�� � ��।�� 䨪�஢��.

// ����� �� ����७��� ���⭮�� ��室���� � ������ ����஫��� ( � MMI,7017)
// � � ������ 横�� ��।����� � ���孨� ����஫���.

// ��� ����䨪�樨  ����७��� ���祭��  ���⭮�� � ⥬������� (१���⮢
// ����७��) � ��஭� ���孥�� ����஫��� �ᯮ����� ᫥���騩 ��堭���:
//  �� ������ � ����� ������ ���孨� ����஫��� ��।��� ������ ���뫪�
//  � �������㥬묨 ����묨 . � �⢥� ������ ����஫��� ��⠭��������
// CMD_OUT=Short_OK; , ��᫥ 祣�  �� ������ � ����� ������ ���孨� ����஫���
// �㤥� ��।����� ����� ����� ( �� ����祭�� CMD_OUT==Short_OK ����
// ����୮ ��।������� ���祭�� ���⭮�� � ⥬�������).


   if( CMD_IN != ( CMD_OUT & ~CMD_ERR ) )
   {

     f_prep_state();
     switch( CMD_IN )
     {
      case 1: //����� �ਥ�� ���
             if(F_GR2) goto m_okk;
             if(Reg_State & (Pump_act | RCV_act | DEN_act) )
             {
      err_cmd:
               CMD_OUT=CMD_IN | CMD_ERR;
               break;
             }
             else if(LIn_val > 0)
             {
               sw_mmi = 201;
               sw_dlv_liq=1;
               dose_dlvr = LIn_val;
               CMD_OUT=CMD_IN;
               Reg_State &= ~(RCV_fin | DEN_fin | RCV_flch);
               break;
             }
             break;

      case 2: // ��⠭�� ����� �ਥ�� � ����७�� ���⭮��
             if(F_GR2) goto m_okk;

             CMD_OUT=CMD_IN;
             if(Reg_State & RCV_act )
             {
               sw_dlv_liq=13;
               break;
             }
             else if(Reg_State & DEN_act )
             {
                sw_mmi=211;
//              sw_dns=12;
                sw_dns=110;
    MmiGotoxy(0,1); MmiPuts("       == Stop (Host) ==      ");
                break;
             }
             break;
          // else  goto err_cmd;

        case 3: // ���਩�� ��⠭�� �ਥ��/ ���᪠
              CMD_OUT=CMD_IN;
              f_icp_errS(Hst_alarm);
         break;

        case 4: // ��� ����஫���
              CMD_OUT=CMD_IN;
              f_rst_slv();
         break;
      case 5: // ���⪠ �訡��
              CMD_OUT=CMD_IN;
        //    Reg_State=0;
              f_cl_error();
              break;

      case 6: // ��⠭�� ���᪠ �१ ������� ,

             if(Reg_State &  Pump_act)
             {
               flag_StopPmp1=1;
               f_stop_cnt();
               CMD_OUT=CMD_IN;
               break;
             }
             else  goto err_cmd;

      case CMD_DIS_PMP:  // (0x100)  ����� ���� ���᪠ �१ �������

             CMD_OUT=CMD_IN;
             break;


      case 0:      // ࠧ�襭�� ���᪠ �१ ������� � ����祭�� �ਥ��,���.����. �� ���
             CMD_OUT=CMD_IN;
             break;


      case 0x1000: // ࠧ�襭�� ���᪠ �१ �������,����� ����祭�� �ਥ��,���.����. �� ���
             CMD_OUT=CMD_IN;
             break;

      case 8: // ���⢥ত���� �⥭�� ������ १���� �ਥ��
             if(F_GR2) goto m_okk;
             Reg_State &= ~(RCV_fin | RCV_flch);
             CMD_OUT=CMD_IN;
             break;

      case 21: // ����� ����७�� ���⭮�� � १���� 1
             if(F_GR2) goto m_okk;

             if(Reg_State & (Pump_act | RCV_act | DEN_act) )
                  goto err_cmd;
             else
             {
                DenMeasSrc=1;
                sw_mmi=210;
                sw_dns=1;
                CMD_OUT=CMD_IN;
                Reg_State &= ~(RCV_fin | DEN_fin | RCV_flch);
                Tank_n=0;
                break;
             }

      case 22: // ����� ����७�� ���⭮�� � १���� 2
             if(F_GR2) goto m_okk;

             if(Reg_State & (Pump_act | RCV_act | DEN_act) )
                  goto err_cmd;
             else
             {
                DenMeasSrc=1;
                sw_mmi=210;
                sw_dns=1;
                CMD_OUT=CMD_IN;
                Reg_State &= ~(RCV_fin | DEN_fin | RCV_flch);
                Tank_n=1;
                break;
             }

      case 23: // ����� ����७�� ���⭮�� � १���� 3

             if(F_GR2) goto m_okk;
             if(Reg_State & (Pump_act | RCV_act | DEN_act) )
                  goto err_cmd;
             else
             {
                DenMeasSrc=1;
                sw_mmi=210;
                sw_dns=1;
                CMD_OUT=CMD_IN;
                Reg_State &= ~(RCV_fin | DEN_fin | RCV_flch);
                Tank_n=2;
                break;
             }

      case  9: // ���⢥ত���� �⥭�� ������ १���� ���.���⭮�� � १���� 1
             if(F_GR2) goto m_okk;
             if((Reg_State & DEN_fin)  && (Tank_n==0))
             {
               Reg_State &= ~DEN_fin;
               CMD_OUT=CMD_IN;
               s_DGM[0].DensReal=Dens_Result  ;
               s_DGM[0].TempReal=Temp_Result  ;

               Tank_n=0;
               f_wr_evt(evt_dn_get);
             }
             else  goto err_cmd;
             break;
      case 10: // ���⢥ত���� �⥭�� ������ १���� ���.���⭮�� � १���� 2
             if(F_GR2) goto m_okk;
             if((Reg_State & DEN_fin)  && (Tank_n==1))
             {
               Reg_State &= ~DEN_fin;
               CMD_OUT=CMD_IN;
               s_DGM[1].DensReal=Dens_Result  ;
               s_DGM[1].TempReal=Temp_Result  ;
               Tank_n=1;
               f_wr_evt(evt_dn_get);
             }
             else  goto err_cmd;
             break;

      case 12: // ���⢥ত���� �⥭�� ������ १���� ���.���⭮�� � १���� 3

             if(F_GR2) goto m_okk;
             if((Reg_State & DEN_fin)  && (Tank_n==2))
             {
               Reg_State &= ~DEN_fin;
               CMD_OUT=CMD_IN;
               s_DGM[2].DensReal=Dens_Result  ;
               s_DGM[2].TempReal=Temp_Result  ;
               Tank_n=2;
               f_wr_evt(evt_dn_get);
             }
             else  goto err_cmd;
             break;

      case  11: // �⬥�� �⥭�� ������ १���� ���.���⭮�� � १���� 1,2
             if(F_GR2) goto m_okk;

             Reg_State &= ~DEN_fin;
             Reg_State &= ~RCV_fin;
             Tank_n=0;
   m_okk:
             CMD_OUT=CMD_IN;
             break;

      case  CMD_BAD :
             CMD_OUT=CMD_BAD;
             break;
      default:
              goto err_cmd;

     }
   }
}
//------------------------------

void f_prep_state()
{
 Reg_State &= ~( Pump_act | RCV_act | DEN_act | CMD_ERR );

 if((sw_dns>0) && (sw_dns<20))
    Reg_State |= DEN_act;

 if((sw_dlv_liq > 0) || (flag_rcv != 0) )
    Reg_State |= RCV_act;

 if( Modef &  pmp1_dlvr_bit)
    Reg_State |= Pump_act;

 if( Reg_err != 0)
    Reg_State |= CMD_ERR;

}

//-----------------------------------

void f_prep_tmp_out()
{
/*
   if( Reg_State & RCV_fin)
    {
      MassT1_tmp = Rcv_Result1;
      MassT2_tmp = Rcv_Result2;
    }
   else
*/
    {
      MassT1_tmp = s_MVD[0].MassT;
      MassT2_tmp = s_MVD[1].MassT;
    }

   if( Reg_State & ( DEN_fin | RCV_fin) )
   {
    Den_tmp    = Dens_Result;
    Temp_tmp   = Temp_Result;
   }
   else
   {
    Den_tmp    = s_MVD[0].Dens;
    Temp_tmp   = s_MVD[0].Temp;
   }
}

//------------------------------
void f_inp_Lin(int ii,struct eee_dat *LIn_D,int Lin_N )
{
int i1,i2;
      for(i1=0,i2=0;i2<Lin_N;i2++ )
      {
             switch( LIn_D[i2].l )
             {
              case T_INT:

                *((int *) LIn_D[i2].ptr)=f_get_int(&cb_COM[ii][i1]);
                i1+=2;
                break;
/*
              case T_INT_L:

                *((long int *) LIn_D[i2].ptr)= f_get_long(&cb_COM[ii][i1+3]);
                i1+=4;
                break;
*/
              case T_FLOAT:

                *((float *) LIn_D[i2].ptr)  = f_get_float( &cb_COM[ii][i1], 1);
                i1+=4;
                break;

              default:
                break;
             }
      }


}
//------------------------------
void f_rst_slv()
{ // return 1 - error, 0 - OK

    f_wr_evt(evt_res);
    _asm cli
    _asm hlt

}
//------------------------------
//------------------------------
