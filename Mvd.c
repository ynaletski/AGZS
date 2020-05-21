

//    f_wr_deb();
// void f_MVD_rd_t(int ii)  08.12.2011


int MVD_FLOW =2;   // индекс расходомера используемого при прокачке
                       // для f_flow_up()
                       // 0 - система с 3-мя расходомерами
                       // 2 - система с 1 расходомером

int MVD_FLOW_t =1;   // количество расходомеров 1 или 3;

float FctBern=1,R_MVD=15.;
float flow_min_c=0.000001; // л/мс , минимальный расход для вычисления коррекции отсекания дозы

int flag_pool=1;
int i_LVL=0;  // OUT6, OUT7 биты выходов LVL10,LVL90
int flag_end_dlvr=0;

unsigned int OUT_VAR_a=0;

// Массовый расходомер Micro Motion MVD

void (*ff_serv_N)()=NULL;
#define RTU_MVD
//---------------
struct s_icp_dev MVD[max_MVD]=
{
// device 1, ЖФ
0,                // status
2,                // port
1,                // addr
"MVD/1",          // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
1,                // protocol
1,                // CRC_flag
400,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&MVD_Rqst[0],   // *request
//--------------------------------
// device 2  , ПФ
//--------------------------------
0,                // status
2,                // port
2,                // addr
"MVD/2",        // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
1,                // protocol
1,                // CRC_flag
400,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&MVD_Rqst[1],   // *request
//--------------------------------
// device 3,  ГРК , GR
//--------------------------------
0,                // status
2,                // port
3,                // addr
"MVD/3",        // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
1,                // protocol
1,                // CRC_flag
400,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&MVD_Rqst[2],   // *request
};
/*----------------------------*/
struct COM_rqst MVD_Rqst[max_MVD]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_MVD_rd,            //  answ_com
f_MVD_rd,            //  answ_flt
"MVD/1",              //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&MVD[0],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsMVD_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_MVD_rd,            //  answ_com
f_MVD_rd,            //  answ_flt
"MVD/2",              //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&MVD[1],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsMVD_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 3
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_MVD_rd,            //  answ_com
f_MVD_rd,            //  answ_flt
"MVD/3",              //  name
2,                     //  n_dev номер устройства в группе однотипных устройств
&MVD[2],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsMVD_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/
struct COM_rqst MVD_Rqst_T[max_MVD]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_MVD_rd_t,            //  answ_com
f_MVD_rd_t,            //  answ_flt
"MVD/1",               //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&MVD[0],               //  *ICP_dd
0,                     //  текущая функция
ToutAnsMVD_rtu,        //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_MVD_rd_t,            //  answ_com
f_MVD_rd_t,            //  answ_flt
"MVD/2",               //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&MVD[1],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsMVD_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 3
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_MVD_rd_t,            //  answ_com
f_MVD_rd_t,            //  answ_flt
"MVD/3",               //  name
2,                     //  n_dev номер устройства в группе однотипных устройств
&MVD[2],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsMVD_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/
struct s_MVD_dev s_MVD[max_MVD];
/*
{
0 , //int r_addr;       // address of MODBUS register
0,  //int n;            // number of data - for read coils and ascii
0,  //int type;         // data type
0,  //int i;            // int,coil value for write
0,  //int exc;          // exception
0,  //long int  l;      // long
0,    //float f;          // float

-1.0, //float FlowM;          // float
-1.0, //float FlowV;          // float
BIG_P, //float MassI;          // float
BIG_P, //float MassT;          // float
BIG_P, //float VolI;          // float
BIG_P, //float VolT;          // float
-1.0, //float VolTd;        // float
-1.0, //float Temp;          // float
-1.0, //float Dens;          // float
-1.0, //float Press;         // float
-1.0, //float Press2;        // float
-1.0, //float Compos;        // float
-1.0, //float PressG;        // float
-1.0, //float PressB;        // float
-1.0, //float PressD;        // float
-1.0, //float DensG;        // float

"",   //char c[n_coil_b]; // coil(s) value(s) read from MVD
"",   //char a[n_char_b]; // ascii
0,    //int reg_err;

//=======================
0 , //int r_addr;       // address of MODBUS register
0,  //int n;            // number of data - for read coils and ascii
0,  //int type;         // data type
0,  //int i;            // int,coil value for write
0,  //int exc;          // exception
0,  //long int  l;      // long
0,    //float f;         // float

-1.0, //float FlowM;          // float
-1.0, //float FlowV;          // float
BIG_P, //float MassI;          // float
BIG_P, //float MassT;          // float
BIG_P, //float VolI;          // float
BIG_P, //float VolT;          // float
-1.0, //float VolTd;         // float
-1.0, //float Temp;          // float
-1.0, //float Dens;          // float
-1.0, //float Press;         // float
-1.0, //float Press2;        // float
-1.0, //float Compos;        // float
-1.0, //float PressG;        // float
-1.0, //float PressB;        // float
-1.0, //float PressD;        // float
-1.0, //float DensG;        // float

"",   //char c[n_coil_b]; // coil(s) value(s) read from MVD
"",   //char a[n_char_b]; // ascii
0,    //int reg_err;
//=======================
0 , //int r_addr;       // address of MODBUS register
0,  //int n;            // number of data - for read coils and ascii
0,  //int type;         // data type
0,  //int i;            // int,coil value for write
0,  //int exc;          // exception
0,  //long int  l;      // long
0,    //float f;         // float

-1.0, //float FlowM;          // float
-1.0, //float FlowV;          // float
BIG_P, //float MassI;          // float
BIG_P, //float MassT;          // float
BIG_P, //float VolI;          // float
BIG_P, //float VolT;          // float
-1.0, //float VolTd;         // float
-1.0, //float Temp;          // float
-1.0, //float Dens;          // float
-1.0, //float Press;         // float
-1.0, //float Press2;        // float
-1.0, //float Compos;        // float
-1.0, //float PressG;        // float
-1.0, //float PressB;        // float
-1.0, //float PressD;        // float
-1.0, //float DensG;        // float

"",   //char c[n_coil_b]; // coil(s) value(s) read from MVD
"",   //char a[n_char_b]; // ascii
0,    //int reg_err;

};
*/

int MVD_fn[max_MVD]={0,0,0};    // регистр функций драйвера

//  Если записать значение функции в регистр функции , например:
//  ComSK_fn[0]= RES_SK_ERR;  драйвер выполнит заданную функцию - в данном случае
// пошлет в устройство ComSK команду сброса привода.
//  После постановки запроса с установленной функцией в очередь на вывод в COM,
// регистр функции очищается драйвером.

int MVD_sw[max_MVD]={0,0,0};

unsigned int MVD_inp_err[max_MVD]={0,0,0};  // счетчик ошибок ввода

  // последовательность функций опроса
/*
int MVD_seq[16]={
mMassT, mDens ,mMassT, mMassI,
mMassT, mDens ,mMassT, mTemp,
mMassT, mDens ,mMassT, mVolI,
mMassT, mDens ,mMassT, mErr,
};
*/
int MVD_seq[16]={
mSlot, mSlot,mSlot, mSlot,
mSlot, mSlot,mSlot, mSlot,
mSlot, mSlot,mSlot, mSlot,
mSlot, mSlot,mSlot, mSlot,
};

int fl_rd_err=0;
int MVD_fl[max_MVD]={0,0,0};
unsigned char MVD_Rd_err[] ={1,3,0x01,0xa2,0x00,0x01};  // R419 - регистр ошибки
unsigned char MVD_Rd_FlowM[]={1,3,0x00,0xF6,0x00,0x02}; // R247  - mass flow
unsigned char MVD_Rd_Dens[]={1,3,0x00,0xF8,0x00,0x02};  // R249  - Density
unsigned char MVD_Rd_Temp[]={1,3,0x00,0xFA,0x00,0x02};  // R251  - Temperature
unsigned char MVD_Rd_FlowV[]={1,3,0x00,0xFC,0x00,0x02}; // R253  - volume flow

unsigned char MVD_Rd_Press[]={1,3,0x01,0x00,0x00,0x02}; // R257  - pressure
unsigned char MVD_Rd_MassT[]={1,3,0x01,0x02,0x00,0x02}; // R259  - mass total
unsigned char MVD_Rd_VolT[] ={1,3,0x01,0x04,0x00,0x02}; // R261  - volume total
unsigned char MVD_Rd_MassI[]={1,3,0x01,0x06,0x00,0x02}; // R263  - mass inventory
unsigned char MVD_Rd_VolI[]={1,3,0x01,0x08,0x00,0x02};  // R265  - Volume inventory
//unsigned char MVD_Rd_Slot[]={1,3,0x02,0xae,0x00,0x0D};  // R687-1  - Slot data , 13 registers 16 bit
unsigned char MVD_Rd_Slot[]={1,3,0x02,0xae,0x00,0x0F};  // R687-1  - Slot data , 15 registers 16 bit

// R419 - регистр ошибки , int
// R247 - mass flow      , float
// R259 - mass total     , float
// R261 - volume total   , float


