// 10 Nov 2010 //ГОСТ 28656-90 ГАЗЫ УГЛЕВОДОРОДНЫЕ СЖИЖЕННЫЕ Расчетный метод определения плотности и давления насыщенных паров
                //Стаскевич Н.Л., Вигдорчик Д.Я. Справочник по сжиженным углеводородным газам

#include "prop_but.h"

#define  ind_50  (20)
//-------------
  // начальная температура в таблицах данных
#define Beg_temp (-50)
  // шаг температуры в таблицах данных
#define Step_t (5.)

float PermPlus = 8;
float PermMinus= 8;
float t20c     =20.;
float v_Proc   =50.;
int   flag_20C =0;

float Temp_kor=0;
float Comp_kor=1.0;

// молекулярные массы газов
float M_Etan=30.068;
float M_Propan=44.097;
float M_Butan=58.124;

float Density=0;
float Temperature=0;
float Temperature1=0;
float Composition=0;

float ComposMass=0, ComposMol=0;
float CompG=0;

//----------------------------

float f_get_dens()
{// возвращает приведенное к 20C значение плотности, если flag_20C != 0
 // или -1. при ошибке

 //  s_MVD[0].Dens - значение плотности
 //  s_MVD[0].Temp - значение температуры
/*

 float ftmp;

   Temperature=s_MVD[0].Temp;
   Temperature1=Temperature;
   ftmp=s_MVD[0].Dens;
   Density = ftmp;

   ftmp=f_get_composition(ftmp,Temperature);
   if(ftmp<= (-PermMinus)) return -1; // плотность смеси больше плотности бутана
   if(ftmp>= PermPlus) return -1;             // плотность смеси меньше плотности пропана

   Composition=ftmp;

*/
   return s_MVD[GR].Dens;

}


//------------------------------------
/*
void f_get_dens_c()
{// заносит в Dens0  приведенное к текущей температуре значение плотности, если flag_20C == 2

 //  s_MVD[0].Dens - значение плотности
 //  s_MVD[0].Temp - значение температуры
 float ftmp;
  if((s_MVD[0].Temp>=-50) && (s_MVD[0].Temp<=50))
  {
   Temperature1=s_MVD[0].Temp;
   Dens0 = s_MVD[0].Dens;
  }
}
*/
//------------------------------------

