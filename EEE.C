
char buf_eep[max_buf_eep];

int Fl_grk=0;

unsigned int Ee_summ_kd;
unsigned int Ee_summ_grk;
unsigned int Ee_summ_flg;

struct eee_dat EE_kd[]=
{
&Mass_kd,sizeof(Mass_kd),
&Vol_kd,sizeof(Vol_kd),
&Ee_summ_kd,sizeof(Ee_summ_kd),          //
NULL,0,
};

struct eee_dat EE_grk[]=
{
&Mass_grk,sizeof(Mass_grk),
&Vol_grk,sizeof(Vol_grk),
&Ee_summ_grk,sizeof(Ee_summ_grk),          //
NULL,0,
};
struct eee_dat EE_flg[]=
{
&Fl_grk,sizeof(Fl_grk),
&Ee_summ_flg,sizeof(Ee_summ_flg),          //
NULL,0,
};

int eee_num=0;
unsigned long int Eeprom_summ;
unsigned int ee_lgth_byte=0;
float fict_ee_float=0;
int fict_int=0;
// Список параметров сохраняемых в EEPROM
struct eee_dat EE_Dat[]=
{
&ComBaud[0],sizeof(ComBaud[0]),  //1
&ComBaud[1],sizeof(ComBaud[1]),  //2
&ComBaud[2],sizeof(ComBaud[2]),  //3
&ComBaud[3],sizeof(ComBaud[3]),  //4
&ComBaud[4],sizeof(ComBaud[4]),  //5

&eee_num,sizeof(eee_num),         //6

#if defined(ICP_7017C)

&I7017C[0].status         ,sizeof(I7017C[0].status        ), //7
&I7017C[0].port           ,sizeof(I7017C[0].port          ), //8
&I7017C[0].addr           ,sizeof(I7017C[0].addr          ), //9
&I7017C[0].type           ,sizeof(I7017C[0].type          ), //10
&I7017C[0].format         ,sizeof(I7017C[0].format        ), //11
&I7017C[0].channels       ,sizeof(I7017C[0].channels      ), //12
&I7017C[0].CRC_flag       ,sizeof(I7017C[0].CRC_flag      ), //13
&I7017C[0].pool_time      ,sizeof(I7017C[0].pool_time     ), //14
&I7017_Rqst[0].timeout    ,sizeof(I7017_Rqst[0].timeout   ), //15

&I7017C[1].status         ,sizeof(I7017C[1].status        ),   //16
&I7017C[1].port           ,sizeof(I7017C[1].port          ),   //17
&I7017C[1].addr           ,sizeof(I7017C[1].addr          ),   //18
&I7017C[1].type           ,sizeof(I7017C[1].type          ),   //19
&I7017C[1].format         ,sizeof(I7017C[1].format        ),   //20
&I7017C[1].channels       ,sizeof(I7017C[1].channels      ),   //21
&I7017C[1].CRC_flag       ,sizeof(I7017C[1].CRC_flag      ),   //22
&I7017C[1].pool_time      ,sizeof(I7017C[1].pool_time     ),   //23
&I7017_Rqst[1].timeout    ,sizeof(I7017_Rqst[1].timeout   ),   //24
#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(I7017_Rqst[0].timeout   ),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(I7017_Rqst[0].timeout   ),

#endif

&MMI.status         ,sizeof(MMI.status        ),            //25
&MMI.port           ,sizeof(MMI.port          ),            //26
&MMI.addr           ,sizeof(MMI.addr          ),            //27
&MMI.type           ,sizeof(MMI.type          ),            //28
&MMI.format         ,sizeof(MMI.format        ),            //29
&MMI.channels       ,sizeof(MMI.channels      ),            //30
&MMI.CRC_flag       ,sizeof(MMI.CRC_flag      ),            //31
&MMI.pool_time      ,sizeof(MMI.pool_time     ),            //32
&MMI_Rqst.timeout    ,sizeof(MMI_Rqst.timeout   ),          //33

&ComPortEcho          ,sizeof(ComPortEcho        ),         //34
&ToutLicpScan         ,sizeof(ToutLicpScan       ),         //35
&licp_amax            ,sizeof(licp_amax          ),         //36
&LICP_Rqst.CRC_flag   ,sizeof(LICP_Rqst.CRC_flag ),         //37

#if (1)    //defined(ICP_7060)

&I7060[0].status         ,sizeof(I7060[0].status        ),    //38
&I7060[0].port           ,sizeof(I7060[0].port          ),    //39
&I7060[0].addr           ,sizeof(I7060[0].addr          ),    //40
&I7060[0].type           ,sizeof(I7060[0].type          ),    //41
&I7060[0].format         ,sizeof(I7060[0].format        ),    //

&I7060[0].channels       ,sizeof(I7060[0].channels      ),    //

&I7060[0].CRC_flag       ,sizeof(I7060[0].CRC_flag      ),    //
&I7060[0].pool_time      ,sizeof(I7060[0].pool_time     ),    //45
&I7060_Rqst[0].timeout    ,sizeof(I7060_Rqst[0].timeout   ),  //46

&I7060[1].status         ,sizeof(I7060[1].status        ),     //47
&I7060[1].port           ,sizeof(I7060[1].port          ),     //
&I7060[1].addr           ,sizeof(I7060[1].addr          ),     //
&I7060[1].type           ,sizeof(I7060[1].type          ),     //50
&I7060[1].format         ,sizeof(I7060[1].format        ),     //
&I7060[1].channels       ,sizeof(I7060[1].channels      ),     //
&I7060[1].CRC_flag       ,sizeof(I7060[1].CRC_flag      ),     //
&I7060[1].pool_time      ,sizeof(I7060[1].pool_time     ),     //
&I7060_Rqst[1].timeout    ,sizeof(I7060_Rqst[1].timeout   ),   //55
#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(I7017_Rqst[0].timeout   ),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(I7017_Rqst[0].timeout   ),

#endif

&WDOG[0].status         ,sizeof( WDOG[0].status        ),   //56
&WDOG[0].port           ,sizeof( WDOG[0].port          ),   //
&WDOG[0].CRC_flag       ,sizeof( WDOG[0].CRC_flag      ),   //
&WDOG[0].pool_time      ,sizeof( WDOG[0].pool_time     ),   //
&WDOG_Rqst[0].timeout   ,sizeof( WDOG_Rqst[0].timeout   ),  //60

&WDOG[1].status         ,sizeof( WDOG[0].status        ),   //61
&WDOG[1].port           ,sizeof( WDOG[0].port          ),   //
&WDOG[1].CRC_flag       ,sizeof( WDOG[0].CRC_flag      ),   //
&WDOG[1].pool_time      ,sizeof( WDOG[0].pool_time     ),   //
&WDOG_Rqst[1].timeout   ,sizeof( WDOG_Rqst[0].timeout   ),  //65

&I7060_wd_E[0]          ,sizeof( I7060_wd_E[0]          ),  //66
&I7060_wd_VV[0]         ,sizeof( I7060_wd_VV[0]         ),  //
&I7060_wd_E[1]          ,sizeof( I7060_wd_E[1]          ),  //
&I7060_wd_VV[1]         ,sizeof( I7060_wd_VV[1]         ),  //69

&ComProt[0]     ,sizeof( ComProt[0] ),                       // 70
&ComProt[1]     ,sizeof( ComProt[1] ),                       //
&ComProt[2]     ,sizeof( ComProt[2] ),                       //
&ComProt[3]     ,sizeof( ComProt[3] ),                       //
&ComProt[4]     ,sizeof( ComProt[4] ),                       // 74


#if (0)   //defined(RTU_ComSK)

&ComSK[0].status         ,sizeof(ComSK[0].status        ),     //75
&ComSK[0].port           ,sizeof(ComSK[0].port          ),     //
&ComSK[0].addr           ,sizeof(ComSK[0].addr          ),     //
&ComSK[0].pool_time      ,sizeof(ComSK[0].pool_time     ),     //
&ComSK_Rqst[0].timeout   ,sizeof(ComSK_Rqst[0].timeout   ),    //
&ComSK_WD_Rqst[0].timeout, sizeof(ComSK_WD_Rqst[0].timeout),   //
&sk_pool_period[0] ,sizeof(sk_pool_period[0]),                 //81

&ComSK[1].status         ,sizeof(ComSK[1].status        ),     //82
&ComSK[1].port           ,sizeof(ComSK[1].port          ),     //
&ComSK[1].addr           ,sizeof(ComSK[1].addr          ),     //
&ComSK[1].pool_time      ,sizeof(ComSK[1].pool_time     ),     //85
&ComSK_Rqst[1].timeout   ,sizeof(ComSK_Rqst[1].timeout   ),    //
&ComSK_WD_Rqst[1].timeout, sizeof(ComSK_WD_Rqst[1].timeout),   //
&sk_pool_period[1] ,sizeof(sk_pool_period[1]),                 //88

#else

#if defined(RTU_Delta)

&Delta[0].status         ,sizeof(Delta[0].status        ),
&Delta[0].port           ,sizeof(Delta[0].port          ),
&Delta[0].addr           ,sizeof(Delta[0].addr          ),
&Delta[0].pool_time      ,sizeof(Delta[0].pool_time     ),
&Delta_Rqst[0].timeout   ,sizeof(Delta_Rqst[0].timeout   ),
&fict_long                ,sizeof(fict_long),
&fict_long                ,sizeof(fict_long),
//&Delta_WD_Rqst[0].timeout, sizeof(Delta_WD_Rqst[0].timeout),
//&Dlt_pool_period[0] ,sizeof(sk_pool_period[0]),

&Delta[1].status         ,sizeof(Delta[1].status        ),
&Delta[1].port           ,sizeof(Delta[1].port          ),
&Delta[1].addr           ,sizeof(Delta[1].addr          ),
&Delta[1].pool_time      ,sizeof(Delta[1].pool_time     ),
&Delta_Rqst[1].timeout   ,sizeof(Delta_Rqst[1].timeout   ),
&fict_long                ,sizeof(fict_long),
&fict_long                ,sizeof(fict_long),
//&Delta_WD_Rqst[1].timeout, sizeof(Delta_WD_Rqst[1].timeout),
//&Dlt_pool_period[1] ,sizeof(sk_pool_period[1]),

#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),
&fict_long                ,sizeof(fict_long),
&fict_long                ,sizeof(fict_long),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),
&fict_long            ,sizeof(fict_long),
&fict_long            ,sizeof(fict_long),