void f_MVD(int ii)
{
 // ii - номер устройства
 //  Функция циклически шлет запрос на получение данных  из
 // устройства MVD  '01 03 01A2 0001' - R419 - регистр ошибки,
 //
 //   Период посылки MVD[ii].pool_time мс.
 //
 //    Если  регистр функции != 0 - заданная функция выполняется
 //  в первую очередь ( приоритет выше, чем приоритет циклического опроса).
 //   После постановки запроса с указанной функцией в очередь COM порта
 //  в регистр функции записывается F_PROGRESS
 //  После приема ответа от устройства в регистр функции запишется либо 0 - OK,
 // либо F_FAULT

int i,fn,sw,i2,itmp;
long int i1,l_tmp;
union  { float f; unsigned long l; } o;
unsigned char buf_tmp[20];

 if(MVD[ii].status == 0) return;

 if( MVD_fl[ii] >= 0 )
            f_init_MVD(ii);

  if((MVD_Rqst[ii].status == Req_Flt) ||
     (MVD_Rqst[ii].status == Req_OK));
  else return;

 if(f_queue_chk(MVD[ii].port)== 0) return;// очередь переполнена

 if((MVD_fn[ii] != 0) && (MVD_fn[ii] < F_PROGRESS))
      fn=MVD_fn[ii];
 else if( f_timer(MVD[ii].time_stamp_pool,MVD[ii].pool_time ))
       {
           fn=MVD_RD_P;
     //  if((MVD_fl[ii] < 0)|| (MVD_fl[ii] >= 55 )) fn=MVD_RD_P;
     //  else return;
       }
 else return;

      switch(fn)
      {
        case MVD_RD_I :
           // чтение данных Integer
            buf_tmp[5]=1;
 m_rdi:
            i1=6;
            buf_tmp[0]=MVD[ii].addr;
            buf_tmp[1]=3;
            buf_tmp[2]=((s_MVD[ii].r_addr-1)>> 8) & 0xff;
            buf_tmp[3]= (s_MVD[ii].r_addr-1) & 0xff;
            buf_tmp[4]=0;
            MVD_fn[ii]=F_PROGRESS;
            s_MVD[ii].type=fn;
            s_MVD[ii].exc=-1;
            break;

          case  MVD_RD_L :
          case  MVD_RD_F:
           // чтение данных Long, Float

           buf_tmp[5]=2;
          goto m_rdi;

          case MVD_RD_C:
           // чтение данных Coil
            buf_tmp[1]=1;
     m_rdc:
            i1=6;
            buf_tmp[0]=MVD[ii].addr;
            buf_tmp[2]=((s_MVD[ii].r_addr-1)>> 8) & 0xff;
            buf_tmp[3]= (s_MVD[ii].r_addr-1) & 0xff;
            buf_tmp[4]=(s_MVD[ii].n>> 8) & 0xff;
            buf_tmp[5]= s_MVD[ii].n & 0xff;
            MVD_fn[ii]=F_PROGRESS;
            s_MVD[ii].type=fn;
            s_MVD[ii].exc=-1;
            break;

          case MVD_RD_S:
           // чтение данных String
            buf_tmp[1]=3;
            goto m_rdc;

          case MVD_WR_I :
           // запись данных Integer
            i1=6;
            buf_tmp[0]=MVD[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=((s_MVD[ii].r_addr-1)>> 8) & 0xff;
            buf_tmp[3]= (s_MVD[ii].r_addr-1) & 0xff;
            buf_tmp[4]=(s_MVD[ii].i>> 8) & 0xff;
            buf_tmp[5]= s_MVD[ii].i & 0xff;
            MVD_fn[ii]=F_PROGRESS;
            s_MVD[ii].type=fn;
            s_MVD[ii].exc=-1;
            break;

          case MVD_WR_L :
           // запись данных Long
            i1=11;
            buf_tmp[0]=MVD[ii].addr;
            buf_tmp[1]=16;
            buf_tmp[2]=((s_MVD[ii].r_addr-1)>> 8) & 0xff;
            buf_tmp[3]= (s_MVD[ii].r_addr-1) & 0xff;
            buf_tmp[4]= 0;  // количество регистров
            buf_tmp[5]= 2;  // --|
            buf_tmp[6]= 4;  // количество байт
             l_tmp=s_MVD[ii].l;
            buf_tmp[7]=(l_tmp >> 24) & 0xff;         // количество регистров
            buf_tmp[8]=(l_tmp >> 16) & 0xff;         // количество регистров
            buf_tmp[9]=(l_tmp >> 8)  & 0xff;         // количество регистров
            buf_tmp[10]=l_tmp & 0xff;    // количество регистров

            MVD_fn[ii]=F_PROGRESS;
            s_MVD[ii].type=fn;
            s_MVD[ii].exc=-1;
            break;

          case MVD_WR_F :
           // запись данных Float
            i1=11;
            buf_tmp[0]=MVD[ii].addr;
            buf_tmp[1]=16;
            buf_tmp[2]=((s_MVD[ii].r_addr-1)>> 8) & 0xff;
            buf_tmp[3]= (s_MVD[ii].r_addr-1) & 0xff;
            buf_tmp[4]= 0;  // количество регистров
            buf_tmp[5]= 2;  // --|
            buf_tmp[6]= 4;  // количество байт
            o.f=s_MVD[ii].f;
            buf_tmp[7] =(o.l >> 8 ) & 0xff;
            buf_tmp[8] =(o.l >> 0 ) & 0xff;
            buf_tmp[9] =(o.l >> 24)  & 0xff;
            buf_tmp[10]=(o.l >> 16) & 0xff;

            MVD_fn[ii]=F_PROGRESS;
            s_MVD[ii].type=fn;
            s_MVD[ii].exc=-1;
            break;

          case MVD_WR_C :
           // запись данных Coil
            i1=6;
            buf_tmp[0]=MVD[ii].addr;
            buf_tmp[1]=5;
            buf_tmp[2]=((s_MVD[ii].r_addr-1)>> 8) & 0xff;
            buf_tmp[3]= (s_MVD[ii].r_addr-1) & 0xff;
//          if(s_MVD[ii].w_coil != 0)
            if(s_MVD[ii].i != 0)
               buf_tmp[4]= 0xff;
            else
               buf_tmp[4]= 0;
            buf_tmp[5]= 0;
            MVD_fn[ii]=F_PROGRESS;
            s_MVD[ii].type=fn;
            s_MVD[ii].exc=-1;
            break;

          case MVD_RD_P:  // циклическое  чтение состояния
          // формирует команды чтения
             MVD_sw[ii]=(MVD_sw[ii]+1) & 0xf;
             sw=MVD_seq[MVD_sw[ii]];
             i1=6;
             MVD_fn[ii]=0;
             switch (sw)
             {
              case mErr:
            _fmemcpy(buf_tmp,MVD_Rd_err,(long int)i1);
                break;

              case mVolT:
             _fmemcpy(buf_tmp,MVD_Rd_VolT,(long int)i1);
                break;

              case mDens:
             _fmemcpy(buf_tmp,MVD_Rd_Dens,(long int)i1);
                break;
       /*
              case mFlowM:
             _fmemcpy(buf_tmp,MVD_Rd_FlowM,(long int)i1);
                break;
       */
              case mSlot:
             _fmemcpy(buf_tmp,MVD_Rd_Slot,(long int)i1);
                break;
              case mMassT:
             _fmemcpy(buf_tmp,MVD_Rd_MassT,(long int)i1);
                break;

      //      case mFlowV:
      //     _fmemcpy(buf_tmp,MVD_Rd_FlowV,(long int)i1);
      //        break;

              case mTemp:
             _fmemcpy(buf_tmp,MVD_Rd_Temp,(long int)i1);
                break;

              case mVolI:
             _fmemcpy(buf_tmp,MVD_Rd_VolI,(long int)i1);
                break;

              case mMassI:
             _fmemcpy(buf_tmp,MVD_Rd_MassI,(long int)i1);
                break;

              default:
                return;
             }
             buf_tmp[0]=MVD[ii].addr;
             fn+=sw;
             break;

          default:
          MVD_fn[ii]=0;
          return;
      }
           MVD_Rqst[ii].function = fn;
           i2=i1;
           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(MVD_Rqst[ii].Cmd,buf_tmp,(long)i2);
           MVD_Rqst[ii].cmd_lgth=i2;
           MVD_Rqst[ii].time_stamp=TimeStamp;
           MVD[ii].time_stamp_pool=TimeStamp;
           MVD[ii].n_transaction++;
           f_queue_put(MVD[ii].port, &MVD_Rqst[ii]) ;
}
/*----------------------------*/
int f_MVD_WR(int ii,int fn,int addr,int i_val,long int l_val,float f_val)
{
 // записывает значение в регистр MVD
 // ii - номер устройства
int i,sw,i2,itmp;
long int i1;
union  { float f; unsigned long l; } o;
unsigned char buf_tmp[20];

#if(SIM0 != 0)
 if(MVD[ii].status == 0) return 1;
#else
 if(MVD[ii].status == 0) return 0;
#endif

 switch(ii)
 {
  case 0:
    if(ICP_error[MVD1] != 0)  return 0;
    break;
  case 1:
    if(ICP_error[MVD2] != 0)  return 0;
    break;
  case 2:
    if(ICP_error[MVD3] != 0)  return 0;
    break;
  default: return 0;
 }

  if((MVD_Rqst_T[ii].status == Req_Flt) ||
     (MVD_Rqst_T[ii].status == Req_OK));
  else return 0;

 if(f_queue_chk(MVD[ii].port)== 0) return 0;// очередь переполнена
     switch(fn)
      {
          case MVD_WR_I :
           // запись данных Integer
            i1=6;
            buf_tmp[0]=MVD[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=((addr-1)>> 8) & 0xff;
            buf_tmp[3]= (addr-1) & 0xff;
            buf_tmp[4]=(i_val>> 8) & 0xff;
            buf_tmp[5]= i_val & 0xff;
            break;

          case MVD_WR_L :
           // запись данных Long
            i1=11;
            buf_tmp[0]=MVD[ii].addr;
            buf_tmp[1]=16;
            buf_tmp[2]=((addr-1)>> 8) & 0xff;
            buf_tmp[3]= (addr-1) & 0xff;
            buf_tmp[4]= 0;  // количество регистров
            buf_tmp[5]= 2;  // --|
            buf_tmp[6]= 4;  // количество байт
            buf_tmp[7]=(l_val >> 24) & 0xff;         // количество регистров
            buf_tmp[8]=(l_val >> 16) & 0xff;         // количество регистров
            buf_tmp[9]=(l_val >> 8)  & 0xff;         // количество регистров
            buf_tmp[10]=l_val & 0xff;    // количество регистров

            break;

          case MVD_WR_F :
           // запись данных Float
            i1=11;
            buf_tmp[0]=MVD[ii].addr;
            buf_tmp[1]=16;
            buf_tmp[2]=((addr-1)>> 8) & 0xff;
            buf_tmp[3]= (addr-1) & 0xff;
            buf_tmp[4]= 0;  // количество регистров
            buf_tmp[5]= 2;  // --|
            buf_tmp[6]= 4;  // количество байт
            o.f=f_val;
            buf_tmp[7] =(o.l >> 8 ) & 0xff;
            buf_tmp[8] =(o.l >> 0 ) & 0xff;
            buf_tmp[9] =(o.l >> 24)  & 0xff;
            buf_tmp[10]=(o.l >> 16) & 0xff;
            break;

          case MVD_WR_C :
           // запись данных Coil
            i1=6;
            buf_tmp[0]=MVD[ii].addr;
            buf_tmp[1]=5;
            buf_tmp[2]=((addr-1)>> 8) & 0xff;
            buf_tmp[3]= (addr-1) & 0xff;
            if(i_val != 0)
               buf_tmp[4]= 0xff;
            else
               buf_tmp[4]= 0;
            buf_tmp[5]= 0;
            break;

          default:
          return 0;
      }

           MVD_Rqst_T[ii].function = fn;
           i2=i1;
           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(MVD_Rqst_T[ii].Cmd,buf_tmp,(long)i2);
           MVD_Rqst_T[ii].cmd_lgth=i2;
           MVD_Rqst_T[ii].time_stamp=TimeStamp;
           MVD[ii].n_transaction++;
           f_queue_put(MVD[ii].port, &MVD_Rqst_T[ii]) ;
           return 1;
}
/*----------------------------*/
/*
char *err_F425[]={
//F245
"Ошибка контр.суммы EEPROM",   //"(E)EPROM checksum failure",        //D0
"Сбой диагностики ОЗУ",        //"RAM diagnostic failure",           //D1
"Сбой сенсора",                //"Sensor failure",                   //D2
"Сбой датчика температуры",    //"Temperature sensor failure",       //D3

"Переполнение по входу",       //"Input overrange",                  //D5
"Частотный выход насыщен",     //"Frequency/pulse output saturated", //D6
"Трансмиттер не сконфигурир.", //"Transmitter not configured",       //D7
"Сбой прерывания Real-time",   //"Real-time interrupt failure",      //D8

"Перв.токовый выход насыщен",  //"Primary mA output saturated",      //D9
"Втор.токовый выход насыщен",  //"Secondary mA output saturated",    //D10
"Перв.ток. выход фиксирован",  //"Primary mA output fixed",          //D11
"Втор.ток. выход фиксирован",  //"Secondary mA output fixed",        //D12

"Плотность за пределами изм.",   //"Density overrange",                //D13
"Ошибка калибровки расходомера", //"Flowmeter zeroing failure",        //D14
"Калибр.значение слишком мало",  // "Zero value too low",               //D15
"Калибр.значение слишком велико",//"Zero value too high",              //D16

"Сбой электроники трансмиттера", //"Transmitter electronics failure",  //D17
"Калибровка расходомера",        //"Flowmeter zeroing in progress",    //D18
"Затрудненное истечение",        //"Slug flow",                        //D19
"Сброс по включению питания",    //"Power reset occurred",             //D20

"Изменена конфигурация трансм.", //"Transmitter configuration changed",   //21
"Инициализация трансмиттера",    //"Transmitter initializing/warming up", //22
""
};

*/
/* ---------------------------------------- */
/*----------------------------*/
int  MVD_t_err[max_MVD]={0,0};
int  MVD_t_rslt[max_MVD]={0,0};

void f_MVD_rd_t(int ii)
{   // принят ответ на команду записи в MVD
// ii - номер порта
// заносит принятые данные в переменные
//
 struct COM_rqst *request;
 int i,nn,i1,im;
 int count,tmp;

 request=COM_que[ii].request[COM_que[ii].empt_ptr];
 nn=request->n_dev; // номер устройства MVD (0,1..)

 count=n_bcom[ii];  // длина команды
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
     s_MVD[nn].exc=cb_COM[ii][2];
     goto m_end;
   }
    // ответ на команду записи
  if ((cb_COM[ii][1] == 6)  || (cb_COM[ii][1] == 16) ||(cb_COM[ii][1] == 5));
  else  goto m_crc_err;

  MVD_t_rslt[nn]+=1;
 m_end:
  MVD[nn].n_success++;
  MVD_inp_err[nn]=0;
  f_queue_free(ii,&MVD_Rqst_T[nn]);
  MVD_t_err[nn]=0;
  return;

m_crc_err:
 MVD[nn].time_stamp_rcv=0;
 MVD[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 MVD[nn].time_stamp_rcv=0;
 MVD[nn].n_timeout_error++;

m_err:
 MVD_t_err[nn]++;

 f_queue_free(ii,&MVD_Rqst_T[nn]);
 if(MVD_t_err[nn]>=MVD_max_t_err)
 {
   MVD_t_err[nn]=MVD_max_t_err;
   f_icp_error(&MVD[nn],WR_ERR );
 }
 else
 {
  MVD_Rqst_T[nn].time_stamp=TimeStamp;
  f_queue_put(ii, &MVD_Rqst_T[nn]) ;
 }
  MVD[nn].n_transaction++;
}

/*
m_err:
 MVD_t_err[nn]++;

 if(MVD_t_err[nn]>=MVD_max_t_err)
 {
   MVD_t_err[nn]=MVD_max_t_err;
   f_icp_error(&MVD[nn],WR_ERR );
 }
 f_queue_free(ii,&MVD_Rqst_T[nn]);

 MVD_Rqst_T[ii].time_stamp=TimeStamp;
 MVD[ii].n_transaction++;
 f_queue_put(MVD[ii].port, &MVD_Rqst_T[ii]) ;
}
*/

/* ---------------------------------------- */
/*----------------------------*/
unsigned long int MVDit[max_MVD];
int n_send[max_MVD];
int flag_mvd_fst[max_MVD]={0,0};
int flag_mvd_ton[max_MVD]={0,0};

int ii_MVD_i[max_MVD]={0,0};
int ii_MVD_fl[max_MVD]={0,0};

float  TimeMVD_Init=30000;  // Max Init time for MVD,ms
float  TimeMVD_evt =1000 ;  // time for generate event,ms

struct mvd_init {
int addr;
int val;
};

struct mvd_init MVD_i[]=
{
 655,258, // Mass total     687 --
 656,248, // Density        689 --
 657,250, // Temperature    691 --
 658,262, // Mass inventory 693 --
 659,264, // Volume invntry 695 --
// 659,252, // Volume flow    701 --
 660,246, // Mass flow      697 --
// 661,418, // Error          699 --

 661,260, // Volume total   699 --
 662,418, // Error          701 --
//39,73,  // unit for mass flow,kg/s
 39,75,  // unit for mass flow,kg/hour
 42,24,  // Volume flow  l/sec
 40,92,  // Density unit kg/m3 --
//  45,61,  // unit for mass total,61- kg, 60-g
//  46,41,  // unit for volume total, l
 41,32,  // unit for temperature, C

  // для отображения объема/массы колонки

// 137, 4,  // Event 1 variable code: Mass inventory
// 138, 4,  // Event 2 variable code: Mass inventory

// 609, 0,  // Event x index  = 0
// 615, 4,  // Event x variable code: Mass inventory
// 610, 2,  // Event x type code :2 IN HI/LO

 17,5 ,   // Flow direction code: 0 - forward flow  only
          //                      2 - bidirectional flow
          //                      5 - reverce bidirectional flow
};

int ii_MVD_max=sizeof(MVD_i) / sizeof(MVD_i[0]);
//----------------------------------------------
/*
#define Addr_Fl_grk    139
#define Fl_grk_OK      1
#define Fl_grk_FLT     2

#define Addr_Mass_kd   241
#define Addr_Vol_kd    243

#define Addr_Mass_grk  611
#define Addr_Vol_grk   613
*/

float Vol_kd=BIG_P;   // добавка к VolI для отображения счетчика колонки
float Mass_kd=BIG_P;  // добавка к MassI для отображения счетчика колонки

float Vol_grk=0;  // показания ГРК на дисплее ВРФ
float Mass_grk=0; // показания ГРК на дисплее ВРФ
float Cut_Vol[3]={3.,3.,3.};
float Cut_Mass[3]={3000.,3000.,3000.};
float M_fct[3]={1.,1.,1.};
float V_fct[3]={1.,1.,1.};
float D_fct[3]={1.,1.,1.};

float M_fct_d[3]={1.,1.,1.};
float V_fct_d[3]={1.,1.,1.};
float D_fct_d[3]={1.,1.,1.};

void f_init_MVD(int ii)
 {

 float ftmp1,ftmp2;

 union  { float f; unsigned long l; } o;

  if((MVD_fn[ii] != 0) && (MVD_fn[ii] != F_FAULT))
  {
    return;
  }
  switch (MVD_fl[ii])
   {
    case 0:
      n_send[ii]=0;
       MVD_fl[ii]++;
       break;

    case 1:  // проверяет текущее значение порядка
             // следования байт в регистрах float
             // если не 1 , то устанавливает =1.

      if(MVD_fn[ii] != 0) break;
      MVD_fn[ii]=MVD_RD_I;;
      s_MVD[ii].r_addr=521; // I521  Byte order code for float registers
      MVD_fl[ii]++;
      break;

    case 2:

      flag_mvd_ton[ii]=1;
      if(sw_mmi==0) f_prn_begin();

      if((MVD_fn[ii] == F_FAULT) || (s_MVD[ii].i == 1) )
       { // byte order is correct or RFT9739 device connected
         MVD_fl[ii]=3;
         ii_MVD_i[ii]=0;
         ii_MVD_fl[ii]=0;
         n_send[ii]=0;
         break;
       }
       MVD_fn[ii]=MVD_WR_I;
       s_MVD[ii].r_addr=521; // I521  Byte order code  for float registers
       s_MVD[ii].i = 1;
       if((n_send[ii]++) > 3)
       {
         flag_mvd_ton[ii]=0;
//    if(flag_prn)
//      printf("\n\rfault of 3 attempts to wtite conf. A521 MVD%d",ii);
         f_icp_error(&MVD[ii],WR_ERR );
         MVD_fl[ii]=-1;
       }
       MVD_fl[ii]=1;
       break;

    case 3:
      MVD_fn[ii]=MVD_RD_I;
      s_MVD[ii].r_addr=MVD_i[ii_MVD_i[ii]].addr;
      MVD_fl[ii]=4;
      break;

    case 4:
      if(s_MVD[ii].i == MVD_i[ii_MVD_i[ii]].val);
      else ii_MVD_fl[ii]=1;
      ii_MVD_i[ii]++;
      if(ii_MVD_i[ii] >= ii_MVD_max)
      {
        if( ii_MVD_fl[ii] == 0 )
        { // значения записаны корректно
         if(flag_prn)
         printf("\r\nMVD%d  Slot OK,%d values",ii,ii_MVD_i[ii]);
         MVD_fl[ii]=53;
         break;
        }
        else
        { // значения нужно записать

         if((n_send[ii]++) > 3)
         { // за 3 попытки записать не удалось
           flag_mvd_ton[ii]=0;
//    if(flag_prn)
//      printf("\n\rfault of 3 attempts to wtite conf.MVD%d",ii);
//         f_icp_error(&MVD[ii],WR_ERR );
           f_icp_errS(MVD_cfg);
           MVD_fl[ii]=-1;
           break;
         }

      if(flag_prn)
        printf("\r\nMVD%d.Slot need to write",ii);
         f_wr_evt(evt_Slot);
         MVD_fl[ii]=5;
         ii_MVD_i[ii]=0;
         ii_MVD_fl[ii]=0;
         break;
        }
      }
      else
      {
        MVD_fl[ii]=3;
        break;
      }

//--------------------------
    case 5: // запись значений Slot
      MVD_fn[ii]=MVD_WR_I;;
      s_MVD[ii].r_addr=MVD_i[ii_MVD_i[ii]].addr;
      s_MVD[ii].i = MVD_i[ii_MVD_i[ii]].val;

      ii_MVD_i[ii]++;
      if(ii_MVD_i[ii] >= ii_MVD_max)
      {  // все значения записаны , проверить запись

      if(flag_prn)
         printf("\r\n MVD%d,Slot is wrote,%d values",ii,ii_MVD_i[ii]);
         ii_MVD_i[ii]=0;
         ii_MVD_fl[ii]=0;
         MVD_fl[ii]=3;
         break;
      }
      break;
//------------------------------------

    case 53:
    if( f_timer(0L,TimeMVD_Init ) )
       flag_mvd_fst[ii] = 0;
    else
//    if(f_get_icp_error(&MVD[ii]))
        flag_mvd_fst[ii] = 1;

     MVD_fl[ii]++;

     case 54:
     if(flag_mvd_fst[ii])
       if( f_timer(0L,TimeMVD_Init ) == 0 ) break;
      flag_mvd_ton[ii]=0;
      f_icp_error(&MVD[ii],0 );
    if(sw_mmi==0) f_prn_begin();

// printf("\n\r MVD init finished");// qqq

       MVD[ii].n_transaction    = 0;
       MVD[ii].n_success        = 0;
       MVD[ii].n_timeout_error  = 0;
       MVD[ii].n_CRC_error      = 0;

       MVDit[ii]=TimeStamp;
       MVD_fl[ii]=80;
       break;

   case 80:
       // cutoff for Mass Flow
      MVD_fn[ii]=MVD_WR_F;
      s_MVD[ii].r_addr = 195;
      s_MVD[ii].f= Cut_Mass[ii];
      MVD_fl[ii]=81;
       break;

   case 81:
       // cutoff for Vol Flow
      MVD_fn[ii]=MVD_WR_F;
      s_MVD[ii].r_addr = 197;
      s_MVD[ii].f= Cut_Vol[ii];
      MVD_fl[ii]=82;
       break;

   case 82:
       // Множитель массы
//    if(ii > 0)
      {
        MVD_fl[ii]=55;
        break;
      }
/*
      MVD_fn[ii]=MVD_WR_F;
      s_MVD[ii].r_addr = 279;
      s_MVD[ii].f= M_fct_d[ii];
      MVD_fl[ii]=83;
       break;

   case 83:
       // Множитель объема
      MVD_fn[ii]=MVD_WR_F;
      s_MVD[ii].r_addr = 281;
      s_MVD[ii].f= V_fct_d[ii];
      MVD_fl[ii]=85;
      break;
   case 84:
       // Множитель  плотности
      MVD_fn[ii]=MVD_WR_F;
      s_MVD[ii].r_addr = 283;
      s_MVD[ii].f= D_fct_d[ii];
      MVD_fl[ii]=85;
      break;
*/
   case 85:
       MVD_fl[ii]=55;
       break;

     case 55:
//  if(ii != 0)
    {
       MVD_fl[ii]=-1;
       break;
    }
/*
    if(ii==0) if((fl_VI==0) || (fl_MI==0)) return;

      MVD_fl[ii]++;
     case 56:
          // проверка состояния отображения колонки
      if(  f_rd_EEE_kd(EE_flg) < 0) goto fin_e;
      if(Fl_grk == Fl_grk_OK)
      {
       if( f_rd_EEE_kd(EE_kd) < 0) goto fin_e;
       Vol_grk=s_MVD[0].VolI+Vol_kd;
       Mass_grk=s_MVD[0].MassI+Mass_kd;
       f_wr_evt(evt_dsp_grk);
      }
      else
      {
       // во время приема произошло отключение питания и данные в колонке не откорретированы на величину приема
       // чтение требуемых показаний на ГРК

         if( f_rd_EEE_kd(EE_grk) < 0) goto fin_e;

         Mass_kd =  Mass_grk - s_MVD[0].MassI;
         Vol_kd  = Vol_grk - s_MVD[0].VolI;

         ftmp1= Mass_kd ;
         ftmp2= Vol_kd  ;

         if(f_wrt_eee_kd( EE_kd )  <0 ) goto fin_e;
         //  записано новое значение Mass_kd
         //  записано новое значение Vol_kd

         if( f_rd_EEE_kd(EE_kd) < 0) goto fin_e;
         if(Mass_kd != ftmp1) goto fin_e;
         if(Vol_kd  !=  ftmp2)  goto fin_e;

         Fl_grk=Fl_grk_OK;
         if(f_wrt_eee_kd( EE_flg ) < 0 ) goto fin_e;

      // в расходомер записано значение Fl_grk_OK
         if( f_rd_EEE_kd(EE_flg) < 0) goto fin_e;
         if(Fl_grk != Fl_grk_OK)  goto fin_e;
         f_wr_evt(evt_kd_grk);

      }
       MVD_fl[ii]=-1;
       break;
fin_e:
       f_icp_errS(EEE_CRC_error);
       MVD_fl[ii]=-1;
       break;
*/

    case -1:
       break;
    default:break;
   }
 }
//-----------------------------------------
int st_MVD_sw=0;
int cl_MVD_sw=0;
long int t_prep_MVD=0;
int flag_prok=0; // флаг прокачки через клапан "Плотность"

float volm_cut_wrk=0; // l/sec
float mass_cut_wrk=0; // kg/sec
//-----------------------------------------
long int Tim_Prep1   = 5000;   // время начальной проверки без открывания клапана 'Плотность'
long int Tim_Prep2   = 120000; // время ожидания завершения прокачки с открытым клапаном 'Плотность'
long int Tim_Prep_ND = 1500;   // время антидребезга при определении стабильности жидкой фазы
long int Tim_Prep3   = 3000;   // задержка между выключением клапана 'Плотность'
                               // и разрешением счета
long int t_prep_MVD1;

int i_CL_dens=0;

int f_pmp_chRQ(); // проверка состояния запросов на вкл.насоса , 1  - есть запрос
int f_pmp_chRQ1(); // проверка состояния запроса 1 на вкл.насоса , 1  - есть запрос
void f_pmp_OK();  // разрешение счета расходомеров
void f_pmp_err(); // ошибка - нет ЖФ, не разрешать запуск отпуска
void f_pmp_no_err(); // снять ошибку , разрешить новый запрос

void f_pmp_ON();  // включение насоса , при этом полость расходомера ЖФ д.б.связана с выходом насоса
void f_den_ON();  // открывание клапана прокачки (режим измерения плотности)
void f_den_OFF(); // закрывание клапана прокачки
void f_pmp_OFF(); // выключение насоса

void f_flow_up(); // проверка состояния MVD ЖФ и прокачка , если необходимо
void f_set_RQ( int flag);  //  выставляет запрос на включение насоса
int f_chk_flup(); //  проверяет состояние включения насоса: -1 - ошибка, 1 - OK, 0 - в процессе включения



int RQ1=0;        // запрос на включение насоса от ГРК N1
int RQ2=0;        // для ГРК N1 считываются  по MODBUS
int RQ2W=0;       // для ГРК N2 передается  по MODBUS

int  st_PMP_sw=0; // состояние процесса включения насоса для колонки N1
int  PMP_sw=10;   // для ГРК N2 принимается по MODBUS, stand alone - OK

int  FL_UP_sw=0;
long int t_MVD_pmp;
long int t_MVD_pmp1;

void f_pump_run()
{
 // Включает насос при наличии запросов от колонок.
 // Проверяется состояние расходомера ЖФ .
 // Если расходомер не заполнен жидкой фазой сначала выполняется прокачка
 // через клапан "Плотность" без разрешения счета.
 // В случае ошибки st_PMP_sw=-1 , f_pmp_err();
 // При заполнении ЖФ, клапан "Плотность" закрывается,
 // st_PMP_sw = 10, f_pmp_OK();
 // насос остается включенныи до снятия обоих запросов от колонок.
 // После снятия запросов st_PMP_sw=0;

 switch (st_PMP_sw)
 {
    case 0:  // ожидание запросов на включение насоса
     {
       if( f_pmp_chRQ() == 0) return;

       st_PMP_sw=1;
       FL_UP_sw=0;
       t_MVD_pmp=TimeStamp;
       t_MVD_pmp1=TimeStamp;

       break;
     }

    case 1:  // включение насоса и проверка состояния расходомера ЖФ
     {

       if( f_pmp_chRQ() == 0)
       {
         f_pmp_no_err();
         st_PMP_sw=0;
         f_den_OFF(); // закрывание клапана прокачки
         f_pmp_OFF(); // выключение насоса
         return;
       }
       f_flow_up(); // проверка состояния
       break;
     }
    case 10:  // насос включен на отпуск, ожидание снятия запросов
     {
       if( f_pmp_chRQ() == 0)
       {
         f_pmp_no_err();
         st_PMP_sw=0;
         f_den_OFF(); // закрывание клапана прокачки
         f_pmp_OFF(); // выключение насоса
         return;
       }
       break;
     }
    case -1:
 // ошибка при прокачке,насос выключен , ожидание снятия запросов
     {

       if( f_pmp_chRQ() == 0)
       {
         f_pmp_no_err();
         st_PMP_sw=0;
         f_den_OFF(); // закрывание клапана прокачки
         f_pmp_OFF(); // выключение насоса
         return;
       }
       break;
     }

 }
}
//-----------------------------------------

int FL_UP_sw_c=-235;
void f_flow_up()
{
 // включает насос, проверяет состояние расходомера ЖФ,
 // при необходимости включает прокачку и ожидает ее завершения


  switch (FL_UP_sw)
  {

    case 0:  // Stop totalizers

  if(MVD_fn[MVD_FLOW] != 0)
  {
    if( f_timer(t_MVD_pmp,(long int)5000L ) == 0 ) return;
    f_icp_errS(No_F_dat_err);
    FL_UP_sw=60;
    return;
  }


            MVD_fn[MVD_FLOW]=MVD_WR_C;
            s_MVD[MVD_FLOW].r_addr=2; // Stop totalizers
            s_MVD[MVD_FLOW].i=0;
            FL_UP_sw=1;
            t_MVD_pmp=TimeStamp;
            break;

    case 1:
       // установка значения среза массового расхода для рабочего режима
  if(MVD_fn[MVD_FLOW] != 0)
  {
    if( f_timer(t_MVD_pmp,(long int)5000L ) == 0 ) return;
    f_icp_errS(No_F_dat_err);
    FL_UP_sw=60;
    return;
  }
        MVD_fn[MVD_FLOW]=MVD_WR_F;
        s_MVD[MVD_FLOW].r_addr=195; // Cuttoff for mass flow
//      s_MVD[MVD_FLOW].f=dens_cutoff;
        s_MVD[MVD_FLOW].f=0;
        FL_UP_sw=2;
        t_MVD_pmp=TimeStamp;
        break;

    case 2:
       // запуск отпуска без открывания клапанов
        f_den_OFF(); // закрывание клапана прокачки
        f_pmp_ON();  // включение насоса , при этом полость расходомера ЖФ д.б.связана с выходом насоса
        t_MVD_pmp=TimeStamp;
        t_MVD_pmp1=TimeStamp;
        FL_UP_sw=31;
        break;

   case 31:
      // насос работает,все клапаны закрыты
      // проверка состояния

      if( f_timer(t_MVD_pmp, Tim_Prep_ND ) == 0 )
      {
        t_MVD_pmp1=TimeStamp;
        break;

      }

      if( f_timer(t_MVD_pmp, Tim_Prep1 ) == 0 )
       {
         if( (s_MVD[0].Press - s_MVD[0].PressG) > PressLvl_on0 );
         else
          t_MVD_pmp1=TimeStamp;

         if( fabs(s_MVD[MVD_FLOW].FlowM) < F_Dns_off) ;
         else
             t_MVD_pmp1=TimeStamp;

         if( f_timer(t_MVD_pmp1, Tim_Prep_ND ) != 0 )
         { // антидребезг прошел
          t_MVD_pmp=TimeStamp;
          FL_UP_sw=3; // перейти к нормальному отпуску
          f_den_OFF();
          if(f_pmp_chRQ1()) f_wr_dat_st();
         }
       }
      else
       {  // время начальной проверки вышло, жидкая фаза не стабильна
          // открыть клапан 'Плотность' для прокачки
         f_den_ON();  // открывание клапана прокачки (режим измерения плотности)
         t_MVD_pmp=TimeStamp;
         t_MVD_pmp1=TimeStamp;
         FL_UP_sw=32;
         if(f_pmp_chRQ1())  flag_prok=1;


         break;
       }
       break;
   case 32:
       // ожидание завершения прокачки с открытым клапаном 'Плотность'
      if( f_timer(t_MVD_pmp, Tim_Prep2 ) == 0 )
       {
         if( (s_MVD[0].Press - s_MVD[0].PressG) > PressLvl_on0 )
         {
            if( f_timer(t_MVD_pmp1, Tim_Prep_ND ) != 0 )
            { // антидребезг прошел
             i_CL_dens= 0;
             t_MVD_pmp=TimeStamp;
             FL_UP_sw=40; // перейти к нормальному отпуску
             f_den_OFF();
             time_t_NDB=TimeStamp;
            }
         }
         else
          t_MVD_pmp1=TimeStamp;

       }
      else
       {  // допустимое время вышло, жидкая фаза не стабильна

         if(f_pmp_chRQ1())  f_wr_dat_st();
         f_icp_errS(No_liq_err);
         FL_UP_sw=60;
//       st_PMP_sw=-1;
         f_den_OFF(); // закрывание клапана прокачки
         f_pmp_OFF(); // выключение насоса

       //  ff_serv_N=NULL;
       }
      break;

    case 40:
      // ожидание закрывания клапана 'Плотность'
    if( f_timer(t_MVD_pmp, T_Dns_off ) )
    {
      if(f_pmp_chRQ1()) f_wr_dat_st();
      f_icp_errS(DNS_not_off);

      FL_UP_sw=60;
      break;
    }

    if( fabs(s_MVD[MVD_FLOW].FlowM) < F_Dns_off) ;
    else
         time_t_NDB=TimeStamp;

    if( f_timer(time_t_NDB, T_Dns_NDB ) )
      {  // клапан "плотность" закрылся
         if(f_pmp_chRQ1()) f_wr_dat_st();
         FL_UP_sw=3;
         t_MVD_pmp=TimeStamp;
//       if(flag_prn)
//            printf("\n\rDNS_off %ld ms",(TimeStamp-t_MVD_pmp));
      }
    break;

    case 3:

       // в расходомере жидкая фаза,

       // установка значения среза массового расхода для режима измерений
  if(MVD_fn[MVD_FLOW] != 0)
  {
    if( f_timer(t_MVD_pmp,(long int)5000L ) == 0 ) return;
    f_icp_errS(No_F_dat_err);
    FL_UP_sw=60;
    return;
  }

          MVD_fn[MVD_FLOW]=MVD_WR_F;
          s_MVD[MVD_FLOW].r_addr=195; // Cuttoff for mass flow
          s_MVD[MVD_FLOW].f=dens_cutoff;
          t_MVD_pmp=TimeStamp;

          f_pmp_OK();  // разрешение счета расходомеров
          st_PMP_sw=10;

          FL_UP_sw=-1;
          break;

    case 60:
       // прерывание запуска отпуска
       // установка значения среза массового расхода для режима ожидания

         f_den_OFF(); // закрывание клапана прокачки
         f_pmp_OFF(); // выключение насоса
         FL_UP_sw=61;
    case 61:
       // прерывание запуска отпуска
       // установка значения среза массового расхода для режима ожидания

    if((MVD_fn[MVD_FLOW] != 0) && (MVD_fn[MVD_FLOW] != F_FAULT))
    {
      if( f_timer(t_MVD_pmp,(long int)900L ) == 0 ) return;
    }
        MVD_fn[0]=MVD_WR_F;
        s_MVD[MVD_FLOW].r_addr=195; // Cuttoff for mass flow
        s_MVD[MVD_FLOW].f=dens_cutoff;
        FL_UP_sw=62;
        t_MVD_pmp=TimeStamp;
        break;


    case 62:
    if((MVD_fn[MVD_FLOW] != 0) && (MVD_fn[MVD_FLOW] != F_FAULT))
    {
      if( f_timer(t_MVD_pmp,(long int)800L ) == 0 ) return;
    }
       f_pmp_err();
       st_PMP_sw=-1;
       FL_UP_sw=-1;
       break;

    default:break;
  }
}
//-----------------------------------------
// QQQQQ
void f_prep_MVD()
{
 // Выставляет запрос на включение насоса.
 // Если получено подтверждение - разрешает счет расходомера
 // Если получена ошибка при прокачке - прерывает отпуск

 int itmp;
 time_beg_fill=TimeStamp;


 if(flag_prn)
 {
  if(FL_UP_sw_c != st_MVD_sw)
  {
        printf("\n\rst_MVD_sw=%d",st_MVD_sw);
        FL_UP_sw_c=st_MVD_sw;
  }

//  if(MVD_fn[GR] != 0) printf("\n\r MVD_fn[GR] %d", MVD_fn[GR]);
 }



  switch (st_MVD_sw)
  {
    case 0:     // начало включения насоса
      // обнуление Mass_Total и Volume_Total

           flag_rcv=0;
           t_prep_MVD=TimeStamp;
           t_prep_MVD1=TimeStamp;

   // запуск отпуска без открывания клапанов
    vol2=0;
    VolT_int=0;
    VolT_id=0;
    VolT_id1=0;
    VolT_id1_1=0;
    // запретить открывание клапанов 1,2
    Out_en=~( OUT1 | OUT2 );

    // закрыть клапан 'Плотность'
//    i_CL_dens = 0;

//    f_enable_cnt();
    st_MVD_sw=1;
//  printf("=1=");
    break;

    case 1: // ожидание разрешения выставления запроса

          itmp= f_chk_flup();
          switch(itmp)
          {
           case 0:     // можно выставлять новый запрос
           case 1:     // насос включается другой колонкой
           case 10:    // насос включен другой колонкой

              f_set_RQ(1);
              st_MVD_sw=2;
              if( (sw_mmi != 0) || (PageD !=0))
               {
                 PageD=0;
                 sw_mmi=0;
                 sw_fst=0;
                 f_paged0();
               }
               t_prep_MVD1=TimeStamp;
              return;

           case -1: // предыдущие запросы не сняты

//         printf("=X=");
              f_set_RQ(0);
              if( f_timer(t_prep_MVD1, Tim_Prep2 ) != 0 )
              {
                goto m_err;
              }
              return;
          }
          break;

    case 2:
       // ожидание подтверждения включения насоса


          if( ( INP_VARi & INP1 ) == 0)
          {   // исчез рычаг при старте отпуска
          //???
          //   f_icp_errS(No_liq_err);
            goto m_err1; // останов запуска
          }
          itmp= f_chk_flup();
          switch(itmp)
          {
           case 0: // включение в процессе
           case 1: // включение в процессе
              t_prep_MVD1=TimeStamp;

              if( f_timer(t_prep_MVD, (Tim_Prep2+Tim_Prep1 ) ) != 0 )
              {
               goto m_err;
              }

              return;

           case 10: // насос включился

              if( f_timer(t_prep_MVD1, Tim_Prep_ND ) != 0 )
              {
               st_MVD_sw=3; // переход к разрешению счета расходомера
               t_prep_MVD1=TimeStamp;
               t_prep_MVD=TimeStamp;
//  printf("=ok=");
              }
              return;

           case -1: // ошибка при прокачке
m_err:
              f_icp_errS(No_liq_err);
              st_MVD_sw=-1;
m_err1:
              Modef &= ~pmp1_dlvr_bit;
              VolT_id   = 0;
              MassT_id=0;
              summ_v_id=0;

              p1_state |= End_dlvr;

//  printf("=V=");
              ff_serv_N=NULL;
              f_set_RQ(0);
              return;
          }
        break;

    case 3:// разрешение счета расходомера

      // обнуление Mass_Total и Volume_Total


          if(MVD_fn[GR] != 0)
          {
            if( f_timer(t_prep_MVD,(long int)5000L ) == 0 ) return;
            f_icp_errS(No_F_dat_err);
            st_MVD_sw=60;
            goto m_err1;
          }


          flag_rcv=0;

           MVD_fn[GR]=MVD_WR_C;
           s_MVD[GR].r_addr=3; // Reset all totalizers
           s_MVD[GR].i=1;

          t_prep_MVD=TimeStamp;
          st_MVD_sw=4;
          break;

    case 4:

          if(MVD_fn[GR] != 0)
          {
            if( f_timer(t_prep_MVD,(long int)5000L ) == 0 ) return;
            f_icp_errS(No_F_dat_err);
            st_MVD_sw=60;
            goto m_err1;
          }
       // в расходомере жидкая фаза,
       // установка значения среза массового расхода для режима измерений

          MVD_fn[GR]=MVD_WR_F;
          s_MVD[GR].r_addr=195; // Cuttoff for mass flow
          s_MVD[GR].f=mass_cut_wrk;
          st_MVD_sw=5;
          t_prep_MVD=TimeStamp;
          break;

    case 5:
          if(MVD_fn[GR] != 0)
          {
            if( f_timer(t_prep_MVD,(long int)5000L ) == 0 ) return;
            f_icp_errS(No_F_dat_err);
            st_MVD_sw=60;
            goto m_err1;
          }
       // установка значения среза объемного расхода для режима измерений
          MVD_fn[GR]=MVD_WR_F;
          s_MVD[GR].r_addr=197; // Cuttoff for volume flow
          s_MVD[GR].f=volm_cut_wrk;
          t_prep_MVD=TimeStamp;
          st_MVD_sw=6;
          break;

    case 6:

          if(MVD_fn[GR] != 0)
          {
            if( f_timer(t_prep_MVD,(long int)5000L ) == 0 ) return;
            f_icp_errS(No_F_dat_err);
            st_MVD_sw=60;
            goto m_err1;
          }

      VolT_id=0;
      VolT_id1=VolT_id;
      vol2=0;
      k_vol=0;
//      VolT_int=0;

      f_enable_cnt();    // ZZZZ
       // Start totalizers
            MVD_fn[GR]=MVD_WR_C;
            s_MVD[GR].r_addr=2; // Start totalizers
            s_MVD[GR].i=1;
            t_prep_MVD=TimeStamp;
            st_MVD_sw=7;
            break;
    case 7:
      // завершение подготовки
    // разрешить открывание клапанов 1,2 и насоса

    Out_en=( OUT1 | OUT2 );
    st_MVD_sw=10;

    beg_flch=0;
    stage_max=0;

    ff_serv_N=NULL;
    break;

    default:break;
   }
 }
//-----------------------------------------
/*
void f_close_MVD()
{
  if((MVD_fn[0] != 0) && (MVD_fn[0] != F_FAULT))
  {
      if( f_timer(t_prep_MVD,(long int)2000L ) == 0 ) return;
      f_icp_error(&MVD[0],WR_ERR );
      return;
  }
  switch (cl_MVD_sw)
   {
    case 0:

       // установка значения среза массового расхода для режима ожидания
        MVD_fn[0]=MVD_WR_F;
        s_MVD[0].r_addr=195; // Cuttoff for mass flow
        s_MVD[0].f=dens_cutoff;
        cl_MVD_sw=1;
        t_prep_MVD=TimeStamp;
        break;

    case 1:
       // установка значения среза объемного расхода для режима ожидания
        MVD_fn[0]=MVD_WR_F;
        s_MVD[0].r_addr=197; // Cuttoff for volume flow
        s_MVD[0].f=dens_cutoff1;
        cl_MVD_sw=10;
        ff_serv_N=NULL;
        break;

    default:break;
   }
}
*/
//-----------------------------------------
//----------------------------------------------
unsigned int f_get_int( char *buf)
{
unsigned int itmp;
        itmp=(buf[4] & 0xff) | ( ((unsigned int)buf[3] << 8) &0xff00)  ;
        return itmp;
}
//-----------------------------------------
float f_get_float( char *buf, int cod)
{
 union  { float f; unsigned long l; } o;

     switch(cod)  // ( Reg 0521 )
     {
     case 0:
    //  code == 0 : 0  1  2  3
    //             [3][4][5][6]
     o.l=
         ( (unsigned long)buf[6]  &0xff) |                // 3
         ( ((unsigned long)buf[5] << 8) &0xff00) |        // 2
         ( ((unsigned long)buf[4] << 16) &0xff0000)  |    // 1
         ( ((unsigned long)buf[3] << 24) &0xff000000);    // 0
        break;
     case 1:
    //  code == 1 : 2  3  0  1
    //             [3][4][5][6]
     o.l=
         (  (unsigned long)buf[4]  &  0x000000ff) |        // 3
         ( ((unsigned long)buf[3] << 8) &  0x0000ff00) |   // 2
         ( ((unsigned long)buf[6] << 16) & 0x00ff0000)  |  // 1
         ( ((unsigned long)buf[5] << 24) & 0xff000000);    // 0
         break;
      case 2:
    //  code == 2 : 1  0  3  2
    //             [3][4][5][6]

     o.l=
         ( (unsigned long)buf[5]  &0xff) |              // 3
         ( ((unsigned long)buf[6] << 8) &0xff00) |      // 2
         ( ((unsigned long)buf[3] << 16) &0xff0000)  |  // 1
         ( ((unsigned long)buf[4] << 24) &0xff000000);  // 0
         break;
      case 3:
    //  code == 3 : 3  2  1  0
    //             [3][4][5][6]
     o.l=( (unsigned long)(buf[3]) & 0x000000ff) |      // 3
         ( ((unsigned long)buf[4] << 8) &0xff00) |      // 2
         ( ((unsigned long)buf[5] << 16) &0xff0000)  |  // 1
         ( ((unsigned long)buf[6] << 24) &0xff000000);  // 0
       break;
       default : return 0.;
     }

  if(( o.l == 0x7fa00000) || ( o.l == 0x00007fa0))
     o.f= BIG_P;
  return o.f;

}
//-----------------------------------------
//-----------------------------------------
void f_set_float( char *buf,float value, int cod)
{
 union  { float f; unsigned long l; } o;

    o.f=value;
    if(( o.l == 0x7fa00000) || ( o.l == 0x00007fa0))
       o.f=BIG_P;

     switch(cod)  // ( Reg 0521 )
     {
     case 0:  //  msb        lsb
    //  code == 0 : 3  2  1  0
    //             [0][1][2][3]
         buf[3] = o.l & 0xff;
         buf[2] = (o.l >> 8) & 0xff;
         buf[1] = (o.l >> 16) & 0xff;
         buf[0] = (o.l >> 24) & 0xff;

        break;
     case 1:
   default :
    //  code == 1 : 2  0  3  2
    //        buf  [0][1][2][3]
         buf[1] = o.l & 0xff;
         buf[0] = (o.l >> 8) & 0xff;
         buf[3] = (o.l >> 16) & 0xff;
         buf[2] = (o.l >> 24) & 0xff;

         break;
      case 2:
    //  code == 2 : 2  3  0  1
    //             [0][1][2][3]

         buf[2] = o.l & 0xff;
         buf[3] = (o.l >> 8) & 0xff;
         buf[0] = (o.l >> 16) & 0xff;
         buf[1] = (o.l >> 24) & 0xff;
         break;
      case 3:
    //  code == 3 : 0  1  2  3
    //             [0][1][2][3]

         buf[0] = o.l & 0xff;
         buf[1] = (o.l >> 8) & 0xff;
         buf[2] = (o.l >> 16) & 0xff;
         buf[3] = (o.l >> 24) & 0xff;

       break;
     }
}
//-----------------------------------------
void f_set_long( char *buf,long int value)
{
         buf[3] = value & 0xff;
         buf[2] = (value >> 8) & 0xff;
         buf[1] = (value >> 16) & 0xff;
         buf[0] = (value >> 24) & 0xff;
}
//-----------------------------------------
void f_set_int( char *buf, int value)
{
         buf[1] = value & 0xff;
         buf[0] = (value >> 8) & 0xff;
}
//-----------------------------------------
//-----------------------------------------
int ModeFill=2;
long int tim_snd_MVD;
void f_MVD_rd(int ii)
{
float ftmp;
// ii - номер порта
// заносит принятые данные в переменные
//
 struct COM_rqst *request;
 int i,nn,fn,i1,im;
 int count,tmp;

 request=COM_que[ii].request[COM_que[ii].empt_ptr];
 nn=request->n_dev; // номер устройства MVD (0,1..)
 fn=request->function;

 count=n_bcom[ii];  // длина команды
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
     s_MVD[nn].exc=cb_COM[ii][2];
     MVD_fn[nn]=F_FAULT ;
     goto m_end;
   }
 if(( fn & 0xf8)==MVD_RD_P) // ответ на команду циклического опроса
  {
      fn &= 0x7;
      switch(  fn )
      {
      case mErr:  // чтение регистра ошибки , int
    //в ответ на 01 03 01 a2 00 01 приходит:
    //  01 03 02 00 05 78 47
        fl_rd_err=1;
        s_MVD[nn].reg_err=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8) &0xff00)  ;
        if(s_MVD[nn].reg_err & 0x8000)
        {
          if( f_get_icp_error(&MVD[nn]) != 0 ) break;
          f_icp_error(&MVD[nn],Drive_ERR | ( s_MVD[nn].reg_err & 0xffff) );
        }
        else flag_mvd_fst[nn]=0;

        break;
      case mVolT:     // чтение Vol total
        s_MVD[nn].VolT=  f_get_float( cb_COM[ii], 1);

        summ_v=s_MVD[nn].VolT*cost_v;
        {
         tim_snd_MVD=Tm_snd[ii];
         f_prep_int_v2();
        }
        break;

      case mDens:    // чтение Density
//     s_MVD[nn].Dens = f_get_float( cb_COM[ii], 1);
       ftmp = f_get_float( cb_COM[ii], 1);
       if(ftmp <=0) ftmp=0.01;
       s_MVD[nn].Dens = ftmp;


         tim_snd_MVD=Tm_snd[ii];
//         f_prep_int_dens();
         break;
/*
      case mFlowM:    // чтение mass Flow
        s_MVD[nn].FlowM=  f_get_float( cb_COM[ii], 1);
         break;
*/
      case mSlot:    // чтение Slot data

       if(count < 33)
       {
         goto m_tout_err;
       }
        s_MVD[nn].MassT= f_get_float( &cb_COM[ii][0], 1);
//      s_MVD[nn].Dens=  f_get_float( &cb_COM[ii][4], 1);
        ftmp = f_get_float( &cb_COM[ii][4], 1);
        if(ftmp <=0.0001) ftmp=0.001;
        s_MVD[nn].Dens = ftmp;

        s_MVD[nn].Temp=  f_get_float( &cb_COM[ii][8], 1);
        s_MVD[nn].MassI=  f_get_float( &cb_COM[ii][12], 1);
        s_MVD[nn].VolI =  f_get_float( &cb_COM[ii][16], 1);
//      s_MVD[nn].FlowV =  f_get_float( &cb_COM[ii][16], 1); // 9,10
        s_MVD[nn].FlowM=  f_get_float( &cb_COM[ii][20], 1);
//      if(s_MVD[nn].Dens > 0.002) s_MVD[nn].FlowV=s_MVD[nn].FlowM/(s_MVD[nn].Dens/1000.);
        if(s_MVD[nn].Dens > 0.002) s_MVD[nn].FlowV=s_MVD[nn].FlowM/(s_MVD[nn].Dens);
        s_MVD[nn].VolT =  f_get_float( &cb_COM[ii][24], 1);
        s_MVD[nn].reg_err=  f_get_int( &cb_COM[ii][28]);

//      s_MVD[nn].VolT =  s_MVD[nn].VolI - VolI_beg;
        tim_snd_MVD=Tm_snd[ii];
      if(nn==0)
      {
        //рассчитывает падение давления вследствие скорости
        s_MVD[0].PressB= FctBern*s_MVD[0].FlowM*s_MVD[0].FlowM/s_MVD[0].Dens;

        //рассчитывает s_MVD[0].Compos и s_MVD[0].PressG
        f_get_pressDT(s_MVD[0].Dens,s_MVD[0].Temp);

//        if(flag_rcv==0);
//        else
           f_prep_int_rcv();

        fl_rd_err=1;

        fl_MI=1;
        fl_VI=1;

        f_chk_stg();  // для MVD[0]

      }
      else if(nn==2)
      {
       if(MVD_FLOW == 2)
       {
        //рассчитывает падение давления вследствие скорости
          s_MVD[2].PressB= FctBern*s_MVD[2].FlowM*s_MVD[2].FlowM/s_MVD[2].Dens;

        //рассчитывает s_MVD[2].Compos и s_MVD[2].PressG
          f_get_pressDT(s_MVD[2].Dens,s_MVD[2].Temp);
       }

//    if(flag_rcv==0)

          f_prep_int_v2();

//        f_prep_int_dens();

        fl_rd_err=1;

        fl_MI=1;
        fl_VI=1;

        f_flow_chk(); // для MVD[2]
      }
      else if(nn==1)
      {
        ftmp=s_MVD[0].Dens;
        if(ftmp < 450 ) ftmp=540.;
        s_MVD[1].VolTd=s_MVD[1].MassT*1000./ftmp;
      }
        if(s_MVD[nn].reg_err & 0x8000)
        {
          if( f_get_icp_error(&MVD[nn]) != 0 ) break;
//        f_icp_error(&MVD[nn],Drive_ERR | ( s_MVD[nn].reg_err & 0xffff) );
          f_icp_error(&MVD[nn],Drive_ERR);
        }
        else flag_mvd_fst[nn]=0;
        break;

      case mMassT:    // чтение mass total
        s_MVD[nn].MassT=  f_get_float( cb_COM[ii], 1);
        tim_snd_MVD=Tm_snd[ii];
        f_prep_int_Mass();
         break;
//    case mFlowV:    // чтение volume Flow
//      s_MVD[nn].FlowV=  f_get_float( cb_COM[ii], 1);
//       break;
      case mTemp:    // чтение volume Flow
        s_MVD[nn].Temp=  f_get_float( cb_COM[ii], 1);
         break;
      case mVolI:    // чтение Vol inv-ry
        s_MVD[nn].VolI=  f_get_float( cb_COM[ii], 1);
        if(nn == 0)
            fl_VI=1;
         break;
      case mMassI:    // чтение Mass inv-ry
        s_MVD[nn].MassI=  f_get_float( cb_COM[ii], 1);
        if(nn == 0)
           fl_MI=1;
     // fl_VI=1;
         break;
      default:
           //      goto m_tout_err;
               goto m_crc_err;
      }
  }
 else
  switch(fn)
 {
   case MVD_RD_I: // чтение Int
     s_MVD[nn].i=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8) &0xff00)  ;
     MVD_fn[nn]=0;
     break;

  case MVD_RD_F: // чтение Float

    if(cb_COM[ii][1] != 3)
    {
     goto m_crc_err;
    }
    if(cb_COM[ii][2] != 4)
    {
       goto m_crc_err;
    }
    s_MVD[nn].f = f_get_float( cb_COM[ii], 1);
    MVD_fn[nn]=0;
    break;

  case MVD_RD_L: // чтение Long
    if(cb_COM[ii][1] != 3)
    {
     goto m_crc_err;
    }
    if(cb_COM[ii][2] != 4)
    {
       goto m_crc_err;
    }
    s_MVD[nn].l =
          ( (unsigned long)(cb_COM[ii][6]) & 0x000000ff) |
          ( ((unsigned long)cb_COM[ii][5] << 8) &0xff00) |
          ( ((unsigned long)cb_COM[ii][4] << 16) &0xff0000)  |
          ( ((unsigned long)cb_COM[ii][3] << 24) &0xff000000);
    MVD_fn[nn]=0;
    break;
  case  MVD_RD_C: // чтение Coil

     if(cb_COM[ii][1] != 1) goto m_err;
     im=cb_COM[ii][2];
     i1=3;
     for(i=0;(i<im)&(i<n_coil_b) ;i++,i1++)
     {
       s_MVD[nn].c[i]=cb_COM[ii][i1]&0xff;
     }
     MVD_fn[nn]=0;
     break;

  case  MVD_RD_S: // чтение String

     if(cb_COM[ii][1] != 3) goto m_err;
     im=cb_COM[ii][2];
     i1=3;
     for(i=0;(i<im)&&(i< (n_char_b-1)) ;i++,i1++)
     {
      s_MVD[nn].a[i]=cb_COM[ii][i1];
     }
     s_MVD[nn].a[i]=0;
     MVD_fn[nn]=0;
     break;
  default:
    // ответ на команду записи
    if ((cb_COM[ii][1] == 6)  || (cb_COM[ii][1] == 16) ||(cb_COM[ii][1] == 5));
    else
    {
      goto m_crc_err;
    }
    MVD_fn[nn]=0;
    break;
 }

 m_end:
  MVD[nn].n_success++;
  MVD_inp_err[nn]=0;
  f_queue_free(ii,&MVD_Rqst[nn]);
  return;

