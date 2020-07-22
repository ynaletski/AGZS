
//27.05.2020 YN --\\//--
int flag_calc=0;
//27.05.2020 YN --//\\--

#if defined(MMI_ICP)

#define d_F2 (2)
#define n_mmi_str 7 // количество строк на дисплее MMI

int  ZeroPage=9;
int  EmptPage=0;

#else
  // #if defined(MMI_NEW)

#define d_F2 (4)
#define n_mmi_str 16 // количество строк на дисплее MMI

int  ZeroPage=2;
int  EmptPage=2;

#endif

#define n_mmi_str_1 (n_mmi_str-1)
#define n_mmi_str_2 (n_mmi_str-2)
#define n_mmi_str_3 (n_mmi_str-3)
#define n_mmi_str_4 (n_mmi_str-4)

int res_ctr[4]={0,0,0,0};

int flag_fault=0;
int Tank_nn=2;
int n_pp=0;

int PageD=0;
float MassStamp;
float VolStamp;
int i_nn_mvd=0;
long int tm_md=0;

int DenMeasSrc=0; // 0 - MMI, 1 - Modbus

float VolT=0;
float MassT=0;


float MassStamp2;
float VolStamp2;

int Ret_cod=0;

long int fl_zeroing=0;
#define Time_zeroing (50000)  // ms
long int zero_time;
char str_empty[]="                              ";
char str_verify[]="    Поверка расходомера(ов)   ";
char str_ESC[]="ESC  Возврат";
char str_def[]=" ------- ";


void f_d_ESC()
{

#if defined(MMI_ICP)
         MoveToXY(0,6);
#endif
#if defined(MMI_NEW)
         MoveToXY(0,15);
#endif
 MmiPuts(str_ESC);

}

void f_prn_Vol()
{
MmiPrintf("K объема:    %.6f",s_MVD[i_nn_mvd].f);
}
void f_prn_Mass()
{
MmiPrintf("K массы:     %.6f",s_MVD[i_nn_mvd].f);
}
void f_prn_Den()
{
     MmiPrintf("K плотн.: %.6f",s_MVD[i_nn_mvd].f);
}

//-------------------
void f_Den_temp( int *ret_ptr,float Density,float Temp )
{

 (*ret_ptr)++;
 if(*ret_ptr<n_mmi_str)
 {
    MoveToXY(0,*ret_ptr);
    MmiPrintf("Плотность %7.3f кг/м3;" ,Density);
   (*ret_ptr)++;
   if(*ret_ptr < n_mmi_str)
   {
    MoveToXY(0,*ret_ptr);
    MmiPrintf("Температ.%6.2fC;" ,Temp);
   }
 }
}
//-------------------
int Flag_f_corr=0,pass_ret=0;
float k_v_flow=1,k_m_flow=1,k_dens=1;
float k_mv_tmp;
long int pass1=0,pass2=0;
int dsp_tmp_in=-1;
int dsp_tmp_out=-1;
//
//  Файл содержит символьные данные в DOS кодировке символов.
//  Для отображения на дисплее MMI необходимо перекодировать в
//  кодировку Windows (выполняется командой dos_win.exe в .bat)
//
//------------------------------------------
char Beg_str[]="      Система учета СУГ       ";
void f_paged0()
{
    f_clr_scr_MMI();
#if defined(MMI_ICP)
    MmiGotoxy(0,2); MmiPuts(Beg_str);
    MoveToXY(0,3);
    MmiPrintf(" Объем,л    : ");
    MoveToXY(0,4);
    MmiPrintf(" Масса,кг   : ");
#else
    MmiGotoxy(0,0); MmiPuts(Beg_str);
    MoveToXY(0,2);
    MmiPrintf(" Объем отп. : ");
    MoveToXY(0,4);
    MmiPrintf(" Масса отп. : ");

    MoveToXY(0,6);
    MmiPrintf(" Давл.расхдм: ");

    MoveToXY(0,7);
    MmiPrintf(" Темп.расхдм: ");

    MoveToXY(0,9);
    MmiPrintf(" Резервуар1 : ");
         if(Tank_nn < 2) return;

    MoveToXY(0,11);
    MmiPrintf(" Резервуар2 : ");
         if(Tank_nn < 3) return;

    MoveToXY(0,13);
    MmiPrintf(" Резервуар3 : ");

#endif
}
//-------------------------------------
void f_paged0a()
{
#if defined(MMI_ICP)
            MoveToXY(14,3);
            MmiPrintf("%10.1f",-(s_MVD[GR].VolI));
            MoveToXY(14,4);
            MmiPrintf("%10.1f",-(s_MVD[GR].MassI));
#else
            MoveToXY(14,2);
            MmiPrintf("%10.1f л",-(s_MVD[GR].VolI));
            MoveToXY(14,4);
            MmiPrintf("%10.1f кг",-(s_MVD[GR].MassI));

            MoveToXY(14,6);
            MmiPrintf("%10.3f МПа",s_MVD[0].Press);

            MoveToXY(14,7);
            MmiPrintf("%10.1f C  ",s_MVD[GR].Temp);

            MoveToXY(14,9);
            MmiPrintf("%10.1f л",s_DGM[0].VolLiq);
         if(Tank_nn < 2) return;
            MoveToXY(14,11);
            MmiPrintf("%10.1f л",s_DGM[1].VolLiq);

         if(Tank_nn < 3) return;
            MoveToXY(14,13);
            MmiPrintf("%10.1f л",s_DGM[2].VolLiq);

#endif
}
//-------------------------------------
void f_paged1()
{
  f_clr_scr_MMI();
#if defined(MMI_ICP)
  MmiGotoxy(0,2);  MmiPuts(Beg_str);
//MmiGotoxy(0,3) ; MmiPrintf("%s",list_un_mmi[un_mmi1]);
//MmiGotoxy(9,3);  MmiPrintf("%s",list_un_mmi[un_mmi2]);
//MmiGotoxy(19,3); MmiPrintf("%s",list_un_mmi[un_mmi3]);
  MmiGotoxy(0,4); MmiPrintf("        |         |          ");
#else

  MmiGotoxy(0,0);  MmiPuts(Beg_str);

//MmiGotoxy(0,3) ; MmiPrintf("%s",list_un_mmi[un_mmi1]);
//MmiGotoxy(9,3);  MmiPrintf("%s",list_un_mmi[un_mmi2]);
//MmiGotoxy(19,3); MmiPrintf("%s",list_un_mmi[un_mmi3]);
  MmiGotoxy(0,4); MmiPrintf("Сумма,р.| Объем,л |  Цена,р.");

  MmiGotoxy(0,6); MmiPrintf("        |         |          ");
#endif
}
//-------------------------------------

void f_paged1a()
{
#if defined(MMI_ICP)
            MmiGotoxy(0,4);  MmiPrintf("%s",TsdBuf[0][0]);
            MmiGotoxy(9,4);  MmiPrintf("%s",TsdBuf[0][1]);
            MmiGotoxy(20,4); MmiPrintf("%s",TsdBuf[0][2]);
#else
            MmiGotoxy(0,6);  MmiPrintf("%s",TsdBuf[0][0]);
            MmiGotoxy(9,6);  MmiPrintf("%s",TsdBuf[0][1]);
            MmiGotoxy(20,6); MmiPrintf("%s",TsdBuf[0][2]);

#endif

}
//-------------------------------------
void f_paged2()
{
    f_clr_scr_MMI();
#if defined(MMI_ICP)
    MmiGotoxy(0,2); MmiPuts(Beg_str);
    MoveToXY(0,3);
    MmiPrintf("MVD1 :");
    MoveToXY(0,4);
    MmiPrintf("MVD2 :");
    MoveToXY(0,5);
    MmiPrintf("Всего:");
#else

  MmiGotoxy(0,0);  MmiPuts(Beg_str);


    MoveToXY(4,2);
    MmiPrintf("Расходомеры приема:");

    MoveToXY(0,5);
    MmiPrintf(" ЖФ  :");
    MoveToXY(0,7);
    MmiPrintf(" ПФ  :");
    MoveToXY(0,9);
    MmiPrintf("Всего:");
#endif
}
//-------------------------------------
char str_tmpd[30];
void f_paged2a()
{
  float ftmp1,ftmp2;
            ftmp1=0.;
            ftmp2=0.;

#if defined(MMI_ICP)
            if(s_MVD[0].MassI != vBIG_P)
            {
              sprintf(str_tmpd,"%9.1f кг",s_MVD[0].MassI);
              sscanf(str_tmpd,"%f",&ftmp1);
              MmiGotoxy(6,3);  MmiPuts(str_tmpd);

              MmiGotoxy(18,3); MmiPrintf("%9.1f л ",s_MVD[0].VolI);
            }

            if(s_MVD[1].MassI != vBIG_P)
            {
              sprintf(str_tmpd,"%9.1f кг",s_MVD[1].MassI);
              sscanf(str_tmpd,"%f",&ftmp2);
              MmiGotoxy(6,4);  MmiPuts(str_tmpd);
              MmiGotoxy(18,4); MmiPrintf("%9.1f л :",s_MVD[1].VolI);
            }

            MmiGotoxy(6,5);

            MmiPrintf("%9.1f кг ",ftmp1+ftmp2);

#else

            if(s_MVD[0].MassI != vBIG_P)
            {
              sprintf(str_tmpd,"%9.1f кг",s_MVD[0].MassI);
              sscanf(str_tmpd,"%f",&ftmp1);
              MmiGotoxy(6,5);  MmiPuts(str_tmpd);

              MmiGotoxy(18,5); MmiPrintf("%9.1f л ",s_MVD[0].VolI);
            }

            if(s_MVD[1].MassI != vBIG_P)
            {
              sprintf(str_tmpd,"%9.1f кг",s_MVD[1].MassI);
              sscanf(str_tmpd,"%f",&ftmp2);
              MmiGotoxy(6,7);  MmiPuts(str_tmpd);
              MmiGotoxy(18,7); MmiPrintf("%9.1f л :",s_MVD[1].VolI);
            }
            MmiGotoxy(6,9);
            MmiPrintf("%9.1f кг ",ftmp1+ftmp2);
#endif
}

//-------------------------------------
void f_paged3()
{
    f_clr_scr_MMI();
}
//-------------------------------------
void f_dsp_p1( int ii)
{
#if defined(MMI_ICP)
    MmiGotoxy(0,ii); MmiPuts("Давление ,МПа: ");MmiPuts(str_empty);
    MmiGotoxy(0,ii+1); MmiPrintf("MVD=%6.4f ", s_MVD[0].Press); MmiPuts(str_empty);
    MmiGotoxy(0,ii+2); MmiPrintf("P1 =%6.4f",Press1);
    if(Tank_nn > 1)
          MmiPrintf(" P2=%6.4f",Press2);
    if(Tank_nn > 2)
          MmiPrintf(" P3=%6.4f",Press3);
    MmiPuts(str_empty);

    MmiGotoxy(0,ii+3); MmiPuts("Температура,град.С: ");MmiPuts(str_empty);
    MmiGotoxy(0,ii+4); MmiPrintf("T1=%6.2f",Temp1);
    if(Tank_nn > 1)
          MmiPrintf(" T2=%6.2f",Temp2);
    if(Tank_nn > 2)
          MmiPrintf(" T3=%6.2f",Temp3);
    MmiPuts(str_empty);
#else
    MmiGotoxy(0,0);  MmiPuts(Beg_str);
    MmiGotoxy(0,ii+2); MmiPuts("Давление ,МПа: ");MmiPuts(str_empty);
    MmiGotoxy(0,ii+4); MmiPrintf("MVD=%6.4f ", s_MVD[0].Press); MmiPuts(str_empty);
    MmiGotoxy(0,ii+5); MmiPrintf("P1 =%6.4f",Press1);
    if(Tank_nn > 1)
          MmiPrintf(" P2=%6.4f",Press2);
    if(Tank_nn > 2)
          MmiPrintf(" P3=%6.4f",Press3);
    MmiPuts(str_empty);

    MmiGotoxy(0,ii+8 ); MmiPuts("Температура,град.С: ");MmiPuts(str_empty);
    MmiGotoxy(0,ii+10); MmiPrintf("T1=%6.2f",Temp1);
    if(Tank_nn > 1)
          MmiPrintf(" T2=%6.2f",Temp2);
    if(Tank_nn > 2)
          MmiPrintf(" T3=%6.2f",Temp3);
    MmiPuts(str_empty);
#endif
}

//------------------------------------------
float *TempArr[3]={&Temp1,&Temp2,&Temp3};
float *PressArr[3]={&Press1,&Press2,&Press3};
void f_dsp_res(int ii)
{
int i1;
    i1=ii+1;

#if defined(MMI_ICP)
    if(res_ctr[ii] != 0)
    {
    MmiGotoxy(0,1);  MmiPrintf("Уровень %dXX: ",i1);
    }
    else
    {
    MmiGotoxy(0,1);  MmiPrintf("Уровень %d  : ",i1);
    }
    MmiGotoxy(13,1); MmiPrintf(" %8.1f мм  ",s_DGM[ii].Prod_lvl);
    MmiGotoxy(0,2);  MmiPrintf("Темп.ждк.%d :",i1);
    MmiGotoxy(13,2); MmiPrintf("  %7.2f град.C ",s_DGM[ii].Temp);
    MmiGotoxy(0,3);  MmiPrintf("Объем ждк.%d: ",i1);
    MmiGotoxy(13,3); MmiPrintf(" %8.1f л",s_DGM[ii].VolLiq);
    MmiGotoxy(0,4);  MmiPrintf("Масса ждк.%d:",i1);
    MmiGotoxy(13,4); MmiPrintf(" %8.1f кг   ",s_DGM[ii].MassLiq);
    MmiGotoxy(0,5);  MmiPrintf("Масса общ.%d:",i1);
    MmiGotoxy(13,5); MmiPrintf(" %8.1f кг ",s_DGM[ii].MassGas+s_DGM[ii].MassLiq);

#else

    MmiGotoxy(0,0);  MmiPuts(Beg_str);
    MmiGotoxy(7,1);  MmiPrintf("  Резервуар N%d",i1);

    if(res_ctr[ii] != 0)
    {
     MmiGotoxy(0,3);  MmiPrintf("Уровень %dXX: ",i1);
    }
    else
    {
     MmiGotoxy(0,3);  MmiPrintf("Уровень %d  : ",i1);
    }

    MmiGotoxy(13,3); MmiPrintf(" %8.1f мм  ",s_DGM[ii].Prod_lvl);


    MmiGotoxy(0, 5); MmiPrintf("Объем ждк.%d: ",i1);
    MmiGotoxy(13, 5);MmiPrintf(" %8.1f л",s_DGM[ii].VolLiq);


    MmiGotoxy(0,7);  MmiPrintf("Темп.ждк.%d :",i1);
    MmiGotoxy(13,7); MmiPrintf(" %8.1f град.C ",*TempArr[ii]);

    MmiGotoxy(0,9);  MmiPrintf("Давление%d  :",i1);
    MmiGotoxy(13,9); MmiPrintf(" %8.3f МПа ",*PressArr[ii]);

    MmiGotoxy(0,11); MmiPrintf("Масса ждк.%d:",i1);
    MmiGotoxy(13,11);MmiPrintf(" %8.1f кг   ",s_DGM[ii].MassLiq);

    MmiGotoxy(0,13); MmiPrintf("Масса общ.%d:",i1);
    MmiGotoxy(13,13);MmiPrintf(" %8.1f кг ",s_DGM[ii].MassGas+s_DGM[ii].MassLiq);

#endif

}

//------------------------------------------
void f_dsp_ddn(int ii,float Den, float Temp )
{
    MmiGotoxy(0,ii);   MmiPuts("Плотность    : ");
    MmiGotoxy(15,ii);  MmiPrintf("%8.2f кг/м3   ",Den);
    MmiGotoxy(0,ii+1);   MmiPuts("Температура  : ");
    MmiGotoxy(15,ii+1);  MmiPrintf(" %7.2f град.   ",Temp);

}
//------------------------------------------
void f_dsp_dn()
{
#if defined(MMI_ICP)
    MmiGotoxy(0,1);   MmiPuts("Плотность изм.кг/м3:");
    MmiGotoxy(0,2);
      MmiPrintf("%8.2f",s_DGM[0].DensReal);
    if(Tank_nn > 1)
                     MmiPrintf(" %8.2f ",s_DGM[1].DensReal);
    if(Tank_nn > 2)
                     MmiPrintf(" %8.2f ",s_DGM[2].DensReal);


    MmiGotoxy(0,3);   MmiPuts("Температура изм. C:");
    MmiGotoxy(0,4);
     MmiPrintf("%8.2f",s_DGM[0].TempReal);
    if(Tank_nn > 1)
         MmiPrintf(" %8.2f",s_DGM[1].TempReal);
    if(Tank_nn > 2)
         MmiPrintf(" %8.2f ",s_DGM[2].TempReal);

#else
    MmiGotoxy(0,0);  MmiPuts(Beg_str);

    MmiGotoxy(0,4);   MmiPuts("Плотность изм.кг/м3:");
    MmiGotoxy(0,6);
      MmiPrintf("%8.2f",s_DGM[0].DensReal);
    if(Tank_nn > 1)
                     MmiPrintf(" %8.2f ",s_DGM[1].DensReal);
    if(Tank_nn > 2)
                     MmiPrintf(" %8.2f ",s_DGM[2].DensReal);

    MmiGotoxy(0,9);   MmiPuts("Температура изм. C:");

    MmiGotoxy(0,11);
     MmiPrintf("%8.2f",s_DGM[0].TempReal);
    if(Tank_nn > 1)
         MmiPrintf(" %8.2f",s_DGM[1].TempReal);
    if(Tank_nn > 2)
         MmiPrintf(" %8.2f ",s_DGM[2].TempReal);



#endif
}