#endif
#endif

//-----------------------

&Com_lgth[0]   ,sizeof(Com_lgth[0]),       //89
&Com_lgth[1]   ,sizeof(Com_lgth[0]),       //90
&Com_lgth[2]   ,sizeof(Com_lgth[0]),       //
&Com_lgth[3]   ,sizeof(Com_lgth[0]),       //
&Com_lgth[4]   ,sizeof(Com_lgth[0]),       //
&Com_parity[0],sizeof(Com_parity[0]),      //
&Com_parity[1],sizeof(Com_parity[0]),      //95
&Com_parity[2],sizeof(Com_parity[0]),      //
&Com_parity[3],sizeof(Com_parity[0]),      //
&Com_parity[4],sizeof(Com_parity[0]),      //
&Com_stop[0]  ,sizeof(Com_stop[0]),        //
&Com_stop[1]  ,sizeof(Com_stop[0]),        //100
&Com_stop[2]  ,sizeof(Com_stop[0]),        //
&Com_stop[3]  ,sizeof(Com_stop[0]),        //
&Com_stop[4]  ,sizeof(Com_stop[0]),        //103

//-----------------------
#if  (1)   // defined(RTU_MVD)

&MVD[0].status         ,sizeof(MVD[0].status        ),   //104
&MVD[0].port           ,sizeof(MVD[0].port          ),   //105
&MVD[0].addr           ,sizeof(MVD[0].addr          ),   //106
&MVD[0].pool_time      ,sizeof(MVD[0].pool_time     ),   //107
&MVD_Rqst[0].timeout   ,sizeof(MVD_Rqst[0].timeout   ),  //108

