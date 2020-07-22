struct s_ver_calc s_CALC=
{
    8000,   // float VolTank;
    5500,   // float VolLiqPhase;
    20,   // float TempLiqPhase;
    20,   // float TempGasPhase;
    550,    // float DensLiqPhase;
    0.2,  // float PressTank;
    0.,     // float DensGasPhase;
    0.,     // float VolGasPhase;
    0.,     // float MassGasPhase;
    0.,     // float MassLiqPhase;
    0.,     // float PressGasPhase;
    0.,     // float CompGasPhase;
};

float D_Etan100_vc,D_Etan_vc,P_Etan_vc;

#define  ind_50_vc  (20)
//-------------
  // начальная температура в таблицах данных
#define Beg_temp_vc (-50)
  // шаг температуры в таблицах данных
#define Step_t_vc (5.)

// молекулярные массы газов
float M_Propan_vc=44.097;
float M_Butan_vc=58.124;

//-------------
void f_get_pressCT_ver_calc ( float Compos, float Temp)
{
// по массовому составу жидкой фазы и температуре возвращает
// давление насыщенного пара, вычисляет плотность и массовый состав паров
// Compos - массовая доля пропана в смеси 0...1.0

int itmp,itmp1;
float r_propan,r_butan,ftmp;
float mProp,mBut,mlProp,mlBut;
float ComposM;
float d_propan,d_butan;
float TempG;

//  printf("\r\nf_get_pressCT Compos=%f,Temp=%f:",Compos,Temp);
 TempG=Temp;

if(TempG > 50) TempG=50.;
 if(TempG < (-50)) TempG=-50.;

  if( TempG == 50.)
  {
   r_propan=PropanP[ind_50_vc];
   r_butan=ButanP[ind_50_vc];
   d_propan=PropGas[ind_50_vc];
   d_butan =ButGas[ind_50_vc];
  }
  else
  {
    ftmp=(TempG-(float)Beg_temp_vc)/Step_t_vc;
    itmp=(int)ftmp;

    ftmp-=(float)itmp;
    itmp1=itmp+1;
    r_propan=(PropanP[itmp1]-PropanP[itmp]) * ftmp + PropanP[itmp];
    r_butan =(ButanP[itmp1] -ButanP[itmp] ) * ftmp + ButanP[itmp];

    d_propan=(PropGas[itmp1]-PropGas[itmp]) * ftmp + PropGas[itmp];
    d_butan =(ButGas[itmp1] -ButGas[itmp] ) * ftmp + ButGas[itmp];
  }

  mlProp = Compos / M_Propan_vc;
  mlBut = (1.- Compos) / M_Butan_vc ;
  ComposM=mlProp/(mlProp+mlBut);

  s_CALC.PressGasPhase=r_butan*(1.0-ComposM)+(r_propan*ComposM) ;
  s_CALC.DensGasPhase =d_butan*(1.0-ComposM)+(d_propan*ComposM) ;
  // массовый состав газа
  s_CALC.CompGasPhase=(d_propan*ComposM)/s_CALC.DensGasPhase;

  return;
}
//-------------

float f_get_PD_Etan_ver_calc(float Temp)
{
// по температуре
// рассчитывает давление насыщенного пара и плотность этана

 float TempG;
 float ftmp;
 float P_Etan100;
 int itmp,itmp1;

 TempG=Temp;

 if(TempG > 50) TempG=50.;
 if(TempG < (-50)) TempG=-50.;

    ftmp=(TempG-(float)Beg_temp_vc)/Step_t_vc;
    itmp=(int)ftmp;
    ftmp-=(float)itmp;
    itmp1=itmp+1;

    D_Etan100_vc=(EtanGas[itmp1]-EtanGas[itmp]) * ftmp + EtanGas[itmp];
    P_Etan100=(EtanP[itmp1]-EtanP[itmp]) * ftmp + EtanP[itmp];

    return P_Etan100;
}
//--------------------

//------------------------------

void f_count_tank_ver_calc()
{ // обсчет резервуара ii

float Vol_liq,ftmp,ftmp1;
float Vol_gas;
float Compos;

//  По текущим значениям уровня , температуре жидкой и паровой фаз,
//  давлению паровой фызы вычисляются:
//  объем,плотность и масса жидкой и паровой фаз.
//  Используется информация о геометрии резервуара.
//  Если уровнемер не измеряет плотность жидкой фазы,
//  плотность жидкой фазы вычисляется по значению текущей температуры
//  и измеренным в процессе прокачки через расходомер значениям плотности
//  и температуры жидкой фазы.

//  По значению уровня и геометрии резервуара вычисляется объем
//  жидкой и паровой фаз.
   Vol_liq=s_CALC.VolLiqPhase;
   Vol_gas=s_CALC.VolTank-s_CALC.VolLiqPhase;
   s_CALC.VolGasPhase=Vol_gas;

//  По измеренным значениям  плотности жидкой фазы и температуры
//  измерения вычисляется состав жидкой фазы -
//  массовая доля пропана для двухкомпонентной смеси пропан-бутан.

    // Определение состава по текущим данным уровнемера для жидкой фазы:
    // плотности и температуре.
    // Плотность жидкой фазы вычислять не требуется, введена при приеме данных
    // из расходомера.

    Compos = f_get_composition(s_CALC.DensLiqPhase,s_CALC.TempLiqPhase);

   if(Compos < 0) goto m_err;

//  По составу жидкой фазы и температуре паровой фазы определяется
//  плотность паровой фазы пропана-бутана (без учета этана) и
//  давление паровой фазы пропан-бутана

   ftmp=s_CALC.TempGasPhase; // Temp1,Temp2,Temp3 - температура паровой фазы
   if(ftmp < -45) ftmp=s_CALC.TempLiqPhase;
   else if(ftmp < s_CALC.TempLiqPhase) ftmp=s_CALC.TempLiqPhase;

   f_get_pressCT_ver_calc(Compos,ftmp);

//  printf("\n\r Press_gas=%f, Dens_gas=%f",s_MVD[0].PressG , s_MVD[0].DensG);
// результат в s_MVD[0].PressG , s_MVD[0].DensG

   P_Etan_vc= s_CALC.PressTank - s_CALC.PressGasPhase;
   if(P_Etan_vc > 0)
   {
     ftmp1=f_get_PD_Etan_ver_calc(ftmp);
     // результат плотности в D_Etan100_vc
     D_Etan_vc=D_Etan100_vc * P_Etan_vc/ftmp1;
//     printf("\n\r D_Etan=%f",D_Etan);
   }
   else
    D_Etan_vc=0;

   s_CALC.DensGasPhase=s_CALC.DensGasPhase+D_Etan_vc;

   s_CALC.MassLiqPhase=Vol_liq * s_CALC.DensLiqPhase/1000.;
   s_CALC.MassGasPhase=Vol_gas * s_CALC.DensGasPhase/1000.;
   return;

m_err:

// s_DGM[ii].VolLiq=-1;
// s_DGM[ii].VolGas=-1;
   s_CALC.MassLiqPhase=-1;
   s_CALC.MassGasPhase=-1;
   s_CALC.DensLiqPhase=-1;
   s_CALC.DensGasPhase=-1;

 return;
}
//------------------------------