//------------------------------------------
//------------------------------------------
void  f_prn_begin()
{
  if(CRC_err) f_prn_CRC_error();
  else if(FL_err) f_prn_error();
  else
  {
    f_clr_scr_MMI();
    if(PageD==0)
    {
      f_paged0();
    }
    else if(PageD==1)
    {
      f_paged1();
    }
    else if(PageD==2)
    {
      f_paged2();
    }
    else if(PageD==3)
    {
      f_paged3();
    }
  }
}
//-------------------------------------
#define msk_d_en 1
int en_mski=0;
// GetDi1() = 0 - цепь Di1 замкнута на землю
//
int f_chk_EN()
{

// проверяет цепь разрешения коррекции параметров
// возвращает 1 - можно корректировать параметры

 return ( (GetDi1()^en_mski) & msk_d_en);
}
//-------------------------------------
int un_mmi1=1;
int un_mmi2=2;
int un_mmi3=3;
/*
char *list_un_mmi[]={
"",
"",
"",
//" Объем,л   " ,  //1
//"Сумма,руб  " ,  //2
//"Цена,руб/л " ,   //3
"" ,  //4
"" ,  //5
"" ,  //6
};
*/
//-------------------------------------
int CRC_err=0;
//-------------------------------------
void f_prn_MassSt_rcv()
{
   MmiGotoxy(0,1);
   if(Flag_check == 0)

    MmiPrintf("  Всего принято %8.2f кг  ",s_MVD[0].MassI+s_MVD[1].MassI-MassStamp-MassStamp2);
   else
    {
      MmiPuts(str_verify);
    }
}
//-------------------------------------
void f_prn_clp()
{
// отображает состояние выходов при приеме по нажатию F1

#if defined(MMI_ICP)

     MmiGotoxy(0,4); MmiPuts("Мнш.:");
     if( OUT_VAR & OUT9  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy( 7,4); MmiPuts("Блш.:");
     if( OUT_VAR & OUT8  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy(14,4); MmiPuts("Насос:");
     if( OUT_VAR & OUT10 )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy(22,4); MmiPuts("Нас.Г:");
     if( OUT_VAR & OUT11 )
       MmiPuts("1   ");
     else
       MmiPuts("0   ");

     MmiGotoxy(0,5); MmiPuts("Плтн:");
     if( OUT_VAR & OUT4 )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy(7,5); MmiPuts("Пар :");
     if( OUT_VAR & OUT5  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy(14,5); MmiPuts("Ур10%:");
     if( OUT_VAR & OUT6  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");


     MmiGotoxy(22,5); MmiPuts("Ур90%:");
     if( OUT_VAR & OUT7  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");


     MmiGotoxy(22,6); MmiPuts("Ур85%:");
  //   if( OUT_VAR & OUT12 )
    if(i_LVL & bit_85p)
       MmiPuts("1 ");
     else
       MmiPuts("0 ");
#else

     MmiGotoxy(0,11); MmiPuts("Мнш.:");
     if( OUT_VAR & OUT9  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy( 7,11); MmiPuts("Блш.:");
     if( OUT_VAR & OUT8  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy(14,11); MmiPuts("Насос:");
     if( OUT_VAR & OUT10 )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy(22,11); MmiPuts("Нас.Г:");
     if( OUT_VAR & OUT11 )
       MmiPuts("1   ");
     else
       MmiPuts("0   ");

     MmiGotoxy(0,12); MmiPuts("Плтн:");
     if( OUT_VAR & OUT4 )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy(7,12); MmiPuts("Пар :");
     if( OUT_VAR & OUT5  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy(14,12); MmiPuts("Ур10%:");
     if( OUT_VAR & OUT6  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");


     MmiGotoxy(22,12); MmiPuts("Ур90%:");
     if( OUT_VAR & OUT7  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");


     MmiGotoxy(22,13); MmiPuts("Ур85%:");
 //    if( OUT_VAR & OUT12 )
     if(i_LVL & bit_85p)
       MmiPuts("1 ");
     else
       MmiPuts("0 ");


#endif

}
//-------------------------------------
void f_prn_dn()
{
#if defined(MMI_ICP)
          if(PageD==0)
          {
    f_dsp_ddn(2,s_MVD[MVD_DNS].Dens,s_MVD[MVD_DNS].Temp);
    MmiGotoxy(0,4);  MmiPuts("Масс.расход  : ");
    MmiGotoxy(15,4); MmiPrintf("%8.1f кг/ч    ",-s_MVD[MVD_DNS].FlowM);


    if(Tank_nn > 2)
    {
    MmiGotoxy(0,5);
            MmiPrintf("V1=%4.0fл   V2=%4.0fл   V3=%4.0fл", s_DGM[0].VolLiq,s_DGM[1].VolLiq,s_DGM[2].VolLiq);
    MmiGotoxy(0,6); MmiPuts(str_empty);
    }
    else
    {
    MmiGotoxy(0,5);  MmiPrintf("Объем ждк.1  : ");
    MmiGotoxy(15,5); MmiPrintf("%8.1f л",s_DGM[0].VolLiq);MmiPuts(str_empty);

    if(Tank_nn > 1)
     {
      MmiGotoxy(0,6);  MmiPrintf("Объем ждк.2  : ");
      MmiGotoxy(15,6); MmiPrintf("%8.1f л",s_DGM[1].VolLiq);MmiPuts(str_empty);
     }
     else
     {
       MmiGotoxy(0,6); MmiPuts(str_empty);
     }
    }
//  MmiGotoxy(0,5);MmiPuts(str_empty);

          }
          else if(PageD==1)
          {
            f_dsp_p1(2);
     //     MmiGotoxy(0,6);MmiPuts(str_empty); // QQQQ
          }
          else if(PageD==3)
          {
//  MmiGotoxy(0,0);MmiPuts(str_empty);
//  MmiGotoxy(0,1);MmiPuts(str_empty);
    MmiGotoxy(0,2);  MmiPuts("     Состояние выходов       ");
    MmiGotoxy(0,3);MmiPuts(str_empty);

     MmiGotoxy(0,6);MmiPuts(str_empty);
     MmiGotoxy(0,6); MmiPuts("Нас.ГРК :");
     if( OUT_VAR & OUT3  )
       MmiPuts("1");
     else
       MmiPuts("0");

            f_prn_clp();
          }
#else

    f_dsp_ddn(3,s_MVD[MVD_DNS].Dens,s_MVD[MVD_DNS].Temp);


    MoveToXY(0,5);   MmiPrintf("Давл.расхдм  : ");
    MoveToXY(15,5);  MmiPrintf("%8.3f МПа",s_MVD[0].Press);MmiPuts(str_empty);

    MmiGotoxy(0,6);    MmiPuts("Масс.расход  : ");
    MmiGotoxy(15,6); MmiPrintf("%8.1f кг/ч    ",-s_MVD[MVD_DNS].FlowM);MmiPuts(str_empty);



            if(Tank_nn > 2)
            {
            MmiGotoxy(0,8);
                    MmiPrintf("V1=%4.0fл   V2=%4.0fл   V3=%4.0fл", s_DGM[0].VolLiq,s_DGM[1].VolLiq,s_DGM[2].VolLiq);
            MmiGotoxy(0,9); MmiPuts(str_empty);
            }
            else
            {
            MmiGotoxy(0,8);  MmiPrintf("Объем ждк.1  : ");
            MmiGotoxy(15,8); MmiPrintf("%8.1f л    ",s_DGM[0].VolLiq);

            if(Tank_nn > 1)
             {
              MmiGotoxy(0,9);  MmiPrintf("Объем ждк.2  : ");
              MmiGotoxy(15,9); MmiPrintf("%8.1f л    ",s_DGM[1].VolLiq);
             }
             else
              MmiGotoxy(0,9);   MmiPuts(str_empty);
            }

          if(PageD!=3)
          {
             MmiGotoxy(0,11); MmiPrintf("P1=%6.3f",Press1);
             if(Tank_nn > 1)

                   MmiPrintf(" P2=%6.3f",Press2);
             if(Tank_nn > 2)
                   MmiPrintf(" P3=%6.3f",Press3);
                   MmiPuts(str_empty);


             MmiGotoxy(0,12); MmiPrintf("T1=%6.1f",Temp1);
             if(Tank_nn > 1)
                   MmiPrintf(" T2=%6.1f",Temp2);
             if(Tank_nn > 2)
                   MmiPrintf(" T3=%6.1f",Temp3);
                   MmiPuts(str_empty);
               MmiGotoxy(0,13); MmiPuts(str_empty);
          }
          else // if(PageD==3)
          {
            f_prn_clp();
     MmiGotoxy(0,13); MmiPuts("Нас.ГРК :");
     if( OUT_VAR & OUT3  )
       MmiPuts("1");
     else
       MmiPuts("0");
          }

#endif

}
//-------------------------------------
void f_prn_proc()
{
#if defined(MMI_ICP)
          if(PageD==0)
          {
              MmiGotoxy(0,4);  MmiPrintf("Ж:%5.0fкг/ч|%5.1fкг/м3|%5.1f C",s_MVD[0].FlowM,s_MVD[0].Dens,s_MVD[0].Temp);
              MmiGotoxy(0,5);  MmiPrintf("П:%5.0fкг/ч|%5.1fкг/м3|%5.1f C",s_MVD[1].FlowM,s_MVD[1].Dens,s_MVD[1].Temp);
              MmiGotoxy(22,6); MmiPuts(str_empty);
          }
          else if(PageD==1)
          {

           MmiGotoxy(0,4);  MmiPuts(" P1, МПа ");
           MmiGotoxy(9,4);  MmiPuts("| Pнп,МПа |  T,град.C");

           MmiGotoxy(0,5); MmiPrintf("%8.4f |",s_MVD[0].Press-0.1);
           MmiGotoxy(9,5); MmiPrintf("|%8.4f |%8.2f   ",s_MVD[0].PressG,s_MVD[0].Temp);
           MmiGotoxy(22,6); MmiPuts(str_empty);
          }
          else if(PageD==3)
          {
            f_prn_clp();
          }
#else
              MmiGotoxy(0,4);  MmiPrintf("Ж:%5.0fкг/ч|%5.1fкг/м3|%5.1f C",s_MVD[0].FlowM,s_MVD[0].Dens,s_MVD[0].Temp);
              MmiGotoxy(0,5);  MmiPrintf("П:%5.0fкг/ч|%5.1fкг/м3|%5.1f C",s_MVD[1].FlowM,s_MVD[1].Dens,s_MVD[1].Temp);


             MmiGotoxy(0,8); MmiPrintf("P1=%6.3f",Press1);
             if(Tank_nn > 1)

                   MmiPrintf(" P2=%6.3f",Press2);
             if(Tank_nn > 2)
                   MmiPrintf(" P3=%6.3f",Press3);
                   MmiPuts(str_empty);


             MmiGotoxy(0,9); MmiPrintf("T1=%6.1f",Temp1);
             if(Tank_nn > 1)
                   MmiPrintf(" T2=%6.1f",Temp2);
             if(Tank_nn > 2)
                   MmiPrintf(" T3=%6.1f",Temp3);
                   MmiPuts(str_empty);


          if(PageD!=3)
          {

           MmiGotoxy(0,11);  MmiPuts("P MVD,МПа");
           MmiGotoxy(9,11);  MmiPuts("| Pнп,МПа |  T,град.C");

           MmiGotoxy(0,12); MmiPrintf("%8.4f |",s_MVD[0].Press-0.1);
           MmiGotoxy(9,12); MmiPrintf("|%8.4f |%8.2f   ",s_MVD[0].PressG,s_MVD[0].Temp);

           MmiGotoxy(22,13); MmiPuts(str_empty);

          }
          else // if(PageD==3)
          { // 11,12,13
            f_prn_clp();
          }
#endif

}
//-------------------------------------
//-------------------------------------
void f_prn_CRC_error()
{

#if defined(MMI_ICP)
 MmiGotoxy(0,2);   MmiPrintf("!Ошибка контрольной суммы");
 MmiGotoxy(0,3);   MmiPrintf("!Параметры потеряны");
 MmiGotoxy(0,4);   MmiPrintf("!Необходима перенастройка");
#else

 MmiGotoxy(0,5);   MmiPrintf("!Ошибка контрольной суммы");
 MmiGotoxy(0,7);   MmiPrintf("!Параметры потеряны");
 MmiGotoxy(0,9);   MmiPrintf("!Необходима перенастройка");
#endif


// MmiGotoxy(0,3);   MmiPrintf("!Ошибка CRC");
 CRC_err=1;
}
//-------------------------------------
int Flag_bot=0;
void f_prn_error()
{

   f_clr_scr_MMI();
   Flag_bot=0;
#if defined(MMI_ICP)
#if( Test == 0)
   if(flag_mvd_ton[0] | flag_mvd_ton[1])
   {
      MmiGotoxy(6,2);  MmiPrintf("Система учета СУГ");
      MmiGotoxy(2,4);  MmiPrintf("!Инициализация расходомера");
   }
   else
#endif
   {
    MmiGotoxy(0,2);   MmiPrintf("!Ошибка функционирования");
    MmiGotoxy(0,3);   MmiPrintf("Sht-F1 отображение ошибок");
    MmiGotoxy(0,4);   MmiPrintf("Sht-ESC очистка ошибок");
   }
#else

#if( Test == 0)
   if(flag_mvd_ton[0] | flag_mvd_ton[1])
   {
      MmiGotoxy(6,6);      MmiPrintf("Система учета СУГ");
      MmiGotoxy(2,8);      MmiPrintf("!Инициализация расходомера");
   }
   else
#endif
   {
    MmiGotoxy(0,5);   MmiPrintf("!Ошибка функционирования");
    MmiGotoxy(0,7);   MmiPrintf("Sht-F1 отображение ошибок");
    MmiGotoxy(0,9 );  MmiPrintf("Sht-ESC очистка ошибок");
   }
#endif

}
//-------------------------------------
void  f_disp_error()
{ // распечатывает ошибки

 struct s_icp_dev *ICPl_DEV;
int i,i1,i2;
int itmp;
   i1=0;
   i2=0;
 for(i=0;i<=icp_lst_max;i++)
   if(ICP_error[i])
   {
    i2++;
    ICPl_DEV=ICP_dd[i];
  if(i != icp_lst_max)
  {
 MmiGotoxy(0,i1++);   MmiPrintf("Ошибка в драйвере %9s:",ICPl_DEV->name );
  }
  else
  {
 MmiGotoxy(0,i1++);   MmiPrintf("Ошибка системы СУГ: ");
  }
    if(i1 >= n_mmi_str) return;
    itmp=ICP_error[i];
  if(i != icp_lst_max)
  {
    if(itmp & RD_ERR)
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка чтения устр.");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & WR_ERR)
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка записи в устр.");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & WD_ERR)
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка Watch Dog");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & Drive_ERR)
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка в устр.,%02X", (itmp>>8) & 0xff);
    if(i1 >= n_mmi_str) return;
    }

  }
  else //(i == icp_lst_max)
  {
    if(itmp & EEE_CRC_error)
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка CRC");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & Flash_erase_error )
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка стир.FLASH");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & Flash_wr_error  )
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка зап.во FLASH");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & No_gas_error  )
    {
MmiGotoxy(0,i1++); MmiPrintf("Расх-р не заполнен СУГ");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & No_Z_mass_err )
    {
MmiGotoxy(0,i1++); MmiPrintf("Расх-р не обнулил M/V");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & No_F_dat_err )
    {
MmiGotoxy(0,i1++); MmiPrintf("Нет обновл.данных для журнала");
    if(i1 >= n_mmi_str) return;
    }

    if(itmp & No_liq_err)
    {
MmiGotoxy(0,i1++); MmiPrintf("Доп.время прокачки истекло");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & DNS_not_off)
    {
MmiGotoxy(0,i1++); MmiPrintf("Клап.'Плотность' не закр.");
    if(i1 >= n_mmi_str) return;
    }

    if((itmp & Hst_all)==LVL_min)
    {
MmiGotoxy(0,i1++); MmiPrintf("Миним.уров. при отпуске");
    if(i1 >= n_mmi_str) return;
    }
    if(itmp & WD_pmlan )
    {
MmiGotoxy(0,i1++); MmiPrintf("Нет связи с базой");
    if(i1 >= n_mmi_str) return;
    }
//  if(itmp & No_flow )
    if(itmp & LVL_sb )
    {
MmiGotoxy(0,i1++); MmiPuts("!Предельный уровень");
    if(i1 >= n_mmi_str) return;
    }

    if((itmp & MVD_com)== MVD_com )
    {
MmiGotoxy(0,i1++); MmiPrintf("Нет связи с расхдм.");
    if(i1 >= n_mmi_str) return;
    }
    if((itmp & Hst_all)==Hst_alm)
    {
MmiGotoxy(0,i1++); MmiPrintf("Авария от Host");
    if(i1 >= n_mmi_str) return;
    }

    if(itmp & MVD_cfg )
    {
MmiGotoxy(0,i1++); MmiPrintf("Ошибка конф.расход-ра ");
    if(i1 >= n_mmi_str) return;
    }
  }
   }
   if(i2==0)
   {
#if defined(MMI_ICP)
    MmiGotoxy(5,2); MmiPrintf("    Ошибок нет");
    MmiGotoxy(5,4); MmiPrintf("Enter - продолжить");
#else
    MmiGotoxy(5,6); MmiPrintf("    Ошибок нет");
    MmiGotoxy(5,10);MmiPrintf("Enter - продолжить");
#endif
   }
}
//-------------------------------------
void f_drv_list_MMI(int *nn)
{ // показать список драйверов устройств
int i,i1,i2;
char str[31];
  struct s_icp_dev *ICPl_DEV;

   f_clr_scr_MMI();

    i=*nn;
    if(i<0) i=0;
    if((i+n_mmi_str_3) < icp_lst_max) i1=i+n_mmi_str_3;
    else i1=icp_lst_max;

    *nn=i;
    MmiGotoxy(0,0); MmiPuts("        Список драйверов      ");
    MmiGotoxy(0,1); MmiPuts(" N     Имя   Сост. Порт Адр.");
   for(i2=0;i<i1;i++,i2++)
    {
      ICPl_DEV=ICP_dd[i];
      if(ICPl_DEV==NULL) break;
      MmiGotoxy(0,3+i2);
//         MmiPuts(" N      Имя   Сост. Порт Адр.");
//                 "xx.....xxxxx  .x    .x   xx"
      MmiPrintf("%2d%10s  %2d    %2d   %02X",i+1,ICPl_DEV->name,ICPl_DEV->status,ICPl_DEV->port,ICPl_DEV->addr);
    }
}
//-------------------------------
void f_drv_stat_MMI(int *nn)
{ // показать статистику драйверов устройств
int i,i1,i2;
char str[31];
  struct s_icp_dev *ICPl_DEV;

   f_clr_scr_MMI();

//  if( (*nn+n_mmi_str_3) > icp_lst_max) i=icp_lst_max-n_mmi_str_3;
//  else
    i=*nn;
    if(i<0) i=0;
    if((i+n_mmi_str_3) < icp_lst_max) i1=i+n_mmi_str_3;
    else i1=icp_lst_max;

    *nn=i;
    MmiGotoxy(0,0);  MmiPuts(" Статистика работы драйверов");
    MmiGotoxy(0,1);  MmiPuts(" N     Имя    Послано  Ошибок");
    for(i2=0;i<i1;i++,i2++)
    {
      ICPl_DEV=ICP_dd[i];
      if(ICPl_DEV==NULL) break;
      MmiGotoxy(0,3+i2);
//         MmiPuts(" N     Имя    Послано  Ошибок");
//                 "xx.....xxxxx  xxxxxxx xxxxxxx"
         MmiPrintf("%2d%10s  %7ld %7ld",i+1,ICPl_DEV->name,ICPl_DEV->n_transaction,ICPl_DEV->n_timeout_error+ICPl_DEV->n_CRC_error);
    }
}
//-------------------------------
//-------------------------------
void f_mod_list_MMI(int *nn)
{ // показать список обнаруженных модулей
int i,i1,i2;
 char str[31];

  struct icp_mod *ICPl_MOD;

  i1=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
  f_clr_scr_MMI();
  if(i1==0)
  {
    MmiGotoxy(3,2);   MmiPuts("    Список пуст." );
//  MmiGotoxy(3,3);   MmiPuts("Используйте поиск для" );
//  MmiGotoxy(3,4);   MmiPuts("  обновления списка." );
  }
  else
  {
    MmiGotoxy(0,0);    MmiPuts(" Список обнаруженных модулей  ");
    MmiGotoxy(0,1);    MmiPuts(" N    Имя Порт.Адр.Скор. TT FF");

      i=*nn;
    if(i<0) i=0;
    if((i+n_mmi_str_3) < i1) i1=i+n_mmi_str_3;
    *nn=i;

    for(i2=0;i<i1;i++,i2++)
    {
      ICPl_MOD=f_get_mod_str(i);
      if(ICPl_MOD==NULL) break;
      MmiGotoxy(0,3+i2);

//         MmiPuts(" N    Имя Порт.Адр.Скор. TT FF");
//                 "xx.....xxxx  x xx xxxxxx xx xx"
      MmiPrintf("%2d%9s  %1d %02X %6ld %02X %02X",i+1,ICPl_MOD->name,ICPl_MOD->port,ICPl_MOD->addr,baudrate_val[ICPl_MOD->CC],ICPl_MOD->TT,ICPl_MOD->FF);
    }
  }
}
//-------------------------------
int mode_prn=1;
/*
void f_prn_current()
{ // показать состояние отпуска на текущий момент
  int year,month,day,hour,min,sec;
 MmiGotoxy(0,0);   MmiPrintf("Текущие пок-я расхдомера(ов):");

 GetDate(&year,&month,&day);GetTime(&hour,&min,&sec);
 MoveToXY(0,2);
   if(s_MVD[0].VolI != vBIG_P)
       MmiPrintf(" Объем 1,л  : %11.3f",s_MVD[0].VolI);
 MoveToXY(0,3);
   if(s_MVD[0].MassI != vBIG_P)
       MmiPrintf(" Масса 1,кг : %11.3f",s_MVD[0].MassI);
 MoveToXY(0,4);
   if(s_MVD[1].VolI != vBIG_P)
       MmiPrintf(" Объем 2,л  : %11.3f",s_MVD[1].VolI);
 MoveToXY(0,5);
   if(s_MVD[1].MassI != vBIG_P)
       MmiPrintf(" Масса 2,кг : %11.3f",s_MVD[1].MassI);

 MoveToXY(0,6);
 MmiPrintf("   Время    :    %02d:%02d:%02d",hour,min,sec);
}
*/
//-------------------------------
void f_prn_den_t()
{
float ftmp,ftmp1;
 //  Отображение Плотности,температуры,состава
 //  Текущее значение.
// MmiGotoxy(0,0);   MmiPrintf("Текущие показания расходомера");
// MmiGotoxy(0,2);   MmiPrintf(" Плотность  %8.2f кг/м3",s_MVD[0].Dens);
 MmiGotoxy(0,2);   MmiPrintf(" Температура  %6.2f град.C",s_MVD[0].Temp);
/*===========
 MoveToXY(0,4);
    if((s_MVD[0].Dens>0)&&(s_MVD[0].Temp>-300))
    {
     ftmp=f_get_composition(s_MVD[0].Dens,s_MVD[0].Temp);
     if(ftmp>1.)
      MmiPrintf("!СУГ легче жидкого пропана");
     else if(ftmp<0.)
      MmiPrintf("!СУГ тяжелее жидкого бутана");
     else
      {
       ftmp1=(1.- ftmp)*100.;
            MmiPrintf("СУГ:пропан %6.2f%c,бутан %6.2f%c",(ftmp*100),'%',ftmp1,'%');
      }
    }
=============*/
 f_d_ESC();
// MmiGotoxy(0,6);    MmiPuts(" ESC   - возврат в меню");
}
//-------------------------------
//-------------------------------
void f_prn_lst_hour()
{ // показать состояние отпуска на последний расч. час
/*
 struct tm *tdat;

int year,month,day,hour,min,sec;
unsigned int seg_tmp,offs_tmp;
struct arch_str *ar_str_ptr;
time_t ltime1,ltime2;

long int ltmp;

 f_prepare_t ( &tim_000);
 ltime1=f_get_time_t(&tim_000); // текущее время

 tim_000.hour=ahour00.hour;
 tim_000.min=ahour00.min;
 tim_000.sec=ahour00.sec;

 ltime0=f_get_time_t(&tim_000);

 if(ltime0 >ltime1) ltime0-=n_sec_day;
 if( f_get_d_dat(ltime0, s_mode) < 0)
 {
  if(ltime0<ltime1)
  {

  if( (ar_str_ptr=f_get_lst_stor()) == NULL) goto m_bad;
  ltime2= f_get_time(ar_str_ptr);

  if(ltime0>ltime2)
   {
    tMassI= s_MVD[0].MassI;
    tVolI = s_MVD[0].VolI;
    f_wr_evt(evt_none);
    goto m_ok;
   }
   else goto m_bad;
  }
  ltime0-=n_sec_day;
  if( f_get_d_dat(ltime0, s_mode) < 0)
  {
  m_bad:
   MmiGotoxy(5,3);   MmiPrintf("Данные отсутствуют");
  }
  else
  {
  m_ok:
 MmiGotoxy(0,0);   MmiPrintf("    Значение отпуска СУГ ");
 MmiGotoxy(0,1);   MmiPrintf(" на последний расчетный час  ");

 MmiGotoxy(0,3);   MmiPrintf("  Дата      Объем,л   Масса,кг");
//MmiGotoxy(0,3);  MmiPrintf("21.02.09 12:10:11  _123456.90");

 MoveToXY(0,5);
    tdat = localtime( &ltime0 );
    MmiPrintf("%02d.%02d.%02d",tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100 );
//MmiGotoxy(18,5);
    if((tMassI != vBIG_P)&&(tVolI != vBIG_P))
    {
       MmiPrintf("% 10.2f %10.2f",tVolI ,tMassI);
    }
    else
       MmiPrintf("     Нет данных ");
  }
 }
 else goto m_ok;
  */
 return;
}
//-------------------------------
void f_prn_date()
{ // показать состояние отпуска на указанную дату
/*
 struct tm *tdat;

int year,month,day,hour,min,sec;
unsigned int seg_tmp,offs_tmp;
struct arch_str *ar_str_ptr;
time_t ltime1,ltime2;

long int ltmp;
 // определение времени последней записи в журнал

 f_prepare_t ( &tim_000);
 ltime1=f_get_time_t(&tim_000); // текущее время

 tim_000=adate00;
 tim_000.hour=ahour00.hour;
 tim_000.min=ahour00.min;
 tim_000.sec=ahour00.sec;

 ltime0=f_get_time_t(&tim_000);

 MmiGotoxy(0,0);   MmiPrintf("    Значение отпуска СУГ ");
 MmiGotoxy(0,1);   MmiPrintf("      на указанную дату  ");

// MmiGotoxy(0,3);   MmiPrintf("  Дата     Время     Масса,кг");
//MmiGotoxy(0,3);  MmiPrintf("21.02.09 12:10:11  _123456.90");
 MmiGotoxy(0,3);   MmiPrintf("  Дата      Объем,л   Масса,кг");

 MoveToXY(0,5);
    tdat = localtime( &ltime0 );
//    MmiPrintf("%02d.%02d.%02d %02d:%02d:%02d",
//    tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);
    MmiPrintf("%02d.%02d.%02d",
    tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100);
//MmiGotoxy(18,5);

 if( f_get_d_dat(ltime0, s_mode) < 0)
 {

  if( (ar_str_ptr=f_get_lst_stor()) == NULL) goto m_bad;

  ltime2= f_get_time(ar_str_ptr);

  if((ltime0<ltime1)&&(ltime0>ltime2) )
  {
   f_wr_evt(evt_none);
   if( f_get_d_dat(ltime0, s_mode) >= 0)
         goto m_ok;
   else goto m_bad;
  }
  else
  {
  m_bad:
       MmiPrintf("     Нет данных ");
  }
 }
 else
 {
  m_ok:
    if((tMassI != vBIG_P)&&(tVolI!= vBIG_P))
    {
       MmiPrintf("% 10.2f %10.2f",tVolI ,tMassI);
    }
    else goto m_bad;
 }
*/
 return;
}
//-------------------------------
char *rcv_mm[]={
"отпуск",
"прием",
};
char *Rcv_mm[]={
"Отпуск",
"Прием",
};
//-------------------------------
int  f_prn_mmi_a(struct arch_str *a_str,int i_n)
{
 int ii,itmp,i_ret;
 float ftmp,ftmp1;
 float Density;
 float Temp   ;
 struct tm *tdat;
 int f_rcv;
union  { float f;long int l; } o;

 if(a_str->event & (int)RCV_FLG)
       f_rcv=1;
 else
       f_rcv=0;


 MoveToXY(0,i_n);
   i_ret=i_n;
   if(i_ret>=n_mmi_str) return i_ret;
//===================================
  if((a_str->event==evt_ext_dt1) || (a_str->event==evt_ext_dt2))
  {
    if(a_str->event==evt_ext_dt1)
     itmp=1;
    else itmp=2;

     o.l=a_str->time;
     ftmp=o.f;

        MmiPrintf("=Рез%d=:Уров.=%6.1fмм;",itmp,a_str->u.fill.VolT);
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
   MoveToXY(0,i_ret);
     MmiPrintf("Т.ждк.=%5.1fC;V.ждк.=%8.1fл ",a_str->u.fill.Amount,a_str->u.fill.UnitPrice);
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
   MoveToXY(0,i_ret);
       MmiPrintf("Mобщ=%7.1fкг;M.ж=%7.1fкг",(a_str->u.fill.Density+a_str->u.fill.Temp),a_str->u.fill.Density);

   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
   MoveToXY(0,i_ret);

         MmiPrintf("P.пар=%5.3f МПа;T.пар=%5.2f C",a_str->VolI,ftmp);

   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
   MoveToXY(0,i_ret);
  ftmp=a_str->MassI;

    if(itmp==1)
         MmiPrintf("Mасса.инв.2 ");
    else
         MmiPrintf("Объем.инв.2 ");

  if(ftmp != vBIG_P)
  {
    if(itmp==1)
         MmiPrintf("%8.1f кг",ftmp);
    else
         MmiPrintf("%8.1f л",ftmp);
  }
  else
     MmiPuts(str_def);

    return i_ret;
  }
//-----------------------------------------------------
  else if(a_str->event==evt_dn_get)
  {
     itmp= a_str->u.fill.Temp;
     MmiPrintf("Ввод плотн.,темп.==Рез.%d==",itmp+1);
     if(itmp==0)
     {
       Temp   = a_str->u.fill.Amount  ;
       Density= a_str->u.fill.VolT    ;
     }
     else if(itmp==1)
     {
     mm_dnsd:
       Temp   = a_str->u.fill.Density  ;
       Density= a_str->u.fill.UnitPrice;
     }
     else
      {
       f_Den_temp( &i_ret,a_str->u.fill.VolT,a_str->u.fill.Amount);
       if(i_ret>=n_mmi_str) return i_ret;
       goto mm_dnsd;
      }
     goto m_dns02;
  }
//===================================
//  MmiPrintf("%02d.%02d.%02d %02d:%02d:%02d" ,a_str->day,a_str->month,a_str->year,a_str->hour,a_str->min,a_str->sec);

  tdat = localtime( &a_str->time );
  MmiPrintf("%02d.%02d.%02d %02d:%02d:%02d",
  tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);

  if(a_str->VolI != vBIG_P)
     MmiPrintf(" %8.1f л" ,a_str->VolI);
  else
     MmiPuts(str_def);
//     MmiPrintf(" ------- ");

   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;

 MoveToXY(0,i_ret);
  if(a_str->MassI != vBIG_P)
     MmiPrintf("%8.1f кг",a_str->MassI);
  else
     MmiPuts(str_def);
//     MmiPrintf(" ------- " );

 switch( (int)( a_str->event & ~(int)RCV_FLG) )
 {
  case evt_start_f:
    MmiPrintf("Старт %sа ",rcv_mm[f_rcv]);

  if(f_rcv!=0)
  {
    if(a_str->u.fill.Amount != 0)
    {
     MmiPrintf("по М.");
    }
    else
    {
     MmiPrintf("по О.");
    }
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);

    if(a_str->u.fill.VolT == vBIG_P)
      MmiPrintf(" Без дозы "   );
    else
      if(a_str->u.fill.Amount != 0)
      {
       MmiPrintf("Mасса %8.3f кг; " ,a_str->u.fill.VolT    );  //%8.3f
      }
      else
      {
       MmiPrintf("Объем %8.3f л; " ,a_str->u.fill.VolT    );  //%8.3f
      }
      goto m_dns;
  }

m_val:

  if(f_rcv ==0)
  {
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_n+2);
    MmiPrintf("О%8.3f л ",a_str->u.fill.VolT    );
    MmiPrintf("M%8.3f кг",a_str->u.fill.Amount  );    // %6.0f
    MmiPrintf("Ц%6g руб/л" ,a_str->u.fill.UnitPrice); // %6.0f

    i_ret=i_n+3;
    if(i_ret>=n_mmi_str) return i_ret;
    MoveToXY(0,i_ret);

    MmiPrintf("Пл.%6.3f кг/м3; " ,a_str->u.fill.Density);
    MmiPrintf("Темп.%6.2fC;" ,a_str->u.fill.Temp);
  }
  else
  {
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
   MoveToXY(0,i_ret);

    MmiPrintf("M=%8.3f кг; " ,a_str->u.fill.Amount  );  //%8.3f
    MmiPrintf("О=%8.3f л; " ,a_str->u.fill.VolT    );  //%8.3f
    goto m_dns;

  }
/*=========================
    if((a_str->u.fill.Density>0)&&(a_str->u.fill.Temp>-300))
    {
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);

     ftmp=f_get_composition(a_str->u.fill.Density,a_str->u.fill.Temp);
     if(ftmp>1.)
      MmiPrintf("!СУГ легче жидкого пропана");
     else if(ftmp<0.)
      MmiPrintf("!СУГ тяжелее жидкого бутана");
     else
      {
       ftmp1=(1.- ftmp)*100.;
            MmiPrintf("СУГ:пропан %6.2f%c,бутан %6.2f%c",(ftmp*100),'%',ftmp1,'%');
      }
    }
 ==========================*/
    break;

  case evt_exm_f:
    MmiPrintf(" Завершено,недолив ");
    goto m_val;
  case evt_exp_f:
    MmiPrintf(" Завершено,перелив");
    goto m_val;
  case evt_end_f:
    MmiPrintf(" Завершено ОК  ");
    goto m_val;
  case evt_brk:
    MmiPrintf(" Данные о нал.восст.");
    goto m_val;

  case evt_est_f:
    MmiPrintf(" Налив прерв.ком-дой");
    goto m_val;
  case evt_err_f:
    MmiPrintf(" Налив прерв.ошибкой");
    goto m_val;

  case evt_ep1_f:
    MmiPrintf(" Налив прерван,Host");
    goto m_val;

  case evt_overfl:
    MmiPrintf(" Налив прерв.переп.");
    goto m_val;

  case evt_ES_off:
    MmiPrintf(" Оп-я прерв.кн. ES ");
    goto m_val;

  case evt_gun:
    MmiPrintf(" Налив прерван,рычаг");
    goto m_val;

  case evt_wd_pmp:
    MmiPrintf(" Налив прерв.WD PMLAN");
    goto m_val;


  case evt_no_dev:
    MmiPrintf(" Старт не принят.Drv");
    goto m_val;

  case evt_clr_err:
    MmiPrintf(" Сброс ошибок. ");
    break;

  case evt_MVD_CL_strt:
    MmiPrintf(" Старт клб.расхдм.");
    break;

  case evt_MVD_CL_BS1 :
    MmiPrintf(" !Поп.клб.расх.-отп");
    break;

  case evt_MVD_CL_BS2 :
    MmiPrintf(" !Сбой стрт.клб.расх.");
    break;

  case evt_MVD_CL_OK :
    MmiPrintf(" Клб.расхдм. OK");
    break;

  case evt_MVD_CL_TM :
    MmiPrintf(" Таймаут клб.расхдм.");
    break;

  case evt_ch_date:
    MmiPrintf(" Дата изм.");
    break;

  case evt_ch_time:
    MmiPrintf(" Время изм.");
    break;
  case evt_ch_sum:
    MmiPrintf(" Пер.на лет.вр.");
    break;
  case evt_ch_win:
    MmiPrintf(" Пер.на зим.вр.");
    break;
  case evt_res:
    MmiPrintf(" Команда Сброс");
    break;
  case evt_verify:
    MmiPrintf(" Старт Поверки");
    break;
  case evt_Slot:
    MmiPrintf(" MVD:Конфиг.Slot");
    break;

  case evt_dsp_grk:
  case evt_kd_grk:
    MmiPrintf(" Показания ГРК");
    goto m_val;

  case evt_deliv_rep:
    MmiPrintf(" Отчет PUMALAN");

   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
    MoveToXY(0,i_ret);
    MmiPrintf("О =%05ld",(long int) a_str->u.fill.VolT  );

   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
    MoveToXY(0,i_ret);
    MmiPrintf("M =%06ld",(long int) a_str->u.fill.Amount);

      i_ret++;
    if(i_ret>=n_mmi_str) return i_ret;
    MoveToXY(0,i_ret);
    MmiPrintf("Ц=%04d",(int)a_str->u.fill.UnitPrice );

    break;

  case evt_chg_param:

    if(a_str->u.fill.VolT == Par_VF)
    {
     MmiPrintf(" !Изм.множ.объема");
    }
    else if(a_str->u.fill.VolT == Par_MF)
    {
     MmiPrintf(" !Изм.множ.массы");
    }
    else if(a_str->u.fill.VolT == Par_PASS)
    {
     MmiPrintf(" !Изменен пароль");
     break;
    }
    else
     {
      MmiPrintf(" !Изм. парам. ");
      MmiPrintf("N%g ",a_str->u.fill.VolT );
     }
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);

    MmiPrintf("Было:%g; ",a_str->u.fill.Amount  );
    MmiPrintf("Стало:%g" ,a_str->u.fill.UnitPrice);
    break;
  case evt_exit:
    MmiPrintf(" Команда Вых.ОС");
    break;
  case evt_re:
    MmiPrintf(" Чтение конф.EEPROM");
    break;
  case evt_re_err:
    MmiPrintf(" Ош.чтения конф.EEPROM");
    break;
  case evt_we:
    MmiPrintf(" Запись конф.EEPROM");
    break;
  case evt_we_err:
    MmiPrintf(" Ош.записи конф.EEPROM");
    break;
  case evt_brk_flt:
    MmiPrintf(" Данные о нал.утеряны");
    break;

  case evt_wrong_start:
    MmiPrintf(" Cтарт не принят.Err");
    goto m_val;

  case evt_dn_strt:
    MmiPrintf(" Нач.изм.плотности ");
    goto m_dns;
  case evt_dn_stop:
    MmiPrintf(" Заверш.изм.плотн. ");
    goto m_dns;
  case evt_dn_ES:
    MmiPrintf(" Изм.плотн.прерв.ES.");
    goto m_dns;

  case evt_dn_err:
    MmiPrintf(" Изм.плотн.прерв.ош.");

m_dns:

   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);
    MmiPrintf("Давление %6.4f МПа;" ,a_str->u.fill.UnitPrice);

m_dns01:
    Temp   = a_str->u.fill.Temp    ;
    Density= a_str->u.fill.Density ;

m_dns02:
/*
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);
    MmiPrintf("Плотность %7.3f кг/м3;" ,Density);
   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);
    MmiPrintf("Температ.%6.2fC;" ,Temp);
*/
    f_Den_temp( &i_ret,Density,Temp);

    break;


  case evt_err:
  case evt_none:
 mm_0:
    itmp=0;
    for(ii=0;ii<=icp_lst_max ;ii++)
      itmp |= a_str->u.err.err[ii];
  if(itmp)
  {
    MmiPrintf(" Ошибка:");
 mm_e:

   i_ret++;
   if(i_ret>=n_mmi_str) return i_ret;
 MoveToXY(0,i_ret);

 MmiPrintf(" %04X" ,a_str->u.err.err[0] );
 MmiPrintf(" %04X" ,a_str->u.err.err[1] );
 MmiPrintf(" %04X" ,a_str->u.err.err[2] );
 MmiPrintf(" %04X" ,a_str->u.err.err[3] );
 MmiPrintf(" %04X" ,a_str->u.err.err[4] );
 MmiPrintf(" %04X" ,a_str->u.err.err[5] );
  }
  else
 MmiPrintf(" Ошибок нет ");

  break;

  case evt_t_on:
 MmiPrintf(" Включение устройства");
    break;
  case evt_t_off:
 MmiPrintf(" Выключение устр-ва");
    break;
  default:
 MmiPrintf(" Событие не известно");
   break;
 }
 return i_ret;
}

//----------------------------------
void f_MVD_M1()
{
// отображает параметры MVD  на MMI

     ff_serv=NULL;
    return;

/*================

  if((MVD_fn[0] != 0) && (MVD_fn[0] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
   MoveToXY(0,sw_MMI_cur); MmiPrintf(" !Таймаут чтения MVD");
     ff_serv=NULL;
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_m1)
  {
  case 20:
   MVD_fn[0]=MVD_RD_L;
   s_MVD[0].r_addr=127;
   sw_mvd_m1=1;
   break;

  case 1:
  if(MVD_fn[0] != F_FAULT)
   {MoveToXY(0,sw_MMI_cur++);MmiPrintf("SN   %ld",s_MVD[0].l);}
  if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};

  case 21:
   MVD_fn[0]=MVD_RD_S;
   s_MVD[0].r_addr=425;
   s_MVD[0].n=8;
   sw_mvd_m1=2;
   break;

  case 2:
  if(MVD_fn[0] != F_FAULT)
  { MoveToXY(0,sw_MMI_cur++); MmiPrintf("S  : %s",s_MVD[0].a);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};

  case 22:
   MVD_fn[0]=MVD_RD_S;
   s_MVD[0].n=8;
   s_MVD[0].r_addr=96;
   sw_mvd_m1=3;
   break;

  case 3:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("TXM:%s",s_MVD[0].a);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};

  case 23:
   MVD_fn[0]=MVD_RD_S;
   s_MVD[0].r_addr=104;
   s_MVD[0].n=16;
   sw_mvd_m1=4;
   break;

  case 4:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("M:%s",s_MVD[0].a); }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 24:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr=16;
   sw_mvd_m1=5;
   break;

  case 5:
   if(MVD_fn[0] != F_FAULT)
    {MoveToXY(0,sw_MMI_cur++);MmiPrintf("TXM версия: %d",s_MVD[0].i);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 25:
   MVD_fn[0]=MVD_RD_S;
   s_MVD[0].r_addr=72;
   s_MVD[0].n=5;
   sw_mvd_m1=6;
   break;

  case 6:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Flow : --%s--",s_MVD[0].a);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 26:
   MVD_fn[0]=MVD_RD_S;
   s_MVD[0].r_addr=80;
   s_MVD[0].n=7;
   sw_mvd_m1=7;
   break;

  case 7:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Тemp.: --%s--",s_MVD[0].a);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 27:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=155;
   sw_mvd_m1=8;
   break;

  case 8:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("D1:                  %.3f",s_MVD[0].f); }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};

  case 28:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=157;
   sw_mvd_m1=9;
   break;

  case 9:
   if(MVD_fn[0] != F_FAULT)
    {MoveToXY(0,sw_MMI_cur++);MmiPrintf("D2:                  %.3f",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 29:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=159;
   sw_mvd_m1=10;
   break;

  case 10:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("K1:                  %.3f",s_MVD[0].f); }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 30:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=161;
   sw_mvd_m1=11;
   break;

  case 11:
   if(MVD_fn[0] != F_FAULT)
    {MoveToXY(0,sw_MMI_cur++);MmiPrintf("K2:                  %.3f",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 31:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=279;
   sw_mvd_m1=12;
   break;

  case 12:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);
     f_prn_Mass();
//     MmiPrintf("Множитель массы:     %.6f",s_MVD[0].f);
   }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 32:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=281;
   sw_mvd_m1=13;
   break;

  case 13:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);
     f_prn_Vol();
//     MmiPrintf("Множитель объема:    %.6f",s_MVD[0].f);
   }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 33:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=283;
   sw_mvd_m1=14;
   break;

  case 14:
   if(MVD_fn[0] != F_FAULT)
    {MoveToXY(0,sw_MMI_cur++);
    // MmiPrintf("Множитель плотности: %.6f",s_MVD[0].f);
      f_prn_Den();
    }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 34:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=195;
   sw_mvd_m1=15;
   break;

  case 15:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Срез масс.расх.:   %.6f",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 35:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=197;
   sw_mvd_m1=16;
   break;

  case 16:
   if(MVD_fn[0] != F_FAULT)
   { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Срез об. расх.:    %.6f",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};

  case 36:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=149;
   sw_mvd_m1=17;
   break;

  case 17:
   if(MVD_fn[0] != F_FAULT)
  { MoveToXY(0,sw_MMI_cur++); MmiPrintf("Срез плотн.:     %.6f",s_MVD[0].f);}
   ff_serv=NULL;
   break;
  }
================*/
}
/* ---------------------------------------- */
void f_MVD_M1f()
{
// отображает параметры коррекции расхода  MVD  на MMI
  if((MVD_fn[i_nn_mvd] != 0) && (MVD_fn[i_nn_mvd] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
     MoveToXY(0,4); MmiPrintf(" !Таймаут чтения MVD%d",i_nn_mvd+1);
     ff_serv=NULL;
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_m1)
  {
  case 31:
   MVD_fn[i_nn_mvd]=MVD_RD_F;
   s_MVD[i_nn_mvd].r_addr=279;
   sw_mvd_m1=12;
   break;

  case 12:
   if(MVD_fn[i_nn_mvd] != F_FAULT)
   { MoveToXY(0,4);
     f_prn_Mass();
//MmiPrintf("Множитель массы:     %.6f",s_MVD[i_nn_mvd].f);
     k_m_flow=s_MVD[i_nn_mvd].f;
   }

  case 32:
   MVD_fn[i_nn_mvd]=MVD_RD_F;
   s_MVD[i_nn_mvd].r_addr=281;
   sw_mvd_m1=13;
   break;

  case 13:
   if(MVD_fn[i_nn_mvd] != F_FAULT)
   { MoveToXY(0,5);
     f_prn_Vol();
//MmiPrintf("Множитель объема:    %.6f",s_MVD[i_nn_mvd].f);
      k_v_flow=s_MVD[i_nn_mvd].f;
   }

  case 33:
   MVD_fn[i_nn_mvd]=MVD_RD_F;
   s_MVD[i_nn_mvd].r_addr=283;
   sw_mvd_m1=14;
   break;

  case 14:
   if(MVD_fn[i_nn_mvd] != F_FAULT)
   { MoveToXY(0,6);
    f_prn_Den();
//MmiPrintf("Множитель  плотности:    %.6f",s_MVD[i_nn_mvd].f);
    k_dens=s_MVD[i_nn_mvd].f;
    ff_serv=NULL;
    Flag_f_corr=1;
    break;
   }
  }
}
/* ---------------------------------------- */
char *unit1=NULL;//I39
char *unit2=NULL;//I45
char *unit3=NULL;//42 // Volume flow codes
char *unit4=NULL;// Volume totalizer or volume inventory codes I 0046
char *unit5=NULL;// Density codes I0040
char *unit6=NULL;// Temperature codes I0041
float MMITemp,MMIDens,MMICompos;

void f_MVD_M2()
{
}
/*=========
char *unit;
int u_cod;
// отображает данные MVD
  if((MVD_fn[0] != 0) && (MVD_fn[0] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
   MoveToXY(0,sw_MMI_cur); MmiPrintf(" !Таймаут чтения MVD");
        ff_serv=NULL;
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_m1)
  {
  case 20:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr=39; // I039 Mass flow codes
   s_MVD[0].i=-1;
   sw_mvd_m1=1;
   break;

  case 1:
   u_cod=s_MVD[0].i;
    if(u_cod>0)  unit1=f_get_unit(u_cod);
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=247;
   sw_mvd_m1++;
   break;

  case 2:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit1 != NULL)
    { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Расход м. : %f %s",s_MVD[0].f,unit1);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }

  case 21:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr=45; // Mass totalizer or mass inventory unit codes I 0045
   s_MVD[0].i=-1;
   sw_mvd_m1=3;
   break;

  case 3:
   u_cod=s_MVD[0].i;
    if(u_cod>0)  unit2=f_get_unit(u_cod);
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=259;
   sw_mvd_m1++;
   break;

  case 4:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit2 != NULL)
    { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Масса общ.: %10.3f %s",s_MVD[0].f,unit2);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 22:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=263;
   sw_mvd_m1=5;
   break;

  case 5:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit2 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Масса инв.: %10.2f %s",s_MVD[0].f,unit2); }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 23:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr= 42 ; // Volume flow codes I0042
   s_MVD[0].i=-1;
   sw_mvd_m1=6;
   break;

  case 6:
   u_cod=s_MVD[0].i;
   if(u_cod>0)  unit3=f_get_unit(u_cod);

   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=253;
   sw_mvd_m1++;
   break;

  case 7:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit3 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Расход об.: %f %s",s_MVD[0].f,unit3);}
     if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 24:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr= 46 ; // Volume totalizer or volume inventory codes I 0046
   s_MVD[0].i=-1;
   sw_mvd_m1=8;
   break;

  case 8:
   u_cod=s_MVD[0].i;
   if(u_cod>0)  unit4=f_get_unit(u_cod);
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=261;
   sw_mvd_m1++;
   break;

  case 9:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit4 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Объем общ.: %10.3f %s",s_MVD[0].f,unit4);}
     if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 25:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=265;
   sw_mvd_m1=10;
   break;

  case 10:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit4 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Объем инв.: %10.2f %s",s_MVD[0].f,unit4);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 26:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr= 40 ; // Density codes I0040
   s_MVD[0].i=-1;
   sw_mvd_m1=11;
   break;

  case 11:
   u_cod=s_MVD[0].i;
   if(u_cod>0)
   {
     unit5=f_get_unit(u_cod);
//     printf("\n\r density unit :%s",unit5);
   }
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=249;
   sw_mvd_m1++;
   break;

  case 12:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit5 != NULL)
    {
      MoveToXY(0,sw_MMI_cur++);MmiPrintf("Плотность  :%.5f %s",s_MVD[0].f,unit5);
//    MMIDens=s_MVD[0].f*K_unit;
      MMIDens=s_MVD[0].f;
    }
   else sw_MMI_cur++;
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  else sw_MMI_cur++;
  case 27:
   MVD_fn[0]=MVD_RD_I;
   s_MVD[0].r_addr= 41 ; // Temperature codes I0041
   s_MVD[0].i=-1;
   sw_mvd_m1=13;
   break;

  case 13:
   u_cod=s_MVD[0].i;
   if(u_cod>0)  unit6=f_get_unit(u_cod);
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=251;
   sw_mvd_m1++;
   break;

  case 14:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit6 != NULL)
   {
    MoveToXY(0,sw_MMI_cur++);MmiPrintf("Темп.потока: %.3f %s",s_MVD[0].f,unit6);
    MMITemp=s_MVD[0].f;
   }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
    if((MMIDens>0)&&(MMITemp>-300))
    {
     MMICompos=f_get_composition(MMIDens,MMITemp);
     if(MMICompos>1)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Смесь легче жидкого пропана");}
     else if(MMICompos<0)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Смесь тяжелее жидкого бутана");}
     else
   {MoveToXY(0,sw_MMI_cur++);MmiPrintf("СУГ,пропан %5.2f%c,бутан %5.2f%c",(MMICompos*100.),'%',(100.-MMICompos*100.),'%' );}
    }
  }
  case 28:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=383;
   sw_mvd_m1=17;
   break;

  case 17:
   if(MVD_fn[0] != F_FAULT)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Темп.платы : %.3f C",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 29:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=385;
   sw_mvd_m1=18;
   break;

  case 18:
   if(MVD_fn[0] != F_FAULT)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Напр.пит.  : %.3f V",s_MVD[0].f);}
   ff_serv=NULL;
   break;
  default:
     ff_serv=NULL;
  }

}
============*/
/* ---------------------------------------- */
/*
void f_MVD_MM2()
{
char *unit;

// отображает данные MVD
  if((MVD_fn[0] != 0) && (MVD_fn[0] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
   MoveToXY(0,sw_MMI_cur); MmiPrintf(" !Таймаут чтения MVD");
        ff_serv=NULL;
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_m1)
  {
  case 20:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=247;
   sw_mvd_m1=2;
   break;

  case 2:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit1 != NULL)
    { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Расход м. : %f %s",s_MVD[0].f,unit1);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }

  case 21:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=259;
   sw_mvd_m1=4;
   break;

  case 4:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit2 != NULL)
    { MoveToXY(0,sw_MMI_cur++);MmiPrintf("Масса общ.: %10.3f %s",s_MVD[0].f,unit2);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 22:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=263;
   sw_mvd_m1=5;
   break;

  case 5:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit2 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Масса инв.: %10.2f %s",s_MVD[0].f,unit2); }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 23:

   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=253;
   sw_mvd_m1=7;
   break;

  case 7:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit3 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Расход об.: %f %s",s_MVD[0].f,unit3);}
     if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 24:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=261;
   sw_mvd_m1=9;
   break;

  case 9:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit4 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Объем общ.: %10.3f %s",s_MVD[0].f,unit4);}
     if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 25:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=265;
   sw_mvd_m1=10;
   break;

  case 10:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit4 != NULL)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Объем инв.: %10.2f %s",s_MVD[0].f,unit4);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  }
  case 26:

   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=249;
   sw_mvd_m1=12;
   break;

  case 12:
  if((MVD_fn[0] != F_FAULT)&&(unit5 != NULL))
  {
     MoveToXY(0,sw_MMI_cur++); MmiPrintf("Плотность  :%.5f %s",s_MVD[0].f,unit5);
     MMIDens=s_MVD[0].f;
  }
  else
  {
     MoveToXY(0,sw_MMI_cur++);MmiPrintf("Ошибка чтения");
  }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 27:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=251;
   sw_mvd_m1=14;
   break;

  case 14:
  if(MVD_fn[0] != F_FAULT)
  {
   if(unit6 != NULL)
    {
     MoveToXY(0,sw_MMI_cur++);MmiPrintf("Темп.потока: %.3f %s",s_MVD[0].f,unit6);
     MMITemp=s_MVD[0].f;
    }
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
    if((MMIDens>0)&&(MMITemp>-300))
    {
     MMICompos=f_get_composition(MMIDens,MMITemp);
     if(MMICompos>1)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Смесь легче жидкого пропана");}
     else if(MMICompos<0)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Смесь тяжелее жидкого бутана");}
     else
   {MoveToXY(0,sw_MMI_cur++);MmiPrintf("СУГ,пропан %5.2f%c,бутан %5.2f%c",(MMICompos*100.),'%',(100.-MMICompos*100.),'%' );}
    }
  }
  case 28:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=383;
   sw_mvd_m1=17;
   break;

  case 17:
   if(MVD_fn[0] != F_FAULT)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Темп.платы : %.3f C",s_MVD[0].f);}
   if(sw_MMI_cur>=sw_MMI_lim)  {ff_serv=NULL;break;};
  case 29:
   MVD_fn[0]=MVD_RD_F;
   s_MVD[0].r_addr=385;
   sw_mvd_m1=18;
   break;

  case 18:
   if(MVD_fn[0] != F_FAULT)
     {MoveToXY(0,sw_MMI_cur++);MmiPrintf("Напр.пит.  : %.3f V",s_MVD[0].f);}
   ff_serv=NULL;
   break;
  default:
     ff_serv=NULL;
  }

}

*/
/* ---------------------------------------- */
void f_MVD_M3()
{
int i,i2,ii;

unsigned long int ltmp,i1;
// отображает ошибки MVD

  ii=i_nn_mvd;

  if((MVD_fn[ii] != 0) && (MVD_fn[ii] != F_FAULT))
  {
    if( f_timer(tm_mvd,f_MVD_tim ) )
    {
   MoveToXY(0,sw_MMI_cur); MmiPrintf(" !Таймаут чтения MVD %d",ii+1);
        ff_serv=NULL;
    }
    return;
  }
  tm_mvd=TimeStamp;
  switch(sw_mvd_m1)
  {
  case 0:

   MVD_fn[ii]=MVD_RD_F;
   s_MVD[ii].r_addr=245;
   sw_mvd_m1++;
   break;

  case 1:
   if(MVD_fn[ii] != F_FAULT)
   {
     ltmp=(unsigned long int)s_MVD[ii].f &0xffffff;
     fl_zeroing=ltmp;
     if(ltmp==0)
     {
        MoveToXY(8,3); MmiPrintf("MVD %d.Ошибок нет.",ii+1);
        ff_serv=NULL;
        return;
     }
     else
     {
       i2=0;
       for(i=0,i1=1;i<24;i++,i1=i1<<1)
       {
         if(ltmp & i1)
         {
          i2++;
         }
       }

     if(sw_mmi!=18)
     {
      MoveToXY(0,0);
/*
      if(i2==1)
          MmiPrintf(" Ошибка расходомера N%d :",ii+1);
      else if( i>1)
*/
          MmiPrintf(" Ошибки расходомера N%d:",ii+1);
       i2=1;
     }
     else
     {

      MoveToXY(0,0);
         MmiPrintf("Сост./ошибки расходомера N%d:",ii+1);
       i2=2;
     }
       for(i=0,i1=1;i<24;i++,i1=i1<<1)
       {
         if(ltmp & i1)
         {
           MoveToXY(0,i2);
           MmiPuts(str_empty);
           MoveToXY(0,i2);
           if(i==0x11) MmiPrintf("Калибровка расходомера");
           else if(i==0x13) MmiPrintf("Сброс по включению питания");
           else if(i==0x15) MmiPrintf("Инициализация трансмиттера");
           else MmiPrintf("!Err %d",i);
//         MmiPrintf("!%s",err_F425R[i]);

           i2++;
           if(i2>=7)
           {
            ff_serv=NULL;
            return;
           }
         }
       }
       for(;i2<6;i2++)
       {
           MoveToXY(0,i2);
           MmiPuts(str_empty);
       }
     }
   }
   ff_serv=NULL;
   break;
  }
}
//------------------------
/*

char *err_F425R[]={
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
//------------------------
/* ---------------------------------------- */
//-------------------------------

char *list1_dsr[]={

"P рсхдм.смещ.,МПа",        //  0
"P рез1.смещ.,МПа",         //  1
"P рез2.смещ.,МПа",         //  2
"T рез1.смещ.,C",           //  3
"T рез2.смещ.,C",           //  4
"KM1_D",               //  5

"7017/1:Статус",                 //  6
"D1,кг/м3",            //  7
"Т1,C",              //  8
"D2,кг/м3",             //  9
"T2,C",               // 10
"KM1_R",            // 11

"P рсхдм.полн.шк,МПа",          // 12
"P рез1.полн.шк,МПа",           // 13
"P рез2.полн.шк,МПа",           // 14
"T рез1.полн.шк,C",             // 15
"T рез2.полн.шк,C",             // 16
"",              // 17

"7060/1:Статус",                 // 18
"",            // 19
"",             // 20
"",              // 21
"",             // 22
"",               // 23

"7060/2:Статус",          // 24

"P рез3.полн.шк,МПа",     // 25
"P рез3.смещ.,МПа",       // 26
"T рез3.полн.шк,C",       // 27
"T рез3.смещ.,C",         // 28
"",              // 29

"Тип изм.плотн.  ",                // 30
"Кол-во резер-в  ",                // 31
"Тип колонки(0,1)",                // 32
"Упр.розеткой    ",                // 33

"Колич.расхдм    ",     // 34
"",           // 35
"",            // 36

"",                // 37
"",           // 38
"",            // 39
"",           // 40
"",              // 41

"Флаг поверки ",                // 42
"",           // 43
"",            // 44
"",           // 45
"",              // 46

"",           // 47
"",        // 48

"",           // 49
"",        // 50

"MVD1:Статус",                 //  51
"MVD2:Статус",                 //  52
"LIN:Статус",                  //  53
"",              //  54
"",             //  55

"TSD:Статус",                 //  56
"",            //  57
"",                  //  58
"",              //  59
"",             //  60

"",                // 61
"",                // 62
"",                // 63
"",                // 64

"",           // 65
"",           // 66
"",           // 67
"",           // 68

"",           // 69
"",           // 70
"",           // 71
"",           // 72

"",           // 73
"",              // 74
"",           // 75
"",           // 76

"",        // 77
"",// "Номер входа Gun",         // 78
"",// "Инвер-я входа Gun",       // 79

"Клапан Блш N вых.",        // 80
"Клапан Мнш N вых.",        // 81
"Флаг расш.отчета.",        // 82
"Адрес PUMALAN",            // 83
"Статус PUMALAN",           // 84
"",        // 85
"",        // 86
"",        // 87
"",           // 88
"Переход на лет.вр.",       // 89
"------------------",       // 90
"",//"Знаков после зпт.1",       // 91
"",//"Знаков после зпт.2",       // 92
"",//"Знаков после зпт.3",       // 93
"Параметр индик. 1",        // 94
"Параметр индик. 2",        // 95
"Параметр индик. 3",        // 96
"",//"Обновл-е индик, мс",        // 97
"ост. объем кл.1, л",           // 98    p1-p6.CLV
"ост. объем кл.2, л",           // 99
"T срабат-я кл.1,мс",       // 100
"T срабат-я кл.2,мс",       // 101
"нач.запр.оп.MMI,мс",      // 102
"длит.запр.о.MMI,мс",          // 103
"подстр.отп. 0...1",        // 104   p1-p3.KA
"подстр.прм. 0...1",        // 105
"зад-ка зап.отч.,мс",       // 106

"",       // 107    p1.CTRL
"",       // 108    p2.CTRL
"",        // 109    p3.CTRL

"",           // 110    p1-p5.DSC
"",        // 111
"",       // 112
"",            // 113
"",           // 114

"",           // 115
"",           // 116
"",           // 117
"",         // 118
"",          // 119
"",        // 1 2 0

"",        // 121
"",       // 122
"T зад.вкл.кл1,  мс",        // 123
"",       // 124
"",        // 1 2 5

"Round add M",        // 126
"",        // 127
"",             // 128
"",        // 129
"Ex_p",         // 130

"1",        // 131
"2",        // 132
"3",        // 133
"MAX недолив,л",              // 134
"MAX перелив,л",              // 135
"WD пер.PUMALAN, мс",        // 136
// FLCH
"V опред.переп BIG",        // 137
"T переполн.,   тик",         // 138
"V нач.кнтр.прп,  л",       // 139
"T запрета прп, тик",       // 140

"T зад.вкл.кл2,  мс",         // 141
"T зад.выкл.нас.,мс",        // 142
"Flow last,l/ms",         // 143
"с",                      // 144
"3",                      // 145
"Насос, N выхода",      // 146
"Клап.Запорн.N вых.",   // 147
"V опред.переп LTL",    // 148
"Flow min,l/ms    ",    // 149
"Round Flag",    // 150
"Round add",    // 151
"Round lim",    // 152
"Round summ",    // 153
"Round summ M",    // 154
"WD смв.PUMALAN,мс",    // 155

"",
};
//-------------------------------
//#define mmi_page   page_MMI_c
#define mmi_page   page_MMI
char mmi_point=0;
//#define size_max 29
int mmi_prm=0;
int mmi_flag=0;

int num_page=0.;
int sw_mmi=0;
int mmi_str=0;
int mmi_com=1;
int flag_chg=0;
int nn_chg=0;
unsigned int pass_cnt;
int max_par=1;
#define NumPlace 22

int FlagWinSum=0;

struct arch_str_t adate11;

struct dis_set_MMI *ds_list=NULL;

int fl_ch_out=0;

long int Password=5555;
long int PassW=0;
int k_ext=0;
int k_ext1=0;
float chg_nn_par=0,chg_old=0,chg_new=0;
int f_menu_MMI()
{
  time_t ltime1;
  time_t ltime2;
  int year,month,day,hour,min,sec;
  int key;
  char str[31];
  int i,j,k,ii;
  char ch_tmp;
  struct s_icp_dev *ICPl_DEV;
  float ftmp1,ftmp2;
struct arch_str *ar_str_ptr;
long int ltmp;
 struct tm *tdat;

   if((i=fstin())==fst_OK)
   {  // введено числовое значение
     key=DATA;  // признак окончания ввода строки
     sw_fst=0;
   }
   else if(i==fst_ESC)
   {
     key=DATA_ESC;  // признак окончания ввода строки
     sw_fst=0;
   }
   else if( MmiKbhit())
   {  // ввод поступившего символа из клавиатуры
    key=MmiGetch();
   }
   else if(sw_fst==0)
   {
    key=PASS; // признак холостого прохода (без введенного символа)
              // используется для индикации изменяющихся значений
   }
   else return 0;

   switch(sw_mmi)
    {
     case 0: /*заголовок*/
/*==============================================
       else if(key==F2)
        {
       m_0f2:
          MmiGotoxy(1,1);
          f_clr_scr_MMI();
          MmiPuts("Введите номер страницы:" );
          sprintf( fst_str,"%d",num_page);
          fst_n=2;
          sw_fst=1;
//        MmiGotoxy(10,3);
          MmiGotoxy(15,3);
#if defined(MMI_ICP)
          SetDisplayPage(15); //  ввод значения
#endif

#if defined(MMI_NEW)
          SetDisplayPage(EmptPage); // ввод значения
#endif
          break;
        }
       else if(key==DATA)
        {
          sscanf(fst_str,"%d",&num_page);
          if(num_page<0) { num_page=0; goto m_0f2;}
          else if(num_page > 63) {num_page=63; goto m_0f2;}

          SetDisplayPage((int)num_page);
          sw_mmi=100;
          f_clr_scr_MMI();
          break;
        }
       else if(key==DATA_ESC)
        {
          goto m_beg0;
        }
======================================*/
//---------------------------------
        if (key==Sht_ESC)
        {  // очистка ошибок

          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);
          sw_mmi=26;
          f_cl_error();
#if defined(MMI_ICP)
          MmiGotoxy(5,2);    MmiPuts("   Ошибки очищены   ");
          MmiGotoxy(5,4);    MmiPuts("Enter - продолжить ");
#else
          MmiGotoxy(5,6);    MmiPuts("   Ошибки очищены   ");
         MmiGotoxy(5,10);    MmiPuts("Enter - продолжить ");

#endif

        }
        else if(key==Sht_F3)
        {  // MVD zeroing
          i_nn_mvd=1;
          goto m_sf2;
        }
        else if(key=='/')
        {  // MVD zeroing
          i_nn_mvd=2;
          goto m_sf2;
        }
        else if(key==Sht_F2)
        {  // MVD zeroing
          i_nn_mvd=0;
m_sf2:
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);


          if(fl_GO == 1)
          {
           f_wr_evt(evt_MVD_CL_BS1);
           MoveToXY(3,3);
           MmiPrintf("!Выполняется отпуск СУГ");
           MoveToXY(3,4);
           MmiPrintf("!Калибровка невозможна");
            //    |                              |
           goto m_SF2;
          }
          if(f_reset_zer_MVD(i_nn_mvd) == 0)
          {
          f_wr_evt(evt_MVD_CL_BS2);
          MoveToXY(0,3);
          MmiPrintf(" Ошибка запуска калибровки ");
  m_SF2:
          f_d_ESC();
          sw_mmi=-1;
          break;
          }

          f_wr_evt(evt_MVD_CL_strt);
          sw_mmi=18;
          zero_time=TimeStamp;
          fl_zeroing=1;
          break;
        }

        else if((key==F1) && (F_GR2==0) )
        {
          PageD=2;
          Flag_bot=1;
          f_paged2();
          goto m_pass0;

        }
        else if(key==F2)
        {
          PageD=0;
          Flag_bot=1;
          f_paged0();
          goto m_pass0;

        }
        else if(key==F3)
        {
            PageD=1;
          Flag_bot=1;
            f_paged1();
          goto m_pass0;
        }
        else if(key=='1')
        {
          PageD=4;
          Flag_bot=1;
          f_paged3();
          goto m_pass0;
        }
        else if(key=='2')
        {
         if(Tank_nn < 2) goto m_pp;

          PageD=5;
          Flag_bot=1;
          f_paged3();
          goto m_pass0;
        }
        else if(key=='3')
        {
         if(Tank_nn < 2) goto m_dd;
         else if(Tank_nn < 3) goto m_pp;

          PageD=6;
          Flag_bot=1;
          f_paged3();
          goto m_pass0;
        }
        else if(key=='4')
        {
         if(Tank_nn == 2) goto m_dd;
         else if(Tank_nn == 3)
         {
m_pp:
          if(F_GR2==0)
          {
            PageD=3;
            Flag_bot=1;
            f_paged3();
          }
          goto m_pass0;
         }
        }
        else if(key=='5')
        {
         if(Tank_nn == 3)
         {
m_dd:
          if(F_GR2==0)
          {
            PageD=7;
            Flag_bot=1;
            f_paged3();
          }
           goto m_pass0;
         }
        }
        else if (key==Sht_F1)
        {  // распечатка ошибок

          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);
          sw_mmi=26;
          f_disp_error();
        }

        else if (key== '+')
        {  // disable res ctrl
          if(PageD==4)
            res_ctr[0]=1;
          else if(PageD==5)
            res_ctr[1]=1;
          else if(PageD==6)
            res_ctr[2]=1;
        }
        else if (key== '-')
        {  // enable res ctrl
          if(PageD==4)
            res_ctr[0]=0;
          else if(PageD==5)
            res_ctr[1]=0;
          else if(PageD==6)
            res_ctr[2]=0;
        }
       else if(key==ESC)
       {


m0_f1:
main_menu:

          Flag_bot=0;
          PageD=0;
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);
          f_prepare_t (&adate00);
          sw_mmi=120;
          Flag_check = 0;
          PassW=0;

            MmiGotoxy(0,0);    MmiPuts("          Меню ");


#if defined(MMI_ICP)

        if( CMD_IN != CMD_DIS_RCV  )
          {
           if(F_GR2==0)
           {
             MmiGotoxy(0,1);
            if(MVD[0].status !=0 )
            {
                MmiPuts("1  Прием СУГ");
            }
            else
                MmiPuts("1  Прием СУГ (не доступен)");
            MmiGotoxy(0,2);    MmiPuts("2  Измерение плотности СУГ");
           }
          }

            MmiGotoxy(0,3);    MmiPuts("3  Просмотр журнала событий");
            MmiGotoxy(0,4);    MmiPuts("4  Сервисные функции");

//          MmiGotoxy(0,4);    MmiPuts("4  Состояние отпуска СУГ");
//          MmiGotoxy(0,5);    MmiPuts("5  Состояние расходомера");
            f_d_ESC();
#else
        if( CMD_IN != CMD_DIS_RCV  )
          {
           if(F_GR2==0)
           {
             MmiGotoxy(0,3);
            if(MVD[0].status !=0 )
            {
                MmiPuts("1  Прием СУГ");
            }
            else
                MmiPuts("1  Прием СУГ (не доступен)");
            MmiGotoxy(0,5);    MmiPuts("2  Измерение плотности СУГ");
           }
          }
            MmiGotoxy(0,7);    MmiPuts("3  Просмотр журнала событий");
            MmiGotoxy(0,9);    MmiPuts("4  Сервисные функции");

//          MmiGotoxy(0,4);    MmiPuts("4  Состояние отпуска СУГ");
//          MmiGotoxy(0,5);    MmiPuts("5  Состояние расходомера");
            f_d_ESC();

#endif


          break;
       }
       else if(key==PASS)
        {  // отображение даты и времени
         if( ((FL_err|CRC_err) == 0) | ( Flag_bot != 0))
         {
  m_pass0:
          if(PageD == 0)
          {
            f_paged0a();
          }
          else if(PageD==1)
          {
            f_paged1a();
          }
          else if(PageD==2)
          {
             f_paged2a();
          }
          else if(PageD==3)
          {
            f_dsp_p1(1);
          }
          else if(PageD==4)
          {
           f_dsp_res( 0);
          }
          else if(PageD==5)
          {
           f_dsp_res( 1);
          }
          else if(PageD==6)
          {
           f_dsp_res( 2);
          }
          else if(PageD==7)
          {
           f_dsp_dn();
          }
         }
         GetDate(&year,&month,&day);GetTime(&hour,&min,&sec);
#if defined(MMI_ICP)
         MoveToXY(4,6);
#else
         MoveToXY(4,15);
#endif
         MmiPrintf("%02d.%02d.%02d   %02d:%02d:%02d" ,day,month,year-2000 ,hour,min,sec);
        }

        break;
 /*========================================*/
    case 11:  /*меню выбора точки для просмотра параметров */
       if(key==ESC)    /* переход в меню M3, Сервисные функции */
       {
         goto m_m3;
       }
       else if(key=='1')  // Базовый список #1
        {
         f_before_MMI();
         ds_list=ds_list1;
         max_par=max_list1;
         mmi_prm=0;
        }
       else if(key=='2')  // Базовый список #2
        {
         f_before_MMI();
         ds_list=ds_list1;
         max_par=max_list1;
         mmi_prm=max_par/5;
        }
       else if(key=='3')   // Базовый список #3
        {
         f_before_MMI();
         ds_list=ds_list1;
         max_par=max_list1;
         mmi_prm=(max_par*2)/5;
        }
       else if(key=='4')  // Базовый список #4
        {
         f_before_MMI();
         ds_list=ds_list1;
         max_par=max_list1;
         mmi_prm=(max_par*3)/5;
        }
       else if(key=='5')  // Дополнительный список
        {
         f_before_MMI();
         ds_list=ds_list1;
         max_par=max_list1;
         mmi_prm=(max_par*4)/5;
        }
       else if(key=='6')  // Дополнительный список
        {
         f_before_MMI();
         ds_list=ds_list1;
         max_par=max_list1;
         mmi_prm=max_par-6;
        }
       else break;
       flag_chg=0;
       nn_chg=0;

       f_clr_scr_MMI();
       SetDisplayPage(EmptPage);    // Чистая страница
  par_out14:
       for(i=0;i<n_mmi_str_1;i++)
       {

        k=mmi_prm+i;
        if(k>= max_par) continue;
        MoveToXY(0,i);
           MmiPrintf("%2d %s",k+1,ds_list[k].descr[ds_list[k].ind]);
        MoveToXY(NumPlace,i);
           PrintMmiOp(&ds_list[k],1);
       }
       if(flag_chg==0)
       {
         MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\"Пред \"F3\"След \"Enter\"Изм. ");
       }
       else
       {
         MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\",\"F3\",\"ESC\",\"Enter\"         ");
         MmiGotoxy(2,nn_chg);  MmiPuts("#");
       }
       sw_mmi=14;
       break;
 /*========================================*/
    case 14:/*просмотр до 6 параметров*/
         if(key==Sht_F2)    /* сохранение изменений в EEPROM */
         {
          pass_ret=10;
mm_14:
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          MmiGotoxy(0,1);    MmiPuts("Сохранение данных в EEPROM");
          MmiGotoxy(0,3);    MmiPuts(" 1    Сохранить");
          MmiGotoxy(0,4);    MmiPuts("ESC   Не сохранять,продолжить");
          sw_mmi=15;
          break;
         }
         if(key==Sht_F3)    /* чтение данных из EEPROM */
         {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          MmiGotoxy(0,1);    MmiPuts("Чтение данных из EEPROM");
          MmiGotoxy(0,3);    MmiPuts(" 1    Прочитать ");
          MmiGotoxy(0,4);    MmiPuts("ESC   Не считывать,продолжить");
          sw_mmi=16;
          break;
         }
       if(flag_chg==0)
       {
         if(key==ESC)    /* переход в меню выбора точки для просмотра*/
         {
          goto m_m3_3;
         }
         else if(key== F2 )
           if(mmi_prm==0);
           else if(mmi_prm>=n_mmi_str_1)
           {
            mmi_prm -=n_mmi_str_1;
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto par_out14;
           }
           else
           {
            mmi_prm = 0;
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto par_out14;
           }
         else if((key== F3 )&& (mmi_prm < (max_par-n_mmi_str_1)) )
         {
          mmi_prm +=n_mmi_str_1;
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto par_out14;
         }
         if(key== ENTER)
         {
          flag_chg=1;

          if((nn_chg+mmi_prm) >= max_par)
             nn_chg=max_par-mmi_prm-1;
          goto par_out14;
         }
       }
       else  // flag_chg!=0
       {
         if(key==ESC)    // очистка флага редактирования
         {
          flag_chg=0;
          goto par_out14;
         }
         if((key== F2 )&& (nn_chg>0) )
         {
          nn_chg -=1;
          goto par_out14;
         }
         else if((key== F3 )&& (nn_chg < n_mmi_str_2) )
         {
          nn_chg +=1;

          if((nn_chg+mmi_prm) >= max_par)
             nn_chg=max_par-mmi_prm-1;
          goto par_out14;
         }
         if(key==ENTER)    // редактирование параметра
         {
          if(PassW != Password)
          {
           pass_ret=0;
m_pass:
           f_clr_scr_MMI();
#if defined(MMI_NEW)
          SetDisplayPage(EmptPage); // ввод значения
#endif
#if defined(MMI_ICP)

#else
          MmiGotoxy(1,4);
          MmiPuts("Введите пароль:" );
#endif



           sprintf( fst_str,"******");
#if defined(MMI_ICP)
          SetDisplayPage(17);  // Ввод пароля
#endif
           sw_mmi=121;
           fst_n=7;
           sw_fst=1;
#if defined(MMI_ICP)
          MmiGotoxy(10,3);
#else
          MmiGotoxy(10,7);
#endif
           break;
          }

          MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"1...9,SHT-7\"-Ввод,\"SHT-4\"-Уд.");
//        MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"ESC - взврат");
          MmiGotoxy(2,nn_chg);  MmiPuts(">");
          MoveToXY(NumPlace,nn_chg);

          PrintMmiOp(&ds_list[nn_chg+mmi_prm],0);
          fst_n=8;
          sw_fst=1;
          break;
         }
       }
       if(key==DATA)    // ввод полученных данных
       {
         ScanfMmiOp(&ds_list[nn_chg+mmi_prm]);
         f_after_MMI();
         chg_nn_par=nn_chg+mmi_prm+1;
         f_wr_evt(evt_chg_param);
m_14_ni:
         MoveToXY(NumPlace,nn_chg); MmiPuts("         ");
         goto par_out14;
       }
       else if(key==DATA_ESC)
        {
          goto m_14_ni;
        }
       break;
 /*========================================*/
    case 15:/* сохранение данных в EEPROM */
         if((key==ESC)||(key==ENTER)  )   /* возврат к отображению параметров */
         {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          if(pass_ret==10)
            goto par_out14;
          else
            goto m_m5;
         }
         else if(key=='1')    // сохранить данные
         {

           if(f_wrt_eee()<0)
           {
              f_wr_evt(evt_we_err);
              MmiGotoxy(0,5);   MmiPuts("Ошибка при сохранении данных");
           }
           else
           {
              f_wr_evt(evt_we);
              MmiGotoxy(0,5);   MmiPuts(" Данные сохранены успешно");
           }
            f_wr_cor();
            MmiGotoxy(0,6);   MmiPuts("      Enter - продолжить");
         }
         break;
 /*========================================*/
    case 16:/* чтение данных из EEPROM */
         if((key==ESC)||(key==ENTER)  )   /* возврат к отображению параметров */
         {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto par_out14;
         }
         else if(key=='1')    // прочитать
         {

           f_chk_EEE_CRC();
           if(f_rd_eee()<0)
           {
              f_wr_evt(evt_re_err);
              MmiGotoxy(0,5);   MmiPuts("Ошибка при чтении данных");
              CRC_err=1;
           }
           else
           {
              f_wr_evt(evt_re);
              MmiGotoxy(0,5);   MmiPuts("  Данные считаны успешно");
              CRC_err=0;
           }
           f_rd_cor();
              MmiGotoxy(0,6);   MmiPuts("      Enter - продолжить");
         }
        break;
 /*========================================*/
    case 17: // отображение буфера канала
    {
         if(key==ESC)    /* переход в меню выбора точки для просмотра*/
         {
           PageD=0;
           goto m0_f1;
         }
         else if(key== F2 )
         {

            mmi_prm -=n_mmi_str;

            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto m_m8_d;

         }
         else if( key== F3 )
         {
          mmi_prm +=n_mmi_str;
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto m_m8_d;
         }
         else if(key== '5' )
         {
            mmi_prm +=10;
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto m_m8_d;
         }
         else if(key== '6' )
         {
            mmi_prm +=100;
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto m_m8_d;
         }
         else if(key== '7' )
         {
            mmi_prm +=1000;
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto m_m8_d;

         }
         else if(key== '1' )
         {
            mmi_prm -=10;
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto m_m8_d;
         }
         else if(key== '2' )
         {
            mmi_prm -=100;
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto m_m8_d;
         }
         else if(key== '3' )
         {
            mmi_prm -=1000;
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto m_m8_d;

         }
         else if(key== '0' )
         {

            cost_v_c=cost_v;
            flag_cst_chg=0;
            cnt_ltch=0;
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto m_m8_d;
         }
         else if(key== '9' )
         {

            cost_v_c=cost_v;
            flag_cst_chg=1;
            cnt_ltch=10;
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);    // Чистая страница
            goto m_m8_d;
         }
         else if(key== F1 )
         {
          PageD ^= 1;
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto m_m8_d;
         }
        break;
    }
 /*========================================*/
    case 18: // калибровка
       if( f_timer(zero_time,Time_zeroing ))
       {
          f_clr_scr_MMI();
          MoveToXY(5,3);
            MmiPrintf(" Таймаут калибровки");
             //   |                              |
          fl_zeroing=0;
          ff_serv=NULL;
          f_wr_evt(evt_MVD_CL_TM);
          goto m_SF2;
       }
          if(ff_serv==NULL)
          {
           if(fl_zeroing==0)
           {
             f_clr_scr_MMI();
             MoveToXY(1,3);
         MmiPrintf("Калибровка завершена успешно");
//                |                              |
             f_wr_evt(evt_MVD_CL_OK);
             goto m_SF2;
           }
           fl_rd_err=0;
           sw_mvd_m1=0;  // нач.параметр
           ff_serv=f_MVD_M3;
          }

         GetDate(&year,&month,&day);GetTime(&hour,&min,&sec);
         MoveToXY(4,6);
         MmiPrintf("%02d.%02d.%02d   %02d:%02d:%02d" ,day,month,year-2000 ,hour,min,sec);
         break;
 /*========================================*/
    case 120:  // Основное меню

       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
           sw_mmi=0;
           SetDisplayPage(ZeroPage);
           f_clr_scr_MMI();
           f_prn_begin();
           break;
       }

      else if(key== '1')   // 1  Прием  СУГ
      {
       if( (CMD_IN != CMD_DIS_RCV ) && (F_GR2==0) && (MVD[0].status !=0 ) )   // 1  Прием  СУГ
       {

         Reg_State  &=  ~( DEN_fin | RCV_fin | RCV_flch);

m_rcv_liq:
         flag_dlv_fst=0;
         flag_prn_mass=0;
         MassStamp=s_MVD[0].MassI;
         MassStamp2=s_MVD[1].MassI;
         VolStamp=s_MVD[0].VolI;
         VolStamp2=s_MVD[1].VolI;
         dose_dlvr=0;
         PageD=0;
         f_prepare_t ( &adate00);
m_m6:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
        if(Flag_check == 0)
        {
            MmiGotoxy(0,0);
            MmiPuts("   Меню 1.Прием СУГ.");
        }
         f_prn_MassSt_rcv();

         if(Flag_check != 0)
         {
          MmiGotoxy(0,3);    MmiPuts("2 Старт отсчета ");
         }
         else
         {
            MmiGotoxy(0,2);    MmiPuts("1 Прием дозы СУГ");
            MmiGotoxy(0,3);    MmiPuts("2 Прием без дозы");
         }

          if(flag_prn_mass != 0)
          {  MmiGotoxy(0,4);    MmiPuts("3 Продолжить:");

             if(Flag_check == 0)
             {
                MmiGotoxy(0,5);
                if(dose_dlvr != vBIG_P)
                    MmiPrintf("Д=%8.2f кг ",dose_dlvr);
                else
                  MmiPrintf("Без дозировки ");
            MmiGotoxy(15,5);   MmiPrintf("П=%8.2f кг    ",s_MVD[0].MassT+s_MVD[1].MassT);
             }
             else
             {
            MmiGotoxy(14,5);   MmiPrintf("П=%9.3f кг    ",s_MVD[0].MassT+s_MVD[1].MassT);
             }
          }

//          MmiGotoxy(0,6);    MmiPuts("ESC  Возврат , F1 - краны ");
          f_d_ESC();
         sw_mmi=200;
         break;
       }
       else break; // Прием не поддерживается
      }
       else if((key== '*') &&  (CMD_IN != CMD_DIS_RCV ) )   //  Sht_1 Поверка
       {
        Flag_check = 1;
        f_wr_evt(evt_verify);
        goto m_rcv_liq;
       }
       else if((key== '2') &&  (CMD_IN != CMD_DIS_RCV ) && (F_GR2==0))   // 2  Измерение плотности
       {

        Reg_State  &=  ~( DEN_fin | RCV_fin);

        f_clr_scr_MMI();
        SetDisplayPage(EmptPage);

        MmiGotoxy(0,0); MmiPrintf("  Выбор номера резервуара ");

        MmiGotoxy(0,2); MmiPrintf("1  Изм. плотности в рез. N1");
        if(Tank_nn > 1 )
        {
          MmiGotoxy(0,3); MmiPrintf("2  Изм. плотности в рез. N2");
        }
        if(Tank_nn > 2 )
        {
          MmiGotoxy(0,4); MmiPrintf("3  Изм. плотности в рез. N3");
        }
        f_d_ESC();
        sw_mmi=209;
        break;
       }
       else if(key== '<') // 3  Ошибки расходомера 2
       {
         i_nn_mvd=1;
         goto m_m61;
       }
       else if(key== '>') // 3  Ошибки расходомера 3
       {
         i_nn_mvd=2;
         goto m_m61;
       }
       else if(key== '6') // 3  Ошибки расходомера 1
       {
         i_nn_mvd=0;
m_m61:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
          f_d_ESC();
         sw_mvd_m1=0;
         ff_serv=f_MVD_M3;
         sw_mmi=123;
         break;
       }
       else if(key== '3') // 2  Просмотр журнала событий
       {
m_m2:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();

            i= f_get_max_n_arch();
            ar_str_ptr=f_get_fst_stor();


#if defined(MMI_ICP)

            MmiGotoxy(0,0);    MmiPuts(" Меню 3.Журнал событий.");
            MmiGotoxy(0,1);    MmiPrintf("%4d  ",i);
            j=i%100;
            if((j<=20)&& (j>5))
              MmiPrintf("записей");
            else
            switch(i%10)
            {
             case 1:
             MmiPrintf("запись");
             break;

             case 2:
             case 3:
             case 4:
             MmiPrintf("записи");
             break;

             case 0:
             case 5:
             case 6:
             case 7:
             case 8:
             case 9:
             MmiPrintf("записей");
             break;
            }

   tdat = localtime( &ar_str_ptr->time );
   MmiPrintf(" с %02d.%02d.%02d",
   tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100);

            MmiGotoxy(0,2);    MmiPuts(" Просмотр журнала событий");
            MmiGotoxy(0,3);    MmiPuts("1  По дате");
            MmiGotoxy(0,4);    MmiPuts("2  По номеру записи");
            MmiGotoxy(0,5);    MmiPuts("3  Последние записи");

#else

            MmiGotoxy(0,0);    MmiPuts(" Меню 3.Журнал событий.");
            MmiGotoxy(0,2);    MmiPrintf("%4d  ",i);
            j=i%100;
            if((j<=20)&& (j>5))
              MmiPrintf("записей");
            else
            switch(i%10)
            {
             case 1:
             MmiPrintf("запись");
             break;

             case 2:
             case 3:
             case 4:
             MmiPrintf("записи");
             break;

             case 0:
             case 5:
             case 6:
             case 7:
             case 8:
             case 9:
             MmiPrintf("записей");
             break;
            }

   tdat = localtime( &ar_str_ptr->time );
   MmiPrintf(" с %02d.%02d.%02d",
   tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100);

            MmiGotoxy(0,4);    MmiPuts(" Просмотр журнала событий");
            MmiGotoxy(0,6);    MmiPuts("1  По дате");
            MmiGotoxy(0, 8);    MmiPuts("2  По номеру записи");
            MmiGotoxy(0,10);    MmiPuts("3  Последние записи");
#endif

            f_d_ESC();

         PassW=0;
         sw_mmi=140;
         break;
       }
       else if((key== '4')&&( f_chk_EN() )  ) // 6  Сервисные функции
       {
m_m3:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
#if defined(MMI_ICP)
            MmiGotoxy(0,0);    MmiPuts(" Меню 4.Сервисные функции.");
            MmiGotoxy(0,1);    MmiPuts("1  Список драйверов");
            MmiGotoxy(0,2);    MmiPuts("2  Статистика  драйверов");
            MmiGotoxy(0,3);    MmiPuts("3  Просмотр/ввод параметров");
            MmiGotoxy(0,4);    MmiPuts("4  Другие функции");
            MmiGotoxy(0,5);    MmiPuts("5  Время, дата");
#else
            MmiGotoxy(0,0);    MmiPuts(" Меню 4.Сервисные функции.");
            MmiGotoxy(0,3);    MmiPuts("1  Список драйверов");
            MmiGotoxy(0,5);    MmiPuts("2  Статистика  драйверов");
            MmiGotoxy(0,7);    MmiPuts("3  Просмотр/ввод параметров");
            MmiGotoxy(0,9);    MmiPuts("4  Другие функции");
            MmiGotoxy(0,11);    MmiPuts("5  Время, дата");

#endif

            f_d_ESC();
         sw_mmi=150;
         PassW=0;
       }
       else if((key== '.')&&( f_chk_EN() )   )//  Sht-7 Коррекция расхода 2
       {
         i_nn_mvd=1;
         goto m_m5;
       }
       else if((key== 'l')&&( f_chk_EN() )   )//  Sht-0  Коррекция расхода 3
       {
         i_nn_mvd=2;
         goto m_m5;
       }
       else if((key== '7')&&( f_chk_EN() )   )//  7  Коррекция расхода 1
       {
         i_nn_mvd=0;

m_m5:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
            MmiGotoxy(0,0);    MmiPrintf(" Меню 7.Коррекция расхода%d.",i_nn_mvd+1);
            MmiGotoxy(0,1);    MmiPuts("1 Коррекция массового расхода");
            MmiGotoxy(0,2);    MmiPuts("2 Коррекция объемного расхода");
            MmiGotoxy(0,3);    MmiPuts("3 Коррекция плотности");
  //        MoveToXY(0,4);   MmiPrintf("Множитель массы:     %.6f",s_MVD[0].f);
  //        MoveToXY(0,5);   MmiPrintf("Множитель объема:    %.6f",s_MVD[0].f);
            f_d_ESC();

         Flag_f_corr=0;
         sw_mvd_m1=31;
         tm_mvd=TimeStamp;
         ff_serv=f_MVD_M1f;
         sw_mmi=125;
         break;
       }
       else if(key== '9') // 7 Отображение дискретных входов/выходов
       {
m_m7:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();

  MmiGotoxy(0,0); MmiPrintf(" Состояния входов-выходов:");

          f_d_ESC();

//    MmiGotoxy(0,7);    MmiPuts(str_ESC); ///qqq
         dsp_tmp_out=0xffff;
         dsp_tmp_in=0xffff;
 //      sw_dns=20;
         sw_mmi=25;
         f_wr_evt(evt_ch_outb);
         break;
       }


       else if(key== '8') // 8 Отображение буфера канала PUMALAN
       {
m_m8:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         max_par=pmln_b_lgth;
         mmi_prm=uk_fin_pm-n_mmi_str;
m_m8_d:
         mmi_prm &= pmln_b_lgth_1;
         for(i=0;i<n_mmi_str;i++)
         {
           MoveToXY(0,i);
           k=(mmi_prm+i)& pmln_b_lgth_1;

           ii= (int)(bf_pml[k].type_n) & 0x1f;
           if(ii == 0)
           {
             MmiPrintf("... ");
             continue;
           }

           if(PageD)
           {
              tdat = localtime( &bf_pml[k].time );
              MmiPrintf("%02d.%02d %02d:%02d:%02d",
              tdat->tm_mday,tdat->tm_mon+1,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);

              if(bf_pml[k].type_n & M_WR)
                MmiPrintf(" Ответ  ");
              else
                MmiPrintf(" Запрос ");
               ltmp=bf_pml[k].time_m & 0x0ffffL;
              MmiPrintf(" %6.3f ", ((float)(ltmp))/1000.);
           }
           else
           {
            for(j=0;j<ii;j++)
            {
             ch_tmp=bf_pml[k].messg[j];
             if(ch_tmp==STX)
                 MmiPutch(STX_c);
             else if(ch_tmp==ETX)
                 MmiPutch(ETX_c);
             else if(ch_tmp==EOT)
                 MmiPutch(EOT_c);
             else if(ch_tmp==ACK)
                 MmiPutch(ACK_c);
             else if(ch_tmp==NAK)
                 MmiPutch(NAK_c);
             else if(ch_tmp==ENQ)
                 MmiPutch(ENQ_c);
             else
              MmiPutch(ch_tmp);
            }

              if(bf_pml[k].type_n & M_WR);
              else if(ii != 1)
                 MmiPutch(ETX_c);

           }
         }

         sw_mmi=17;
         break;

       }
