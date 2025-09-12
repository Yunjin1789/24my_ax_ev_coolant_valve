#ifndef APP_FAILSAFETY_H_
#define APP_FAILSAFETY_H_

#define CONTROLLER_CONDITION()          (CoolValveInfo.uFaultFlag.unVal == (uint32_t)NORMAL_STATUS)

#define IGN1_VOLT_FAIL_MAX              17000U
#define IGN1_VOLT_FAIL_MIN              8000U
#define IGN1_VOLT_NORM_MAX              16500U
#define IGN1_VOLT_NORM_MIN              8500U

#define MOTOR_ADC_CUR_MAX               65535U
#define MOTOR_OPEN_CUR_MAX              10U
#define MOTOR_OPEN_CUR_MIN              65300U
#define MOTOR_SHORT_CUR_MAX             50000U
#define MOTOR_SHORT_CUR_MIN             5777U

#define MOTOR_TEMP_FAIL_MAX             1120U
#define MOTOR_TEMP_WARN_MAX             1110U
#define MCU_TEMP_FAIL_MAX               1120U
#define MCU_TEMP_WARN_MAX               1110U
#define MCU_TEMP_NORMAL_MAX             960U

#define MCU_WAKEUP_TIMER                200U            // 2s
#define MOTOR_FAULT_CNT                 50U

typedef union tag_uFaultFlag
{
    uint32_t unVal;
    struct{
        unsigned int    u8FaultFlagMotorCoilShort    : 1;
        unsigned int    u8FaultFlagMotorCoilOpen     : 1;
        unsigned int    u8FaultFlagOverTemp          : 1;
        unsigned int    u8FaultFlagReserved1         : 1;
        unsigned int    u8FaultFlagMotorStallFail    : 1;
        
        unsigned int    u8FaultFlagOverVoltage       : 1;
        unsigned int    u8FaultFlagUnderVoltage      : 1;
        unsigned int    u8FaultFlagOverTempWarn      : 1;
    }bits;
}FAULTFLAG;

void Failsafety_Task(void);

#endif
