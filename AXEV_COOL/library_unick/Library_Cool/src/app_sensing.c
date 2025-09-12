#include <app_common.h>

static void Sensing_Voltage(void)
{
    static uint8_t u8_voltage_avg_cnt = 0;
    static uint16_t u16_voltage_value = 0;
    static uint32_t u32_voltage_sum_value = 0;
    uint32_t u32_voltage_avg_value = 0;

    AD1_GetChanValue16(CHANNEL_IGN1,&u16_voltage_value);
    if(u8_voltage_avg_cnt >= (uint8_t)USER_VOLT_ADC_CNT){
        u32_voltage_avg_value = u32_voltage_sum_value / (uint32_t)USER_VOLT_ADC_CNT;
        CoolValveInfo.sAdc.u32IgnVoltage = u32_voltage_avg_value;
        u32_voltage_sum_value = 0;
        u8_voltage_avg_cnt = 0;
    }
    else{
        u32_voltage_sum_value += u16_voltage_value;
        u8_voltage_avg_cnt++;
    }
}

static void Sensing_Current(void)
{
    static uint8_t u8_current_avg_cnt = 0;
    static uint16_t u16_current_value = 0;
    static uint32_t u32_current_sum_value = 0;
    uint32_t u32_current_avg_value = 0;

    AD1_GetChanValue16(CHANNEL_CURRENT,&u16_current_value);
    if(u8_current_avg_cnt >= (uint8_t)USER_CUR_ADC_CNT){
        u32_current_avg_value = u32_current_sum_value / (uint32_t)USER_CUR_ADC_CNT;
        CoolValveInfo.sAdc.u32MotorCurrent = u32_current_avg_value;
        u32_current_sum_value = 0;
        u8_current_avg_cnt = 0;
    }
    else{
        u32_current_sum_value += u16_current_value;
        u8_current_avg_cnt++;
    }
}

static void Sensing_Temp(void)
{
    static uint8_t u8_temp_avg_cnt = 0;
    static int16_t s16_temp_value = 0;
    static uint32_t u32_temp_sum_value = 0;
    uint32_t u32_temp_avg_value = 0;
    uint32_t u32_kTempMinusVaule = 1000;

    MES_GetChipTemperature(&s16_temp_value);
    if(u8_temp_avg_cnt >= (uint8_t)USER_TEMP_ADC_CNT){
        u32_temp_avg_value = u32_temp_sum_value / (uint32_t)USER_TEMP_ADC_CNT;
        CoolValveInfo.sAdc.u32McuTemperature = u32_temp_avg_value;
        u32_temp_sum_value = 0;
        u8_temp_avg_cnt = 0;
    }
    else{
        u32_temp_sum_value += (uint32_t)s16_temp_value + u32_kTempMinusVaule;
        u8_temp_avg_cnt++;
    }
}

static void Sensing_Stall_Hall(void)
{
    static uint8_t u8_stall_pulse_value = 0;
    static uint16_t u16_stall_Herror_cnt = 0;
    static uint16_t u16_stall_Lerror_cnt = 0;
    
    if(CoolValveInfo.sMotor.u8MotorRunning == (uint8_t)MOVING){
        u8_stall_pulse_value = GPIO_Read(GPIO_PORT_1);
        if(u8_stall_pulse_value == (uint8_t)1){
            u16_stall_Herror_cnt++;
            if(u16_stall_Herror_cnt >= (uint16_t)USER_STALL_CNT){
                CoolValveInfo.sMotor.u8MotorStallStatus = (uint8_t)ERROR_STATUS;
                u16_stall_Herror_cnt = 0;
            }
            else{
                CoolValveInfo.sMotor.u8MotorStallStatus = (uint8_t)NORMAL_STATUS;
                u16_stall_Lerror_cnt = 0;
            }
        }
        
        if(u8_stall_pulse_value == (uint8_t)0){
            u16_stall_Lerror_cnt++;
            if(u16_stall_Lerror_cnt >= (uint16_t)USER_STALL_CNT){
                CoolValveInfo.sMotor.u8MotorStallStatus = (uint8_t)ERROR_STATUS;
                u16_stall_Lerror_cnt = 0;
            }
            else{
                CoolValveInfo.sMotor.u8MotorStallStatus = (uint8_t)NORMAL_STATUS;
                u16_stall_Herror_cnt = 0;
            }
        }
    }
}

static void Sensing_Stall_Current(void)
{
    if((CoolValveInfo.sAdc.u32MotorCurrent >= (uint32_t)MOTOR_STALL_CUR_MIN) && (CoolValveInfo.sAdc.u32MotorCurrent <= (uint32_t)MOTOR_STALL_CUR_MAX)){
        CoolValveInfo.sMotor.u8MotorCurrnetStatus = (uint8_t)ERROR_STATUS;
    }
    else{
        CoolValveInfo.sMotor.u8MotorCurrnetStatus = (uint8_t)NORMAL_STATUS;
    }
}

uint8_t Hall_Sampling(void)
{
    static uint16_t u16_hall_pulse_cnt = 0;
    static uint8_t u8_hall_pulse_start = 0;
    uint8_t u8_hall_pulse_value = 0;
    uint8_t u8_hall_pulse_return = 0;
  
    u8_hall_pulse_value = GPIO_Read(GPIO_PORT_1);
    if(u8_hall_pulse_value == (uint8_t)1){
        if(u8_hall_pulse_start == (uint8_t)0){
            u8_hall_pulse_start = 1;
        }
        else if(u8_hall_pulse_start == (uint8_t)1){
            u16_hall_pulse_cnt++;
        }
        else{
            /* empty */
        }
    }
    else if(u8_hall_pulse_value == (uint8_t)0){
        if(u8_hall_pulse_start == (uint8_t)1){
            u8_hall_pulse_return = 1;
            u8_hall_pulse_start = 0;
            u16_hall_pulse_cnt = 0;
        }
        else if(u8_hall_pulse_start == (uint8_t)0){
            u8_hall_pulse_return = 0;
            u8_hall_pulse_start = 0;
            u16_hall_pulse_cnt = 0;
        }
        else{
            /* empty */
        }
    }
    else{
        /* empty */
    }
  
    return u8_hall_pulse_return;
}

void Sensing_Task(void)
{
    Sensing_Stall_Hall();
    Sensing_Voltage();
    Sensing_Current();
    Sensing_Stall_Current();
    Sensing_Temp();
}