/*
       else
       {
          if(f_chk_EN())
          {
              MmiGotoxy(0,4);    MmiPuts("4  Сервисные функции");
              MmiGotoxy(0,5);    MmiPuts("5  Коррекция расхода");
          }
          else
          {
              MmiGotoxy(0,4);    MmiPuts(str_empty);
              MmiGotoxy(0,5);    MmiPuts(str_empty);
          }
       }
*/
       break;
 /*========================================*/
    case 121: // // ввод пароля на изменение данных
       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
 m121:
          switch(pass_ret)
          { // пароль не введен
          case 0:  goto par_out14;
          case 1:  goto m_m5;
          case 2:  goto m_m5;
          case 4:  goto m_m5;
          case 3:  goto m_m3;
          default:
                goto main_menu;
          }
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%ld",&ltmp);
          if(ltmp == Password)
                PassW=Password;
          else goto m_pass;
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          switch(pass_ret)
          { // пароль введен корректно
          case 0:  goto par_out14;
          case 1:
                sw_mmi=125;
                goto m_m5_1;
          case 2:
                sw_mmi=125;
                goto m_m5_2;
          case 3:
                sw_mmi=162;
                goto m_m3_6_1;
          case 4:
                sw_mmi=125;
                goto m_m5_4;
          default:
                goto main_menu;
          }
        }
       else if(key==DATA_ESC)
        {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto m121;
        }

 /*========================================*/
    case 122:
 //  Отображение Плотности,температуры,состава
 //  Текущее значение.
       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
         goto m_m3_4;
       }
       else if(key==PASS)
       {
         f_prn_den_t();
       }
       break;
 /*========================================*/
    case 123:

       if((key==ESC)||(key==ENTER))  /* переход на начальную страницу */
       {
         goto main_menu;
       }
       break;
 /*========================================*/
 /*========================================*/
    case 125:
 //  Меню 5.Коррекция расхода.
       if((key==ESC)||(key==ENTER) )    /* переход в меню */
       {
         sw_fst=0;
         ff_serv=NULL;
         goto main_menu;
       }

       if((key=='1')&&(Flag_f_corr!=0))
       {

          pass_ret=1;
          if(PassW != Password)
          {
           goto m_pass;
          }

m_m5_1:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPrintf("Введите коррекцию массы%d:",i_nn_mvd+1 );
//        k_mv_tmp=k_m_flow;
          k_mv_tmp=M_fct_d[i_nn_mvd];
m_m5_1_1:

          MmiGotoxy(0,n_mmi_str_2);  MmiPuts("\"1...9,SHT-7\"-Ввод,\"SHT-4\"-Уд.");
          MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"ESC - возврат");

          sprintf( fst_str,"%.6f",k_mv_tmp);
          fst_n=9;
          sw_fst=1;
          MmiGotoxy(10,3);
          break;
       }
       else if(key==Sht_F2)    /* сохранение изменений в EEPROM */
       {
        goto mm_14;
       }
       else if((key=='2')&&(Flag_f_corr!=0))
       {
          pass_ret=2;
          if(PassW != Password)
          {
           goto m_pass;
          }
m_m5_2:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPrintf("Введите коррекцию объема%d:",i_nn_mvd+1 );
          k_mv_tmp=V_fct_d[i_nn_mvd];
          goto m_m5_1_1;
       }
       else if((key=='3')&&(Flag_f_corr!=0))
       {
          pass_ret=4;
          if(PassW != Password)
          {
           goto m_pass;
          }
m_m5_4:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPrintf("Введите коррекцию плотности %d:",i_nn_mvd+1 );
          k_mv_tmp=D_fct_d[i_nn_mvd];
          goto m_m5_1_1;
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%f",&k_mv_tmp);
          if((k_mv_tmp<0.8) || (k_mv_tmp > 1.2))
           {
             switch(pass_ret)
             {

               case 1:
                        goto m_m5_1;
               case 2:
                        goto m_m5_2;
               case 4:
                        goto m_m5_4;
               default:
                      goto m_m5;
             }
           }

      // введенное значение в k_mv_tmp
          if(pass_ret==1)
          {
           s_MVD[i_nn_mvd].r_addr=279; // Множитель массы.

           chg_nn_par=Par_MF;
           chg_old=k_m_flow;
           chg_new= k_mv_tmp;
           M_fct_d[i_nn_mvd]= k_mv_tmp;
           f_wr_evt(evt_chg_param);
          }
          else if(pass_ret==2)
          {
           s_MVD[i_nn_mvd].r_addr=281; // Множитель объема.

           chg_nn_par=Par_VF;
           chg_old=k_v_flow;
           chg_new= k_mv_tmp;
           V_fct_d[i_nn_mvd]= k_mv_tmp;
           f_wr_evt(evt_chg_param);
          }
          else if(pass_ret==4)
          {
           s_MVD[i_nn_mvd].r_addr=283; // Множитель плотности

           chg_nn_par=Par_DN;
           chg_old=k_dens;
           chg_new= k_mv_tmp;
           D_fct_d[i_nn_mvd]= k_mv_tmp;
           f_wr_evt(evt_chg_param);
          }
          MVD_fn[i_nn_mvd]=MVD_WR_F;;
          s_MVD[i_nn_mvd].f = k_mv_tmp;
          goto m_m5;
        }
       else if(key==DATA_ESC)
        {
          goto m_m5;
        }

       break;
 /*========================================*/
 /*========================================*/
    case 140: // 2  Просмотр журнала событий

       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
           goto main_menu;
       }
        else if (key==DEL)
        {

          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          MmiGotoxy(0,1);    MmiPuts(" 1 - Перезагрузка");
          MmiGotoxy(0,2);    MmiPuts(" 6 - Выход из программы в ОС");
          MmiGotoxy(0,3);    MmiPuts(" для загрузки новой версии");
          MmiGotoxy(0,5);    MmiPuts("ESC   отмена");
          sw_mmi=142;
          break;
        }
       else if(key== '1')  //1  По дате
       {
m_m2_1:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
#if defined(MMI_ICP)
           MmiGotoxy(0,0);    MmiPuts("   Дата записи журнала ");
           MmiGotoxy(0,1);    MmiPuts("      День.Мес.Год");
        // MmiGotoxy(0,2);    MmiPuts("        08.01.2009");
            MmiGotoxy(0,4);    MmiPuts(" 0...9,'.' - изменить");
            MmiGotoxy(0,5);    MmiPuts(" Enter - ввод");
            MmiGotoxy(0,6);    MmiPuts(" ESC   - отменить");
            MmiGotoxy(8,2);
#else

           MmiGotoxy(0,1);    MmiPuts("   Дата записи журнала ");
           MmiGotoxy(0,3);    MmiPuts("      День.Мес.Год");
        // MmiGotoxy(0,4);    MmiPuts("        08.01.2009");
            MmiGotoxy(0,8);    MmiPuts(" 0...9,'.' - изменить");
            MmiGotoxy(0,9);    MmiPuts(" Enter - ввод");
            MmiGotoxy(0,10);    MmiPuts(" ESC   - отменить");
            MmiGotoxy(8,4);

#endif

      sprintf(fst_str,"%02d.%02d.%04d",(int)adate00.day,(int)adate00.month,(int)(adate00.year+2000) );
          fst_n=11;
          sw_fst=1;
          sw_mmi=143;
          break;
       }
       else if(key== '2')  //2  По номеру записи
       {
m_m2_2:
          max_par= f_get_max_n_arch();
          MmiGotoxy(1,1);
          f_clr_scr_MMI();
          MmiPuts("Введите номер записи:" );
          sprintf( fst_str,"%d",max_par);
          fst_n=4;
          sw_fst=1;
          MmiGotoxy(15,3);
#if defined(MMI_ICP)
          SetDisplayPage(15); //  ввод значения
#endif

#if defined(MMI_NEW)
          SetDisplayPage(EmptPage); // ввод значения
#endif
          break;
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%d",&i);
          if(i<1) { goto m_m2_2;}
          else if(i>max_par) { goto m_m2_2;}
          mmi_prm=i-1;
          goto m_m2_3v;
        }
       else if(key==DATA_ESC)
        {
          goto m_m2;
        }
       else if(key== '3')  //3  Последние записи
       {
m_m2_3:
         max_par= f_get_max_n_arch();
         mmi_prm=max_par-d_F2;

m_m2_30:
m_m2_3v:
         if(mmi_prm>(max_par-1))
            mmi_prm=max_par-2;
         if(mmi_prm<0) mmi_prm=0;
         f_clr_scr_MMI();
         SetDisplayPage(EmptPage);    // Чистая страница

   //----------------
       for(n_pp=0,k=0;k<n_mmi_str_2;n_pp++,k++)
       {
        if((mmi_prm+n_pp)>=max_par)
        {
          n_pp-=1;
          k=0;
          break;
        }
        ar_str_ptr= f_get_astr(mmi_prm+n_pp);
        k=f_prn_mmi_a(ar_str_ptr,k);
       }

       if(k==n_mmi_str_2);
       else n_pp-=1;
       if(n_pp<1) n_pp=1;

   //----------------
m_m2_32:
        MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\"Пред \"F3\"След \"ESC\"Отмена");

        sw_mmi=141;
       }
       break;
 /*========================================*/
    case 141:/*просмотр 2 записей журнала*/
         if(key==ESC)    /* переход в меню выбора точки для просмотра*/
         {
          goto m_m2;
         }
         else if(key== F2 )
           if(mmi_prm==0);
           else if(mmi_prm>=d_F2)
           {
            mmi_prm -=d_F2;
            goto m_m2_3v;
           }
           else
           {
            mmi_prm = 0;
            goto m_m2_3v;
           }
         else if (key== F3 )
         {
           max_par= f_get_max_n_arch();
           if(mmi_prm <= (max_par-n_pp))
           {
               mmi_prm +=n_pp;
            goto m_m2_3v;
           }
         }
    break;
 /*========================================*/
    case 142:/* выход из программы */
             /* возврат к просмотру */
       if((key==ESC)||(key==ENTER))    /* переход в меню */
         {
          goto m_m2;
         }
         else if(key== '1' )
         { //  1 - Перезагрузка
           f_wr_evt(evt_res);

  StopUserTimerFun();
  RestoreCom(1);
  RestoreCom(2);
  RestoreCom(3);
  while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */
  RestoreCom(4);
//f_free_SlvBuf();

          _asm cli
          _asm hlt
         }
         else if(key== '6' )
         {// 6 - Выход из программы в ОС
            f_wr_evt(evt_exit);
            return 1; // выход из программы *.exe в ОС
         }
         break;
 /*========================================*/
    case 143:

 //  Ввод даты
       if((key==ESC)||(key==ENTER)||(key==DATA_ESC))    /* переход в меню */
       {
         goto m_m2;
       }
       else if(key==DATA)
        {
          year=(int)(adate00.year+2000);
          month=(int)adate00.month;
          day=(int)adate00.day;

          sscanf(fst_str,"%d.%d.%d",&day,&month,&year);
          if((year<2000)||(year > 2100)) goto m_m2_1;
          if((month<1)|| (month > 12))  goto m_m2_1;
          if((day<1)|| (day > 31))  goto m_m2_1;

             adate00.year = year-2000 ;
             adate00.month = month;
             adate00.day = day;
            // дата введена

m_m2_1_1:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
#if defined(MMI_ICP)
           MmiGotoxy(0,0);    MmiPuts("   Время записи журнала ");
           MmiGotoxy(0,1);    MmiPuts("       Час Мин.");
        // MmiGotoxy(0,2);    MmiPuts("        08.01");
            MmiGotoxy(0,4);    MmiPuts(" 0...9,'.' - изменить");
            MmiGotoxy(0,5);    MmiPuts(" Enter - ввод");
            MmiGotoxy(0,6);    MmiPuts(" ESC   - отменить");
            MmiGotoxy(8,2);
#else

           MmiGotoxy(0,1);    MmiPuts("   Время записи журнала ");
           MmiGotoxy(0,3);    MmiPuts("       Час Мин.");
        // MmiGotoxy(0,4);    MmiPuts("        08.01");
            MmiGotoxy(0,8);    MmiPuts(" 0...9,'.' - изменить");
            MmiGotoxy(0,9);    MmiPuts(" Enter - ввод");
            MmiGotoxy(0,10);    MmiPuts(" ESC   - отменить");
            MmiGotoxy(8,4);

#endif
      sprintf(fst_str,"%02d.%02d",(int)adate00.hour,(int)adate00.min );
          fst_n=6;
          sw_fst=1;
          sw_mmi=144;
       }
          break;

 /*========================================*/
    case 144:
 //  Ввод времени
       if((key==ESC)||(key==ENTER)||(key==DATA_ESC))    /* переход в меню */
       {
         goto m_m2_1;
       }
       else if(key==DATA)
        {
          hour=(int)adate00.hour;
          min=(int)adate00.min;

          sscanf(fst_str,"%d.%d",&hour,&min);
          if((min<0)|| (min > 59))  goto m_m2_1_1;
          if((hour<0)|| (hour > 23))  goto m_m2_1_1;


             adate00.min = min;
             adate00.hour = hour;
             adate00.sec = 0;
            // время введено

            SetDisplayPage(EmptPage);
            f_clr_scr_MMI();

         ltime1=f_get_time_t(&adate00);
         k=f_get_dat_n(ltime1);
         if(k<0)
         {
           ltime2=f_get_time(f_get_fst_stor());
           if(ltime1<=ltime2)
           {
             k=0;
           }
           else k=f_get_max_n_arch();
         }
         mmi_prm=k;
         max_par= f_get_max_n_arch();
         goto m_m2_30;
        }
         break;
 /*========================================*/
    case 150: // 4  Сервисные функции

       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
           goto main_menu;
       }

       else if(key== '1')  // 1  Список драйверов
       {
         mmi_str=0;
         max_par=icp_lst_max;
m20_2:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         f_drv_list_MMI(&mmi_str);
         sw_mmi=22;
       }
       else if(key== '2')  // Статистика  драйверов
       {
         mmi_str=0;
         max_par=icp_lst_max;
m20_3:
         SetDisplayPage(EmptPage);  // чистый экран
         f_clr_scr_MMI();
         f_drv_stat_MMI(&mmi_str);
         pass_cnt=0;
         sw_mmi=23;
       }
       else if(key== '3')   //  3  Просмотр/ввод параметров
       {
m_m3_3:

   //     PassW=0;
        // переход в меню выбора точки для просмотра параметров
//        SetDisplayPage(11);
         SetDisplayPage(EmptPage);  // чистый экран
         f_clr_scr_MMI();
          sw_mmi=11;
          f_clr_scr_MMI();
            MmiGotoxy(0,0);    MmiPuts(" Выбор номера параметра ");
            MmiGotoxy(0,3);    MmiPrintf("1...6  N1 ... N%d",max_list1);
          f_d_ESC();
          break;
       }
       else if(key== '4')   //  4  Другие функции
       {
m_m3_4:
         SetDisplayPage(EmptPage);  // чистый экран
         f_clr_scr_MMI();
#if defined(MMI_ICP)
            MmiGotoxy(0,0);    MmiPuts(" Меню 4.4.Другие функции. ");
            MmiGotoxy(0,1);    MmiPuts("1  Версия программн.обеспеч.");
            MmiGotoxy(0,2);    MmiPuts("2  Поиск ICP/RTU модулей");
            MmiGotoxy(0,3);    MmiPuts("3  Список найденных модулей");
#else
            MmiGotoxy(0,0);    MmiPuts(" Меню 4.4.Другие функции. ");
            MmiGotoxy(0,4);    MmiPuts("1  Версия программн.обеспеч.");
            MmiGotoxy(0,6);    MmiPuts("2  Поиск ICP/RTU модулей");
            MmiGotoxy(0,8);    MmiPuts("3  Список найденных модулей");
            MmiGotoxy(0,10);   MmiPuts("4  Проверка расчета объема фаз");

#endif

            f_d_ESC();
         sw_mmi=160;
          break;
       }
       else if(key== '5')   //  5  Время, дата
       {
m_m3_5:
         f_prepare_t ( &adate00);
         adate11=adate00;
         SetDisplayPage(EmptPage);  // чистый экран
         f_clr_scr_MMI();
#if defined(MMI_ICP)
           MmiGotoxy(0,0);    MmiPuts("      Текущая дата ");
           MmiGotoxy(0,1);    MmiPuts("      День.Мес.Год");
        // MmiGotoxy(0,2);    MmiPuts("        08.01.2009");
           MmiGotoxy(0,3);
if(FlagWinSum==0)             MmiPuts("Переход лет/зим.время выключен");
else                          MmiPuts("Переход лет/зим.время включен");

            MmiGotoxy(0,4);   MmiPuts(" 0...9,'.' - изменить");
            MmiGotoxy(0,5);   MmiPuts(" Enter - ввод");
            MmiGotoxy(0,6);   MmiPuts(" ESC   - отменить");
            MmiGotoxy(8,2);
#else

           MmiGotoxy(0,1);    MmiPuts("      Текущая дата ");
           MmiGotoxy(0,3);    MmiPuts("      День.Мес.Год");
        // MmiGotoxy(0,4);    MmiPuts("        08.01.2009");
           MmiGotoxy(0,6);
if(FlagWinSum==0)             MmiPuts("Переход лет/зим.время выключен");
else                          MmiPuts("Переход лет/зим.время включен");

            MmiGotoxy(0,8);   MmiPuts(" 0...9,'.' - изменить");
            MmiGotoxy(0,9);   MmiPuts(" Enter - ввод");
            MmiGotoxy(0,10);   MmiPuts(" ESC   - отменить");
            MmiGotoxy(8,4);

#endif

      sprintf(fst_str,"%02d.%02d.%04d",(int)adate00.day,(int)adate00.month,(int)(adate00.year+2000) );
          fst_n=11;
          sw_fst=1;
          sw_mmi=165;
          break;
       }
       else if(key== '6')   //  6  Изменить пароль
       {
m_m3_6:
         SetDisplayPage(EmptPage);  // чистый экран
         f_clr_scr_MMI();
            MmiGotoxy(0,0);    MmiPuts(" Меню 5.6.Изменение пароля ");
            MmiGotoxy(0,2);    MmiPuts("1  Изменить пароль");
            f_d_ESC();
         sw_mmi=162;
          break;
       }
       break;
 /*========================================*/
    case 160: // 4  Другие функции

       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
           goto m_m3;
       }
       else if(key== '1') // 8 Версия sw
       {
          SetDisplayPage(EmptPage);
          f_clr_scr_MMI();
          MmiGotoxy(0,0);   MmiPrintf("   VER %s",sw_ver);
          MmiGotoxy(0,2);   MmiPrintf("   MD5 counting ...");
          f_d_ESC();
          tm_md=TimeStamp;
          sw_mmi=271;
          break;
       }