m_crc_err:
 MVD[nn].time_stamp_rcv=0;
 MVD[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 MVD[nn].time_stamp_rcv=0;
 MVD[nn].n_timeout_error++;

m_err:
 MVD_inp_err[nn]++;

 if (fn>0)
  if(( fn & 0xf8) != MVD_RD_P)
 {
// printf("\r\n if(( fn & ~0x7) != MVD_RD_P)  %d , %d",( fn & ~0x7),MVD_RD_P);
     MVD_fn[nn]=fn;
 }
 if(MVD_inp_err[nn]>=MVD_max_inp_err)
 {
   MVD_inp_err[nn]=MVD_max_inp_err;
   f_icp_error(&MVD[nn],RD_ERR );
 }
 f_queue_free(ii,&MVD_Rqst[nn]);
}
/* ---------------------------------------- */

char *unit_lst[]={
// Pressure codes I0044
/*
"Inch water",
"Inch Hg",
"Ft water",
"mm water",
"mm Hg",
"pnd/inch2",
"bar",
"mlbar",
"g/cm2",
"kg/cm2",
"Pa",
"kPa",
"Torr",
"Atm",
*/
// Volume flow codes I0042
//"ft3/min",
//"gal_US/min",
//"l/min",
//"gal_i/min",
//"m3/hour",
//"gal_US/sec",
//"Mega_gal_US/day",
  "l/sec",
//"Mega_l/day",
//"ft3/sec",
//"ft3/day",
//"m3/sec",
//"m3/day",
//"gal_i/hour",
//"gal_i/day",
//"ft3/hour",
//"m3/min",
//"brl/sec",
//"brl/min",
//"brl/hour",
//"brl/day",

//"gal_US/hour",
//"gal_i/sec",
  "l/hour",
//"gal_US/day",
// Temperature codes I0041
"C",
"F",
"R",
"K",
// Volume totalizer or volume inventory codes I 0046
//"gal_US",
  "l",
//"gal_i",
  "m3",
//"brl",
//"ft3",
// Mass totalizer or mass inventory unit codes I 0045
//"g",
  "kg",
//"ton_m",
//"pnd",
//"ton_s",
//"ton_l",
// I039 Mass flow codes
//"g/sec",
//"g/min",
//"g/hour",
  "kg/sec",
  "kg/min",
  "kg/hour",
  "kg/day",
//"ton_m/min",
//"ton_m/hour",
//"ton_m/day",
//"pnd/sec",
//"pnd/min",
//"pnd/hour",
//"pnd/day",

//"ton_s/min",
//"ton_s/hour",
//"ton_s/day",
//"ton_l/hour",
//"ton_l/day",

// Density codes I0040
//"sgu",
  "g/cm3",
  "kg/m3",
//"pnd/gal_US",
//"pnd/ft3",
//"g/ml",
//"kg/l",
//"g/l",
//"pnd/inch3",
//"ton_s/yard3",
//"DegBaume_h",
//"DegBaume_l",
//"DegAPI",

"Special",

};