&MVD[1].status         ,sizeof(MVD[1].status        ),   //109
&MVD[1].port           ,sizeof(MVD[1].port          ),   //110
&MVD[1].addr           ,sizeof(MVD[1].addr          ),   //111
&MVD[1].pool_time      ,sizeof(MVD[1].pool_time     ),   //
&MVD_Rqst[1].timeout   ,sizeof(MVD_Rqst[1].timeout   ),  //113

#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),

#endif
//-----------------------
#if (1)      //defined(ICP_TSD)

&TSD[0].status         ,sizeof(TSD[0].status        ),  //114
&TSD[0].port           ,sizeof(TSD[0].port          ),  //115
&TSD[0].addr           ,sizeof(TSD[0].addr          ),  //
&TSD[0].pool_time      ,sizeof(TSD[0].pool_time     ),  //
&TSD_Rqst[0].timeout   ,sizeof(TSD_Rqst[0].timeout   ), //118

#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),

#endif

//-----------------------
#if  (1)   // defined(RTU_MVD)

&MVD[2].status         ,sizeof(MVD[0].status        ),   //104
&MVD[2].port           ,sizeof(MVD[0].port          ),   //105
&MVD[2].addr           ,sizeof(MVD[0].addr          ),   //106
&MVD[2].pool_time      ,sizeof(MVD[0].pool_time     ),   //107
&MVD_Rqst[2].timeout   ,sizeof(MVD_Rqst[0].timeout   ),  //108