//27.05.2020 YN --\\//--
       else if(key== '4') // 8 Версия sw
       {
m_m4:         
          SetDisplayPage(EmptPage);
          f_clr_scr_MMI();
          MmiGotoxy(0,0);    MmiPrintf("Проверка расчета объема фаз");

          f_count_tank_ver_calc();

          MmiGotoxy(0,2);    MmiPrintf("1.Объем р-ра: ");
          MmiGotoxy(13, 2);     MmiPrintf(" %8.1f л",s_CALC.VolTank);

          MmiGotoxy(0,3);    MmiPrintf("2.Объем ж.ф.: ");
          MmiGotoxy(13, 3);     MmiPrintf(" %8.1f л",s_CALC.VolLiqPhase);

          MmiGotoxy(0,4);    MmiPrintf("3.Темп. ж.ф.: ");
          MmiGotoxy(13, 4);     MmiPrintf(" %8.1f град.C",s_CALC.TempLiqPhase);

          MmiGotoxy(0,5);    MmiPrintf("4.Темп. п.ф.: ");
          MmiGotoxy(13, 5);     MmiPrintf(" %8.1f град.C",s_CALC.TempGasPhase);

          MmiGotoxy(0,6);    MmiPrintf("5.Плотн.ж.ф.: ");
          MmiGotoxy(13, 6);     MmiPrintf(" %8.1f кг/м3",s_CALC.DensLiqPhase);

          MmiGotoxy(0,7);    MmiPrintf("6.Давл. р-ра: ");
          MmiGotoxy(13, 7);     MmiPrintf(" %8.3f МПа",s_CALC.PressTank);

          MmiGotoxy(0,9);    MmiPrintf("Давл. н.пара: ");
          MmiGotoxy(13, 9);     MmiPrintf(" %8.3f МПа",s_CALC.PressGasPhase);

          MmiGotoxy(0,10);    MmiPrintf("Плотн.пар.ф.:");
          MmiGotoxy(13, 10);     MmiPrintf(" %8.3f кг/м3",s_CALC.DensGasPhase);

          MmiGotoxy(0,11);   MmiPrintf("Объем пар.ф.:");
          MmiGotoxy(13, 11);     MmiPrintf(" %8.3f л",s_CALC.VolGasPhase);

          MmiGotoxy(0,12);   MmiPrintf("Масса пар.ф.:");
          MmiGotoxy(13,12);     MmiPrintf(" %8.3f кг",s_CALC.MassGasPhase);

          MmiGotoxy(0,13);   MmiPrintf("Масса ждк.ф.:");
          MmiGotoxy(13,13);     MmiPrintf(" %8.3f кг",s_CALC.MassLiqPhase);

          MmiGotoxy(0,14);   MmiPrintf("Масса общая :");
          MmiGotoxy(13,14);     MmiPrintf(" %8.3f кг",(s_CALC.MassGasPhase+s_CALC.MassLiqPhase));

          MmiGotoxy(0,15);      MmiPuts("                 ESC  Возврат ");
          tm_md=TimeStamp;
          sw_mmi=272;
          break;
       }