float f_get_composition(float Dens,float Temp)
{
// возвращает массовый состав пропан-бутановой смеси

int itmp,itmp1;
float r_propan,r_butan,ftmp;

float mProp,mBut,mlProp,mlBut;

if((Temp > 50) || (Temp< -50)) return -1;
 // вычисляет объемный состав пропан-бутановой смеси
  if( Temp == 50.)
  {
   r_propan=PropLiq[ind_50];
   r_butan=ButLiq[ind_50];
  }
  else
  {
    ftmp=(Temp-(float)Beg_temp)/Step_t;
    itmp=(int)ftmp;
    ftmp-=(float)itmp;
    itmp1=itmp+1;
    // плотность жидкой фазы пропана,бутанв при температуре Temp , кг/м3
    r_propan=(PropLiq[itmp1]-PropLiq[itmp]) * ftmp + PropLiq[itmp];
    r_butan =(ButLiq[itmp1] -ButLiq[itmp] ) * ftmp + ButLiq[itmp];

//  printf("\r\n itmp=%d,ftmp=%.3f,r_prop=%.3f,r_but=%.3f",itmp,ftmp,r_propan,r_butan);
  }

// объемная доля пропана в смеси 0...1.0

  Composition =(r_butan-Dens)/(r_butan-r_propan);

 // масса пропана, бутана в 1 м3

  mProp=r_propan * Composition;
  mBut =r_butan  * (1.0-Composition);

 // массовая доля пропана в смеси 0...1.0
  ComposMass=mProp/(mProp+mBut);
/*
 // количество киломоль пропана , бутана в 1 м3 смеси
  mlProp=mProp/M_Propan;
  mlBut =mBut /M_Butan ;

 // молярная доля пропана в смеси 0...1.0
  ComposMol=mlProp/(mlProp+mlBut);
*/
  return ComposMass;
}
//-------------
float f_get_Dns(float Compos,float Temp)
{
// по массовому составу Compos (0...1.0) и температуре Temp  (-50C ... 50 C)
// вычисляет плотность жидкой фазы пропан-бутановой смеси

int itmp,itmp1;
float r_propan,r_butan,ftmp;
if((Temp > 50) || (Temp< -50)) return -1;
//if((Compos > 1.0) || (Compos< 0)) return -1;

  if( Temp == 50.)
  {
   r_propan=PropLiq[ind_50];
   r_butan=ButLiq[ind_50];
  }
  else
  {
    ftmp=(Temp-(float)Beg_temp)/Step_t;
    itmp=(int)ftmp;
    ftmp-=(float)itmp;
    itmp1=itmp+1;

    r_propan=(PropLiq[itmp1]-PropLiq[itmp]) * ftmp + PropLiq[itmp];
    r_butan =(ButLiq[itmp1] -ButLiq[itmp] ) * ftmp + ButLiq[itmp];
  }
  //  r_propan, r_butan - плотности жидких фаз при заданной температуре,кг/м3
   // если  Compos - объемный состав  D= ( Compos*r_propan + (1.- Compos) * r_butan );
  return ( r_propan * r_butan / ( r_propan * (1.0-Compos) + Compos * r_butan  ) );
}
//-------------
float f_get_pressCT(float Compos,float Temp)
{
// по массовому составу жидкой фазы и температуре возвращает
// давление насыщенного пара, вычисляет плотность и массовый состав паров
// Compos - массовая доля пропана в смеси 0...1.0

int itmp,itmp1;
float r_propan,r_butan,ftmp;
float mProp,mBut,mlProp,mlBut;
float ComposM;
float d_propan,d_butan;

if((Temp > 50) || (Temp< -50)) return -1;

  if( Temp == 50.)
  {
   r_propan=PropanP[ind_50];
   r_butan=ButanP[ind_50];
// d_propan=PropGas[ind_50];
// d_butan =ButGas[ind_50];
  }
  else
  {
    ftmp=(Temp-(float)Beg_temp)/Step_t;
    itmp=(int)ftmp;
    ftmp-=(float)itmp;
    itmp1=itmp+1;

    r_propan=(PropanP[itmp1]-PropanP[itmp]) * ftmp + PropanP[itmp];
    r_butan =(ButanP[itmp1] -ButanP[itmp] ) * ftmp + ButanP[itmp];

//  d_propan=(PropGas[itmp1]-PropGas[itmp]) * ftmp + PropGas[itmp];
//  d_butan =(ButGas[itmp1] -ButGas[itmp] ) * ftmp + ButGas[itmp];
  }

  mlProp=Compos/M_Propan;
  mlBut =(1.- Compos) /M_Butan ;
  ComposM=mlProp/(mlProp+mlBut);

  s_MVD[0].PressG=r_butan*(1.0-ComposM)+(r_propan*ComposM) ;
 // s_MVD[0].DensG=d_butan*(1.0-ComposM)+(d_propan*ComposM) ;
  // массовый состав газа
//  CompG=(d_propan*ComposM)/s_MVD[0].DensG;

  return (s_MVD[0].PressG);
}
//-------------