char *f_get_unit(int code)
{ // возвращает указатель на наименование единиц
int i;
  for(i=0;unit_codes[i]>0;i++)
  {
   if(unit_codes[i]== code) return unit_lst[i];
  }
  return NULL;
}
/* ---------------------------------------- */

int unit_codes[]={
// Pressure codes I0044
/*
1,
2,
3,
4,
5,
6,
7,
8,
9,
10,
11,
12,
13,
14,
*/
// Volume flow codes I0042
//15,
//16,
//17,
//18,
//19,
//22,
//23,
  24,
//25,
//26,
//27,
//28,
//29,
//30,
//31,
//130,
//131,
//132,
//133,
//134,
//135,

//136,
//137,
  138,
//235,
//Temperature codes I0041
32,
33,
34,
35,
// Volume totalizer or volume inventory codes I 0046
//40,
  41,
//42,
  43,
//46,
//112,
// Mass totalizer or mass inventory unit codes I 0045
//60,
  61,
//62,
//63,
//64,
//65,
// I039 Mass flow codes
//70,
//71,
//72,
  73,
  74,
  75,
  76,
//77,
//78,
//79,
//80,
//81,
//82,
//83,

//84,
//85,
//86,
//87,
//88,

// Density codes I0040
//90,
  91,
  92,
//93,
//94,
//95,
//96,
//97,
//98,
//99,
//102,
//103,
//104,

253,
-1
};
/* ---------------------------------------- */
/* ---------------------------------------- */
int sw_mvd_1=0;
long int tm_mvd;
long int tm_mvd_p;
int mode_mvd=0;
int ii_MVD=0;

