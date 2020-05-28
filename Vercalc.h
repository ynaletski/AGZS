struct s_ver_calc
{
    float VolTank;
    float VolLiqPhase;
    float TempLiqPhase;
    float TempGasPhase;
    float DensLiqPhase;
    float PressTank;
    float DensGasPhase;
    float VolGasPhase;
    float MassGasPhase;
    float MassLiqPhase;
    float PressGasPhase;
    float CompGasPhase; //не используется особо вроде
};

extern struct s_ver_calc s_CALC;

void f_get_pressCT_ver_calc ( float Compos, float Temp);
float f_get_PD_Etan_ver_calc(float Temp);
void f_count_tank_ver_calc();