#endif
//-----------------------

&n_dot[0][0]    ,sizeof(n_dot ),                   //124
&CL_val[0]      ,sizeof(CL_val[0] ),               //
&CL_val[1]      ,sizeof(CL_val[1] ),               //
&TSD_vn[0][0]   ,sizeof(TSD_vn ),                  //
&byte_timout_rtu[0]   ,sizeof(byte_timout_rtu),    //
&scom_tim[0]   ,sizeof(scom_tim),                  //
&t_ctrl   ,sizeof(t_ctrl),                         //130
&tim_MVD_delay   ,sizeof(tim_MVD_delay),           //
&TSD_period   ,sizeof(TSD_period),                 //
&CL_T1   ,sizeof(CL_T1),                           //
&CL_T2   ,sizeof(CL_T2),                           //
&fct_offs   ,sizeof(fct_offs),                     //135
&fct_delay   ,sizeof(fct_delay),                   //
&t_mvd_cor   ,sizeof(t_mvd_cor),                   //
&CL_T3   ,sizeof(CL_T3),                           //
&CL_T4   ,sizeof(CL_T4),                           //139

&num_out1   ,sizeof(num_out1),                    //140
&num_out2   ,sizeof(num_out2),                    //141

&dens_cutoff ,sizeof(dens_cutoff),                 //142

&TimeMVD_Init ,sizeof(TimeMVD_Init),               //143
&TimeMVD_evt  ,sizeof(TimeMVD_evt ),               //144

&ahour00.hour ,sizeof(ahour00.hour),               //145
&ahour00.min  ,sizeof(ahour00.min),
&ahour00.sec  ,sizeof(ahour00.sec),                //147

//-----------------------
&ComBaudPmp1    ,sizeof(ComBaudPmp1   ),           //148
&Com_lgthPmp1   ,sizeof(Com_lgthPmp1  ),           //
&Com_parityPmp1 ,sizeof(Com_parityPmp1),           //
&Com_stopPmp1   ,sizeof(Com_stopPmp1  ),           //151
//-----------------------
&ComPortPmp1,sizeof(ComPortPmp1),                  //152
&P_addr     ,sizeof(P_addr  ),                     //
&flag_spec   ,sizeof(flag_spec  ),                 //154
//-----------------------
&num_out3   ,sizeof(num_out3  ),       //155
&num_out4   ,sizeof(num_out4  ),       //
&num_in1    ,sizeof(num_in1   ),       //
&num_in2    ,sizeof(num_in2   ),       //
&num_in3    ,sizeof(num_in3   ),       //
&num_in4    ,sizeof(num_in4   ),       //160
&mski_inp1  ,sizeof(mski_inp1 ),       //161
&mski_inp2  ,sizeof(mski_inp2 ),       //
&mski_inp3  ,sizeof(mski_inp3 ),       //
&mski_inp4  ,sizeof(mski_inp4 ),       //164

&flag_Serv  ,sizeof(flag_Serv ),       //165

&FlagWinSum ,sizeof(FlagWinSum ),      //166

&f_ns[0]        ,sizeof(f_ns[0]        ), //167
&f_ns[1]        ,sizeof(f_ns[1]        ), //
&f_ns[2]        ,sizeof(f_ns[2]        ), //
&f_ns[3]        ,sizeof(f_ns[3]        ), //170
&f_ns[4]        ,sizeof(f_ns[4]        ), //
&i_nn_d         ,sizeof(i_nn_d         ), //
&dim_xyz        ,sizeof(dim_xyz        ), //
&fl_trace_loop  ,sizeof(fl_trace_loop  ), //174