void f_MVD_1()
{
// отображает параметры MVD
  if((MVD_fn[ii_MVD] != 0) && (MVD_fn[ii_MVD] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
        ff_serv=NULL;
        printf("\n\r  ! Answer timeout happend");
        f_prompt();
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_1)
  {
  case 0:
   MVD_fn[ii_MVD]=MVD_RD_L;
   s_MVD[ii_MVD].r_addr=127;
   sw_mvd_1=1;
   break;

  case 1:
  if(MVD_fn[ii_MVD] != F_FAULT)
    printf("\n\r Serial number %ld",s_MVD[ii_MVD].l);

   MVD_fn[ii_MVD]=MVD_RD_S;
   s_MVD[ii_MVD].r_addr=425;
   s_MVD[ii_MVD].n=8;
   sw_mvd_1++;
   break;

  case 2:
  if(MVD_fn[ii_MVD] != F_FAULT)
    printf("\n\r Sensor description: %s",s_MVD[ii_MVD].a);
   MVD_fn[ii_MVD]=MVD_RD_S;
   s_MVD[ii_MVD].n=8;
   s_MVD[ii_MVD].r_addr=96;
   sw_mvd_1++;
   break;

  case 3:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Tramsmitter description: %s",s_MVD[ii_MVD].a);
   MVD_fn[ii_MVD]=MVD_RD_S;
   s_MVD[ii_MVD].r_addr=104;
   s_MVD[ii_MVD].n=16;
   sw_mvd_1++;
   break;

  case 4:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Message: %s",s_MVD[ii_MVD].a);
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr=16;
   sw_mvd_1++;
   break;

  case 5:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Transmitter software revision : %d",s_MVD[ii_MVD].i);
   MVD_fn[ii_MVD]=MVD_RD_S;
   s_MVD[ii_MVD].r_addr=72;
   s_MVD[ii_MVD].n=8;
   sw_mvd_1++;
   break;

  case 6:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Flow calibration: %s",s_MVD[ii_MVD].a);
   MVD_fn[ii_MVD]=MVD_RD_S;
   s_MVD[ii_MVD].r_addr=80;
   s_MVD[ii_MVD].n=7;
   sw_mvd_1++;
   break;

  case 7:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Temp.calibration: %s",s_MVD[ii_MVD].a);
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=155;
   sw_mvd_1++;
   break;

  case 8:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r D1 F155: %.3f",s_MVD[ii_MVD].f);
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=157;
   sw_mvd_1++;
   break;

  case 9:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r D2 F157: %.3f",s_MVD[ii_MVD].f);
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=159;
   sw_mvd_1++;
   break;

  case 10:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r K1 F159: %.3f",s_MVD[ii_MVD].f);
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=161;
   sw_mvd_1++;
   break;

  case 11:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r K2 F161: %.3f",s_MVD[ii_MVD].f);
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=279;
   sw_mvd_1++;
   break;

  case 12:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Mass flow meter factor    F279: %.6f",s_MVD[ii_MVD].f);
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=281;
   sw_mvd_1++;
   break;

  case 13:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Volume flow meter factor  F281: %.6f",s_MVD[ii_MVD].f);
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=283;
   sw_mvd_1++;
   break;

  case 14:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Density flow meter factor F283: %.6f",s_MVD[ii_MVD].f);
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=195;
   sw_mvd_1++;
   break;

  case 15:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Cutoff for mass flow   F195: %.6f",s_MVD[ii_MVD].f);
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=197;
   sw_mvd_1++;
   break;

  case 16:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Cutoff for volume flow F197: %.6f",s_MVD[ii_MVD].f);
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=149;
   sw_mvd_1++;
   break;

  case 17:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Cutoff for density     F149: %.6f",s_MVD[ii_MVD].f);
   ff_serv=NULL;
   f_prompt();
   break;
  }

}
/* ---------------------------------------- */
int sw_MMI_cur=0; // начальная строка на дисплее MMI
int sw_MMI_lim=7;
int sw_mvd_m1=0;
/* ---------------------------------------- */
/* ---------------------------------------- */
int u_cod;
float HTemp,HDens,HCompos;
void f_MVD_2()
{
 float ftmp;
char *unit;
// отображает данные MVD
  if((MVD_fn[ii_MVD] != 0) && (MVD_fn[ii_MVD] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
        ff_serv=NULL;
        printf("\n\r  ! Answer timeout happend");
        f_prompt();
    }
    return;
  }
  tm_mvd=TimeStamp;
  ftmp=BIG_P;
  switch(sw_mvd_1)
  {
  case 0:
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr=39; // I039 Mass flow codes
   s_MVD[ii_MVD].i=-1;
   sw_mvd_1=1;
   break;

  case 1:
   u_cod=s_MVD[ii_MVD].i;
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=247;
   sw_mvd_1++;
   break;

  case 2:
  if((MVD_fn[ii_MVD] != F_FAULT)&& (u_cod>0))
  {
    unit=f_get_unit(u_cod);
   if(unit != NULL)
     printf("\n\r Mass flow %f %s",s_MVD[ii_MVD].f,unit);
  }

   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr=45; // Mass totalizer or mass inventory unit codes I 0045
   s_MVD[ii_MVD].i=-1;
   sw_mvd_1++;
   break;

  case 3:
   u_cod=s_MVD[ii_MVD].i;
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=259;
   sw_mvd_1++;
   break;

  case 4:
  if((MVD_fn[ii_MVD] != F_FAULT)&& (u_cod>0))
  {
    unit=f_get_unit(u_cod);
   if(unit != NULL)
     printf("\n\r Mass total: %f %s",s_MVD[ii_MVD].f,unit);
  }
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=263;
   sw_mvd_1++;
   break;

  case 5:
  if((MVD_fn[ii_MVD] != F_FAULT)&& (u_cod>0))
  {
    unit=f_get_unit(u_cod);
   if(unit != NULL)
     printf("\n\r Mass inventory : %f %s",s_MVD[ii_MVD].f,unit);
  }
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr= 42 ; // Volume flow codes I0042
   s_MVD[ii_MVD].i=-1;
   sw_mvd_1++;
   break;

  case 6:
   u_cod=s_MVD[ii_MVD].i;

   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=253;
   sw_mvd_1++;
   break;

  case 7:
  if((MVD_fn[ii_MVD] != F_FAULT)&& (u_cod>0))
  {
    unit=f_get_unit(u_cod);
   if(unit != NULL)
     printf("\n\r Volume flow: %f %s",s_MVD[ii_MVD].f,unit);
  }
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr= 46 ; // Volume totalizer or volume inventory codes I 0046
   s_MVD[ii_MVD].i=-1;
   sw_mvd_1++;
   break;

  case 8:
   u_cod=s_MVD[ii_MVD].i;
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=261;
   sw_mvd_1++;
   break;

  case 9:
  if((MVD_fn[ii_MVD] != F_FAULT)&& (u_cod>0))
  {
    unit=f_get_unit(u_cod);
   if(unit != NULL)
     printf("\n\r Volume total : %f %s",s_MVD[ii_MVD].f,unit);
  }
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=265;
   sw_mvd_1++;
   break;

  case 10:
  if((MVD_fn[ii_MVD] != F_FAULT)&& (u_cod>0))
  {
    unit=f_get_unit(u_cod);
   if(unit != NULL)
     printf("\n\r Volume inventory: %f %s",s_MVD[ii_MVD].f,unit);
  }
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr= 40 ; // Density codes I0040
   s_MVD[ii_MVD].i=-1;
   sw_mvd_1++;
   break;

  case 11:
   u_cod=s_MVD[ii_MVD].i;

   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=249;
   sw_mvd_1++;
   break;

  case 12:
  if((MVD_fn[ii_MVD] != F_FAULT)&& (u_cod>0))
  {
    unit=f_get_unit(u_cod);
   if(unit != NULL)
   {
     printf("\n\r Density: %f %s",s_MVD[ii_MVD].f,unit);
//   HDens=s_MVD[ii_MVD].f*K_unit;
     HDens=s_MVD[ii_MVD].f;
   }
  }
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr= 41 ; // Temperature codes I0041
   s_MVD[ii_MVD].i=-1;
   sw_mvd_1++;
   break;

  case 13:
   u_cod=s_MVD[ii_MVD].i;
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=251;
   sw_mvd_1++;
   break;

  case 14:
  if((MVD_fn[ii_MVD] != F_FAULT)&& (u_cod>0))
  {
    unit=f_get_unit(u_cod);
   if(unit != NULL)
   {
     printf("\n\r Process temperature %f %s",s_MVD[ii_MVD].f,unit);
     HTemp=s_MVD[ii_MVD].f;
    if((HDens>0)&&(HTemp>-300))
    {
     HCompos=f_get_composition(HDens,HTemp);
     if(HCompos>1.)
      printf("\r\n !СУГ легче жидкого пропана");
     else if(HCompos<0.)
      printf("\r\n !СУГ тяжелее жидкого бутана");
     else
      {
       ftmp=(1.- HCompos)*100.;
       printf("\r\n СУГ: пропан %8.2f%c, бутан %8.2f%c",(HCompos*100),'%',ftmp,'%');
      }
    }
   }
  }

   sw_mvd_1+=2;
/*
   goto m16;
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr= 44 ; // Pressure codes I0044
   s_MVD[ii_MVD].i=-1;
   sw_mvd_1++;
   break;

  case 15:
   u_cod=s_MVD[ii_MVD].i;
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=257;
   sw_mvd_1++;
   break;

  case 16:
  if((MVD_fn[ii_MVD] != F_FAULT)&& (u_cod>0))
  {
    unit=f_get_unit(u_cod);
   if(unit != NULL)
   {
     printf("\n\r Pressure : ");
     if(s_MVD[ii_MVD].f == ftmp)
      printf(" ! bad process variable");
     else printf(" %f %s",s_MVD[ii_MVD].f,unit);
   }
  }

m16:
*/
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=383;
   sw_mvd_1++;
   break;

  case 17:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Board temperature: %.3f C",s_MVD[ii_MVD].f);
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=385;
   sw_mvd_1++;
   break;

  case 18:
   if(MVD_fn[ii_MVD] != F_FAULT)
     printf("\n\r Supply voltage: %.3f V",s_MVD[ii_MVD].f);
   ff_serv=NULL;
   f_prompt();
   break;
  }

}
/* ---------------------------------------- */
void f_MVD_3()
{
int i;
long int ltmp,i1;
// отображает ошибки MVD
  if((MVD_fn[ii_MVD] != 0) && (MVD_fn[ii_MVD] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
        ff_serv=NULL;
        printf("\n\r  ! Answer timeout happend");
        f_prompt();
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_1)
  {
  case 100:
   MVD_fn[ii_MVD]=MVD_RD_C;
   s_MVD[ii_MVD].r_addr=21;
   s_MVD[ii_MVD].n=16;
   sw_mvd_1=1;
   break;

  case 101:
  if(MVD_fn[ii_MVD] != F_FAULT)
    printf("\n\r Coils 21 ... 34: %02X%02X",s_MVD[ii_MVD].c[1]&0xff,s_MVD[ii_MVD].c[ii_MVD]&0xff);

   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr=1;
   sw_mvd_1++;
   break;

  case 2:
  if(MVD_fn[ii_MVD] != F_FAULT)
    printf("\n\r INT REG 001: %04X",s_MVD[ii_MVD].i);
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr=125;
   sw_mvd_1++;
   break;

  case 3:
   if(MVD_fn[ii_MVD] != F_FAULT)
    printf("\n\r INT REG 125: %04X",s_MVD[ii_MVD].i);
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr=126;
   sw_mvd_1++;
   break;

  case 4:
   if(MVD_fn[ii_MVD] != F_FAULT)
    printf("\n\r INT REG 126: %04X",s_MVD[ii_MVD].i);
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr=400;
   sw_mvd_1++;
   break;

  case 5:
   if(MVD_fn[ii_MVD] != F_FAULT)
    printf("\n\r INT REG 400: %04X",s_MVD[ii_MVD].i);
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr=419;
   sw_mvd_1++;
   break;

  case 6:
   if(MVD_fn[ii_MVD] != F_FAULT)
    printf("\n\r INT REG 419: %04X",s_MVD[ii_MVD].i);
    if(s_MVD[ii_MVD].i & 0x8000)
     printf("\n\r !!! High severity alarm(s) active");
    else
     printf("\n\r  No high severity alarm active");
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr=420;
   sw_mvd_1++;
   break;

  case 7:
   if(MVD_fn[ii_MVD] != F_FAULT)
    printf("\n\r INT REG 420: %04X",s_MVD[ii_MVD].i);
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr=421;
   sw_mvd_1++;
   break;

  case 8:
   if(MVD_fn[ii_MVD] != F_FAULT)
    printf("\n\r INT REG 421: %04X",s_MVD[ii_MVD].i);
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr=422;
   sw_mvd_1++;
   break;

  case 9:
   if(MVD_fn[ii_MVD] != F_FAULT)
    printf("\n\r INT REG 422: %04X",s_MVD[ii_MVD].i);
   MVD_fn[ii_MVD]=MVD_RD_I;
   s_MVD[ii_MVD].r_addr=423;
   sw_mvd_1++;
   break;

  case 10:
   if(MVD_fn[ii_MVD] != F_FAULT)
    printf("\n\r INT REG 423: %04X",s_MVD[ii_MVD].i);

  case 0:
   MVD_fn[ii_MVD]=MVD_RD_F;
   s_MVD[ii_MVD].r_addr=245;
   sw_mvd_1++;
   break;

  case 1:
   if(MVD_fn[ii_MVD] != F_FAULT)
   {
     ltmp=(long int)s_MVD[ii_MVD].f &0xffffff;
     printf("\n\r Float REG 245: %.0f (%06lX)",s_MVD[ii_MVD].f,ltmp);
  /*
     if(ltmp)
     {
       printf("\n\r Error(s) occured:");
       for(i=0,i1=1;i<24;i++,i1=i1<<1)
       {
         if(ltmp & i1) printf("\n\r!%s",err_F425[i]);
       }
     }
     else printf("  No errors");
   */
   }
   ff_serv=NULL;
   f_prompt();
   break;
  }

}


float CL_val[3]={0.5,0.1,0};
float CL_valR[3]={0.5,0.1,0};


// 25-30 л/мин = 500 мл/сек
//     5 л/мин =  83 мл/сек

int t_ctrl=0;
int fl_GO=0;
int fl_take_temp=0;
/*
void  f_ctrl_v1()
{ // непосредственное  управление клапанами
float ftmp;
int itmp;

  itmp=0;
  ftmp=  vol_f-s_MVD[0].VolT;

 if(fl_GO == 1)
 {
   if( ftmp > CL_val[0]) itmp |= OUT1;
   if( ftmp > CL_val[1]) itmp |= OUT2;
   if(itmp == 0) fl_GO=0;
 }

 i7060_out[0]=itmp;
}
*/
/* ---------------------------------------- */
float vol2=0,k_vol=1.;
long int tt_vol1=0;
long int tt_vol2=0;

long int tim_MVD_delay=35;  // ms , время задержки чтения MVD
float last_incr=0;
void  f_prep_int_v2()
{ // подготовка параметров интерполяции volume total
  // индикация и отпуск по объему
float ftmp,ftmp1;
int itmp;
long int ltmp;
  ftmp=-s_MVD[GR].VolT;
  summ_v=ftmp*cost_v;
  ltmp=tim_snd_MVD-tt_vol1;
 if(ltmp>0)
 {
  ftmp1=ftmp-vol2;
  last_incr=ftmp1;
  k_vol=ftmp1/(float)ltmp;
  vol2=ftmp;
  tt_vol2=tim_snd_MVD+tim_MVD_delay;
  ltmp=TimeStamp-tt_vol2;
  if( ( ltmp > 5000 ) ||  ( ltmp < 0 ))  tt_vol2=TimeStamp;
 }
 else
 {
  last_incr=0;
  k_vol=0;
  vol2=ftmp;
  tt_vol2=tim_snd_MVD;
 }
 tt_vol1= tim_snd_MVD;

}
/* ---------------------------------------- */
float  f_int_v2()
{ // экстраполяция volume total во времени
  return (vol2+k_vol*(TimeStamp-tt_vol2));
}
/* ---------------------------------------- */
float VolT_int=0;
float summ_v_int=0;

float VolT_id=0;
float summ_v_id=0;

float VolT_id1=0;
float summ_v_id1=0;


float CL_T1=100 ; // ms
float CL_T2=150 ; // ms
float CL_T3=250 ; // ms
long int CL_T4=350 ; // ms
float CL_T1R=100 ; // ms
float CL_T2R=150 ; // ms

/* ---------------------------------------- */
long int tim_snd_MVDT=0;
float MVDT1=0;
float k_Test=0.0001;
void f_sim_MVD()
{
 //float k2;
long int ltmp;
//  if( f_timer(tim_snd_MVD,MVD[0].pool_time ))
  if( f_timer(tim_snd_MVD,300L))
  {
     ltmp=TimeStamp;
   if(fl_GO == 1)
     s_MVD[0].VolT=(float)(ltmp-tim_snd_MVDT)*k_Test;
     tim_snd_MVD=ltmp;
    fl_VI=1;
    fl_MI=1;
  }

// s_MVD[0].VolI+=s_MVD[0].VolT-MVDT1;
   summ_v=s_MVD[0].VolT*cost_v;
   MVDT1=s_MVD[0].VolT;
// s_MVD[0].MassI=s_MVD[0].VolI*0.85;
   f_prep_int_v2();
}
/* ---------------------------------------- */
//-------------------------------
int f_reset_zer_MVD(int ii)
{ // обнуление MVD (усреднение)
// Return 1  - OK, 0 - error
/*
   if(MVD_fn[0] != 0) return 0;
   MVD_fn[0]=MVD_WR_C;
   s_MVD[0].r_addr=5; // Start/stop zero
   s_MVD[0].i=1;
   return 1;
*/
 MVD_Rqst_T[ii].answ_com=f_MVD_rd_t;
 MVD_Rqst_T[ii].answ_flt=f_MVD_rd_t;
 return  f_MVD_WR(ii,(int)MVD_WR_C,(int) 5,(int)1,(long int) 0,(float)0);
}
//-------------------------------
float k_forv=1.0;

int io_1=0;
int io_2=0;
unsigned long int TimeOut3=0;
unsigned long int TimeO4g=0;
int flag_O1=0;
int flag_O3=0;
int flag_O4=0;
int flag_O4g=0;
int flag_fill_ok=0;
float vol_mom=0,vol_mom0=0,flow_mom=0;
float VolT_id1_1=-1;
void  f_ctrl_v2()
{ // управление клапанами по экстраполированному значению
float ftmp;
int itmp,itmp1;
unsigned long int ltmp;

if((sw_dns<=0) && (sw_dlv_liq <=0) && (flag_rcv == 0) )
{
//========== режим ГРК =============

  // при отпуске расходомер считает в -
  // изменение знака происходит в зависимости от ModeFill в:
  // f_prep_int_v2(),f_prep_int_Mass(),f_prep_int_MassF()

  VolT_int= f_int_v2();
  summ_v_int=VolT_int*cost_v;
  ftmp=  vol_f + rnd_add - VolT_int;

// в ftmp сколько литров осталось

 itmp=0;
 if(fl_GO == 1)
 {
  if(FL_err)
  {
    flag_O3=0;
    flag_O4=0;
    flag_O4g=0;
    f_stop_cnt();
    goto m_end;
  }
  TimeOut3=TimeStamp;
  if(flag_O1 != 0)
  {
/*
 if(flag_prn)
   printf(",");
*/
    if( (ftmp-k_vol*CL_T1 )> CL_val[0]) itmp |= OUT1;
    else
      flag_O1=0;
  }
  else
  {
/*
 if(flag_prn)
   printf(".");
*/
   flag_O1=0;
   flag_pool=0;
   ltmp=TimeStamp+CL_T4;
// I7060[0].time_stamp_pool=ltmp;
// I7060[1].time_stamp_pool=ltmp;
   I7017C[0].time_stamp_pool=ltmp;
// if(f_chk_flup() == 10  ) LIn[0].time_stamp_pool=ltmp;
   MMI.time_stamp_pool=ltmp;
// WDOG[0].time_stamp_pool=ltmp;
  }

  if( (ftmp-k_vol*CL_T2 )> CL_val[1]) itmp |= OUT2;

/*
  if(flag_pool==1)
   if( (ftmp-k_vol*CL_T3 )<= 0)
    {
     flag_pool=0;
     ltmp=TimeStamp+CL_T4;
     I7060[0].time_stamp_pool=ltmp;
     I7060[1].time_stamp_pool=ltmp;
     I7017C[0].time_stamp_pool=ltmp+CL_T4;
     LIn[0].time_stamp_pool=ltmp;
     MMI.time_stamp_pool=ltmp+CL_T4;
//   WDOG[0].time_stamp_pool=ltmp;
    }

*/
  if(itmp == 0)
  {// завершение по достижении заданного объема
    vol_mom=VolT_int;
    vol_mom0=vol2;
    flow_mom=k_vol;

    flag_fill_ok=1;
    f_stop_cnt();

    VolT_id=vol_f;
    summ_v_id=VolT_id*cost_v;
    VolT_id1=VolT_id;
    summ_v_id1=summ_v_id;

  }

//if(flag_O1)
  {
    f_density();
    itmp &= i_dens;
  }
 }
 if(flag_O3)  // управление насосом
  if( f_timer(TimeOut3,Out3_delay_off ))
  {
   flag_O3=0;
   f_set_RQ(0);
  }
//else  itmp |= OUT3;

 m_end:

//  if( Modef & pmp1_dlvr_bit)
  if( flag_nal )
  {
     if(VolT_id< VolT_int)
             VolT_id= VolT_int;
     if(VolT_id>vol_f)  VolT_id=vol_f;
     summ_v_id=VolT_id*cost_v;

//  VolT_id1=VolT_id+last_incr*k_forv;
    VolT_id1=VolT_id;
    if(VolT_id1>vol_f)  VolT_id1=vol_f;
     summ_v_id1=VolT_id1*cost_v;
  }


 OUT_VAR &= ~( OUT1 | OUT2 );
 OUT_VAR |= (itmp & Out_en);
// OUT_VAR |= i_LVL;
 f_set_o67();
 if(dnc_mode==0)
 {
   OUT_VAR &= ~(  OUT5 | OUT8 | OUT9 | OUT10 | OUT11 );
 }
 else
   OUT_VAR &= ~(  OUT5 | OUT10 | OUT11 );

//======= конец режима  ГРК ==========
}
else if(sw_dlv_liq > 0)
{
//=======  Прием СУГ  =============================

 itmp = f_ctrl_rcv();

// OUT_VAR &= ~(  OUT5 | OUT6 | OUT7 | OUT8 | OUT9 | OUT10 | OUT11 );
 OUT_VAR &= ~(  OUT5 | OUT8 | OUT9 | OUT10 | OUT11 );

 if(Flag_check == 0)
   OUT_VAR |= (itmp & Out_en & Out_en_r);

// OUT_VAR |= i_LVL;
 f_set_o67();

 OUT_VAR &= ~( OUT1 | OUT2 | OUT3 | OUT4);

//======= конец режима: Прием СУГ  ================
}

 m1:

 itmp=OUT_VAR | OUT_VAR_a;
/*
 i7060_out[0] = OUT_VAR & 0xff;
 i7060_out[1] = (OUT_VAR>>8) & 0xff;
*/

 i7060_out[0] = itmp & 0xff;
 i7060_out[1] = (itmp>>8) & 0xff;


 if(I7060[0].status)  f_7060(0);
 if(I7060[1].status)  f_7060(1);

 ServiceCOM(I7060[0].port);
 ServiceCOM(I7060[1].port);

 return;
}
/* ---------------------------------------- */
int flag_pool_c=-1;
float ex_p_d=0.10;

void f_dsp_out()
{
 if(flag_prn)
 {
   if(io_1!=OUT_VAR)
   {
    printf("\r\nOut  = %04lx,%ld ms",(long int)(OUT_VAR) & 0xffff,TimeStamp-time_beg_fill);
   }
   if(io_2 != OUT_VAR_c_r)
   {
    printf("\r\nOut_c= %04lx,%ld ms",(long int)(OUT_VAR_c_r) & 0xffff,TimeStamp-time_beg_fill);
   }
   if(flag_pool != flag_pool_c)
   {
    printf("\r\npool= %d,%ld ms",flag_pool,TimeStamp-time_beg_fill);
   }

   if( (io_1 != OUT_VAR) || (io_2 != OUT_VAR_c_r) )
   {
     f_wr_dat_st();
   }

    io_1 = OUT_VAR;
    io_2 = OUT_VAR_c_r;
    flag_pool_c = flag_pool;
 }
 else
  if( (io_1 != OUT_VAR) || (io_2 != OUT_VAR_c_r) )
  {
    f_wr_dat_st();
    io_1 = OUT_VAR;
    io_2 = OUT_VAR_c_r;
  }
}

// ----------------------------------------
// #define deb_s_lgth  (10) see mvd.h
struct deb_str deb_s[deb_s_lgth];
int debs_ptr=0;
long int time_debs=0;

void f_wr_dat_st()
{
 struct deb_str *a_str;

 if(debs_ptr <  deb_s_lgth);
 else debs_ptr=deb_s_lgth-1;

 a_str = &deb_s[debs_ptr];

 a_str->time=TimeStamp - time_debs;
 a_str->out_var     = OUT_VAR;
 a_str->out_var_c_r = OUT_VAR_c_r;
 a_str->VolT        = s_MVD[GR].VolT;
 a_str->MassT       = s_MVD[GR].MassT;
 a_str->FlowM       = s_MVD[GR].FlowM;
 a_str->P           = s_MVD[0].Press;
 debs_ptr++;
}
// ----------------------------------------
int i_deb_s=0;
void  f_wr_deb()
{ // записывает данные о процессе в журнал
 if(ex_p_d==0) return;
 for( i_deb_s=0; i_deb_s < debs_ptr; i_deb_s++ )
 {
    f_wr_evt(evt_deb);
 }
}
// ----------------------------------------
float Den2=0,k_Den=1.;
long int tt_Dens1=0;
long int tt_Dens2=0;
void  f_prep_int_dens()
{ // подготовка параметров интерполяции density
float ftmp;
int itmp;
unsigned long int ltmp;
 ftmp=s_MVD[0].Dens;
 ltmp=tim_snd_MVD-tt_Dens1;
 if(ltmp>0)
 {
  k_Den=(ftmp-Den2)*1000./(float)ltmp;
  Den2=ftmp;
  tt_Dens2=tim_snd_MVD+tim_MVD_delay;
  tt_Dens1= tim_snd_MVD;
 }
}
/* ---------------------------------------- */
float Dens0=1000.;
float Mas2=0,k_Mas=1.;
long int tt_Mas1=0;
long int tt_Mas2=0;

void  f_prep_int_Mass()
{ // подготовка параметров интерполяции Mass
  // индикация объема, объем вычисляется через отпущенную
  //  массу и плотность
float ftmp;
int itmp;
unsigned long int ltmp;
 ftmp=-s_MVD[GR].MassT;

 s_MVD[GR].VolTd=ftmp*1000./Dens0;
 summ_v=s_MVD[GR].VolTd*cost_v;

 ltmp=tim_snd_MVD-tt_Mas1;
 if(ltmp>0)
 {
  k_vol=((ftmp-Mas2)*1000.)/((float)ltmp * Dens0);
  Mas2=ftmp;
  vol2=(ftmp*1000.)/Dens0;
  tt_vol2=tim_snd_MVD+tim_MVD_delay;
  tt_Mas1= tim_snd_MVD;
 }
}
/* ---------------------------------------- */

void  f_prep_int_MassF()
{ // подготовка параметров интерполяции Mass
  // индикация массы, отпуск по массе
float ftmp;
int itmp;
unsigned long int ltmp;
 ftmp=-s_MVD[GR].MassT;

 summ_v=ftmp*cost_v;

 ltmp=tim_snd_MVD-tt_Mas1;
 if(ltmp>0)
 {
  k_vol=(ftmp-Mas2)/(float)ltmp;
  Mas2=ftmp;
  vol2=ftmp;
  tt_vol2=tim_snd_MVD+tim_MVD_delay;
  tt_Mas1= tim_snd_MVD;
 }
}
/* ---------------------------------------- */

float Mas2r=0,k_Masr=1.;
long int tt_Mas1r=0;
long int tt_Mas2r=0;

float vol2r=0,k_volr=1.;
long int tt_vol1r=0;
long int tt_vol2r=0;

void f_prep_int_rcv()
{ // подготовка параметров интерполяции для приема
  // используются показания двух расходомеров
float ftmp;
int itmp;
long int ltmp;

 ftmp=s_MVD[0].MassT + s_MVD[1].MassT;

// summ_v=ftmp*cost_v;

 ltmp=tim_snd_MVD-tt_Mas1r;
 if(ltmp>0)
 {
  k_volr=(ftmp-Mas2r)/(float)ltmp;
  Mas2r=ftmp;
  vol2r=ftmp;
  tt_vol2r=tim_snd_MVD+tim_MVD_delay;
  ltmp=TimeStamp-tt_vol2r;
  if( ( ltmp > 5000 ) ||  ( ltmp < 0 ))  tt_vol2r=TimeStamp;
  tt_Mas1r= tim_snd_MVD;
 }
}
/* ---------------------------------------- */
float  f_int_v2r()
{ // экстраполяция volume total во времени
  return (vol2r+k_volr*(TimeStamp-tt_vol2r));
}
/* ---------------------------------------- */


long int time_beg_fill=0;
int i_dens=0xffff;
long int time_chg_den=0,tim_dens=0;

long int TimDensOff   =500; // мс,минимальное время нахождения клапана в выкл.состоянии
long int TimDensOn    =200; // мс,минимальное время нахождения клапана во вкл.состоянии
long int cl1_delay_off=1000;
long int cl2_delay_off=500;
long int Out3_delay_off=4000;

long int cl1_delayR_off=1000;
long int cl2_delayR_off=500;
long int Out3_delayR_off=2000;

// Out4 - нормальнооткрытый клапан

long int Out4_delay_on=1000;   // время после выключения отсечного клапана,
                               // через которое клапан 3 закроет выходной канал

long int Out4_delay_off=300000;// время после выключения отсечного клапана,
                               // через которое клапан 3 откроет выходной канал
                               // для предотвращения гидростатического разрыва
                               // замкнутой полости в шланге

long int Out4_dl_off_gun=300;  // время после выключения рычага,
                               // через которое клапан 3 откроет выходной канал
                               // для предотвращения гидростатического разрыва
                               // замкнутой полости в шланге

float    k_Den_Off    = 0.0001;
float    k_Den_On     = 0.00001;


void f_density()
{// функция анализирует состояние плотности при отпуске и формирует маску выключения
 // клапана CL1: i_dens=~OUT1; при необходимости уменьшения расхода отпуска
 float ftmp;

  i_dens = 0;

  if( f_timer(time_beg_fill,cl2_delay_off) ==0 )
  { // запретить включение CL2 первые cl21_delay_off мс после начала наполнения
//  i_dens &= ~OUT2 ;
    time_chg_den=time_beg_fill-Tim_prb_plus ;
    time_antd_m=time_chg_den;
    time_antd_p=time_chg_den;
    time_prb_plus=time_chg_den;
    beg_flch=0;

    if( f_timer(time_beg_fill,150) !=0 )
      if( fabs(s_MVD[GR].FlowM) > F_Dns_off_DLVs)
      {
          f_icp_errS(DNS_not_off);
      }
  }
  else
       i_dens |=  OUT2;

  if( f_timer(time_beg_fill,cl1_delay_off) ==0 )
  { // запретить включение CL1 первые cl1_delay_off мс после начала наполнения
//    i_dens &=~OUT1;
  }
  else
       i_dens |=  OUT1;
/*
    ftmp = s_MVD[0].Press - s_MVD[0].PressG - s_MVD[0].PressB;
    s_MVD[0].PressD=ftmp;

    if( ftmp < PressLvl_off0)
    {
       f_prb_minus();
       if( f_timer(time_beg_fill,150) !=0 )
         if(fabs(s_MVD[0].VolT) > 2.0)
         {
            flag_flch=1;
            f_stop_cnt(); // останов отпуска по переполнению бака
         }
    }
    else if( ftmp > PressLvl_on0 )
          f_prb_plus();
    else
    {
      time_antd_p=TimeStamp;
      time_antd_m=TimeStamp;
    }

     if(stage_max==0)
       i_dens=0;              // закрыть оба клапана
     else if(stage_max==1)
       i_dens &= ~(OUT1) ;  // закрыть больший клапан
*/
}
/* ---------------------------------------- */
void f_prep_bern()
{
 if(R_MVD>=0)
   FctBern=ConstBern/(R_MVD*R_MVD*R_MVD*R_MVD);
 else
   FctBern=0;
}
//---------------------
/* ---------------------------------------- */
void f_stop_cnt()
{
//printf("\r\n f_stop_cnt() ");
  if(fl_GO)
  {
   cl_MVD_sw=0;
   ff_serv_N=f_MVD_cor;
   tm_mvd_p=TimeStamp;
   i_CL_dens=0;
  }
  else
  {
   if(  ff_serv_N != f_MVD_cor)
       Modef &= ~pmp1_dlvr_bit;
  }

  i_CL_dens=0;
  fl_GO=0;
  flag_pool=1;
  fl_take_temp=0;
}
/* ---------------------------------------- */
void f_enable_cnt()
{
//printf("\r\n f_enable_cnt()");
  uk_buf=0;
  if(FL_err)
  {
    f_wr_evt(evt_wrong_start);
    return;
  }

  flag_nal=1;
  fl_GO=1;

  stage_max=0;

 flag_0=1;
 flag_prok=0;
  flag_pool=1;
  flag_O1=1;
  flag_O3=1;
  flag_O4=1;
  flag_O4g=0;

  fl_take_temp=0;

  beg_flch=0;
  flag_flch=0;
  flag_gun=0;
  WD_PMP_Fl=0;
  flag_StopPmp1=0;
  flag_fill_ok=0;
  flag_end_dlvr=0;
  f_wr_evt(evt_start_f);

 // установить флаг налива в NVRAM
 WriteNVRAM((int) nr_flg_n ,(int)NAL_B);

 //  Modef |= pmp1_dlvr_bit;

  time_beg_fill=TimeStamp;
}
/* ---------------------------------------- */
void f_start_MVD()
{ // запускается по получению команды отпуска из Pumalan
float ftmp;
int itmp;
//  if( (i7060_inp[0] ^ mski_inp1 ) & INP1)
  {
          flag_rcv=0;
          ftmp=f_get_dens();

          Dens0=ftmp;
          st_MVD_sw=0;
          debs_ptr=0;
          time_debs=TimeStamp;
//printf("\n\r f_prep_MVD");
          t_prep_MVD=TimeStamp;
          ff_serv_N=f_prep_MVD;
  }
}
/* ---------------------------------------- */
int  fl_VI=0, fl_MI=0;
long int t_mvd_cor=3500;
int flag_nal=0;
long int t_end_dlvr=0;
int flag_round=0;

float round_summ=0;
float rnd_add=0;
float rnd_lim=0.012;

float MassT_id=0;
float round_summ_m=0;
float rnd_add_m=0;
float rnd_lim_m=0.001; // kg

#define Off_Cor_Min (-0.05)
#define Off_Cor_Max  (0.5 )
#define Off_Cor_Dlt  (0.08 )

void f_MVD_cor()
{
  // запускается после окончания налива
  // через время t_mvd_cor мс состояние процесса записывается в журнал
int i;
long int ltmp,i1;
float ftmp;


  if( f_timer(tm_mvd_p,(t_mvd_cor-1000) ) == 0 )
  {
   fl_VI=0;
   fl_MI=0;
   cl_MVD_sw=0;
   t_prep_MVD=TimeStamp;
   return;
  }
  if(cl_MVD_sw == 10) goto m00;

  if((MVD_fn[GR] != 0) && (MVD_fn[GR] != F_FAULT))
  {
      if( f_timer(t_prep_MVD,(long int)2000L ) == 0 ) return;
      f_icp_errS(No_F_dat_err);

//  printf("=P="); //WWWW
      return;
  }
  switch (cl_MVD_sw)
   {
    case 0:

       if( fabs(s_MVD[GR].FlowM) > F_Dns_off_DLV)
             f_icp_errS(DNS_not_off);

       // // Stop totalizers
        MVD_fn[GR]=MVD_WR_C;
        s_MVD[GR].r_addr=2; // Stop totalizers
        s_MVD[GR].i=0;

        cl_MVD_sw=1;
        t_prep_MVD=TimeStamp;
//        printf("\n\r0");
        return;

    case 1:
       // установка значения среза объемного расхода для режима ожидания
        MVD_fn[GR]=MVD_WR_F;
        s_MVD[GR].r_addr=197; // Cuttoff for volume flow
        s_MVD[GR].f=dens_cutoff1;

        t_prep_MVD=TimeStamp;
//        printf("\n\r1");
        cl_MVD_sw=2;
        return;
    case 2:

       // установка значения среза массового расхода для режима ожидания
        MVD_fn[GR]=MVD_WR_F;
        s_MVD[GR].r_addr=195; // Cuttoff for mass flow
        s_MVD[GR].f=dens_cutoff;

        t_prep_MVD=TimeStamp;
//        printf("\n\r1");
        cl_MVD_sw=3;
        return;

    case 3:
//        printf("\n\r2");
        cl_MVD_sw=10;
        fl_VI=0;
        fl_MI=0;
        return;

    default:
        return;
   }

m00:
  if( f_timer(tm_mvd_p,t_mvd_cor )==0 )
  {
   fl_VI=0;
   fl_MI=0;
   return;
  }

  if((fl_VI==0) || (fl_MI==0))
  {
//    if( f_timer(tm_mvd_p,(t_mvd_cor*3))==0 )
    if( f_timer(tm_mvd_p,(MVD[GR].pool_time*24))==0 )
       return;
    else
     {
//     f_icp_error(&MVD[GR],RD_ERR );
       f_icp_errS(No_F_dat_err);

//  printf("=Q="); //WWWW
       goto m1;
     }
  }

m1:
//printf("\r\n f_MVD_cor() ");
  if(flag_fill_ok)
  {

     ftmp=-s_MVD[GR].VolT-vol_f-rnd_add;

     if(flag_StopPmp1)
       f_wr_evt(evt_ep1_f);
     else if(ftmp > ex_p)
      {
//     f_icp_errS(exp_err);
       f_wr_evt(evt_exp_f);
       f_wr_deb();
       goto m11;
      }
     else if(ftmp < (-ex_m) )
      {
//     f_icp_errS(exm_err);
       f_wr_evt(evt_exm_f);
       f_wr_deb();
       goto m11;
      }
     else
       f_wr_evt(evt_end_f);

     if((ftmp > ex_p_d) || (ftmp < (-ex_p_d) ))
      {
       f_wr_deb();
      }
     else if(flag_prok != 0)
       f_wr_deb();

  }
  else
  {
     if(FL_err)
     {
        f_wr_evt(evt_err_f);
        f_wr_deb();
        flag_prok=0;
     }
     else if(flag_flch)
        f_wr_evt(evt_overfl);
     else if(flag_gun)
        f_wr_evt(evt_gun);
     else if(WD_PMP_Fl)
        f_wr_evt(evt_wd_pmp);
     else if(flag_StopPmp1)
        f_wr_evt(evt_ep1_f);
     else
        f_wr_evt(evt_est_f);

 m11:
       VolT_id=-s_MVD[GR].VolT;
  /*
       switch(ModeFill)
       {
       case F_Vol:
            VolT_id=-s_MVD[GR].VolT;
            break;
       case F_Vol_Dens:
            VolT_id=s_MVD[GR].VolTd;
            break;
       case F_Mass:
            VolT_id=-s_MVD[GR].MassT;
            break;
       default:
            VolT_id=s_MVD[GR].VolTd;
            break;
       }
  */

    if(flag_prok != 0)
       f_wr_deb();

  }

     VolT_id=((long int)(VolT_id * 100.+0.5));
     VolT_id *= 0.01;

     rnd_add_m = round_summ_m;

     if(rnd_add_m > rnd_lim_m) rnd_add_m = rnd_lim_m;
     else if(rnd_add_m < -rnd_lim_m) rnd_add_m = -rnd_lim_m;

     MassT_id=-s_MVD[GR].MassT+rnd_add_m;
     MassT_id=((long int)(MassT_id * 1000.+0.5));
     MassT_id *= 0.001;
     round_summ_m  -= MassT_id+s_MVD[GR].MassT;

 //    VolT_id будет послано в отчете
 // Если счетчик колонки > отчета в PC , round_summ < 0

       switch(ModeFill)
       {
       case F_Vol:
//          round_summ+=VolT_id-s_MVD[GR].VolT;
            round_summ+=VolT_id+s_MVD[GR].VolT;
            break;
       case F_Vol_Dens:
            round_summ+=VolT_id+s_MVD[GR].VolTd;
            break;
       case F_Mass:
//          round_summ+=VolT_id-s_MVD[GR].MassT;
            round_summ+=VolT_id+s_MVD[GR].MassT;
            break;
       default:
            round_summ+=VolT_id-s_MVD[GR].VolTd;
            break;
       }

     summ_v_id=VolT_id*cost_v;

     summ_v_id=f_round(summ_v_id);

     VolT_id1=VolT_id;
     summ_v_id1=summ_v_id;

     Modef &= ~pmp1_dlvr_bit;
     p1_state |= End_dlvr;

     flag_end_dlvr=1;

     t_end_dlvr=TimeStamp;

     flag_nal=0;
     offs_cor=(vol2 - vol_f - rnd_add);
//   if(flow_mom > 0) dsc_tmp=offs_cor/flow_mom;
//   else
         dsc_tmp=0;

 // вычисление rnd_add для следующего отпуска
   rnd_add =round_summ;

   if(rnd_add > rnd_lim) rnd_add = rnd_lim;
   else if(rnd_add < -rnd_lim) rnd_add = -rnd_lim;

// корректирует параметры наполнения
  if(fct_offs==0)
     {
m_end:
          ff_serv_N=NULL;
          return;
     }
     if(flag_fill_ok != 1) goto m_end;
     if(FL_err)   goto m_end;
     if(flag_flch+flag_gun+WD_PMP_Fl+flag_StopPmp1) goto m_end;
     if(flow_mom < flow_min_c) goto m_end;


 // 09.01.12
     if( fabs(offs_cor) > Off_Cor_Dlt) goto m_end;

     if(fct_offs != 0)
       CL_val[1] += fct_offs*offs_cor;
//   if(fct_delay != 0)
//     CL_T2 += fct_delay*dsc_tmp;

 // 09.01.12
     if(CL_val[1] < Off_Cor_Min) CL_val[1]=0;
     if(CL_val[1] > Off_Cor_Max) CL_val[1]=0;

     ff_serv_N=NULL;
     f_wr_cor();
}
/* ---------------------------------------- */
long int t_MVD_flg;
void f_MVD_cor_rcv()
{
  // запускается после окончания отпуска
  // через время t_mvd_cor мс состояние процесса записывается в журнал
int i;
long int ltmp,i1;
float ftmp,ftmp1,ftmp2;

//  if( f_timer(tm_mvd_p,t_mvd_cor-1000 )==0 )
  if( f_timer(tm_mvd_p,100 )==0 )
  {
   fl_VI=0;
   fl_MI=0;
   cl_MVD_sw=0;
   t_prep_MVD=TimeStamp;
   return;
  }


//  if(t_ctrl==0) goto m00;
  if(cl_MVD_sw == 10) goto m00;

//  if((MVD_fn[0] != 0) && (MVD_fn[0] != F_FAULT))
  if(MVD_fn[0] != 0)
  {
    if((MVD[0].status==0 ) || ( (ErrMVD1 != 0) && (Flag_check != 0) ) ) ;
    else
      {
      if( f_timer(t_prep_MVD,(long int)2000L ) == 0 ) return;
      f_icp_errS(No_F_dat_err);
      return;
      }
  }
//  if((MVD_fn[1] != 0) && (MVD_fn[1] != F_FAULT))
  if(MVD_fn[1] != 0)
  {
    if((MVD[1].status==0 ) || ( (ErrMVD2 != 0) && (Flag_check != 0) ) ) ;
    else
    {
      if( f_timer(t_prep_MVD,(long int)2000L ) == 0 ) return;
      f_icp_errS(No_F_dat_err);
      return;
    }
  }
  switch (cl_MVD_sw)
   {
       // Расходомер 1
       // Останов счета
    case 0:  // Stop totalizers


   ltmp=TimeStamp+5000;
   LIn[0].time_stamp_pool=ltmp;
   MVD[0].time_stamp_pool=ltmp;
   MVD[1].time_stamp_pool=ltmp;


            MVD_fn[0]=MVD_WR_C;
            s_MVD[0].r_addr=2; // Stop totalizers
            s_MVD[0].i=0;
            cl_MVD_sw=1;
            t_prep_MVD=TimeStamp;
            return;
    case 1:
       // Расходомер 1
       // установка значения среза массового расхода для режима ожидания
        MVD_fn[0]=MVD_WR_F;
        s_MVD[0].r_addr=195; // Cutoff for mass flow
        s_MVD[0].f=cutoff_off_M;
        cl_MVD_sw=2;
        t_prep_MVD=TimeStamp;
//        printf("\n\r0");
        return;

    case 2:
       // Расходомер 1
       // установка значения среза объемного расхода для режима ожидания
        MVD_fn[0]=MVD_WR_F;
        s_MVD[0].r_addr=197; // Cutoff for volume flow
        s_MVD[0].f=cutoff_off_V;
        t_prep_MVD=TimeStamp;
//        printf("\n\r1");
        cl_MVD_sw=50;
        return;

    case 50:
       // Расходомер 2
       // установка значения среза массового расхода для режима ожидания
        MVD_fn[1]=MVD_WR_F;
        s_MVD[1].r_addr=195; // Cutoff for mass flow
        s_MVD[1].f=cutoff_off_M;
        cl_MVD_sw=51;
        t_prep_MVD=TimeStamp;
//        printf("\n\r0");
        return;

    case 51:
       // Расходомер 2
       // установка значения среза объемного расхода для режима ожидания
        MVD_fn[1]=MVD_WR_F;
        s_MVD[1].r_addr=197; // Cutoff for volume flow
        s_MVD[1].f=cutoff_off_V;
        t_prep_MVD=TimeStamp;
//        printf("\n\r1");
        cl_MVD_sw=22;
        return;
/* ===========================================
    case 52:
       // Расходомер 1
       // установка значения коррекции массы для отпуска
        MVD_fn[0]=MVD_WR_F;
        s_MVD[0].r_addr = 279;   // Множитель массы
        s_MVD[0].f= M_fct_d[0];
        t_prep_MVD=TimeStamp;
//        printf("\n\r1");
        cl_MVD_sw=53;
        return;

    case 53:
       // Расходомер 1
       // установка значения коррекции объема для отпуска
        MVD_fn[0]=MVD_WR_F;
        s_MVD[0].r_addr = 281;   // Множитель объема
        s_MVD[0].f= V_fct_d[0];
        t_prep_MVD=TimeStamp;
//        printf("\n\r1");
        cl_MVD_sw=22;
        return;
 =================================================== */
    case 22:
//        printf("\n\r2");
        t_MVD_flg=TimeStamp;
        cl_MVD_sw=3;
        fl_VI=0;
        fl_MI=0;
        return;
    case 3:
   /*
       if( f_timer(t_MVD_flg,t_mvd_cor )==0 )
       {
        fl_VI=0;
        fl_MI=0;
        return;
       }
       if((fl_VI==0) || (fl_MI==0))
       {
       }
       else
   */

       {
 /*===============================
       // установка Mass_kd

         Mass_kd =  Mass_grk - s_MVD[0].MassI;
         Vol_kd  = Vol_grk - s_MVD[0].VolI;

           if(f_wrt_eee_kd( EE_kd )  <0 ) goto fin_e;

           ftmp1=Mass_kd;
           ftmp2=Vol_kd;
           if( f_rd_EEE_kd(EE_kd) < 0) goto fin_e;

           if(Mass_kd != ftmp1) goto fin_e;
           if(Vol_kd != ftmp2)  goto fin_e;

           Fl_grk=Fl_grk_OK;
           if(f_wrt_eee_kd( EE_flg ) <0 ) goto fin_e;
           if( f_rd_EEE_kd(EE_flg) < 0) goto fin_e;
           if(Fl_grk != Fl_grk_OK)  goto fin_e;
 =========================*/
         cl_MVD_sw=6;
       }
       return;
fin_e:
       f_icp_errS(EEE_CRC_error);
       cl_MVD_sw=-1;
       return;

    case 6:
        cl_MVD_sw=10;

   LIn[0].time_stamp_pool=TimeStamp+500;
   MVD[0].time_stamp_pool=TimeStamp;
   MVD[1].time_stamp_pool=TimeStamp+250;

//        if(ErrMVD1 == 0)
          f_wr_evt(evt_kd_grk);
        return;
    default:
        return;
   }

m00:
/*
  if( f_timer(tm_mvd_p,t_mvd_cor )==0 )
  {
   fl_VI=0;
   fl_MI=0;
   return;
  }
*/
  if((fl_VI==0) || (fl_MI==0))
  {

    if((MVD[0].status==0 ) || ( (ErrMVD1 != 0) && (Flag_check != 0) ) )  goto m1;
    if( f_timer(tm_mvd_p,t_mvd_cor*2)==0 )
//  if( f_timer(tm_mvd_p,(MVD[0].pool_time*24))==0 )
       return;
    else
     {
       f_icp_errS(No_F_dat_err);
       goto m1;
     }
  }
m1:

 Temperature=s_MVD[0].Temp;
 Density=s_MVD[0].Dens;

  if(flag_fill_ok)
  {
       f_wr_evt(evt_end_f+flag_rcv);
  }
  else
  {
     if(flag_flch)
      {
        f_wr_evt(evt_overfl+flag_rcv);
        Reg_State |= RCV_flch;
      }
     else if(flagS_ES)
        f_wr_evt(evt_ES_off+flag_rcv);
     else if(flagS_UZA)
      {
        switch(flagE_UZA)
        {
         case UZA_E:
           f_wr_evt(evt_UZA_off+flag_rcv);
           break;
  /*
         case LVL_E:
           f_wr_evt(evt_LVL_off+flag_rcv);
           break;
         case TRAP_E:
           f_wr_evt(evt_TRAP_off+flag_rcv);
           break;
         case CONS_E:
           f_wr_evt(evt_CONS_off+flag_rcv);
           break;
  */
         default:
           f_wr_evt(evt_UZA_off+flag_rcv);
           break;
        }
      }
     else if(flagS_DLT)
        f_wr_evt(evt_DLT_off+flag_rcv);
     else if(FL_err)
        f_wr_evt(evt_err_f+flag_rcv);
     else
        f_wr_evt(evt_est_f+flag_rcv);
  }

     flag_nal_r=0;
     ff_serv_N=NULL;
     flag_rcv=0;

     Rcv_Result1= s_MVD[0].MassT;
     Rcv_Result2= s_MVD[1].MassT;

     Reg_State |= RCV_fin;

     if(offs_cor == -1000 )
        offs_cor=(vol2r-dose_dlvr1);

// корректирует параметры наполнения
     if(fct_offs_r==0)
     {
m_end:
          return;
     }
    if(flag_fill_ok != 1) goto m_end;

#define max_cor_rcv 20
#define d_max_cor_rcv  20
#define min_cor_rcv -20

    if( fabs(offs_cor) > d_max_cor_rcv) return;
//     CL_valR[0] += fct_offs_r*offs_cor;  // больший
       CL_valR[1] += fct_offs_r*offs_cor;

    if(CL_valR[1] < min_cor_rcv) CL_valR[1]=0;
    if(CL_valR[1] > max_cor_rcv ) CL_valR[1]=0;

    f_wr_cor();
}
/* ---------------------------------------- */

/* ---------------------------------------- */
int igun_1=0;
int flag_fst_gun=1;

#define TimeGunInit 2000
#define TimeDelivSent 2000
int flag_0=1;

long int time_dlv_r=0;

void f_gun()
{ // проверяет состояние заправочного пистолета
  // если пистолет снят, формируется запрос на отпуск
  // если при наливе сигнал отключается - налив прерывается
long int l_itmp;

//if( (i7060_inp[0] ^ mski_inp1 ) & INP1)
  if( INP_VARi & INP1 )
  {
    Modef |= pmp1_gun_bit;
    if(igun_1 == 0)
    {
      if( (flag_nal==0) && ((p1_state & End_dlvr)==0 )  );
      else return;

      if( fl_GO != 0) return;
      if(FL_err) return;
      if(flag_0 != 0) return;

      if( ff_serv_N != NULL)
      {
m0:    // сбросить запрос на налив
        p1_state &= ~(int)(Dlvr_rqst);
        goto m1;
      }

      VolT_id=0;
      summ_v_id=0;

      VolT_id1=VolT_id;
      summ_v_id1=summ_v_id;

      p1_state |= Dlvr_rqst;
      time_dlv_r=TimeStamp;
    }
    else
    {
      if( f_timer(time_dlv_r,TimeDelivSent )== 0 );
      else
        { // установить запрос на налив

         time_dlv_r=TimeStamp;

         if( fl_GO != 0)
         {
            flag_0 = 1;
            goto m0;
         }

         if( (flag_nal==0) && ((p1_state & End_dlvr)==0 )  );
         else
         {
            flag_0 = 1;
            goto m0;
         }

         if(  FL_err != 0)
         {
            goto m0;
         }

          if(flag_0 != 0) goto m0;

          if( (Modef & pmp1_dlvr_bit) || ( p1_state & End_dlvr))  goto m0;

          p1_state |= Dlvr_rqst;

        }
    }

 m1:
    igun_1 = 1;
  }
  else
  {
    if( (flag_nal==0) && ((p1_state & End_dlvr)==0 )  )
        Modef &= ~pmp1_gun_bit;

    p1_state &= ~Dlvr_rqst;

    if(fl_GO)
    {
      flag_gun=1;
  //  f_icp_errS(Gun_off);
      flag_O4=0;
      f_stop_cnt();
    }
    else if((igun_1 == 1)&& (flag_O4 !=0) && (flag_O4g ==0)  )
    {
     flag_O4g=1;
     TimeO4g=TimeStamp;
    }
    if(flag_O4g != 0)
      if( f_timer(TimeO4g,Out4_dl_off_gun) !=0 )
      {
        flag_O4g=0;
        flag_O4=0;
      }
    igun_1 = 0;

    flag_0=0;
  }
  return;
}
//-----------------------------

float  fl_chp[max_fl_ch];
int last_flch=0;

int flag_flch=0;
int flag_gun=0;
int time_flch=20;
long int beg_flch=0;

int TimeBegFlch=15;
float val_flch=0.05; // предельное  значение потока для останова
float val_flch2=0.05; // предельное  значение потока для останова
float val0_flch=0.195; // значение для начала контроля потока

int TimeBegFlch_R=15;
int time_flch_R=20;
float val_flch_R=0.05; // предельное  значение потока для останова
float val_flch2_R=0.05; // предельное  значение потока для останова
float val0_flch_R=0.195; // значение для начала контроля потока

void  f_flow_chk()
{
// контролирует прохождение жидкости при наливе
 int itmp;
 float ftmp;
 float val_cur;

 if(flag_rcv == 0)
 {  // отпуск через колонку
   /*
       switch(ModeFill)
       {
       case F_Vol:
            val_cur=-s_MVD[2].VolT;
            break;
       case F_Vol_Dens:
            val_cur=-s_MVD[2].VolT;
            break;
       case F_Mass:
            val_cur=-s_MVD[2].MassT;
            break;
       default:
            val_cur=-s_MVD[2].VolT;
            break;
       }

   */
   val_cur=fabs(s_MVD[2].VolT);


  if(fl_GO)
  {
   last_flch=(last_flch+1) & max_fl_ch_m1;
   fl_chp[last_flch]=val_cur;
   if(beg_flch < (time_flch+TimeBegFlch))
   {  // начало налива, не проверять первые time_flch тиков

     beg_flch++;
     if((val_cur>val0_flch)&& (beg_flch > max_fl_ch )     )
     {
       beg_flch=time_flch+TimeBegFlch;
     }
   }
   else
   {
     itmp=(last_flch-time_flch) & max_fl_ch_m1;
     ftmp=val_cur-fl_chp[itmp];
//   if(OUT1 & i7060_out[0])
     if((val_flch != 0.) &&  (val_flch2 != 0.))
        if(OUT1 & OUT_VAR)
        {
          if(ftmp<val_flch)
          { // за time_flch тиков прошло < val_flch литров
            flag_flch=1;
//          f_icp_errS(No_flow);
            f_stop_cnt(); // останов отпуска по переполнению бака
          }
        }
        else
          if(ftmp<val_flch2)
          { // за time_flch тиков прошло < val_flch литров
            flag_flch=1;
//          f_icp_errS(No_flow);
            f_stop_cnt(); // останов отпуска по переполнению бака
          }
   }
  }
 }
 /*
 else
 { // прием СУГ
   f_chk_stg();
 }
 */
 return;
}
//-----------------------------
long int base_r=10;
float base_r_1=0.1;

float f_round(float value)
{
float ret_val;
long int ltmp;

     ret_val=value;
//   printf("\r\nSrc  %g ",value );

     if(flag_round == 1)
     {
       ltmp = value*base_r_1 + 0.5;
       ret_val=ltmp* base_r;
     }
     else if(flag_round == 2)
     {
       ltmp= value*base_r_1 ;
       ret_val=ltmp* base_r;
     }
     else if(flag_round == 3)
     {
       ltmp= value*base_r_1 + 0.99;
       ret_val=ltmp* base_r;
     }

//   printf("\r\nRet  %g \r\n",ret_val );
     return ret_val;
}

//----------------------------------------------
void f_pmp_OK()
{
  // разрешение счета расходомеров
}
void f_pmp_err()
{
 // ошибка - нет ЖФ, не разрешать запуск отпуска
}
void f_pmp_no_err()
{
// снять ошибку , разрешить новый запрос
}
//---------------------------
int dnc_mode=0; // 0 - для измерения плотности нужно включить OUT4
                // 1 - для измерения плотности нужно включить OUT4 и OUT9
                //     при включении OUT4 полость расходомера связывается с насосом
//---------------------------
void f_pmp_ON()
{
 // включение насоса , при этом полость расходомера ЖФ д.б.связана с выходом насоса
// if(flag_prn) printf("\n\rf_pmp_ON");
 if(dnc_mode==0)
    OUT_VAR |= OUT3;
 else
    OUT_VAR |= OUT3 | OUT4;
}
//---------------------------
void f_pmp_OFF()
{
 // выключение насоса
// if(flag_prn) printf("\n\rf_pmp_OFF");
 if(dnc_mode==0)
   OUT_VAR &= ~(OUT3  );
 else
   OUT_VAR &= ~(OUT3 | OUT4  );
}
//---------------------------
void f_den_ON()
{
 // открывание клапана прокачки (режим измерения плотности)
// if(flag_prn) printf("\n\rf_den_ON");

 if(dnc_mode==0)
   OUT_VAR |=  OUT4;
 else
   OUT_VAR |= OUT9 ;
}
//---------------------------
void f_den_OFF()
{
// if(flag_prn) printf("\n\rf_den_OFF");
 // закрывание клапана прокачки
 if(dnc_mode==0)
   OUT_VAR &= ~(OUT4 );   // клапан "Плотность"
 else
   OUT_VAR &= ~(OUT9 | OUT4 );     // клапан "Прием"
}
//----------------------------------------
void f_set_RQ( int flag)
{
//  выставляет запрос на включение насоса
 if(F_GR2==0)
 {
  if(flag)
   {
     RQ1=1;
   }
   else
   {
     RQ1=0;
   }
 }
 else
  if(flag)
   {
     RQ2W=1;
     OUT_VAR |= OUT3;
   }
   else
   {
     RQ2W=0;
     OUT_VAR &= ~OUT3;
   }
}
//----------------------------------------
int f_pmp_chRQ()
 // проверка состояния запросов на вкл.насоса , 1  - есть запрос
{
 // проверка состояния запросов на вкл.насоса , 1  - есть запрос
return (RQ1 | RQ2);

}
//----------------------------------------
int f_pmp_chRQ1()
{
 // проверка состояния запроса 1 на вкл.насоса , 1  - есть запрос
  return RQ1;
}
//----------------------------------------
int f_chk_flup()
{
 //  проверяет состояние включения насоса:
 //  1 - OK,
 //  0 - в процесса включения
 // -1 - ошибка,
 if(F_GR2)
   return PMP_sw;
 else
  return  st_PMP_sw;
}
//----------------------------------------

float Sim_MassT   = 0.0187;
float Sim_Dens    = 550;
float Sim_Temp    =  23.4;
float Sim_MassI   =  12345;
float Sim_VolI    =  23456 ;
float Sim_FlowM   = 0.04;
float Sim_VolT    = 0.023;
int   Sim_reg_err = 0;


void f_sim_MVD0()
{

#if(SIM0 != 0)
int nn=0;

 if( f_timer(MVD[0].time_stamp_pool,MVD[0].pool_time ) )
 {
        MVD[0].time_stamp_pool=TimeStamp;

        s_MVD[nn].MassT  = Sim_MassT  ;
        s_MVD[nn].Dens   = Sim_Dens   ;
        s_MVD[nn].Temp   = Sim_Temp   ;
        s_MVD[nn].MassI  = Sim_MassI  ;
        s_MVD[nn].VolI   = Sim_VolI   ;
        s_MVD[nn].FlowM  = Sim_FlowM  ;
        s_MVD[nn].VolT   = Sim_VolT   ;
        s_MVD[nn].reg_err= Sim_reg_err;

      if(s_MVD[nn].Dens > 0.00000002) s_MVD[nn].FlowV=s_MVD[nn].FlowM/(s_MVD[nn].Dens/1000.);
        tim_snd_MVD=TimeStamp;

       if(fl_GO_rcv)
       {

        Sim_MassT+=Sim_FlowM;
        Sim_MassI+=Sim_FlowM;
        Sim_VolT +=s_MVD[nn].FlowV;
        Sim_VolI +=s_MVD[nn].FlowV;
       }


        //рассчитывает падение давления вследствие скорости
        s_MVD[0].PressB= FctBern*s_MVD[0].FlowM*s_MVD[0].FlowM/s_MVD[0].Dens;

        //рассчитывает s_MVD[0].Compos и s_MVD[0].PressG
        f_get_pressDT(s_MVD[0].Dens,s_MVD[0].Temp);


        flag_mvd_fst[0]=0;

        f_prep_int_rcv();

        f_chk_stg();  // для MVD[0]

        fl_rd_err=1;
        fl_MI=1;
        fl_VI=1;
        MVD_Rqst[0].status = Req_OK;
        MVD[0].n_transaction++;
        MVD_fn[0]=0;
        MVD_fn[1]=0;
        MVD_t_rslt[0]=1;
        MVD_t_rslt[1]=1;
        MVD_t_rslt[2]=1;


 }
#endif

}
/* ---------------------------------------- */
//----------------------------------------