//27.05.2020 YN --//\\--
/*
       else if(key== '>') //  1  Показания расходомера
       {
         max_par=10;
         mmi_prm=0;

m_m3_4_1v:

         if(ff_serv==NULL)
         {
          SetDisplayPage(EmptPage);
          f_clr_scr_MMI();
          MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\"Пред \"F3\"След \"ESC\"Отмена");
          sw_MMI_cur=0; // строка MMI
          sw_mvd_m1=mmi_prm+20;  // нач.параметр
          sw_MMI_lim=6; // конечная строка
          MMIDens=-1;
          MMITemp=-300;
          ff_serv=f_MVD_M2;
          sw_mmi=171;
         }
         else
          sw_mmi=173;
         break;
       }

       else if(key== '2') //  2  Параметры расходомера
       {
         max_par=17;
         mmi_prm=0;

m_m3_4_2v:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\"Пред \"F3\"След \"ESC\"Отмена");

         if(ff_serv==NULL)
         {
          sw_MMI_cur=0; // строка MMI
          sw_mvd_m1=mmi_prm+20;  // нач.параметр
          sw_MMI_lim=6; // конечная строка

          ff_serv=f_MVD_M1;
          sw_mmi=172;
         }
         else
          sw_mmi=174;
         break;
       }


       else if(key== '3') // 3 Плотность,темп.,состав
       {
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         f_prn_den_t();
         sw_mmi=122;
         break;

       }
*/
       else if(key== '3') //  5  Список подключенных модулей
       {
         mmi_str=0;
         max_par=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
m20_1:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         f_mod_list_MMI(&mmi_str);
         sw_mmi=21;
       }
       else if(key== '2')  // 4  Поиск ICP/RTU модулей
       {
m20_4:
          f_clr_scr_MMI();
          MmiGotoxy(1,1);
          MmiPuts("Введите номер порта : " );
          sprintf( fst_str,"%2d",mmi_com);
          fst_n=3;
          sw_fst=1;
          MmiGotoxy(13,3);
#if defined(MMI_ICP)
          SetDisplayPage(15); //  ввод значения
#endif

#if defined(MMI_NEW)
          SetDisplayPage(EmptPage); // ввод значения
#endif
          break;
       }
       else if(key== DATA )
       {
          sscanf(fst_str,"%d",&mmi_com);  // номер COM
          if((mmi_com>0)&&(mmi_com<4)) licp_port=mmi_com;
          else
          {
            if(mmi_com<1)  mmi_com=1;
            else if (mmi_com>3)  mmi_com=3;
            goto m20_4;
          }

          if(ComProt[mmi_com]==0)
          {
            licp_sw=1;
            ff_serv=f_look_ICP;
            flag_print_licp=0;
            LICP_Rqst.timeout=ToutLicpScan;
            licp_crc_flag=LICP_Rqst.CRC_flag;
          }
          else
          {
            licp_sw=1;
            ff_serv=f_look_RTU;
            flag_print_licp=0;
            LICP_Rqst_RTU.timeout=ToutLicpScan;
          }

       f_clr_scr_MMI();
       SetDisplayPage(EmptPage);

       if(ComProt[mmi_com]==0)
       {
         MmiGotoxy(0,0);     MmiPrintf("ICP COM%d: таймаут %d мс",licp_port,ToutLicpScan);
         MmiGotoxy(0,1);  MmiPrintf("Макс.Адр %d,флаг CRC=%d",licp_amax,LICP_Rqst.CRC_flag);
       }
       else
       {
         MmiGotoxy(0,0);     MmiPrintf("RTU COM%d: таймаут %d мс",licp_port,ToutLicpScan);
         MmiGotoxy(0,1);  MmiPrintf("Макс.Адр %d",licp_amax);
       }

       MmiGotoxy(0,6);  MmiPuts("  Enter - продолжить");
       sw_mmi=24;
       break;
       }
       else if(key==DATA_ESC)
        {
          goto m_m3_4;
        }
       break;
 /*========================================*/
    case 162:
 //  Изменение пароля
       if((key==ESC)||(key==ENTER)||(key==DATA_ESC))    /* переход в меню */
       {
         goto m_m3;
       }
       else if(key== '1')   // 1  Изменить пароль
       {

          if(PassW != Password)
          {
           pass_ret=3;
           goto m_pass;
          }
m_m3_6_1:

          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите новый пароль(1-й раз):" );
           pass1=-1;
           sprintf( fst_str,"******");
           SetDisplayPage(17);    // Ввод пароля
           sw_mmi=163;
           fst_n=7;
           sw_fst=1;
           MmiGotoxy(11,3);
           break;
       }
       break;
 /*========================================*/
    case 163: // // ввод нового пароля 1 раз
       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
 m163:
          goto m_m3;
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%ld",&pass1);
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
           // пароль введен корректно
           if(pass1>= 0)
           {
             MmiGotoxy(0,1);
             SetDisplayPage(EmptPage); // ввод значения
             f_clr_scr_MMI();
             MmiPuts("Введите новый пароль(2-й раз):" );
             pass2=-1;
             sprintf( fst_str,"******");
             SetDisplayPage(17);    // Ввод пароля
             fst_n=7;
             sw_fst=1;
             MmiGotoxy(11,3);
             sw_mmi=164;
             break;
           }
           else goto m_m3_6_1;
        }
       else if(key==DATA_ESC)
        {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto m163;
        }

 /*========================================*/
    case 164: // // ввод нового пароля 2 раз
       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto m163;
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%ld",&pass2);
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
           // пароль введен корректно
           if(pass2 == pass1)
           {
             chg_nn_par=Par_PASS;
             chg_old=Password;
             chg_new= pass1;
             Password=pass1;
             f_wr_evt(evt_chg_param);

             if(f_wrt_eee()<0)
             {
                f_wr_evt(evt_we_err);
                MmiGotoxy(1,3);   MmiPuts("Ошибка при сохранении пароля");
             }
             else
             {
                f_wr_evt(evt_we);
                MmiGotoxy(3,3);   MmiPuts("Пароль сохранен успешно");
             }
             f_wr_cor();
             MmiGotoxy(0,6);   MmiPuts("      Enter - продолжить");
             break;
           }
           else goto m_m3_6_1;

        }
       else if(key==DATA_ESC)
        {
          f_clr_scr_MMI();
          SetDisplayPage(EmptPage);    // Чистая страница
          goto m163;
        }

 /*========================================*/


 /*========================================*/
    case 165:

 //  Ввод даты для изменения
       if((key==ESC)||(key==ENTER)||(key==DATA_ESC))    /* переход в меню */
       {
         goto m_m3;
       }
       else if(key==DATA)
        {
          year=(int)(adate00.year+2000);
          month=(int)adate00.month;
          day=(int)adate00.day;

          sscanf(fst_str,"%d.%d.%d",&day,&month,&year);
          if((year<2000)||(year > 2100)) goto m_m3_5;
          if((month<1)|| (month > 12))  goto m_m3_5;
          if((day<1)|| (day > 31))  goto m_m3_5;

             adate00.year = year-2000 ;
             adate00.month = month;
             adate00.day = day;
            // дата введена
           if((adate00.year  != adate11.year ) ||
              (adate00.month != adate11.month) ||
              (adate00.day   != adate11.day  ) )
           {

// printf("\n\r %02d.%02d.%02d" ,day,month,year-2000);

            f_wr_evt(evt_none);
            SetDate( year, month, day);

            WriteNVRAM((int)nr_year  ,(int)(year-2000));
            WriteNVRAM((int)nr_month ,(int)month  );
            WriteNVRAM((int)nr_day   ,(int)day    );
            init_win_sum(0);
            f_wr_evt(evt_ch_date);
            adate11=adate00;
//          printf("\n\r Date have changed ");
           }

m_m3_5_1:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
#if defined(MMI_ICP)
           MmiGotoxy(0,0);    MmiPuts("      Текущее время ");
           MmiGotoxy(0,1);    MmiPuts("       Час Мин Сек");
        // MmiGotoxy(0,2);    MmiPuts("         08.01.00");
           MmiGotoxy(0,3);  MmiPuts("Переход лет/зим.время ");
if(FlagWinSum==0)             MmiPuts("выключен");
else                          MmiPuts("включен");

            MmiGotoxy(0,4);    MmiPuts(" 0...9,'.' - изменить");
            MmiGotoxy(0,5);    MmiPuts(" Enter - ввод");
            MmiGotoxy(0,6);    MmiPuts(" ESC   - отменить");
            MmiGotoxy(8,2);
#else

           MmiGotoxy(0,1);    MmiPuts("      Текущее время ");
           MmiGotoxy(0,3);    MmiPuts("       Час Мин Сек");
        // MmiGotoxy(0,4);    MmiPuts("         08.01.00");
           MmiGotoxy(0,6);  MmiPuts("Переход лет/зим.время ");
if(FlagWinSum==0)             MmiPuts("выключен");
else                          MmiPuts("включен");

            MmiGotoxy(0,8);    MmiPuts(" 0...9,'.' - изменить");
            MmiGotoxy(0,9);    MmiPuts(" Enter - ввод");
            MmiGotoxy(0,10);   MmiPuts(" ESC   - отменить");
            MmiGotoxy(8,4);

#endif

      sprintf(fst_str,"%02d.%02d.%02d",(int)adate00.hour,(int)adate00.min,(int)adate00.sec);
          fst_n=9;
          sw_fst=1;
          sw_mmi=166;
       }
          break;

 /*========================================*/
    case 166:
 //  Ввод времени
       if((key==ESC)||(key==ENTER)||(key==DATA_ESC))    /* переход в меню */
       {
         goto m_m3_5;
       }
       else if(key==DATA)
        {
          hour=(int)adate00.hour;
          min=(int)adate00.min;
          sec=(int)adate00.sec;

          sscanf(fst_str,"%d.%d.%d",&hour,&min,&sec);
          if((min<0)|| (min > 59))  goto m_m3_5_1;
          if((sec<0)|| (sec > 59))  goto m_m3_5_1;
          if((hour<0)|| (hour > 23))  goto m_m3_5_1;

             adate00.min = min;
             adate00.hour = hour;
             adate00.sec = sec;
            // время введено
           if((adate00.hour  != adate11.hour ) ||
              (adate00.min   != adate11.min) ||
              (adate00.sec   != adate11.sec  ) )
           {
//          printf("\n\r Time have changed ");
            f_wr_evt(evt_none);
            SetTime( adate00.hour, adate00.min,adate00.sec);
    WriteNVRAM((int)nr_hour   ,(int)hour   );
    WriteNVRAM((int)nr_min    ,(int)min    );
    WriteNVRAM((int)nr_sec    ,(int)sec    );
            init_win_sum(0);
            f_wr_evt(evt_ch_time);
           }

         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();

         goto m_m3;
        }
         break;
 /*========================================*/
    case 21:  // просмотр списка модулей

       max_par=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
       if((key==ESC)||(key==ENTER))   /* переход в меню выбора точки для просмотра*/
       {
          goto m_m3_4;
       }
       else if((key== F2 )&& (mmi_str>0) )
       {
        mmi_str -=n_mmi_str_3;
        goto m20_1;
       }
       else if((key== F3 )&& (mmi_str < (max_par-n_mmi_str_3)) )
       {
        mmi_str +=n_mmi_str_3;
        goto m20_1;
       }
       break;
 /*========================================*/
    case 22: // просмотр списка драйверов

       if((key==ESC)||(key==ENTER))    /* переход в меню выбора точки для просмотра*/
       {
        goto m_m3;
       }
       else if((key== F2 )&& (mmi_str>0) )
       {
        mmi_str -=n_mmi_str_3;
        goto m20_2;
       }
       else if((key== F3 )&& (mmi_str < (max_par-n_mmi_str_3)) )
       {
        mmi_str +=n_mmi_str_3;
        goto m20_2;
       }
       break;
 /*========================================*/
    case 23: // просмотр статистики драйверов

       if((key==ESC)||(key==ENTER))    /* переход в меню выбора точки для просмотра*/
       {
        goto m_m3;
       }
       else if((key== F2 )&& (mmi_str>0) )
       {
        mmi_str -=n_mmi_str_3;
        goto m20_3;
       }
       else if((key== F3 )&& (mmi_str < (max_par-n_mmi_str_3)) )
       {
        mmi_str +=n_mmi_str_3;
        goto m20_3;
       }
       else if (key==Sht_ESC)
        {  // обнуление счетчиков ошибок
           for(i=0;i<icp_lst_max;i++)
           {
             ICPl_DEV=ICP_dd[i];
             if(ICPl_DEV!=NULL)
             {
      //      ICPl_DEV->n_transaction=0;
              ICPl_DEV->n_timeout_error=0;
              ICPl_DEV->n_CRC_error=0;
             }
           }
        }
       else //PASS
       {
         pass_cnt++;
         if((pass_cnt & 0xf) == 0 )
           f_drv_stat_MMI(&mmi_str);
       }
       break;
 /*========================================*/
    case 24: // отображение сканирования порта
       if((key==ESC)||(key==ENTER))
       {
           goto m_m3_4; //  4  Другие функции
       }
       else //PASS
       {
          MmiGotoxy(0,3);  MmiPrintf("Опрошено   %3d",licp_addr);
          MmiGotoxy(0,4);  MmiPrintf("Обнаружено %3d",licp_fnd);
       }
       break;
 /*========================================*/
    case 25: // отображение дискретных входов - выходов

       if((key==ESC)||(key==ENTER))
       {
           f_wr_evt(evt_ch_oute);
           sw_dns=-2;
           goto m0_f1;
       }
       else if(key==Sht_F3)
       {
        if( sw_dns==20)
        {
          MmiGotoxy(28,0); MmiPuts("  ");
          sw_dns=-2;
          break;
        }
        else if( sw_dns != 20)
        {
          MmiGotoxy(28,0); MmiPuts("!!");
          sw_dns=20;
          break;
        }
       }

    chg_old=OUT_VAR;