//-----------------------
&PermPlus       ,sizeof(PermPlus       ),  //175
&PermMinus      ,sizeof(PermMinus      ),  //
&t20c           ,sizeof(t20c           ),  //
&v_Proc         ,sizeof(v_Proc         ),  //
&flag_20C       ,sizeof(flag_20C       ),  //
&fict_ee_float  ,sizeof(fict_ee_float  ),  //180

&TimDensOff     ,sizeof(TimDensOff     ),  //181
&TimDensOn      ,sizeof(TimDensOn      ),  //
&cl1_delay_off  ,sizeof(cl1_delay_off  ),  //
&k_Den_Off      ,sizeof(k_Den_Off      ),  //
&k_Den_On       ,sizeof(k_Den_On       ),  //185

&Password       ,sizeof(Password       ),  //186

&ModeFill       ,sizeof(ModeFill      ),   //187

&un_mmi1       ,sizeof(un_mmi1      ),     //188
&un_mmi2       ,sizeof(un_mmi2      ),     //189
&un_mmi3       ,sizeof(un_mmi3      ),     //190

&ex_p       ,sizeof(ex_p     ),            //191
&ex_m       ,sizeof(ex_m     ),            //192

// cost_v
&fict_ee_float     ,sizeof(fict_ee_float  ),            //193

&WD_PMP_Per ,sizeof(WD_PMP_Per  ),         //194

&time_flch ,sizeof(time_flch  ),           //195
&val_flch  ,sizeof(val_flch   ),           //196

&cl2_delay_off  ,sizeof(cl2_delay_off  ),  //197
&Out3_delay_off  ,sizeof(Out3_delay_off),  //198

&TimeBegFlch  ,sizeof(TimeBegFlch),        //199
&val0_flch    ,sizeof(val0_flch  ),        //200

&Out4_delay_on          ,sizeof( Out4_delay_on     ),        //201
&Out4_delay_off         ,sizeof( Out4_delay_off    ),        //202
&Out4_dl_off_gun        ,sizeof( Out4_dl_off_gun   ),        //203

&en_mski        ,sizeof( en_mski),        //204
&val_flch2      ,sizeof( val_flch2),        //205
&k_forv      ,sizeof( k_forv),        //206
&flag_round  ,sizeof( flag_round),        //207
&rnd_lim  ,sizeof( rnd_lim),        //208

&fl_id1  ,sizeof( fl_id1),        //209
&chr_id[0] ,sizeof( chr_id),     //210
&fl_id  ,sizeof( fl_id),          //211
&WD_PMP_Char  ,sizeof( WD_PMP_Char),          //212

//-----------------------
&ZeroPage  ,sizeof(ZeroPage  ),
&EmptPage ,sizeof(EmptPage),
//-----------------------
&num_out5   ,sizeof(num_out5  ),       //155
&num_out6   ,sizeof(num_out6  ),       //
&num_out7   ,sizeof(num_out7  ),       //
&num_out8   ,sizeof(num_out8  ),       //
&num_out9   ,sizeof(num_out9  ),       //
&num_out10  ,sizeof(num_out10 ),       //

&analog_num[0] , sizeof(analog_num[0] ),       //
&analog_num[1]      , sizeof( analog_num[1]       ),       //

&analog_scale[0]    , sizeof( analog_scale[0]     ),       //
&analog_scale[1]    , sizeof( analog_scale[1]     ),       //

&analog_offset[0]   , sizeof( analog_offset[0]    ),       //
&analog_offset[1]   , sizeof( analog_offset[1]    ),       //

&k_p        ,sizeof(k_p      ),
&k_i        ,sizeof(k_i      ),
&k_d        ,sizeof(k_d      ),

&maxP_errP  ,sizeof(maxP_errP      ),
&maxP_errM  ,sizeof(maxP_errM      ),
&LIM_P      ,sizeof(LIM_P          ),
&LIM_M      ,sizeof(LIM_M          ),
&f_Beg      ,sizeof(f_Beg     ),
&P_dem      ,sizeof(P_dem     ),

&dens_cutoff1 ,sizeof(dens_cutoff1),                 //142

&Tim_prb_plus,    sizeof( Tim_prb_plus  ),
&Tim_prb_minus,   sizeof( Tim_prb_minus ),
&Tim_antd,        sizeof( Tim_antd      ),