void f_get_pressDT(float Dens,float Temp)
{
// по плотности и температуре
// рассчитывает массовый состав смеси жидкой фазы s_MVD[0].Compos
// давление насыщенного пара s_MVD[0].PressG, плотность и массовый состав паров
// s_MVD[0].Compos - массовая доля пропана в смеси 0...1.0

int itmp,itmp1;
float r_propan,r_butan,ftmp,Compos,Ttmp;
float d_propan,d_butan;
float mProp,mBut,mlProp,mlBut;

if((Temp > 50) || (Temp< -50))
 {
  s_MVD[0].PressG=10.00 ;
  s_MVD[0].DensG=1000.;
  return;
 }

  if( Temp == 50.)
  {
   r_propan=PropLiq[ind_50];
   r_butan=ButLiq[ind_50];
  }
  else
  {
    ftmp=(Temp-(float)Beg_temp)/Step_t;
    itmp=(int)ftmp;
    ftmp-=(float)itmp;
    itmp1=itmp+1;

    r_propan=(PropLiq[itmp1]-PropLiq[itmp]) * ftmp + PropLiq[itmp];
    r_butan =(ButLiq[itmp1] -ButLiq[itmp] ) * ftmp + ButLiq[itmp];
  }

// объемная доля пропана в смеси 0...1.0
  Compos = (r_butan-Dens)/(r_butan-r_propan);

 // масса пропана, бутана в 1 м3

  mProp=r_propan * Compos;
  mBut =r_butan  * (1.0-Compos);

 // массовая доля пропана в смеси 0...1.0
  s_MVD[0].Compos=mProp/(mProp+mBut);

 // количество киломоль пропана , бутана в 1 м3 смеси
  mlProp=mProp/M_Propan;
  mlBut =mBut /M_Butan ;

 // молярная доля пропана в смеси 0...1.0
//ComposMol=mlProp/(mlProp+mlBut);
  Compos=mlProp/(mlProp+mlBut);

    Ttmp=Temp + Temp_kor;
    if(Ttmp > 50) Ttmp=50;

  if((Compos<=1.) && (Compos>=0.))
  {
    Compos=Compos * Comp_kor;
    if(Compos>1.)  Compos=1.;

    if( Ttmp == 50.)
    {
     r_propan=PropanP[ind_50];
     r_butan=ButanP[ind_50];
//   d_propan= PropGas[ind_50];
//   d_butan=  ButGas[ind_50];

    }
    else
    {
    ftmp=(Temp-(float)Beg_temp)/Step_t;
    itmp=(int)ftmp;
    ftmp-=(float)itmp;
    itmp1=itmp+1;

      r_propan=(PropanP[itmp1]-PropanP[itmp]) * ftmp + PropanP[itmp];
      r_butan =(ButanP[itmp1] -ButanP[itmp] ) * ftmp + ButanP[itmp];

//    d_propan=(PropGas[itmp1]-PropGas[itmp]) * ftmp + PropGas[itmp];
//    d_butan =(ButGas[itmp1] -ButGas[itmp] ) * ftmp + ButGas[itmp];
    }

    s_MVD[0].PressG=r_butan*(1.0-Compos)+(r_propan*Compos) ;
 //   s_MVD[0].DensG=d_butan*(1.0-Compos)+(d_propan*Compos) ;
 //   CompG=(d_propan*Compos)/s_MVD[0].DensG;
  }
  else
  {
  /*
   if( flag_prn )
   {
    flag_prn=0;
    printf("\n\rDens=%f,Temp=%f, Compos=%f,PressG=%f",Dens,Temp,Compos,s_MVD[0].PressG);
   }
  */
    s_MVD[0].PressG=10.00 ;
    s_MVD[0].DensG=1000.;
  }
}
//--------------------
//--------------------
// Табличные данные
//--------------------

//---------------------------------------
//Пропан, плотность жидкой фазы, кг/м3
//-50С ... 60С
float PropLiq[]=
{
590.9,     // -50
585.2,     // -45
579.4,     // -40
573.7,   // -35
567.7,     // -30
561.6,   // -25
555.5,     // -20
549.3,   // -15
542.9,     // -10
536.4,     //  -5
529.7,     // 0
522.8,     //   5
515.8,     //  10
508.6,   //  15
501.1,     //  20
493.4,     //  25
485.5,     //  30
477.5,  //  35
468.9,   //  40
460.4,  //  45
451.3,     //  50
};