#if defined(MMI_ICP)
 if(MVD_DNS == 0)
 {
    if( sw_dns==20)
       if(key == '2')
       {
        OUT_VAR ^= OUT1;
        fl_ch_out=1;
       }
       else if(key == '1')
       {
        OUT_VAR ^= OUT2;
        fl_ch_out=1;
       }
       else if(key== '3')
       {
        OUT_VAR ^= OUT3;
        fl_ch_out=1;
       }
       else if(key== '4')
       {
        OUT_VAR ^= OUT4;
        fl_ch_out=1;
       }
       else if(key== '0')
       {
        OUT_VAR ^= OUT5;
        fl_ch_out=1;
       }
       else if(key== '*')   // 11
       {
        OUT_VAR ^= OUT6;
        fl_ch_out=1;
       }
       else if(key== '/')   // 12
       {
        OUT_VAR ^= OUT7;
        fl_ch_out=1;
       }
       else if(key== '6')
       {
        OUT_VAR ^= OUT8;
        fl_ch_out=1;
       }
       else if(key== '5')
       {
        OUT_VAR ^= OUT9;
        fl_ch_out=1;
       }
       else if(key== '7')
       {
        OUT_VAR ^= OUT10;
        fl_ch_out=1;
       }
       else if(key== '8')
       {
        OUT_VAR ^= OUT11;
        fl_ch_out=1;
       }
       else if(key== '9')
       {
        OUT_VAR ^= OUT12;
        fl_ch_out=1;
       }


 //      else // PASS
       {
          MmiGotoxy(0,1); MmiPuts("Рычаг ");
          if( INP_VARi & INP1 )
            MmiPuts(" 1");
          else
            MmiPuts(" 0");

          MmiGotoxy(10,1); MmiPuts("УЗА ");
          if( INP_VARi & INP2 )
            MmiPuts(" 1");
          else
            MmiPuts(" 0");

          MmiGotoxy(20,1); MmiPuts("СТОП ");
          if( INP_VARi & INP3 )
            MmiPuts(" 1");
          else
            MmiPuts(" 0");

          MmiGotoxy(0,2); MmiPuts("1-ГРК меньш.");
          if( OUT_VAR & OUT2 )
            MmiPuts("1");
          else
            MmiPuts("0");

         MmiGotoxy(15,2); MmiPuts("2-ГРК больш. ");
          if( OUT_VAR & OUT1 )
            MmiPuts("1");
          else
            MmiPuts("0");

          MmiGotoxy(0,3); MmiPuts("3-ГРК Насос ");
          if( OUT_VAR & OUT3 )
            MmiPuts("1");
          else
            MmiPuts("0");

         MmiGotoxy(15,3); MmiPuts("4-Плотн.     ");
          if( OUT_VAR & OUT4 )
            MmiPuts("1");
          else
            MmiPuts("0");

          f_prn_clp() ;
       }
 }
 else
 {    // 1 MVD


    if( sw_dns==20)
       if(key == '2')
       {
        OUT_VAR ^= OUT1;
        fl_ch_out=1;
       }
       else if(key == '1')
       {
        OUT_VAR ^= OUT2;
        fl_ch_out=1;
       }
       else if(key== '3')
       {
        OUT_VAR ^= OUT3;
        fl_ch_out=1;
       }
       else if(key== '4')
       {
        OUT_VAR ^= OUT4;
        fl_ch_out=1;
       }


       else if(key== '5')   // LVL 10
       {
        OUT_VAR ^= OUT6;
        fl_ch_out=1;
       }
       else if(key== '6')   // LVL 90
       {
        OUT_VAR ^= OUT7;
        fl_ch_out=1;
       }
       else if(key== '7')   // LVL 85
       {
        OUT_VAR ^= OUT12;
        fl_ch_out=1;
       }

 //      else // PASS
       {
          MmiGotoxy(0,1); MmiPuts("Рычаг ");
          if( INP_VARi & INP1 )
            MmiPuts(" 1");
          else
            MmiPuts(" 0");

          MmiGotoxy(10,1); MmiPuts("Разр.гудок ");
          if( INP_VARi & L85 )
            MmiPuts(" 1");
          else
            MmiPuts(" 0");

          MmiGotoxy(0,2); MmiPuts("1-ГРК меньш.");
          if( OUT_VAR & OUT2 )
            MmiPuts("1");
          else
            MmiPuts("0");

         MmiGotoxy(15,2); MmiPuts("2-ГРК больш. ");
          if( OUT_VAR & OUT1 )
            MmiPuts("1");
          else
            MmiPuts("0");

          MmiGotoxy(0,3); MmiPuts("3-ГРК Насос ");
          if( OUT_VAR & OUT3 )
            MmiPuts("1");
          else
            MmiPuts("0");

         MmiGotoxy(15,3); MmiPuts("4-Плотн.     ");
          if( OUT_VAR & OUT4 )
            MmiPuts("1");
          else
            MmiPuts("0");


     MmiGotoxy( 0,4); MmiPuts("5-Ур10%:");
     if( OUT_VAR & OUT6  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");


     MmiGotoxy(15,4); MmiPuts("6-Ур90%:");
     if( OUT_VAR & OUT7  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy( 15,5); MmiPuts("7-Ур85%:");
     if( OUT_VAR & OUT12 )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

       }

}
#else

 if(MVD_DNS == 0)
 {
    if( sw_dns==20)
       if(key == '2')
       {
        OUT_VAR ^= OUT1;
        fl_ch_out=1;
       }
       else if(key == '1')
       {
        OUT_VAR ^= OUT2;
        fl_ch_out=1;
       }
       else if(key== '3')
       {
        OUT_VAR ^= OUT3;
        fl_ch_out=1;
       }
       else if(key== '4')
       {
        OUT_VAR ^= OUT4;
        fl_ch_out=1;
       }
       else if(key== '9')
       {
        OUT_VAR ^= OUT5;
        fl_ch_out=1;
       }
       else if(key== '*')   // 11
       {
        OUT_VAR ^= OUT6;
        fl_ch_out=1;
       }
       else if(key== '/')   // 12
       {
        OUT_VAR ^= OUT7;
        fl_ch_out=1;
       }
       else if(key== '6')
       {
        OUT_VAR ^= OUT8;
        fl_ch_out=1;
       }
       else if(key== '5')
       {
        OUT_VAR ^= OUT9;
        fl_ch_out=1;
       }
       else if(key== '7')
       {
        OUT_VAR ^= OUT10;
        fl_ch_out=1;
       }
       else if(key== '8')
       {
        OUT_VAR ^= OUT11;
        fl_ch_out=1;
       }
       else if(key== '0')
       {
        OUT_VAR ^= OUT12;
        fl_ch_out=1;
       }


 //      else // PASS
       {
     MmiGotoxy(0,1); MmiPuts("----------- Входы-------------");
          MmiGotoxy(0,2); MmiPuts("Рычаг ");
          if( INP_VARi & INP1 )
            MmiPuts(" 1");
          else
            MmiPuts(" 0");

          MmiGotoxy(10,2); MmiPuts("УЗА ");
          if( INP_VARi & INP2 )
            MmiPuts(" 1");
          else
            MmiPuts(" 0");

          MmiGotoxy(20,2); MmiPuts("СТОП ");
          if( INP_VARi & INP3 )
            MmiPuts(" 1");
          else
            MmiPuts(" 0");


          MmiGotoxy( 0,3); MmiPuts("Разр.гудок ");
          if( INP_VARi & L85 )
            MmiPuts(" 1");
          else
            MmiPuts(" 0");


     MmiGotoxy(0,4); MmiPuts("------------ ГРК -------------");

          MmiGotoxy(0,5); MmiPuts("1-ГРК меньш.");
          if( OUT_VAR & OUT2 )
            MmiPuts("1");
          else
            MmiPuts("0");

         MmiGotoxy(15,5); MmiPuts("2-ГРК больш. ");
          if( OUT_VAR & OUT1 )
            MmiPuts("1");
          else
            MmiPuts("0");

          MmiGotoxy(0,6); MmiPuts("3-ГРК Насос ");
          if( OUT_VAR & OUT3 )
            MmiPuts("1");
          else
            MmiPuts("0");

         MmiGotoxy(15,6); MmiPuts("4-Плотн.     ");
          if( OUT_VAR & OUT4 )
            MmiPuts("1");
          else
            MmiPuts("0");

//-------- f_prn_clp

     MmiGotoxy(0,7);  MmiPuts("--------- Прием СУГ --------");
     MmiGotoxy(0,8); MmiPuts("5-Мнш. :");
     if( OUT_VAR & OUT9  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy( 15,8); MmiPuts("6-Блш. :");
     if( OUT_VAR & OUT8  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy(0, 9); MmiPuts("7-Насос:");
     if( OUT_VAR & OUT10 )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy(15, 9); MmiPuts("8-Нас.Г:");
     if( OUT_VAR & OUT11 )
       MmiPuts("1   ");
     else
       MmiPuts("0   ");

     MmiGotoxy(0,10); MmiPuts("9-Пар  :");
     if( OUT_VAR & OUT5  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy(0,11); MmiPuts("---- Сигналы уровнемеров ----");
     MmiGotoxy( 0,12); MmiPuts("*-Ур10%:");
     if( OUT_VAR & OUT6  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");


     MmiGotoxy(15,12); MmiPuts("/-Ур90%:");
     if( OUT_VAR & OUT7  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");


     MmiGotoxy(0,13); MmiPuts("0-Ур85%:");
     if( OUT_VAR & OUT12 )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

       }
 }
 else
 {    // 1 MVD


    if( sw_dns==20)
       if(key == '2')
       {
        OUT_VAR ^= OUT1;
        fl_ch_out=1;
       }
       else if(key == '1')
       {
        OUT_VAR ^= OUT2;
        fl_ch_out=1;
       }
       else if(key== '3')
       {
        OUT_VAR ^= OUT3;
        fl_ch_out=1;
       }
       else if(key== '4')
       {
        OUT_VAR ^= OUT4;
        fl_ch_out=1;
       }


       else if(key== '5')   // LVL 10
       {
        OUT_VAR ^= OUT6;
        fl_ch_out=1;
       }
       else if(key== '6')   // LVL 90
       {
        OUT_VAR ^= OUT7;
        fl_ch_out=1;
       }
       else if(key== '7')   // LVL 85
       {
        OUT_VAR ^= OUT12;
        fl_ch_out=1;
       }

 //      else // PASS
       {
          MmiGotoxy(0,2); MmiPuts("Рычаг ");
          if( INP_VARi & INP1 )
            MmiPuts(" 1");
          else
            MmiPuts(" 0");

          MmiGotoxy(10,2); MmiPuts("Разр.гудок ");
          if( INP_VARi & L85 )
            MmiPuts(" 1");
          else
            MmiPuts(" 0");

          MmiGotoxy(0,4); MmiPuts("1-ГРК меньш.");
          if( OUT_VAR & OUT2 )
            MmiPuts("1");
          else
            MmiPuts("0");

         MmiGotoxy(15,4); MmiPuts("2-ГРК больш. ");
          if( OUT_VAR & OUT1 )
            MmiPuts("1");
          else
            MmiPuts("0");

          MmiGotoxy(0,6); MmiPuts("3-ГРК Насос ");
          if( OUT_VAR & OUT3 )
            MmiPuts("1");
          else
            MmiPuts("0");

         MmiGotoxy(15,6); MmiPuts("4-Плотн.     ");
          if( OUT_VAR & OUT4 )
            MmiPuts("1");
          else
            MmiPuts("0");


     MmiGotoxy( 0,8); MmiPuts("5-Ур10%:");
     if( OUT_VAR & OUT6  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");


     MmiGotoxy(15,8); MmiPuts("6-Ур90%:");
     if( OUT_VAR & OUT7  )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

     MmiGotoxy( 0,10); MmiPuts("7-Ур85%:");
     if( OUT_VAR & OUT12 )
       MmiPuts("1 ");
     else
       MmiPuts("0 ");

       }

}

#endif

       if(fl_ch_out!=0)
       {
        fl_ch_out=0;
        chg_new= OUT_VAR;
        chg_nn_par=Par_OUT;
        f_wr_evt(evt_chg_param);
       }
       break;
 /*========================================*/
    case 26:/* ожидание нажатия ESC,Enter */
         if((key==ESC)||(key==ENTER)  )
         {
          goto m_main;
         }
         break;
 //--------------------------------------------------
/*
    case 171://  1  Показания расходомера
         if((key==ESC)||(key==ENTER)  )
         {
          if((ff_serv==f_MVD_MM2)||(ff_serv==f_MVD_M2) ) ff_serv=NULL;
          goto m_m3_4;
         }
         else if(key== PASS )
         {
//        if ((ff_serv==NULL) && ( (flag_nal==0) || (fl_rd_err!=0) ) )
          if ((ff_serv==NULL) && ( (fl_rd_err!=0) ) )
          {
           fl_rd_err=0;
           sw_MMI_cur=0; // строка MMI
           sw_mvd_m1=mmi_prm+20;  // нач.параметр
           sw_MMI_lim=6; // конечная строка
           MMIDens=-1;
           MMITemp=-300;
           ff_serv=f_MVD_MM2;
          }
         }
         else if(key== F2 )
           if(mmi_prm==0);
           else if(mmi_prm>=6)
           {
            mmi_prm -=6;
    //       printf("\n\rmmi_prm=%d",mmi_prm);
            goto m_m3_4_1v;
           }
           else
           {
            mmi_prm = 0;
            goto m_m3_4_1v;
           }
         else if (key== F3 )
         {
           if(mmi_prm <= (max_par-6))
           {
            mmi_prm +=6;
      //     printf("\n\rmmi_prm=%d",mmi_prm);
            goto m_m3_4_1v;
           }
         }
 //--------------------------------------------------
         break;

    case 172://  2  Параметры расходомера
         if((key==ESC)||(key==ENTER)  )
         {
          if(ff_serv==f_MVD_M1) ff_serv=NULL;
          goto m_m3_4;
         }
         else if(key== F2 )
           if(mmi_prm==0);
           else if(mmi_prm>=6)
           {
            mmi_prm -=6;
            goto m_m3_4_2v;
           }
           else
           {
            mmi_prm = 0;
            goto m_m3_4_2v;
           }
         else if (key== F3 )
         {
           if(mmi_prm <= (max_par-6))
           {
            mmi_prm +=6;
            goto m_m3_4_2v;
           }
         }
         break;

 //--------------------------------------------------
    case 173:
         if((key==ESC)||(key==ENTER)  )
         {
          ff_serv=NULL;
          goto m_m3_4;
         }
         if(key==PASS  )
         {
          if(ff_serv==NULL)
          {
           SetDisplayPage(EmptPage);
           f_clr_scr_MMI();
           MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\"Пред \"F3\"След \"ESC\"Отмена");
           sw_MMI_cur=0; // строка MMI
           sw_mvd_m1=mmi_prm+20;  // нач.параметр
           sw_MMI_lim=6; // конечная строка
           MMIDens=-1;
           MMITemp=-300;
           ff_serv=f_MVD_M2;
           sw_mmi=171;
          }
         }
         break;
 //--------------------------------------------------
    case 174:
         if((key==ESC)||(key==ENTER)  )
         {
          ff_serv=NULL;
          goto m_m3_4;
         }
         if(key==PASS  )
         {
          if(ff_serv==NULL)
          {
           SetDisplayPage(EmptPage);
           f_clr_scr_MMI();
           MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"F2\"Пред \"F3\"След \"ESC\"Отмена");
           sw_MMI_cur=0; // строка MMI
           sw_mvd_m1=mmi_prm+20;  // нач.параметр
           sw_MMI_lim=6; // конечная строка

           ff_serv=f_MVD_M1;
           sw_mmi=172;
          }
         }
         break;
 //--------------------------------------------------
*/
    case 180:

       if((key==ESC)||(key==ENTER))  // переход на начальную страницу
       {
         if(ff_serv==f_MVD_M3) ff_serv=NULL;
            goto m_m3_4;
       }
       break;

 /*========================================*/
    case 271:/* ожидание нажатия ESC,Enter */
         if((key==ESC)||(key==ENTER)  )
         {
           goto m0_f1;
         }
         if( (f_timer(tm_md,(long int)1000 )) | fl_md_fst )
         {
           f_md5(1);
           sw_mmi=180;
         }
         break;
 /*========================================*/
//27.05.2020 YN --\\//--
     case 272:/* ожидание нажатия ESC,Enter */
         if((key==ESC)||(key==ENTER)  )
         {
           goto m_m3_4;
         }
       if(key=='1')
       {
m_m4s_1:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите объем резервуара л: " );
          k_mv_tmp=s_CALC.VolTank;
          flag_calc=1;

m_m4s_1_1:
          MmiGotoxy(0,n_mmi_str_2);  MmiPuts("\"1...9,SHT-7\"-Ввод,\"SHT-4\"-Уд.");
          MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"ESC - возврат");
          if(flag_calc!=0)
          {
           sprintf( fst_str,"%8.3f",k_mv_tmp);
           fst_n=9;
          }
          else
          {
           sprintf( fst_str,"%8.1f",k_mv_tmp);
           fst_n=9;
          }
          sw_fst=1;
          MmiGotoxy(10,3);
          break;
       }
       if(key=='2')
       {
m_m4s_2:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите объем ждк.фазы л:" );
          k_mv_tmp=s_CALC.VolLiqPhase;
          flag_calc=0;
          goto m_m4s_1_1;
       }
       if(key=='3')
       {
m_m4s_3:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите темп.ждк.фазы С:" );
          k_mv_tmp=s_CALC.TempLiqPhase;
          flag_calc=2;
          goto m_m4s_1_1;
       }
       if(key=='4')
       {
m_m4s_4:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите темп.пар.фазы С:" );
          k_mv_tmp=s_CALC.TempGasPhase;
          flag_calc=3;
          goto m_m4s_1_1;
       }
       if(key=='5')
       {
m_m4s_5:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите плотн.ждк.фазы кг/м3:" );
          k_mv_tmp=s_CALC.DensLiqPhase;
          flag_calc=4;
          goto m_m4s_1_1;
       }
       if(key=='6')
       {
m_m4s_6:
          MmiGotoxy(0,1);
          SetDisplayPage(EmptPage); // ввод значения
          f_clr_scr_MMI();
          MmiPuts("Введите давл. в рез-ре МПа:" );
          k_mv_tmp=s_CALC.PressTank;
          flag_calc=5;
          goto m_m4s_1_1;
       }
       else if(key==DATA)
        {
          sscanf(fst_str,"%f",&k_mv_tmp);

         if(flag_calc==1)
         {
            if((k_mv_tmp<1000)||(k_mv_tmp>20000.) )
                goto m_m4s_1;
         }
         else if (flag_calc==0)
         {
          if((k_mv_tmp<999)||(k_mv_tmp>19999.) )
                goto m_m4s_2;
         }
         else if (flag_calc==2)
         {
           if((k_mv_tmp<-50)||(k_mv_tmp>50.) )
                goto m_m4s_3;
         }
         else if (flag_calc==3)
         {
            if((k_mv_tmp<-50)||(k_mv_tmp>50) )
                goto m_m4s_4;
         }
         else if (flag_calc==4)
         {
            if((k_mv_tmp<1)||(k_mv_tmp>700) )
                goto m_m4s_5;
         }
         else if (flag_calc==5)
         {
            if((k_mv_tmp<0.01)||(k_mv_tmp>10.) )
                goto m_m4s_6;
         }

      // введенное значение в k_mv_tmp
          if(flag_calc==1)
          {
           s_CALC.VolTank= k_mv_tmp;
          }
          else if (flag_calc==0)
          {
           s_CALC.VolLiqPhase= k_mv_tmp;
          }
          else if (flag_calc==2)
          {
           s_CALC.TempLiqPhase= k_mv_tmp;
          }  
          else if (flag_calc==3)
          {
           s_CALC.TempGasPhase= k_mv_tmp;
          }               
          else if (flag_calc==4)
          {
           s_CALC.DensLiqPhase= k_mv_tmp;
          }        
          else if (flag_calc==5)
          {
           s_CALC.PressTank= k_mv_tmp;
          }     
          goto m_m4;
        }
       else if(key==DATA_ESC)
        {
          goto m_m4;
        }
         break;
//27.05.2020 YN --//\\--
 /*========================================*/
    case 200: //  2  Прием  СУГ
       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
           goto main_menu;
       }
       /*
       else if(key==F1)   // Help
       {
        f_dsp_H2();
        Ret_cod= sw_mmi;
        sw_mmi=220;
        break;
       }
       */
       else if((key== '1')&&(Flag_check == 0) ) // 1  Прием дозы СУГ
       {
         Reg_State  &=  ~(  RCV_fin | RCV_flch);

m_m6_1:
         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();
         MmiGotoxy(0,0);       MmiPuts("   Прием  дозы СУГ");
         f_prn_MassSt_rcv();

         MmiGotoxy(0,2);       MmiPuts(" Введите дозу приема ,кг");
      // sprintf( fst_str,"");
         strcpy( fst_str," ");
         fst_n=5;
         sw_fst=1;
         MmiGotoxy(10,4);
         break;
       }
       else if(key==DATA)
        {
          dose_dlvr=-1;
          sscanf(fst_str,"%f",&dose_dlvr);
          if(dose_dlvr<=0) { dose_dlvr=0; goto m_m6_1;}
          flag_dlv_fst=0;
          flag_prn_mass=0;
m_m6_1o:
          sw_mmi=201;
          break;
        }
       else if(key==DATA_ESC)
        {
          goto m_m6;
        }
       else if(key== '2')  // 2  Прием до опорожнения
       {

         Reg_State  &=  ~(  RCV_fin | RCV_flch);
m_m6_2:
         dose_dlvr=vBIG_P;
         flag_dlv_fst=0;
         flag_prn_mass=0;
         goto m_m6_1o;
       }
       else if(key== '3')  // 3 Продолжить с задан. дозой
       {
         Reg_State  &=  ~(  RCV_fin | RCV_flch);
         if(flag_prn_mass)
          {
           goto m_m6_1o;
          }
       }
       else
            f_prn_MassSt_rcv();
       break;
 /*========================================*/
    case 201: //  Прием СУГ, выбор режима старта

         SetDisplayPage(EmptPage);
         f_clr_scr_MMI();

     MmiGotoxy(0,0);
     if(Flag_check == 0)
     {
          if(dose_dlvr != vBIG_P)
             MmiPrintf("    Прием дозы СУГ %g кг",dose_dlvr);
          else
             MmiPrintf("    Прием СУГ без дозировки");
     }
     else  MmiPuts(str_verify);

         MmiGotoxy(0,6);   MmiPuts("ESC - завершить.");

         sw_dlv_liq=1;
         sw_mmi=203;
m_m6_1d:

          if(flag_prn_mass)
          {
             MassT=s_MVD[0].MassT+s_MVD[1].MassT;
             VolT =s_MVD[0].VolT+s_MVD[1].VolTd;
              //  вычисляется в f_MVD_rd() при приеме данных от расходомера паровой фазы
              //  s_MVD[1].VolTd=s_MVD[1].MassT*1000./s_MVD[0].Dens;
          }
          else
          {
             MassT=0.;
             VolT =0.;
          }

          if(Flag_check == 0)
          {

            MmiGotoxy(0,2);MmiPrintf("М=%6.1fкг  Принято  О=%6.1fл",MassT,VolT );
            MmiGotoxy(0,3);
             if(Tank_nn > 2)
               MmiPrintf("V1=%4.0fл   V2=%4.0fл   V3=%4.0fл", s_DGM[0].VolLiq,s_DGM[1].VolLiq,s_DGM[2].VolLiq);
             else
               MmiPrintf("V1=%4.0fл  Резервуары  V2=%4.0fл", s_DGM[0].VolLiq,s_DGM[1].VolLiq);
          }
          else
          {
              MmiGotoxy(0,2);
              MmiPuts("           Принято           ");

              MmiGotoxy(0,3);
              MmiPrintf("М=%10.3f кг|О=%10.3f л ",MassT,VolT );
          }

          f_prn_proc();

          break;

//===========================================
    case 203: //  Прием
       if(key==F2)    // индикация Давление,Температура
       {
           PageD=0;
       }
       else if(key==F3)    // индикация Плотность,Состав
       {
           PageD=1;
       }
       else if(key==F1)    // индикация выходов
       {
           PageD=3;
       }
       key=f_rcv_liq(key);
       if(key==ESC)    // переход в меню
       {
           goto m_m6;
       }
  // отображение общей и отпущенной массы
       goto m_m6_1d;
 /*========================================*/
    case 205: // Прием СУГ
        goto m_rcv_liq;
 /*========================================*/
    case 209: // Выбор номера резервуара
       {
         if(key==ESC)    // переход в меню
         {
           goto main_menu;
         }
         else if(key=='1')  // Tank1
         {
           Tank_n=0;
           DenMeasSrc=0;
           sw_mmi=210;
         }
         else if((key=='2') && (Tank_nn > 1 ))  // Tank2
         {
           Tank_n=1;
           DenMeasSrc=0;
           sw_mmi=210;
         }
         else if((key=='3') && (Tank_nn > 2 ) )  // Tank3
         {
           Tank_n=2;
           DenMeasSrc=0;
           sw_mmi=210;
         }
         break;
       }
 /*========================================*/
 /*========================================*/
    case 210: // Подготовка к измерению плотности
       {
         sw_dns=1;
         sw_mmi=211;
         f_clr_scr_MMI();
         SetDisplayPage(EmptPage);

   MmiGotoxy(0,0); MmiPrintf(" Изм.плотности, резервуар N%d",Tank_n+1);
   f_d_ESC();
         PageD=0;
         break;
       }
 /*========================================*/
    case 211: // Измерение плотности

       if(key==F2)    // индикация Давление,Температура
       {
           PageD=0;
       }
       else if(key==F3)    // индикация Плотность,Состав
       {
           PageD=1;
       }
       else if(key==F1)    // индикация выходов
       {
           PageD=3;
       }
       key=f_dens_m(key);
       if(key==ESC)    // переход в меню
       {
        if( FL_err  != 0)
          goto main_menu;
        if(DenMeasSrc==1)
          goto main_menu;
        else
        {
         sw_mmi=212;
         f_clr_scr_MMI();
         SetDisplayPage(EmptPage);

    MmiGotoxy(0,0); MmiPrintf("Измерение плотности завершено",Tank_n+1);
    MmiGotoxy(0,1); MmiPrintf("Резервуар N%d: ",Tank_n+1);

    f_dsp_ddn(2,Dens_Result,Temp_Result);

     MmiGotoxy(0,5); MmiPuts("1 Ввести  данные ");
//   MmiGotoxy(0,6); MmiPuts("ESC  Возврат ");
         f_d_ESC();MmiPuts(" без ввода данных ");
         break;

        }
       }
       f_prn_dn();
       break;
 /*========================================*/
    case 212: // Подтверждение ввода данных измерения
       {
         if((Tank_n > 2) || (Tank_n<0))  goto main_menu;
         if(key==ESC)    // переход в меню
         {
           goto main_menu;
         }
         else if(key=='1')  // принять данные
         {

           s_DGM[Tank_n].DensReal=Dens_Result  ;
           s_DGM[Tank_n].TempReal=Temp_Result  ;

           f_wr_evt(evt_dn_get);

           MmiGotoxy(0,5); MmiPuts(" === Данные введены ===");
           MmiGotoxy(0,6); MmiPuts(str_empty);
           f_d_ESC();

           sw_mmi=220;
           Ret_cod=1;
           break;
         }
         break;
       }
 /*========================================*/
    case 220: // Out from Help
 /*========================================*/
       if((key==ESC)||(key==ENTER))    /* переход в меню */
       {
         switch(Ret_cod)
         {
             //  1  Отпуск СУГ
           case 1:  goto main_menu;
           case 200:  goto m_m6;
           default:  goto m_main;
         }
       }
       break;
 /*========================================*/
       default:
       if((key==ESC)||(key==ENTER))  /* переход на начальную страницу */
       {
 m_main:
           SetDisplayPage(ZeroPage);
           sw_mmi=0;
           f_clr_scr_MMI();
           f_prn_begin();
           break;
       }
       break;
    }
    return 0;
}
/*-----------------------------------------*/
int flag_Serv_c=0;
void f_before_MMI()
{
    flag_Serv_c=flag_Serv;
    ftmp_naMMI[0]=analog_scale[0]*NA_scale;
    ftmp_naMMI[1]=analog_scale[1]*NA_scale;
    ftmp_naMMI[2]=analog_scale[2]*NA_scale;
    ftmp_naMMI[3]=analog_scale[3]*NA_scale;
    ftmp_naMMI[4]=analog_scale[4]*NA_scale;
    ftmp_naMMI[5]=analog_scale[5]*NA_scale;
    ftmp_naMMI[6]=analog_scale[6]*NA_scale;

    if(MVD_FLOW != 0) MVD_FLOW_t=1;
    else  MVD_FLOW_t=3;

}
/*-----------------------------------------*/
void f_after_MMI()
{
int i;
    analog_scale[0]=ftmp_naMMI[0]/NA_scale;
    analog_scale[1]=ftmp_naMMI[1]/NA_scale;
    analog_scale[2]=ftmp_naMMI[2]/NA_scale;
    analog_scale[3]=ftmp_naMMI[3]/NA_scale;
    analog_scale[4]=ftmp_naMMI[4]/NA_scale;
    analog_scale[5]=ftmp_naMMI[5]/NA_scale;
    analog_scale[6]=ftmp_naMMI[6]/NA_scale;
    f_init_mki();
    if(flag_Serv_c != flag_Serv)
    {
        flag_Serv_c=flag_Serv;

        if(flag_Serv != 0)
        {
           RestoreCom(ComPortPmp1);
           InstallCom(ComPortPmp1,ComBaudPmp1,Com_lgthPmp1,Com_parityPmp1,Com_stopPmp1);
        }
        else
        {
           RestoreCom( ComPortHost );
           i=ComPortHost;
           InstallCom(i,ComBaud[i],Com_lgth[i],Com_parity[i],Com_stop[i]);
           flag_wdp=0;
        }
    }

    if(MVD_FLOW_t != 3) MVD_FLOW=2;
    else  MVD_FLOW=0;
}
//-------------------------------