&PressLvl_on0           ,        sizeof( PressLvl_on0                  ),
&PressLvl_off0          ,        sizeof( PressLvl_off0                 ),
&Temp_kor               ,        sizeof( Temp_kor                      ),
&R_MVD                  ,        sizeof( R_MVD                         ),
&Tim_prb_plus           ,        sizeof( Tim_prb_plus                  ),
&Tim_prb_minus          ,        sizeof( Tim_prb_minus                 ),
&Tim_antd               ,        sizeof( Tim_antd                      ),

&val_flch_R             ,        sizeof( val_flch_R                    ),
&val_flch2_R            ,        sizeof( val_flch2_R                   ),
&val0_flch_R            ,        sizeof( val0_flch_R                   ),
&time_flch_R            ,        sizeof( time_flch_R                   ),
&TimeBegFlch_R          ,        sizeof( TimeBegFlch_R                 ),

&CL_valR[0]             ,        sizeof( CL_valR[0]                    ),
&CL_valR[1]             ,        sizeof( CL_valR[1]                    ),
&CL_T1R                 ,        sizeof( CL_T1R                        ),
&CL_T2R                 ,        sizeof( CL_T2R                        ),

&fct_offs_r             ,        sizeof( fct_offs_r                    ),

&num_out11  ,sizeof(num_out11 ),       //
&num_out12  ,sizeof(num_out12 ),       //
&num_out13  ,sizeof(num_out13 ),       //
&num_out14  ,sizeof(num_out14 ),       //
&num_out15  ,sizeof(num_out15 ),       //
&num_out16  ,sizeof(num_out16 ),       //

&analog_num[2] , sizeof(analog_num[2] ),       //
&analog_num[3] , sizeof(analog_num[3] ),       //
&analog_num[4] , sizeof(analog_num[4] ),       //
&analog_num[5] , sizeof(analog_num[5] ),       //
&analog_num[6] , sizeof(analog_num[6] ),       //
&analog_num[7] , sizeof(analog_num[7] ),       //

&analog_scale[2]    , sizeof( analog_scale[2]     ),       //
&analog_scale[3]    , sizeof( analog_scale[3]     ),       //
&analog_scale[4]    , sizeof( analog_scale[4]     ),       //
&analog_scale[5]    , sizeof( analog_scale[5]     ),       //
&analog_scale[6]    , sizeof( analog_scale[6]     ),       //
&analog_scale[7]    , sizeof( analog_scale[7]     ),       //

&analog_offset[2]   , sizeof( analog_offset[2]    ),       //
&analog_offset[3]   , sizeof( analog_offset[3]    ),       //
&analog_offset[4]   , sizeof( analog_offset[4]    ),       //
&analog_offset[5]   , sizeof( analog_offset[5]    ),       //
&analog_offset[6]   , sizeof( analog_offset[6]    ),       //
&analog_offset[7]   , sizeof( analog_offset[7]    ),       //

//-----------------------
&cl1_delayR_off ,     sizeof( cl1_delayR_off    ),
&cl2_delayR_off ,     sizeof( cl2_delayR_off    ),
&Out3_delayR_off,     sizeof( Out3_delayR_off   ),
//-----------------------
&PressLvlR_on0    , sizeof( PressLvlR_on0  ),
&PressLvlR_off0   , sizeof( PressLvlR_off0 ),
//-----------------------
&Tim_stg_ctrl     , sizeof( Tim_stg_ctrl   ),
&Tim_stg_ctrl0    , sizeof( Tim_stg_ctrl0  ),
//-----------------------
&P_gas            , sizeof( P_gas     ),
&P_bypass         , sizeof( P_bypass ),
//-----------------------
&Tim_Prep1        , sizeof( Tim_Prep1       ),
&Tim_Prep2        , sizeof( Tim_Prep2       ),
&Tim_Prep_ND      , sizeof( Tim_Prep_ND     ),
&Tim_Prep3        , sizeof( Tim_Prep3       ),

//-----------------------
#if defined(RTU_Lnk)

&LIn[0].status         ,sizeof(LIn[0].status        ), //7
&LIn[0].port           ,sizeof(LIn[0].port          ), //8
&LIn[0].addr           ,sizeof(LIn[0].addr          ), //9
&LIn[0].pool_time      ,sizeof(LIn[0].pool_time     ), //14
&LIn_Rqst[0].timeout   ,sizeof(LIn_Rqst[0].timeout   ), //15

