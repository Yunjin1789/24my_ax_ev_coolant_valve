#ifndef APP_SENSING_H_
#define APP_SENSING_H_

#define MOTOR_STALL_CUR_MAX             50000U
#define MOTOR_STALL_CUR_MIN             1300U

#define USER_VOLT_ADC_CNT               100U
#define USER_CUR_ADC_CNT                100U
#define USER_TEMP_ADC_CNT               100U
#define USER_STALL_CNT                  250U

#define CHANNEL_TEMP                    1U
#define CHANNEL_CURRENT                 2U
#define CHANNEL_IGN1                    3U

typedef struct tag_sAdcInformation
{
    uint32_t              u32IgnVoltage;
    uint32_t              u32McuTemperature;
    uint32_t              u32MotorCurrent;
}ADCINFO;

uint8_t Hall_Sampling(void);
void Sensing_Task(void);

#endif
