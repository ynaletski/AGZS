
float MassTotal=0,VolTotal=0,DenTotal=-1,TempTotal=-100;
char str_def1[]="------------";
//-------------------------
void f_cnt_totals()
{
  float Dens,M1,M2,V1;
     if((Dens=s_MVD[0].Dens) < 450.) Dens=540.;
     if((M1=s_MVD[0].MassT) == vBIG_P) M1=0.;
     if((M2=s_MVD[1].MassT) == vBIG_P) M2=0.;
     if((V1=s_MVD[0].VolT) == vBIG_P) V1=0.;

     MassTotal = M1+M2 ;
     VolTotal=V1+ M2 * 1000./Dens;
//     if(VolTotal != 0)
//        DenTotal=MassTotal*1000./VolTotal;
//     else
//        DenTotal= s_MVD[0].Dens;
//     TempTotal=s_MVD[0].Temp;

 //  Compos_rcv=f_get_composition(s_MVD[0].Dens,s_MVD[0].Temp); ...
 // !!! Недоделано
 //  TempTotal =f_get_temp(DenTotal,Compos_rcv);

}
//-------------------------

unsigned int flash_seg   =0xC000;
unsigned int flash_offset=0;
long int time_last_evt=0;
float vBIG_P=BIG_P;
struct arch_str ar_str;
struct arch_str_t tim_000 = {
0,34,1,
30,7,9
};