//---------------------------------------
// Бутан, плотность жидкой фазы, кг/м3
//-50С ... 60С
float ButLiq[]=
{
651.1,          // -50
646.4,        // -45
641.5,          // -40
636.7,        // -35
631.7,          // -30
626.8,        // -25
621.8,          // -20
616.6,        // -15
611.5,          // -10
606.6,          //  -5
601,          // 0
595.7,          //   5
590.2,          //  10
584.6,          //  15
578.9,          //  20
573.2,          //  25
567.3,          //  30
561.3,          //  35
555.2,          //  40
549,          //  45
542.6,          //  50
};

//---------------------------------------
// ИзоБутан, плотность жидкой фазы, кг/м3
//-50С ... 60С
/*float ButILiq[]=
{
633,          // -50
628.5,        // -45
624,          // -40
619.5,        // -35
615,          // -30
610,          // -25
605,          // -20
600,          // -15
594,          // -10
588,          //  -5
582,          // 0
576,          //   5
570,          //  10
564,          //  15
558,          //  20
552,          //  25
546,          //  30
540,          //  35
534,          //  40
528,          //  45
522,          //  50
515,          //  55
505,          //  60
};*/
//---------------------------------------
//---------------------------------------
//Этан, плотность паровой фазы, кг/м3
//-50С ... 60С
float EtanGas[]={
5.99,         // -50
7.25,         // -45
8.72,         // -40
10.42,        // -35
12.38,        // -30
14.62,        // -25
17.19,        // -20
20.08,        // -15
23.37,        // -10
27.06,        //  -5
31.20,        // 0
34.47,        //   5
40.95,        //  10
46.62,        //  15
52.90,        //  20
59.82,        //  25
67.41,        //  30
73.06,        //  35
78.86,        //  40
84.81,        //  45
90.90,        //  50
97.15,        //  55
103.54,       //  60
};
//---------------------------------------
//Пропан, плотность паровой фазы, кг/м3
//-50С ... 60С
float PropGas[]={
1.81,         // -50
2.07,         // -45
2.61,         // -40
3.25,         // -35
3.87,         // -30
4.62,         // -25
5.48,         // -20
6.4,          // -15
7.57,         // -10
9.05,         //  -5
10.34,        // 0
11.9,         //   5
13.6,         //  10
15.51,        //  15
17.74,        //  20
20.15,        //  25
22.8,         //  30
25.3,         //  35
28.6,         //  40
34.5,         //  45
36.8,         //  50
40.22,        //  55
44.6,         //  60
};
//---------------------------------------
//Бутан, плотность паровой фазы, кг/м3
//-50С ... 60С
float ButGas[]={
0.41,         // -50
0.50,         // -45
0.60,         // -40
0.73,         // -35
0.89,         // -30
1.08,         // -25
1.32,         // -20
1.602,        // -15
1.947,        // -10
2.1,          //  -5
2.82,         // 0
3.35,         //   5
3.94,         //  10
4.65,         //  15
5.39,         //  20
6.18,         //  25
7.19,         //  30
8.17,         //  35
9.334,        //  40
10.571,       //  45
12.1,         //  50
12.38,        //  55
15.4,         //  60
};
//------------------------------
//---------------------------------------
//Изобутан, плотность паровой фазы, кг/м3
//-50С ... 60С
/*float ButIGas[]={
0.73,         // -50
0.89,         // -45
1.08,         // -40
1.32,         // -35
1.49,         // -30
1.65,         // -25
1.96,         // -20
2.5,          // -15
3.04,         // -10
3.59,         //  -5
4.31,         // 0
5.07,         //   5
5.92,         //  10
6.95,         //  15
7.94,         //  20
9.21,         //  25
11.5,         //  30
13,           //  35
14.7,         //  40
16.8,         //  45
18.94,        //  50
20.56,        //  55
24.2,         //  60
};
*/
//---------------------------------------