#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(I7017_Rqst[0].timeout   ),

#endif
//-----------------------
&T_Dns_off, sizeof( T_Dns_off ),
&T_Dns_NDB, sizeof( T_Dns_NDB ),
&F_Dns_off, sizeof( F_Dns_off ),
&F_Dns_off_DLV, sizeof( F_Dns_off_DLV ),
&F_Dns_off_DLVs, sizeof( F_Dns_off_DLVs ),
//-----------------------
&scale_cost, sizeof( scale_cost ),
&flow_min_c, sizeof( flow_min_c ),
//-----------------------
&M_fct[0]          , sizeof(M_fct[0]   ),
&V_fct[0]          , sizeof(V_fct[0]   ),
&D_fct[0]          , sizeof(D_fct[0]   ),
&M_fct_d[0]        , sizeof(M_fct_d[0] ),
&V_fct_d[0]        , sizeof(V_fct_d[0] ),
&D_fct_d[0]        , sizeof(D_fct_d[0] ),
//-----------------------
&dnc_mode   , sizeof( dnc_mode ),
&Tank_nn    , sizeof( Tank_nn  ),
//-----------------------
&F_GR2    , sizeof( F_GR2 ),
//-----------------------
&Socket_mode, sizeof( Socket_mode),
&MVD_FLOW   , sizeof( MVD_FLOW ),
//-----------------------
&Eeprom_summ,sizeof(Eeprom_summ),          //

NULL,0,
};
//---------------------------
void f_ee_num_init()
{
int i;
   for( i=0;EE_Dat[i].ptr != 0;i++) {}
   eee_num=i;
}
//---------------------------
int ee_members=0;
int f_wrt_eee()
/* Store all variables in EEE  */
{
int i,i1,i2,j;
int val;
char *ch_ptr;

 f_init_mki();

 Eeprom_summ=0;
        for(i=0,i1=0;EE_Dat[i].ptr!=0;i++)
        {
          i2=EE_Dat[i].l;
          ch_ptr=(char *)EE_Dat[i].ptr;
          for(j=0;j<i2;j++)
          {
            buf_eep[i1]=*ch_ptr++;
            if(EE_Dat[i].ptr!= &Eeprom_summ)
              Eeprom_summ +=((unsigned long)(buf_eep[i1])) & 0xff;
            i1++;
          }
        }

          i2=EE_Dat[i].l;
          ch_ptr=(char *)EE_Dat[i].ptr;

 EE_WriteEnable();
   i2= EE_MultiWrite_L(0,i1,buf_eep);
 EE_WriteProtect();

    ee_members=i;

// printf (" %d",i);
 return i2;
}
//--------------------------------------

#define Addr_m_grk_ee   2020

#define Addr_m_kd_ee    2030

#define Addr_flg_ee     2040
//--------------------------------------
int jj_n;
unsigned int Addr_ee;