int f_wr_evt( unsigned int evt )
{
 int i_ret;


  f_prepare_evt ( &ar_str, evt);
  i_ret= f_wr_astr(&ar_str);

 switch( (int)( evt & ~(int)RCV_FLG) )
  {
   case evt_wrong_start:
   case evt_no_dev:
   case evt_start_f:

   case evt_exm_f  :   // недолив
   case evt_exp_f  :   // перелив
   case evt_end_f  :   // End OK
   case evt_est_f  :   // Command Stop filling
   case evt_err_f  :   // Error Stop filling
   case evt_ep1_f  :   // Host  Stop filling
   case evt_overfl :   // Переполнение
   case evt_gun    :   // Отключен рычаг
   case evt_wd_pmp :   // Таймаут связи
   case evt_ES_off :   // Нажата аварийная кнопка

   case evt_dn_strt:   //
   case evt_dn_stop:   //
   case evt_dn_err :   //
   case evt_dn_ES  :   //

      f_prepare_evt ( &ar_str,evt_ext_dt1 );
      i_ret |= f_wr_astr(&ar_str);

      f_prepare_evt ( &ar_str,evt_ext_dt2 );
      i_ret |= f_wr_astr(&ar_str);

      break;

   default: break;
  }
  return i_ret;
}
//----------------------------------
int  f_wr_astr (struct arch_str *s_arch )
{// записывает подготовленную структуру во флэш
long int ltmp;
int i,ret_val;
unsigned int utmp;
char *data;

 if(ICP_error[icp_lst_max] & (Flash_erase_error | Flash_wr_error) ) return -1;

  ret_val=0;
  ltmp= ((long int)flash_offset & 0xffff) + sizeof(ar_str);
  if(ltmp >= (long int)0x10000)
  {
   flash_seg= f_get_next_seg(flash_seg);
   flash_offset=0;
  }
 utmp=flash_offset;
 data=(char *)s_arch;
 for(i=0;i< sizeof(ar_str);i++,data++)
   if (FlashWrite(flash_seg, (flash_offset+i), *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 flash_offset= utmp + sizeof(ar_str);

 //printf("\n\r Seg=%04x,offset=%04X",flash_seg,flash_offset);
 return ret_val;
}
//----------------------------------
int  f_get_max_n_arch()
{// возвращает количество записанных структур
int nn;
unsigned int seg_tmp;
unsigned int  *Flash_ptr_i;

  nn=((long int)flash_offset & 0xffff) / sizeof(ar_str);

  seg_tmp= f_get_prev_seg(flash_seg);
  Flash_ptr_i=(unsigned int  *) _MK_FP_(seg_tmp,(unsigned int)0);

  if(*Flash_ptr_i == 0xffff)
  {
   return nn;
  }
    nn+=(long int)0x10000 / sizeof(ar_str);

  seg_tmp= f_get_prev_seg(seg_tmp);
  Flash_ptr_i=(unsigned int  *) _MK_FP_(seg_tmp,(unsigned int)0);
  if(*Flash_ptr_i == 0xffff)
  {
   return nn;
  }
  nn+=(long int)0x10000 / sizeof(ar_str);
  return nn;
}
//----------------------------------

void  f_print_a_str(int i)
{
 struct arch_str *a_str;
 if( (a_str= f_get_astr(i)) == NULL ) return;
 f_print_str_a(a_str);
}
//----------------------------------
char *rcv_m[]={
"отпуск",
"прием",
};
char *Rcv_m[]={
"Отпуск",
"Прием",
};
//----------------------------------
int f_print_str_a(struct arch_str *a_str)
{
 int ii,itmp;
 float ftmp,ftmp1;
 float Density;
 float Temp   ;

 struct tm *tdat;
union  { float f;long int l; } o;

 time_t tim_tmp;
// tim_tmp=a_str->time - GM_COR;
// tdat = gmtime( &tim_tmp);

 int f_rcv;

 if(a_str->event & (int)RCV_FLG)
       f_rcv=1;
 else
       f_rcv=0;

  if((a_str->event==evt_ext_dt1) || (a_str->event==evt_ext_dt2))
  {
    if(a_str->event==evt_ext_dt1)
     itmp=1;
    else itmp=2;
     printf("\r\n=Рез%d=",itmp);
     o.l=a_str->time;
     ftmp=o.f;

     printf(" Уров.=%6.1f мм;Т.ждк.=%6.2f C;Об.ждк.=%8.1f л;M.общ.=%8.1f кг ",a_str->u.fill.VolT,a_str->u.fill.Amount,a_str->u.fill.UnitPrice,a_str->u.fill.Density+a_str->u.fill.Temp);
     printf("\r\nM.ждк.=%8.1f кг;P.пар.=%5.3f МПа;T.пар=%5.2f C;",a_str->u.fill.Density,a_str->VolI,ftmp);

  ftmp=a_str->MassI;

    if(itmp==1)
         printf("\r\nMасса.инв.2 ");
    else
         printf("\r\nОбъем.инв.2 ");

    if(ftmp != vBIG_P)
    {
      if(itmp==1)
           printf("%8.1f кг",ftmp);
      else
           printf("%8.1f л",ftmp);
    }
    else
       puts(str_def1);

    return 0;
  }

  if(a_str->event==evt_deb)
  {

   printf("\r\nDeb= %ld мс:OV=%04X;OVc=%04X;V=%.3f;M=%.3f;F=%.1f;P=%.4f",

    a_str->time,
    (int)a_str->u.fill.UnitPrice,
    (int)a_str->u.fill.Density,

    a_str->VolI,
    a_str->MassI,
    a_str->u.fill.Temp,
    a_str->u.fill.Amount
);

    return 0;
  }


  tdat = localtime( &a_str->time );
  printf("\r\n%02d.%02d.%02d %02d:%02d:%02d",
  tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);

// printf("\n\r mdat.tm_isdst = %d\n\r", tdat->tm_isdst );

  if(a_str->VolI != vBIG_P)
     printf(" V=%10.3f" ,a_str->VolI);
  else
     puts(str_def1);
//     printf(" ------------ ");

  if(a_str->MassI != vBIG_P)
     printf(" M=%10.3f" ,a_str->MassI);
  else
//   printf("------------" );
     puts(str_def1);

 switch( (int)( a_str->event & ~(int)RCV_FLG) )
 {
  case evt_start_f:
    printf(" Старт %sа",rcv_m[f_rcv]);
  if(f_rcv!=0)
  {
    if(a_str->u.fill.Amount != 0)
    {
     printf(" по массе");
    }
    else
    {
     printf(" по объему");
    }
    if(a_str->u.fill.VolT == vBIG_P)
      printf("\r\nБез дозы "   );
    else
      if(a_str->u.fill.Amount != 0)
      {
       printf("\r\nM.%8.3f кг; " ,a_str->u.fill.VolT    );  //%8.3f
      }
      else
      {
       printf("\r\nОб.%8.3f л; " ,a_str->u.fill.VolT    );  //%8.3f
      }
      goto m_dns;
  }

m_val:
  if(f_rcv==0)
  {
    printf("\r\nОб.%8.3f л; " ,a_str->u.fill.VolT    );  //%8.3f
    printf("M.%7.3f кг;",a_str->u.fill.Amount  );  //%6.0f
    printf("Цена %6g руб/л;" ,a_str->u.fill.UnitPrice);
    printf("Пл.%7.3f кг/м3;" ,a_str->u.fill.Density);
    printf("Темп.%6.2fC;" ,a_str->u.fill.Temp);
  }
  else
  {
       printf("\r\nMасса %8.3f кг; " ,a_str->u.fill.Amount  );  //%8.3f
       printf("Объем %8.3f л; " ,a_str->u.fill.VolT    );  //%8.3f
    goto m_dns;

  }
mm_dn:
 Density=a_str->u.fill.Density;
 Temp   =a_str->u.fill.Temp;

mm_dn01:
    if((Density>0)&&(Temp>-300))
    {
     ftmp=f_get_composition(Density,Temp);
     if(ftmp>1.)
      printf("\r\n !СУГ легче жидкого пропана");
     else if(ftmp<0.)
      printf("\r\n !СУГ тяжелее жидкого бутана");
     else
      {
       ftmp1=(1.- ftmp)*100.;
       printf("\r\n СУГ: пропан %8.2f%c, бутан %8.2f%c",(ftmp*100),'%',ftmp1,'%');
      }
    }
    break;

  case evt_brk:
    printf(" Данные об отпуске восстановлены");
    goto m_val;
  case evt_end_f:
    printf(" Завершено ОК  ");
    goto m_val;

  case evt_dn_strt:
    printf(" Начато измерение плотности ");
    goto m_dns;
  case evt_dn_stop:
    printf(" Завершено измерение плотности ");
    goto m_dns;
  case evt_dn_ES:
    printf(" Измер.плотн.прервано авар.кн.");
    goto m_dns;
  case evt_dn_err:
    printf(" Изм.плотн.прервано,ошибка ");
m_dns:
    printf("\r\nПлотн.%7.3f кг/м3;" ,a_str->u.fill.Density);
    printf("Темп.%6.2fC;" ,a_str->u.fill.Temp);
    printf("Давление %6.4f МПа;" ,a_str->u.fill.UnitPrice);
    goto mm_dn;

  case evt_dn_get:
       itmp= a_str->u.fill.Temp;
  printf(" Введена плотн.,темп для Рез.%d",itmp+1);
  printf("\r\nD1=%7.3f,T1=%6.2f;D2=%7.3f,T2=%6.2f;N=%d",
 a_str->u.fill.VolT,
 a_str->u.fill.Amount,
 a_str->u.fill.UnitPrice,
 a_str->u.fill.Density,
 (int)(a_str->u.fill.Temp+1) );

  if(itmp==0)
  {
    Temp   = a_str->u.fill.Amount  ;
    Density= a_str->u.fill.VolT    ;
  }
  else
  {
    Temp   = a_str->u.fill.Density  ;
    Density= a_str->u.fill.UnitPrice;
  }
  goto mm_dn01;

  case evt_exp_f:
    printf(" Завершено.Перелив > допустимого");
    goto m_val;
  case evt_exm_f:
    printf(" Завершено.Недолив > допустимого");
    goto m_val;

  case evt_est_f:
    printf(" Отпуск прерван командой");
    goto m_val;
  case evt_err_f:
    printf(" Отпуск прерван,ошибка ");
    goto m_val;

  case evt_ep1_f:
    printf(" Отпуск прерван,ком.Host");
    goto m_val;

  case evt_overfl:
    printf(" Отпуск прерван.Переполнение бака");
    goto m_val;

  case evt_gun:
    printf(" Отпуск прерван.Отключен рычаг");
    goto m_val;

  case evt_ES_off:
    printf(" Операция прервана:Нажата ав.кнопка");
    goto m_val;

  case evt_wd_pmp:
    printf(" Отпуск прерван.Таймаут связи с базой.");
    goto m_val;

  case evt_no_dev:
    printf(" Старт не принят,Dr");
    goto m_val;

  case evt_clr_err:
    printf(" Сброс ошибок. ");
    break;

  case evt_MVD_CL_strt:
    printf(" Старт калибровки расходомера");
    break;

  case evt_MVD_CL_BS1 :
    printf(" !Поп-ка калибр.расхдм.при отпуске");
    break;

  case evt_MVD_CL_BS2 :
    printf(" !Сбой запуска калибр.расходомера");
    break;

  case evt_MVD_CL_OK :
    printf(" Кал-ка расхдм.завершена успешно");
    break;

  case evt_MVD_CL_TM :
    printf(" Таймаут калибровки расходомера");
    break;

  case evt_ch_date:
    printf(" Изменена дата.");
    break;
  case evt_ch_time:
    printf(" Изменено время.");
    break;
  case evt_ch_sum :
    printf(" Переход на лет.время.");
    break;
  case evt_ch_win :
    printf(" Переход на зим.время.");
    break;

  case evt_res:
    printf(" Команда \"Сброс\"");
    break;
  case evt_verify:
    printf(" Старт Поверки");
    break;
  case evt_Slot:
    printf(" Запись конфиг.в MVD");
    break;
  case evt_dsp_grk:
  case evt_kd_grk:
    printf(" Показания ГРК");
    goto m_val;

  case evt_deliv_rep:
    printf(" Отчет PUMALAN");
    printf("\r\nО=%05ld,M=%06ld;Ц=%04d",
(long int) a_str->u.fill.VolT     ,
(long int) a_str->u.fill.Amount   ,
(int) a_str->u.fill.UnitPrice
);

    break;

  case evt_chg_param:
    if(a_str->u.fill.VolT == Par_VF)
    {
     printf(" !Изменен множитель объема");
    }
    else if(a_str->u.fill.VolT == Par_MF)
    {
     printf(" !Изменен множитель массы");
    }
    else if(a_str->u.fill.VolT == Par_PASS)
    {
     printf(" !Изменен пароль");
    }
    else
    {
     printf(" !Изменен параметр ");
     printf("N%g",a_str->u.fill.VolT );
    }
    printf("\r\nБыло:%8g;  ",a_str->u.fill.Amount  );
    printf("Стало:%8g" ,a_str->u.fill.UnitPrice);
    break;
  case evt_exit:
    printf(" Команда\"Выход в ОС\"");
    break;
  case evt_re:
    printf(" Чтение конф.из EEPROM");
    break;
  case evt_re_err:
    printf(" Ош.чтения конф.EEPROM");
    break;
  case evt_we_err:
    printf(" Ош.записи конф.EEPROM");
    break;
  case evt_we:
    printf(" Запись конф.в EEPROM");
    break;
  case evt_brk_flt:
    printf(" Данные об отпуске утеряны");
    break;

  case evt_wrong_start:
    printf(" Cтарт не принят,Er");
    goto m_val;

  case evt_err:
  case evt_none:
 mm_0:
    itmp=0;
  for(ii=0;(ii<=icp_lst_max)&&(ii<6);ii++)
      itmp |= a_str->u.err.err[ii];
  if(itmp)
  {
    printf(" Ошибка:");
 mm_e:
    printf("%04X" ,a_str->u.err.err[0] );
    printf(" %04X" ,a_str->u.err.err[1] );
    printf(" %04X" ,a_str->u.err.err[2] );
    printf(" %04X" ,a_str->u.err.err[3] );
    printf(" %04X" ,a_str->u.err.err[4] );
    printf(" %04X" ,a_str->u.err.err[5] );
  }
  else
    printf(" Ошибок нет ");

  break;

  case evt_t_on:
    printf(" Включение устройства ");
    break;
  case evt_t_off:
    printf(" Выключение устройства ");
    break;
  default:
    printf(" Код события не распознан");
    break;
 }
return 0;
}

//----------------------------------
struct arch_str *f_get_astr(int nn )
{// Возвращает указатель на стуктуру события с заданным номером
 // nn == 0 - первая записанная структура
long int ltmp;
unsigned int seg_tmp,seg_tmp_1,seg_tmp_2;
unsigned int offset_tmp;
int mm,jj;
struct arch_str *a_str;

  mm=(long int)0x10000 / sizeof(ar_str);

  seg_tmp_1= f_get_prev_seg(flash_seg);
  seg_tmp_2=f_get_prev_seg(seg_tmp_1);

 if( nn < mm)
 {
   offset_tmp= nn * sizeof(ar_str);
   a_str=(struct arch_str *) _MK_FP_(seg_tmp_1,0);
   if(a_str->event == (unsigned int)0xffff)
     seg_tmp=flash_seg;
   else
   {
     a_str=(struct arch_str *) _MK_FP_(seg_tmp_2,0);
     if(a_str->event == (unsigned int)0xffff)
       seg_tmp=seg_tmp_1;
     else
       seg_tmp=seg_tmp_2;
   }
 }
 else
 {
  if((nn-mm)<mm)
  {
   offset_tmp= (nn-mm) * sizeof(ar_str);

   a_str=(struct arch_str *) _MK_FP_(seg_tmp_2,0);

   if(a_str->event == (unsigned int)0xffff)
     seg_tmp= flash_seg;
   else
     seg_tmp= seg_tmp_1;
  }
  else
  {
   offset_tmp= (nn-mm-mm) * sizeof(ar_str);
   seg_tmp= flash_seg;
  }
 }

 a_str=(struct arch_str *) _MK_FP_(seg_tmp,offset_tmp);
 if(a_str->event == (unsigned int)0xffff) return NULL;
 return a_str;
}

//----------------------------------
unsigned int retf_seg;
unsigned int retf_iii;
unsigned int retl_offs,retl_seg=0;
struct arch_str *f_get_fst_stor()
{ // возвращает указатель на первую запись
unsigned int seg_tmp,seg_tmp_2,offs_tmp,utmp;
struct arch_str *ar_str_ptr;

 // определение начала журнала

  seg_tmp= f_get_prev_seg(flash_seg);
  ar_str_ptr=(struct arch_str *) _MK_FP_(seg_tmp,(unsigned int)0);
  if(ar_str_ptr->event != 0xffff)
  {
   seg_tmp_2= f_get_prev_seg(seg_tmp);
   ar_str_ptr=(struct arch_str *) _MK_FP_(seg_tmp_2,(unsigned int)0);
   if(ar_str_ptr->event != 0xffff)
   {
    seg_tmp=seg_tmp_2;
   }
  }
  else
  {
    seg_tmp= flash_seg;
    ar_str_ptr=(struct arch_str *) _MK_FP_(seg_tmp,(unsigned int)0);
//    if(ar_str_ptr->event == 0xffff) return NULL; //
  }

//  seg_tmp - начальный сегмент
 retf_seg=seg_tmp;
 ar_str_ptr=(struct arch_str *) _MK_FP_(seg_tmp,(unsigned int)0);
 return ar_str_ptr;
}
//-------------------------------
struct arch_str *f_get_lst_stor()
{ // возвращает указатель на последнюю запись

 struct tm *tdat;

unsigned int seg_tmp,offs_tmp;
struct arch_str *ar_str_ptr;
time_t ltime1;
long int ltmp;

  if(flash_offset != 0)
  {
    seg_tmp=flash_seg;
    offs_tmp=flash_offset-sizeof(ar_str);
    ar_str_ptr=(struct arch_str *) _MK_FP_(seg_tmp,offs_tmp);
 m1:
    ltime1= f_get_time(ar_str_ptr);
    if(ltime1 < 0)
    {
//    printf("\r\n return NULL ");
      retl_seg=seg_tmp;
      retl_offs=offs_tmp;
      return NULL;
    }
  }
  else
  {
    ltmp=0x10000;
    ltmp=((ltmp/(long int)sizeof(ar_str))-1) * (int)sizeof(ar_str);
    offs_tmp=ltmp & 0xffff;
    seg_tmp= f_get_prev_seg(flash_seg);
    ar_str_ptr=(struct arch_str *) _MK_FP_(seg_tmp,offs_tmp);

    if(ar_str_ptr->event == 0xffff) return NULL;
    else goto m1;
  }

 retl_seg=seg_tmp;
 retl_offs=offs_tmp;
//  printf("\r\nf_get_lst_stor() : seg_tmp=%04x,offs_tmp=%04x  ",seg_tmp,offs_tmp);
 return ar_str_ptr;
}
//-------------------------------
void f_set_cost()
{
     cost_old=cost_v;
     cost_vvc=cost_v;
     n_cnt_cst = N_cst;
     flag_fst_cst=0;
     cost_v_c=cost_v;
     flag_cst_chg=0;
     cnt_ltch=0;
}
//-------------------------------
float  N_rev=0;
void f_chk_break()
{
  int itmp,i,i1;
  struct arch_str *a_str;
  unsigned  int event;
  int flag_ok=0;

 itmp=ReadNVRAM( nr_flg_n );
 if(itmp==NAL_B)
 { // информация о наливе не передана в компьютер
  m1:
   i1= f_get_max_n_arch();

  for(i=i1-1;i>=0;i--)
  {
   if( (a_str= f_get_astr(i)) == NULL )
   {
//    printf("\n не найдена структура N%d",i);
m_flt:
     f_wr_evt(evt_brk_flt); // данные утеряны
     return;
   }
   event=a_str->event;
   if(event==evt_start_f)
   { // найден последний старт

     cost_v=a_str->u.fill.UnitPrice;

     if(cost_v <=0)
     {
       goto m_flt;
     }
     f_set_cost();


    if(flag_ok)
    {
       VolT_id=a_str->u.fill.VolT;
       MassT_id=a_str->u.fill.Amount;
    }
    else
    {

/*=======================
       switch(ModeFill)
       {
       case F_Vol:
            if(vVolI== vBIG_P)
            {
//     printf("\nvVolI=%f",vVolI);
              goto m_flt;
            }
            VolT_id=fabs(vVolI-a_str->VolI);
            MassT_id=fabs(vMassI-a_str->MassI);
            break;
       case F_Vol_Dens:
            if(vVolI == vBIG_P)
            {
//     printf("\nvVolI=%f",vVolI);
              goto m_flt;
            }
            VolT_id=fabs(vVolI-a_str->VolI);
            MassT_id=fabs(vMassI-a_str->MassI);
            break;

       case F_Mass:
            if(vMassI == vBIG_P)
            {
//     printf("\nvMassI=%f",vMassI);
              goto m_flt;
            }
            VolT_id=fabs(vVolI-a_str->VolI);
            MassT_id=fabs(vMassI-a_str->MassI);
            break;

       default:
            if(vVolI == vBIG_P)
            {
//     printf("\nvVolI=%f",vVolI);
              goto m_flt;
            }
            VolT_id=fabs(vVolI-a_str->VolI);
            MassT_id=fabs(vMassI-a_str->MassI);
            break;
       }

========*/

     VolT_id=fabs(vVolI-a_str->VolI);
     MassT_id=fabs(vMassI-a_str->MassI);

//  printf("\n\rvMassI=%f,MassI=%f",vMassI,a_str->MassI);// WWWW

     VolT_id=((long int)(VolT_id * 100.+0.5));
     VolT_id *= 0.01;
    }

     summ_v_id=VolT_id*cost_v;
     summ_v_id=f_round(summ_v_id);

     VolT_id1=VolT_id;
     summ_v_id1=summ_v_id;
     Dens0=0;
     N_rev=i;
     p1_state |= End_dlvr;
     n_msg_t='1';

     f_wr_evt(evt_brk); // данные восстановлены
     Dens0=1000.;
     WriteNVRAM((int) nr_flg_n ,(int)NAL_VB);
     return;
   }
  else if(event==evt_end_f  )
   {
     flag_ok=1;
     continue;
   }
   else if(
              (event==evt_exm_f  )
           || (event==evt_exp_f  )
           || (event==evt_est_f  )
           || (event==evt_err_f  )
           || (event==evt_dn_err )
           || (event==evt_ep1_f  )
           || (event==evt_overfl )
           || (event==evt_gun    )
           || (event==evt_ES_off )
           || (event==evt_wd_pmp )
          )
   {  // найдена запись о завершении налива
     cost_v=a_str->u.fill.UnitPrice;
     if(cost_v <=0)
     {
       goto m_flt;
     }
     f_set_cost();

     VolT_id=a_str->u.fill.VolT;
     MassT_id=a_str->u.fill.Amount;

     if(event==evt_end_f  )
     {
      flag_ok=1;
      continue;
     }
      VolT_id=((long int)(VolT_id * 100.+0.5));
      VolT_id *= 0.01;

     summ_v_id=VolT_id*cost_v;

     summ_v_id=f_round(summ_v_id);

     VolT_id1=VolT_id;
     summ_v_id1=summ_v_id;
//   Dens0=0;
     Dens0=a_str->u.fill.Density;
     N_rev=i;
     p1_state |= End_dlvr;
     n_msg_t='1';
     f_wr_evt(evt_brk); // данные восстановлены
     Dens0=1000.;
     WriteNVRAM((int) nr_flg_n ,(int)NAL_VB);
     return;
   }

  }
//  printf("\nВсе структуры просмотрены");
  goto m_flt;
 }
 else if(itmp==NAL_VB)
 { // информация о наливе не передана в компьютер и содержится в востановленной записи в журнал

  i1= f_get_max_n_arch();

  for(i=i1-1;i>=0;i--)
  {
   if( (a_str= f_get_astr(i)) == NULL )
   {
     goto m1;
   }
   if(a_str->event==evt_brk)
   { // найдена последняя восстановленная запись
m_rev:
     cost_v=a_str->u.fill.UnitPrice;
     if(cost_v <=0)
     {
       goto m_flt;
     }
     f_set_cost();

     VolT_id   = a_str->u.fill.VolT;
     MassT_id=a_str->u.fill.Amount;
//   VolT_id=((long int)(VolT_id * 100.+0.5));
//   VolT_id *= 0.01;

     summ_v_id=VolT_id*cost_v;

     summ_v_id=f_round(summ_v_id);

//   summ_v_id = a_str->u.fill.Amount;
     Dens0= a_str->u.fill.Density;
     N_rev=i;
     p1_state |= End_dlvr;
     n_msg_t='1';
     f_wr_evt(evt_brk); // данные восстановлены
     if(Dens0 <=0)
           Dens0=1000.;
     return;
   }
  }
//  printf("\nВсе структуры просмотрены");
  goto m1;
 }
 else  // вся информация была передана до выключения,
       // попытка восстановить последнюю цену
 {
   i1= f_get_max_n_arch();

  for(i=i1-1;i>=0;i--)
  {
   if( (a_str= f_get_astr(i)) == NULL )
   {
     return;
   }
   if(a_str->event==evt_start_f)
   { // найден последний старт
     cost_v=a_str->u.fill.UnitPrice;

     if(cost_v <=0)
     {
       cost_v=-1;
       return;
     }
     f_set_cost();
     return;
   }
  }
 }
}
//-------------------------------

float vVolI=-1,vMassI=-1;
void f_prepare_evt (struct arch_str *s_arch, unsigned int evt )
{
  struct tm  mdat;
  TIME_DATE tim_dat;
  int year,month,day,hour,min,sec,i;
union  { float f; char c[4]; long int l; } o;
int  f_rcv;

  s_arch->event=evt;

 if(evt & (int)RCV_FLG)
       f_rcv=1;
 else
       f_rcv=0;


 switch( (int)( evt & ~(int)RCV_FLG) )
 {
   case evt_t_off:
/*
 s_arch->year  = ReadNVRAM( nr_year )  ;
 s_arch->month = ReadNVRAM( nr_month) ;
 s_arch->day   = ReadNVRAM( nr_day  ) ;
 s_arch->hour  = ReadNVRAM( nr_hour ) ;
 s_arch->min   = ReadNVRAM( nr_min  ) ;
 s_arch->sec   = ReadNVRAM( nr_sec  ) ;
*/
 mdat.tm_year = ((int)(ReadNVRAM( nr_year ))&0xff)+100;
 mdat.tm_mon   = ReadNVRAM( nr_month)-1 ;
 mdat.tm_mday  = ReadNVRAM( nr_day  ) ;
 mdat.tm_hour  = ReadNVRAM( nr_hour ) ;
 mdat.tm_min   = ReadNVRAM( nr_min  ) ;
 mdat.tm_sec   = ReadNVRAM( nr_sec  ) ;


// mdat.tm_isdst = 1;
// ReadNVRAM( nr_win_sum ) ;

 s_arch->time  = mktime( &mdat );
 time_last_evt = s_arch->time;

 o.c[0]= ReadNVRAM( nr_v0 );
 o.c[1]= ReadNVRAM( nr_v1 );
 o.c[2]= ReadNVRAM( nr_v2 );
 o.c[3]= ReadNVRAM( nr_v3 );
 s_arch->VolI   = o.f;
 vVolI = o.f;
 o.c[0]= ReadNVRAM( nr_m0 );
 o.c[1]= ReadNVRAM( nr_m1 );
 o.c[2]= ReadNVRAM( nr_m2 );
 o.c[3]= ReadNVRAM( nr_m3 );
 s_arch->MassI  = o.f;
 vMassI=o.f;

 return;

   case evt_t_on:
   break;

   case evt_ext_dt1:

    s_arch->u.fill.VolT       = s_DGM[0].Prod_lvl ;
    s_arch->u.fill.Amount     = s_DGM[0].Temp     ;
    s_arch->u.fill.UnitPrice  = s_DGM[0].VolLiq   ;
    s_arch->u.fill.Density    = s_DGM[0].MassLiq  ;
    s_arch->u.fill.Temp       = s_DGM[0].MassGas  ;
//  s_arch->VolI              = s_DGM[0].DensLiq  ;
    s_arch->VolI              = Press1  ;
    s_arch->MassI             =  s_MVD[1].MassI   ;
        o.f                   = Temp1;
    s_arch->time              = o.l;
    return;


   case evt_ext_dt2:
    s_arch->u.fill.VolT       = s_DGM[1].Prod_lvl ;
    s_arch->u.fill.Amount     = s_DGM[1].Temp     ;
    s_arch->u.fill.UnitPrice  = s_DGM[1].VolLiq   ;
    s_arch->u.fill.Density    = s_DGM[1].MassLiq  ;
    s_arch->u.fill.Temp       = s_DGM[1].MassGas  ;
    s_arch->VolI              = Press2 ;
    s_arch->MassI             =  s_MVD[1].VolI    ;
        o.f                   =  Temp2;
    s_arch->time              = o.l;
    return;

   case evt_deb:

    s_arch->time              =  deb_s[i_deb_s].time        ;  // TimeStamp - time_beg_fill ;
    s_arch->u.fill.UnitPrice  =  deb_s[i_deb_s].out_var     ;  // OUT_VAR ;
    s_arch->u.fill.Density    =  deb_s[i_deb_s].out_var_c_r ;  // OUT_VAR_c_r ;
    s_arch->VolI              =  deb_s[i_deb_s].VolT        ;  // s_MVD[0].Volt  ;
    s_arch->MassI             =  deb_s[i_deb_s].MassT       ;  // s_MVD[0].Masst ;
    s_arch->u.fill.Temp       =  deb_s[i_deb_s].FlowM       ;  // s_MVD[0].FlowM ;

    s_arch->u.fill.Amount     =  deb_s[i_deb_s].P           ;  // s_MVD[0].Press
//  s_arch->u.fill.VolT       =         // s_DGM[0].Prod_lvl ;
//  s_arch->u.fill.Amount     =         // s_DGM[0].Temp     ;
//  s_arch->VolI              =         // s_DGM[0].DensLiq  ;
    return;


   case evt_wrong_start:
   case evt_no_dev:
   case evt_start_f:
   if(f_rcv==0)
   {
    s_arch->u.fill.VolT       = volume_f;
    s_arch->u.fill.Amount     = amount_f;
    s_arch->u.fill.UnitPrice  = cost_v;
    s_arch->u.fill.Density    = Density;
    s_arch->u.fill.Temp       = Temperature;
   }
   else
   {
    s_arch->u.fill.VolT       = dose_dlvr;
    s_arch->u.fill.Amount     = Flag_massR;
    s_arch->u.fill.UnitPrice  = s_MVD[0].Press;
    s_arch->u.fill.Density    = s_MVD[0].Dens;
    s_arch->u.fill.Temp       = s_MVD[0].Temp;
   }
   break;

   case evt_dn_strt:
   case evt_dn_stop:
   case evt_dn_err:
   case evt_dn_ES:

   if(f_rcv != 0)
    {
     s_arch->u.fill.UnitPrice  = s_MVD[0].Press;
     s_arch->u.fill.Density    = s_MVD[0].Dens;
     s_arch->u.fill.Temp       = s_MVD[0].Temp;
    }
   else
    {
     s_arch->u.fill.UnitPrice  = s_MVD[GR].Press;
     s_arch->u.fill.Density    = s_MVD[GR].Dens;
     s_arch->u.fill.Temp       = s_MVD[GR].Temp;
    }


   break;


   case evt_dn_get :
         s_arch->u.fill.VolT       = s_DGM[0].DensReal;
         s_arch->u.fill.Amount     = s_DGM[0].TempReal;
         s_arch->u.fill.UnitPrice  = s_DGM[1].DensReal;
         s_arch->u.fill.Density    = s_DGM[1].TempReal;
         s_arch->u.fill.Temp       = Tank_n;
    break;


   case evt_brk:
    s_arch->u.fill.VolT       = VolT_id;
    s_arch->u.fill.Amount     = MassT_id;
//  s_arch->u.fill.Amount     = summ_v_id;
    s_arch->u.fill.UnitPrice  = cost_v;
    s_arch->u.fill.Density    = Dens0;
    s_arch->u.fill.Temp       = -N_rev;

              break;
   case evt_exm_f  :   // недолив
   case evt_exp_f  :   // перелив
   case evt_end_f  :   // End OK
   case evt_est_f  :   // Command Stop filling
   case evt_err_f  :   // Error Stop filling
   case evt_ep1_f  :   // Host  Stop filling
   case evt_overfl :   // Переполнение
   case evt_gun    :   // Отключен рычаг
   case evt_wd_pmp :   // Таймаут связи
   case evt_ES_off :   // Нажата аварийная кнопка

   if(f_rcv==0)
   {
m_grk:
       s_arch->u.fill.VolT=-s_MVD[GR].VolT;
    /*
       switch(ModeFill)
       {
       case F_Vol:
            s_arch->u.fill.VolT=s_MVD[GR].VolT;
            break;
       case F_Vol_Dens:
            s_arch->u.fill.VolT=s_MVD[GR].VolTd;
            break;
       case F_Mass:
            s_arch->u.fill.VolT=s_MVD[GR].MassT;
            break;
       default:
            s_arch->u.fill.VolT=s_MVD[GR].VolT;
            break;
       }
    */
    s_arch->u.fill.Amount     = -s_MVD[GR].MassT;
    s_arch->u.fill.UnitPrice  = cost_v;
    s_arch->u.fill.Density    = s_MVD[GR].Dens;
    s_arch->u.fill.Temp       = s_MVD[GR].Temp;
   }
   else
   {
    f_cnt_totals();
    s_arch->u.fill.VolT       = VolTotal;
    s_arch->u.fill.Amount     = MassTotal;
    s_arch->u.fill.UnitPrice  = s_MVD[0].Press;
    s_arch->u.fill.Density    = s_MVD[0].Dens;
    s_arch->u.fill.Temp       = s_MVD[0].Temp;
   }
   break;

    case evt_chg_param:   // Изменен параметр через MMI

    s_arch->u.fill.VolT       = chg_nn_par;
    s_arch->u.fill.Amount     = chg_old;
    s_arch->u.fill.UnitPrice  = chg_new;
   break;
   case evt_dsp_grk:
   case evt_kd_grk :
                  goto m_grk;

  case evt_deliv_rep:
    s_arch->u.fill.VolT       = vol_deliv ;
    s_arch->u.fill.Amount     = mass_deliv;
    s_arch->u.fill.UnitPrice  = cost_deliv;
   break;

   case evt_err    :
   case evt_clr_err:
   case evt_ch_date:
   case evt_ch_time:
   case evt_ch_sum :
   case evt_ch_win :
   case evt_res    :
   case evt_verify :
   case evt_Slot   :
   case evt_exit   :
   case evt_re     :
   case evt_re_err :
   case evt_we     :
   case evt_we_err :
   case evt_brk_flt:

   case evt_MVD_CL_strt :
   case evt_MVD_CL_BS1  :
   case evt_MVD_CL_BS2  :
   case evt_MVD_CL_OK   :
   case evt_MVD_CL_TM   :

 m1:
 /*
  for(i=0;(i<=icp_lst_max)&&(i<6);i++)
    s_arch->u.err.err[i]  = ICP_error[i];
 */
    s_arch->u.err.err[0]=((ICP_error[0]<<12) & 0xf000) |
                         ((ICP_error[2]<<8) & 0xf00)   |
                         ((ICP_error[3]<<4) & 0xf0)    |
                         (ICP_error[4] & 0xf)         ;

    s_arch->u.err.err[1]=((ICP_error[5]<<12) & 0xf000) |
                         ((ICP_error[6]<<8) & 0xf00)   |
                         ((ICP_error[7]<<4) & 0xf0)    |
                         (ICP_error[8] & 0xf)         ;
    s_arch->u.err.err[2]=ICP_error[9];
    s_arch->u.err.err[3]=0;
    s_arch->u.err.err[4]=Reg_err;
    s_arch->u.err.err[5]=0;


    break;
   default:
    s_arch->event=evt_none;
    goto m1;
 }

 GetTimeDate(&tim_dat);
 mdat.tm_year  = tim_dat.year -1900 ;
 mdat.tm_mon   = tim_dat.month-1 ;
 mdat.tm_mday  = tim_dat.day   ;
 mdat.tm_hour  = tim_dat.hour  ;
 mdat.tm_min   = tim_dat.minute;
 mdat.tm_sec   = tim_dat.sec   ;

// mdat.tm_isdst = 1;
 s_arch->time  = mktime( &mdat );
 time_last_evt = s_arch->time;

 s_arch->VolI   = s_MVD[GR].VolI;
 s_arch->MassI  = s_MVD[GR].MassI;

}
//-------------------------
struct arch_str ar_str;

void f_get_flash_p()
{
unsigned int seg_tmp;
unsigned long int i,i1,vc,vd,ve;
char  *Flash_ptr_c;
struct arch_str *a_str;

 seg_tmp=0xC000;

 vc= get_vdate((unsigned int)0xC000);
 vd= get_vdate((unsigned int)0xD000);
 ve= get_vdate((unsigned int)0xE000);

 if((vc==0)&&(vd==0)&&(ve==0))
  {  // все секторы очищены, начало записи
    flash_seg=0xC000;
    flash_offset=0;
    return;
  }
  i=0;
/*

  if((vc!=0)&& (vd!=0) && (ve!=0))
  { // нет ни одного пустого сектора
    // очистка самого старого сектора
  i=0xffffffff;
  if(vc<i)
   {i=vc; seg_tmp=0xC000;}
  if(vd<i)
   {i=vd; seg_tmp=0xD000;}
  if(ve<i)
   {i=ve; seg_tmp=0xE000;}

    if( FlashErase(seg_tmp) != 0)
    {
     ICP_error[icp_lst_max] |= Flash_erase_error;
    }
  }
*/
  i=0;
  if(vc>i)
   {i=vc; seg_tmp=0xC000;}
  if(vd>=i)
   {i=vd; seg_tmp=0xD000;}
  if(ve>=i)
   {i=ve; seg_tmp=0xE000;}
  if(vc>=i)
   {i=vc; seg_tmp=0xC000;}

   Flash_ptr_c=(char *) _MK_FP_(seg_tmp,(unsigned int)0);

   for(i=sizeof(ar_str),i1=0;i<= 0xffff;i+= sizeof(ar_str),i1+= sizeof(ar_str) )
     if(Flash_ptr_c[i]== (char)0xff)
     { // сегмент с последней датой заполнен не полностью
      flash_seg=seg_tmp;
      flash_offset=i;
      return;
     }

    flash_seg=f_get_next_seg(seg_tmp);
    flash_offset=0;
    return;
}
//-----------------
unsigned int f_get_next_seg(unsigned int seg)
{
unsigned int seg_tmp;

 if(seg==0xC000) seg_tmp=0xD000;
 else if(seg==0xD000) seg_tmp=0xE000;
 else if(seg==0xE000) seg_tmp=0xC000;
 else seg_tmp=0xC000;

 Flash_ptr=(unsigned long int *) _MK_FP_(seg_tmp,(unsigned int)0);
 if(*Flash_ptr==0xffffffff) ;
 else  if( FlashErase(seg_tmp) != 0)
 {
  ICP_error[icp_lst_max] |= Flash_erase_error;
 }
 return seg_tmp;
}
//-------------------
unsigned int f_get_nxt_seg(unsigned int seg)
{
unsigned int seg_tmp;

 if(seg==0xC000) seg_tmp=0xD000;
 else if(seg==0xD000) seg_tmp=0xE000;
 else if(seg==0xE000) seg_tmp=0xC000;
 else seg_tmp=0xC000;
 return seg_tmp;
}
//-------------------
unsigned int f_get_prev_seg(unsigned int seg)
{
unsigned int seg_tmp;

 if(seg==0xD000) seg_tmp=0xC000;
 else if(seg==0xE000) seg_tmp=0xD000;
 else if(seg==0xC000) seg_tmp=0xE000;
 else seg_tmp=0xC000;

 return seg_tmp;
}
//-------------------
unsigned long int get_vdate(unsigned int seg)
{
 // если дата не корректна , сектор очищается
 struct arch_str *a_str;
 unsigned long int ltmp;
 struct tm *tdat;
 time_t tim_tmp;
 unsigned int evt;
 int nn;

 nn=0;

 a_str=(struct arch_str *) _MK_FP_(seg,(unsigned int)0);

m1:
// evt = a_str->event;
 evt = a_str[nn].event;
// qqqqqqq
// printf("\r\n%d,%d",nn,evt);

 if( evt == (unsigned int)0xffff)
 {
   return 0;
 }
 else if( (evt == evt_deb) || (evt == evt_ext_dt1)|| (evt == evt_ext_dt2)  )
 {
   nn++;
   if(nn > 30) goto erase;
   goto m1;
 }

// tim_tmp=a_str->time - GM_COR;
// tdat = gmtime( &tim_tmp);

// printf(" t=%ld",a_str[nn].time);

 tdat = localtime(&a_str[nn].time);

 if( tdat == NULL ) goto erase;
 return a_str[nn].time;

erase:
    if( FlashErase(seg) != 0)
    {
     ICP_error[icp_lst_max] |= Flash_erase_error;
    }
  return 0;
}
//-------------------
struct arch_str_t ahour00=
{
0,       //   sec
0,       //   min
8,       //   hour
};
//-------------------
struct arch_str_t adate00=
{
0,       //   sec
0,       //   min
0,       //   hour
1,       //   day
1,       //   month
9,       //   year
};
//-------------------
//                   sec min hour
struct tm  mhour00 = { 0, 0,  8 };
//                sec min hour day   month-1, year-1900
struct tm  mdat0= { 0, 34, 01,   30,     6,   109 };
struct tm  mdat = { 0, 0, 12,   25,     11,    88 };
time_t ltime0,ltime;
//----------------------------
time_t f_get_time(struct arch_str *ar_str_ptr)
{
 return ar_str_ptr->time;
/*
struct tm  mdat;
    mdat.tm_sec  = ar_str_ptr->sec  ;
    mdat.tm_min  = ar_str_ptr->min  ;
    mdat.tm_hour = ar_str_ptr->hour ;
    mdat.tm_mday = ar_str_ptr->day  ;
    mdat.tm_mon  = ar_str_ptr->month-1;
    mdat.tm_year = ((int)(ar_str_ptr->year)&0xff)+100;
    return mktime( &mdat );
*/
}
//-------------------------------
void f_prepare_t(struct arch_str_t *s_arch)
{
  TIME_DATE tim_dat;
 GetTimeDate(&tim_dat);
 s_arch->year  = tim_dat.year -2000 ;
 s_arch->month = tim_dat.month ;
 s_arch->day   = tim_dat.day   ;
 s_arch->hour  = tim_dat.hour  ;
 s_arch->min   = tim_dat.minute;
 s_arch->sec   = tim_dat.sec   ;
}

//-------------------------------
time_t f_get_time_t(struct arch_str_t *ar_str_ptr)
{
struct tm  mdat;
    mdat.tm_sec  = ar_str_ptr->sec  ;
    mdat.tm_min  = ar_str_ptr->min  ;
    mdat.tm_hour = ar_str_ptr->hour ;
    mdat.tm_mday = ar_str_ptr->day  ;
    mdat.tm_mon  = ar_str_ptr->month-1;
    mdat.tm_year = ((int)(ar_str_ptr->year)&0xff)+100;

// mdat.tm_isdst = 1;

    return mktime( &mdat );
}
//----------------------------------
float tMassI,tVolI;

int s_mode=2;
//--------------------------------------------

int f_get_d_dat(time_t time_00, int mode)
{ // возвращает 0 - OK, данные записаны в tMassI,tVolI
  // если данных на эту дату нет - возвращает -1;

float k_tmp;
//unsigned int seg_tmp,seg_tmp_2,offs_tmp,utmp;
struct arch_str *ar_str_ptr,*ar_str_ptr_1,*ar_str_ptr_p1;
long int ltmp;
int nn,nn1,nn2,nn3,i;
time_t ltim,ltim_1,ltim_p1,ltime_l;
 struct tm *tdat;

 // определение начала журнала

      f_cycle1();
/*
  seg_tmp= f_get_prev_seg(flash_seg);
  ar_str_ptr=(struct arch_str *) _MK_FP_(seg_tmp,(unsigned int)0);
  if(ar_str_ptr->event != 0xffff)
  {
   seg_tmp_2= f_get_prev_seg(seg_tmp);
   ar_str_ptr=(struct arch_str *) _MK_FP_(seg_tmp_2,(unsigned int)0);
   if(ar_str_ptr->event != 0xffff)
   {
    seg_tmp=seg_tmp_2;
   }
  }
  else
  {
    seg_tmp= flash_seg;
    ar_str_ptr=(struct arch_str *) _MK_FP_(flash_seg,(unsigned int)0);
    if(ar_str_ptr->event == 0xffff) return -1;
  }
  ar_str_ptr=(struct arch_str *) _MK_FP_(seg_tmp,(unsigned int)0);

//  seg_tmp - начальный сегмент
*/


  ar_str_ptr=f_get_fst_stor();
  ltim= f_get_time(ar_str_ptr);   // время первой записи
//seg_tmp=retf_seg;

  ar_str_ptr= f_get_lst_stor();
  ltime_l=f_get_time(ar_str_ptr); // время последней записи

/*
    tdat = localtime( &ltim );
    printf("\r\nBeg:%02d.%02d.%02d %02d:%02d:%02d",
    tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);

    tdat = localtime( &time_00 );
    printf("\r\nT00:%02d.%02d.%02d %02d:%02d:%02d",
    tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);

    tdat = localtime( &ltime_l );
    printf("\r\nEnd:%02d.%02d.%02d %02d:%02d:%02d",
    tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);
*/

 if(time_00 < ltim)
 {
   goto m_fault;
 }

 if(time_00 > ltime_l)
 {
   goto m_fault;
 }

 nn=f_get_max_n_arch();
 nn1=0;

for(nn2=4096;nn2>0;nn2=nn2>>1)
{
 nn3=nn1+nn2;
 if(nn3> (nn-1)) continue;

 ar_str_ptr=f_get_astr(nn3);

 if(ar_str_ptr->time > time_00);
 else nn1=nn3;

}
/*
    tdat = localtime( &ar_str_ptr->time );
    printf("\r\nFound:%02d.%02d.%02d %02d:%02d:%02d",
    tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);
*/
  // найти 1-ю запись со значениями
   for(i=nn1;i>=0;i--)
   {
    ar_str_ptr=f_get_astr(i);
    if((ar_str_ptr->MassI != vBIG_P)&& (ar_str_ptr->VolI != vBIG_P))
    {
     ar_str_ptr_1=f_get_astr(i);
     ltim_1=ar_str_ptr_1->time;
     goto m0;
    }
   }
   // printf("\r\n  1, i=%d",i);
m_fault:
   return -1;


m0:
  // найти 2-ю запись со значениями
   for(i=nn1+1;i<nn;i++)
   {
    ar_str_ptr=f_get_astr(i);
    if((ar_str_ptr->MassI != vBIG_P)&& (ar_str_ptr->VolI != vBIG_P))
    {
      ar_str_ptr=f_get_astr(i);
      ltim=ar_str_ptr->time;
      goto m_ok;
    }
   }
   // printf("\r\n  2, i=%d",i);
   return -1;


m_ok:

  if(ltim==time_00)
  {
   tMassI=ar_str_ptr->MassI;
   tVolI =ar_str_ptr->VolI;
   //printf("\r\n 1");
   goto m_prn;
  }
  if(ar_str_ptr_1==NULL) return -1;
  if(ltim==ltim_1) return -1;

 // искомое время между двумя указателями

 if( (ar_str_ptr->MassI==ar_str_ptr_1->MassI)  && (ar_str_ptr->VolI==ar_str_ptr_1->VolI) )
 {
   tMassI=ar_str_ptr->MassI;
   tVolI =ar_str_ptr->VolI;
   //printf("\r\n 2");
   goto m_prn;
 }
   // данные изменялись между двумя записями
  switch(mode)
  {
   case tMin:
   m3:
     tMassI=ar_str_ptr_1->MassI;
     tVolI =ar_str_ptr_1->VolI;
     break;
   case tMax:
    m4:
     tMassI=ar_str_ptr->MassI;
     tVolI =ar_str_ptr->VolI;
     break;
   case tNear:
        if((time_00-ltim_1) < (ltim-time_00))
        {
            goto m3;
        }
        else
        {
            goto m4;
        }
   case tIntrp:
        if(ltim==ltim_1)
        {
          tMassI= ar_str_ptr->MassI;
          tVolI = ar_str_ptr->VolI;
        }
        else
        {
          k_tmp= (float)(time_00-ltim_1)/(float)(ltim-ltim_1);
          tMassI= ar_str_ptr_1->MassI + k_tmp*(ar_str_ptr->MassI-ar_str_ptr_1->MassI);
          tVolI = ar_str_ptr_1->VolI   + k_tmp*(ar_str_ptr->VolI-ar_str_ptr_1->VolI);
        }
        break;
    default: return -1;
  }
m_prn:

   //printf("\r\n End ok");
   return 0;
}
//------------------------------------------
unsigned int d_fl_seg=0xC000, d_fl_offs=0;
struct arch_str  *f_get_dat_s(time_t time_00)
{ // возвращает указатель на структуру с данными
  // или NULL ;

struct arch_str *ar_str_ptr;
long int ltmp;
int nn,nn1,nn2,nn3,i;
time_t ltim,ltim_1,ltim_p1,ltime_l;
 struct tm *tdat;

      f_cycle1();

  ar_str_ptr=f_get_fst_stor();
  ltim= f_get_time(ar_str_ptr);   // время первой записи

  ar_str_ptr= f_get_lst_stor();
  ltime_l=f_get_time(ar_str_ptr); // время последней записи

/*
    tdat = localtime( &ltim );
    printf("\r\nBeg:%02d.%02d.%02d %02d:%02d:%02d",
    tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);

    tdat = localtime( &time_00 );
    printf("\r\nT00:%02d.%02d.%02d %02d:%02d:%02d",
    tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);

    tdat = localtime( &ltime_l );
    printf("\r\nEnd:%02d.%02d.%02d %02d:%02d:%02d",
    tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);
*/

 if(time_00 < ltim)
 {
   goto m_fault;
 }

 if(time_00 > ltime_l)
 {
   goto m_fault;
 }

 nn=f_get_max_n_arch();
 nn1=0;

for(nn2=4096;nn2>0;nn2=nn2>>1)
{
 nn3=nn1+nn2;
 if(nn3> (nn-1)) continue;

 ar_str_ptr=f_get_astr(nn3);

 if(ar_str_ptr->time > time_00);
 else nn1=nn3;

}
/*
    tdat = localtime( &ar_str_ptr->time );
    printf("\r\nFound:%02d.%02d.%02d %02d:%02d:%02d",
    tdat->tm_mday,tdat->tm_mon+1,tdat->tm_year-100,tdat->tm_hour,tdat->tm_min,tdat->tm_sec);
*/

m_ok:
 retf_iii=nn1;
 return ar_str_ptr;

m_fault:
 retf_iii=0;
 return NULL;
}

//----------------------------------
int f_get_dat_n(time_t time_00)
{ // возвращает номер записи или -1 ,если нет данных
  // или NULL ;
int nn;

 if( f_get_dat_s(time_00)== NULL) return -1;

 return retf_iii;

}

//----------------------------------



//------------------------------------------