#define k_Pa (0.0010)

//Этан, давление насыщенных паров, MПа
//-50С ... 60С
float EtanP[]={
553 * k_Pa,          // -50
655 * k_Pa,          // -45
771 * k_Pa,          // -40
902 * k_Pa,          // -35
1050 * k_Pa,         // -30
1215 * k_Pa,         // -25
1400 * k_Pa,         // -20
1604 * k_Pa,         // -15
1831 * k_Pa,         // -10
2081 * k_Pa,         //  -5
2355 * k_Pa,         // 0
2555 * k_Pa,         //   5
2982 * k_Pa,         //  10
3336 * k_Pa,         //  15
3721 * k_Pa,         //  20
4137 * k_Pa,         //  25
4585 * k_Pa,         //  30
4889 * k_Pa,         //  35
5193 * k_Pa,         //  40
5497 * k_Pa,         //  45
5801 * k_Pa,         //  50
6105 * k_Pa,         //  55
6409 * k_Pa,         //  60
};

//---------------------------------------
//Пропан, давление насыщенных паров, кПа
//-50С ... 60С
float PropanP[]={
70  * k_Pa,          // -50
88  * k_Pa,          // -45
109 * k_Pa,          // -40
134 * k_Pa,          // -35
164 * k_Pa,          // -30
197 * k_Pa,          // -25
236 * k_Pa,          // -20
285 * k_Pa,          // -15
338 * k_Pa,          // -10
399 * k_Pa,          //  -5
465 * k_Pa,          // 0
543 * k_Pa,          //   5
629 * k_Pa,          //  10
725 * k_Pa,          //  15
833 * k_Pa,          //  20
951  * k_Pa,          //  25
1080 * k_Pa,         //  30
1226 * k_Pa,         //  35
1382 * k_Pa,         //  40
1554 * k_Pa,         //  45
1740 * k_Pa,         //  50
1943 * k_Pa,         //  55
2162 * k_Pa,         //  60
};
//---------------------------------------
//Бутан, давление насыщенных паров, кПа
//-50С ... 60С
float ButanP[]={
4  * k_Pa,           // -50
6  * k_Pa,           // -45
8  * k_Pa,           // -40
12 * k_Pa,           // -35
20 * k_Pa,           // -30
32 * k_Pa,           // -25
44 * k_Pa,           // -20
56 * k_Pa,           // -15
68 * k_Pa,           // -10
84 * k_Pa,           //  -5
102 * k_Pa,          // 0
123 * k_Pa,          //   5
146 * k_Pa,          //  10
174 * k_Pa,          //  15
205 * k_Pa,          //  20
240 * k_Pa,          //  25
280 * k_Pa,          //  30
324 * k_Pa,          //  35
374 * k_Pa,          //  40
429 * k_Pa,          //  45
490 * k_Pa,          //  50
557 * k_Pa,          //  55
631 * k_Pa,          //  60

};
//---------------------------------------
//Изобутан, давление насыщенных паров, кПа
//-50С ... 60С
/*float ButanIP[]={
8  * k_Pa,            // -50
12 * k_Pa,           // -45
20 * k_Pa,           // -40
32 * k_Pa,           // -35
44 * k_Pa,           // -30
56  * k_Pa,          // -25
68  * k_Pa,          // -20
88  * k_Pa,          // -15
107 * k_Pa,          // -10
128 * k_Pa,          //  -5
153 * k_Pa,          // 0
182 * k_Pa,          //   5
215 * k_Pa,          //  10
252 * k_Pa,          //  15
294 * k_Pa,          //  20
341 * k_Pa,          //  25
394 * k_Pa,          //  30
452 * k_Pa,          //  35
518 * k_Pa,          //  40
590 * k_Pa,          //  45
669 * k_Pa,          //  50
759 * k_Pa,          //  55
853 * k_Pa,          //  60
};*/
//---------------------------------------