int f_rd_EEE_kd( struct eee_dat *EE_D_ptr )
{

// вводит данные перечисленные в одной из 3 структур в EEPROM
//struct eee_dat EE_kd[];
//struct eee_dat EE_grk[];
//struct eee_dat EE_flg[];

int i,i1,i2,j;
int val;
char *ch_ptr;
unsigned int Ee_summ,*Ee_sum_rl;

 if(EE_D_ptr == EE_kd)
 {
  jj_n=10;
  Addr_ee=Addr_m_kd_ee;
 }
 else if(EE_D_ptr == EE_grk)
 {
  jj_n=10;
  Addr_ee = Addr_m_grk_ee;
 }
 else if(EE_D_ptr == EE_flg)
 {
  jj_n=4;
  Addr_ee=Addr_flg_ee;
 }
 else return -2;

 Ee_summ=0;

 if((i1=EE_MultiRead_L(Addr_ee,jj_n,buf_eep))<0 )
 {
   return i1;
 }
//-----------------------------
   // Check CRC

        for(i=0,i1=0;EE_D_ptr[i].ptr!=0;i++)
        {
          i2=EE_D_ptr[i].l;
          ch_ptr=(char *)EE_D_ptr[i].ptr;

          for(j=0;j<i2;j++)
          {
            if(EE_D_ptr[i+1].ptr == 0)
            {
              Ee_sum_rl=(unsigned int *)EE_D_ptr[i].ptr;
              *ch_ptr++=buf_eep[i1];
              i1++;
            }
            else
            {
              Ee_summ +=((unsigned int)(buf_eep[i1++])) & 0xff;
            }
          }
        }

 if(Ee_summ != *Ee_sum_rl) return -100;


   for(i=0,i1=0;EE_D_ptr[i].ptr!=0;i++)
   {
     i2=EE_D_ptr[i].l;
     ch_ptr=(char *)EE_D_ptr[i].ptr;
     for(j=0;j<i2;j++)
     {
       *ch_ptr++=buf_eep[i1++];
     }
   }
   return 0;
}
//--------------------------------------
int f_wrt_eee_kd( struct eee_dat *EE_D_ptr )
{
int i,i1,i2,j;
int val;
char *ch_ptr;

unsigned int Ee_summ,*Ee_sum_rl;

 if(EE_D_ptr == EE_kd)
 {
  jj_n=10;
  Addr_ee=Addr_m_kd_ee;
 }
 else if(EE_D_ptr == EE_grk)
 {
  jj_n=10;
  Addr_ee = Addr_m_grk_ee;
 }
 else if(EE_D_ptr == EE_flg)
 {
  jj_n=4;
  Addr_ee=Addr_flg_ee;
 }
 else return -2;

        Ee_summ=0;
        for(i=0,i1=0;EE_D_ptr[i].ptr!=0;i++)
        {
          i2=EE_D_ptr[i].l;
          ch_ptr=(char *)EE_D_ptr[i].ptr;

          for(j=0;j<i2;j++)
          {
            if(EE_D_ptr[i+1].ptr != 0)
            {
              buf_eep[i1]=*ch_ptr++;
              Ee_summ +=((unsigned int)(buf_eep[i1])) & 0xff;
            }
            else
            {
              *(unsigned int *)EE_D_ptr[i].ptr=Ee_summ;
              buf_eep[i1]=*ch_ptr++;
            }
            i1++;
          }
        }

 EE_WriteEnable();
   i2= EE_MultiWrite_L(Addr_ee,jj_n,buf_eep);
 EE_WriteProtect();

 return i2;
}
//--------------------------------------
//--------------------------------------

int f_chk_EEE_CRC()
{
int i,i1,i2,j;
int val;
char *ch_ptr;
unsigned long int Ee_summ;

 Ee_summ=0;

 if((i1=EE_MultiRead_L(0,2048,buf_eep))<0 )
 {
   return i1;
 }
//-----------------------------
   // Check CRC
        for(i=0,i1=0;EE_Dat[i].ptr!=0;i++)
        {
          i2=EE_Dat[i].l;
          ch_ptr=(char *)EE_Dat[i].ptr;
          for(j=0;j<i2;j++)
          {
            if(EE_Dat[i].ptr==&Eeprom_summ)
              *ch_ptr++=buf_eep[i1++];
            else
              Ee_summ +=((unsigned long)(buf_eep[i1++])) & 0xff;
          }
        }

     ee_lgth_byte=i1;
//  printf("\n\rf_chk_EEE_CRC: %d записей,%dбайт,CRC_count=%08lx,CRC_rd%08lx\n\r",i,i1,Ee_summ,Eeprom_summ);
   if(Ee_summ != Eeprom_summ) return -100;
   return 0;
}
//--------------------------------------
int f_rd_eee()
/* Read all variables from  buf_eep  */
{
int i,i1,i2,j;
int val;
char *ch_ptr;

unsigned long int Ee_summ;
 Ee_summ=0;
//-----------------------------

   for(i=0,i1=0;(EE_Dat[i].ptr!=0)&&(i<(eee_num-1)) ;i++)
   {
     i2=EE_Dat[i].l;
     ch_ptr=(char *)EE_Dat[i].ptr;
     for(j=0;j<i2;j++)
     {
       *ch_ptr++=buf_eep[i1];
       Ee_summ +=buf_eep[i1++];
     }
   }
   if(dim_xyz>=dim_xyz_c) dim_xyz=dim_xyz_c-6;

   if(MVD_FLOW != 0) MVD_FLOW=2;

   f_ee_num_init();
   f_prep_bern();
   f_count_cf();
   f_init_mki();
   return 0;
}
//--------------------------------------

