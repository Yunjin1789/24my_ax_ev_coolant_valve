#include <app_common.h>

CONTROLINFO CoolValveInfo;


static void Motor_Start(void)
{
    static double pwm_value_20khz_base;
    static double pwm_value_20khz;
    static double pwm_value_16khz;
    static double target_duty;

    if (CoolValveInfo.sAdc.u32IgnVoltage >= (uint32_t)MOTOR_MAXPWM_VOLTAGE)
    {
        pwm_value_20khz_base = (double)PWM_DUTY68;
    }
    else if (CoolValveInfo.sAdc.u32IgnVoltage <= (uint32_t)MOTOR_MINPWM_VOLTAGE)
    {
        pwm_value_20khz_base = (double)PWM_DUTY100;
    }
    else if (CoolValveInfo.sAdc.u32IgnVoltage > (uint32_t)MOTOR_MINPWM_VOLTAGE && CoolValveInfo.sAdc.u32IgnVoltage < (uint32_t)MOTOR_MAXPWM_VOLTAGE)
    {
        pwm_value_20khz_base = 100 - ((((double)CoolValveInfo.sAdc.u32IgnVoltage - (double)MOTOR_MINPWM_VOLTAGE) / 7000) * 32);
    }
        
    if (CoolValveInfo.sMotor.u8ValveSoftstopMode == (uint8_t)DISABLED)
    {
        pwm_value_20khz = pwm_value_20khz_base;
    }
    else
    {
        if (CoolValveInfo.sMotor.u8HallValue < CoolValveInfo.sMotor.u8SoftHallValue)
        {
            pwm_value_20khz = pwm_value_20khz_base;
        }
        else if (CoolValveInfo.sMotor.u8HallValue < CoolValveInfo.sMotor.u8SealHallValue)
        {
            target_duty = pwm_value_20khz_base * 0.7;
            if (target_duty < 60.0) target_duty = 60.0;

            pwm_value_20khz = pwm_value_20khz_base - ((pwm_value_20khz_base - target_duty) *  (CoolValveInfo.sMotor.u8HallValue - CoolValveInfo.sMotor.u8SoftHallValue) / (CoolValveInfo.sMotor.u8SealHallValue - CoolValveInfo.sMotor.u8SoftHallValue));
        }
        else
        {
            target_duty = pwm_value_20khz_base * 0.7;
            pwm_value_20khz = (target_duty < 60.0) ? 60.0 : target_duty;
        }
    }

    pwm_value_16khz = (pwm_value_20khz * PWM_DUTY62) / 100.0; /* 20khz - 16khz cal */
    CoolValveInfo.sMotor.u8MotorRunning = (uint8_t)MOVING;
    Motor_PWM_SetRatio16((uint32_t)pwm_value_16khz);
}

static void Motor_Stop(void)
{
    M_ENBL_PutVal(0);
    M_DIS_PutVal(1);
    M_IN1_PutVal(1);
    Motor_PWM_SetRatio16(0);
}

static void Motor_Direction(MOTORDIRECTION num)
{
    if(num == (MOTORDIRECTION)MotorDirectionCCW){
        M_ENBL_PutVal(1);
        M_DIS_PutVal(0);
        M_IN1_PutVal(0);
        CoolValveInfo.sMotor.u8MotorDirection = (uint8_t)MotorDirectionCCW;
    }
    else if(num == (MOTORDIRECTION)MotorDirectionCW){
        M_ENBL_PutVal(1);
        M_DIS_PutVal(0);
        M_IN1_PutVal(1);
        CoolValveInfo.sMotor.u8MotorDirection = (uint8_t)MotorDirectionCW;
    }
    else{
        /* empty */
    }
}

static void Motor_Wakeup_Check(void)
{
    static uint8_t u8_motor_wakeup_cnt = 0;
    
    if(CoolValveInfo.sMotor.u8ValveWakeupMode == (uint8_t)DISABLED){
        u8_motor_wakeup_cnt++;
        if(u8_motor_wakeup_cnt > (uint8_t)100){
            u8_motor_wakeup_cnt = 0;
            M_ENBL_PutVal(1);
            M_DIS_PutVal(0);
            CoolValveInfo.sMotor.u8ValveWakeupMode = (uint8_t)ENABLED;
        }
    }
}

static void Motor_InitReq_Check(void)
{
    CoolValveInfo.sMotor.u8MotorInitModeFirst = (uint8_t)ENABLED;
    if((CoolValveInfo.sMotor.u8MotorInitReq == (uint8_t)ENABLED) && (CoolValveInfo.sMotor.u8MotorInitReqFirst == (uint8_t)DISABLED)){
        CoolValveInfo.sMotor.u8MotorInitReqFirst = (uint8_t)ENABLED;
        CoolValveInfo.sMotor.u8MotorInitDone = (uint8_t)INIT_NOT_DONE;
    }
}

static void Motor_Reset_Release(void)
{
    static uint16_t u16_motor_release_cnt = 0;
    static uint16_t u16_motor_tempwarn_cnt = 0;
    
    if((CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail == (uint8_t)ERROR_STATUS) || (CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp == (uint8_t)ERROR_STATUS)){
        u16_motor_release_cnt++;
        if(u16_motor_release_cnt >= (uint16_t)MOTOR_RELEASE_TMR){
            u16_motor_release_cnt = 0;
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)NORMAL_STATUS;
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp = (uint8_t)NORMAL_STATUS;
            CoolValveInfo.sMotor.u8MotorInitDone = (uint8_t)INIT_NOT_DONE;
        }
    }
    else{
        u16_motor_release_cnt = 0;
    }
    
    if(CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTempWarn == (uint8_t)ERROR_STATUS){
        u16_motor_tempwarn_cnt++;
        if(u16_motor_tempwarn_cnt >= (uint16_t)MOTOR_TRELEASE_TMR){
            u16_motor_tempwarn_cnt = 0;
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTempWarn = (uint8_t)NORMAL_STATUS;
        }
    }
    else{
        u16_motor_tempwarn_cnt = 0;
    }
}

static void Motor_RunCnt_Release(void)
{
    if(CoolValveInfo.sMotor.u8MotorRunning == (uint8_t)MOVING){
        CoolValveInfo.sMotor.u16MotorRunningCount++;
    }
    else if(CoolValveInfo.sMotor.u8MotorRunning == (uint8_t)MOVING_DONE){
        CoolValveInfo.sMotor.u16MotorRunningCount = 0;
    }
    else{
        /* empty */
    }
}

static void Motor_Init_Func(void)
{
    static uint16_t u16_initmotor_stop_cnt = 0;
    static uint8_t u8_initmotorCW_stall_flag = 0;
    static uint8_t u8_initmotorCCW_stall_flag = 0;
    static uint8_t u8_initmotor_Rotation = 0;
    static uint8_t u8_initmotor_mode = 0;

    static uint8_t u8_inithall_pulse_sampling = 0;
    static uint8_t u8_inithall_pulse_error = 0;
    static uint8_t u8_init_retryCW_cnt = 0;
    static uint8_t u8_init_retryCCW_cnt = 0;
    
    if(u8_initmotor_Rotation == (uint8_t)0){
        Motor_Direction(MotorDirectionCCW);
        Motor_Start();
        if(CoolValveInfo.sMotor.u8MotorInitDone != (uint8_t)INIT_DLEAY){
            CoolValveInfo.sMotor.u8MotorInitDone = (uint8_t)INIT_ING;
        }

        u8_inithall_pulse_sampling = Hall_Sampling();
        if(u8_inithall_pulse_sampling == (uint8_t)1){
            u8_inithall_pulse_sampling = 0;
            CoolValveInfo.sMotor.u8HallValue++;
        }

        if(u8_initmotorCCW_stall_flag == (uint8_t)0){
            if((CoolValveInfo.sMotor.u8MotorCurrnetStatus == (uint8_t)ERROR_STATUS) && (CoolValveInfo.sMotor.u8MotorStallStatus == (uint8_t)ERROR_STATUS)){
                CoolValveInfo.sMotor.u8MotorCurrnetStatus = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.sMotor.u8MotorStallStatus = (uint8_t)NORMAL_STATUS;
                if(CoolValveInfo.sMotor.u8HallValue >= (uint8_t)u8_inithall_pulse_error){
                    u8_initmotorCCW_stall_flag = 1;
                    u8_init_retryCCW_cnt = 0;
                    CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)NORMAL_STATUS;
                }
                else if(CoolValveInfo.sMotor.u8HallValue < (uint8_t)u8_inithall_pulse_error){
                    Motor_Stop();
                    u8_initmotor_Rotation = 1;
                    CoolValveInfo.sMotor.u8HallValue = 0;
                    u8_init_retryCCW_cnt++;
                    if(u8_init_retryCCW_cnt > (uint8_t)INIT_RETRY_CNT){
                        CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)ERROR_STATUS;
                        u8_init_retryCCW_cnt = 0;
                        u8_init_retryCW_cnt = 0;
                    }
                }
                else{
                    /* empty */
                }
            }
            else if((CoolValveInfo.sMotor.u8MotorCurrnetStatus == (uint8_t)NORMAL_STATUS) && (CoolValveInfo.sMotor.u8MotorStallStatus == (uint8_t)ERROR_STATUS)){
                if(CoolValveInfo.sMotor.u16MotorRunningCount <= (uint8_t)MOTOR_RUNERROR_TMR){
                    CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)NORMAL_STATUS;
                }
                else if(CoolValveInfo.sMotor.u16MotorRunningCount > (uint8_t)MOTOR_RUNERROR_TMR){
                    CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)ERROR_STATUS;
                }
                else{
                    /* empty */
                }
            }
            else{
                /* empty */
            }
        }
        else if(u8_initmotorCCW_stall_flag == (uint8_t)1){
            if(u8_initmotor_mode == (uint8_t)ENABLED){
                u16_initmotor_stop_cnt++;
                CoolValveInfo.sMotor.u8MotorInitDone = (uint8_t)INIT_DLEAY;
                if(u16_initmotor_stop_cnt >= (uint16_t)MOTOR_INITSTOP_CNT){
                    Motor_Stop();
                    u16_initmotor_stop_cnt = 0;
                    u8_initmotorCCW_stall_flag = 0;
                    
                    u8_init_retryCCW_cnt = 0;
                    u8_init_retryCW_cnt = 0;
                    u8_initmotor_mode = 0;
                    u8_initmotor_Rotation = 0;
                    u8_inithall_pulse_error = 0;
                    CoolValveInfo.sMotor.u8HallValue = 0;
                    CoolValveInfo.sMotor.u8MotorRunning = (uint8_t)MOVING_DONE;
                    CoolValveInfo.sMotor.u16MotorRunningCount = 0;
                    CoolValveInfo.sMotor.u8MotorInitMode = (uint8_t)DISABLED;
                    CoolValveInfo.sMotor.u8MotorInitDone = (uint8_t)INIT_DONE;
                    CoolValveInfo.sMotor.u8MotorPositionFB = (uint8_t)MotorDirectionCCW;
                }
            }
            else{
                u16_initmotor_stop_cnt++;
                if(u16_initmotor_stop_cnt >= (uint16_t)MOTOR_INITSTOP_CNT){
                    Motor_Stop();
                    u16_initmotor_stop_cnt = 0;
                    u8_initmotorCCW_stall_flag = 0;
                    
                    u8_initmotor_Rotation = 1;
                    CoolValveInfo.sMotor.u8MotorRunning = (uint8_t)MOVING_DONE;
                    CoolValveInfo.sMotor.u16MotorRunningCount = 0;
                }
            }
        }
        else{
            /* empty */
        }
    }
    
    if(u8_initmotor_Rotation == (uint8_t)1){
        Motor_Direction(MotorDirectionCW);
        Motor_Start();
        if(CoolValveInfo.sMotor.u8MotorInitDone != (uint8_t)INIT_DLEAY){
            CoolValveInfo.sMotor.u8MotorInitDone = (uint8_t)INIT_ING;
        }
        
        u8_inithall_pulse_sampling = Hall_Sampling();
        if(u8_inithall_pulse_sampling == (uint8_t)1){
            u8_inithall_pulse_sampling = 0;
            CoolValveInfo.sMotor.u8HallValue++;
        }
      
        if(u8_initmotorCW_stall_flag == (uint8_t)0){
            if((CoolValveInfo.sMotor.u8MotorCurrnetStatus == (uint8_t)ERROR_STATUS) && (CoolValveInfo.sMotor.u8MotorStallStatus == (uint8_t)ERROR_STATUS)){
                CoolValveInfo.sMotor.u8MotorCurrnetStatus = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.sMotor.u8MotorStallStatus = (uint8_t)NORMAL_STATUS;
                if(CoolValveInfo.sMotor.u8HallValue >= (uint8_t)MOTOR_INIT_STALL_CNT){
                    u8_initmotorCW_stall_flag = 1;
                    u8_init_retryCW_cnt = 0;
                    CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)NORMAL_STATUS;
                }
                else if(CoolValveInfo.sMotor.u8HallValue < (uint8_t)MOTOR_INIT_STALL_CNT){
                    Motor_Stop();
                    u8_initmotor_Rotation = 0;
                    CoolValveInfo.sMotor.u8HallValue = 0;
                    u8_init_retryCW_cnt++;
                    if(u8_init_retryCW_cnt > (uint8_t)INIT_RETRY_CNT){
                        CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)ERROR_STATUS;
                        u8_init_retryCW_cnt = 0;
                        u8_init_retryCCW_cnt = 0;
                    }
                }
                else{
                    /* empty */
                }
            }
            else if((CoolValveInfo.sMotor.u8MotorCurrnetStatus == (uint8_t)NORMAL_STATUS) && (CoolValveInfo.sMotor.u8MotorStallStatus == (uint8_t)ERROR_STATUS)){
                if(CoolValveInfo.sMotor.u16MotorRunningCount <= (uint8_t)MOTOR_RUNERROR_TMR){
                    CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)NORMAL_STATUS;
                }
                else if(CoolValveInfo.sMotor.u16MotorRunningCount > (uint8_t)MOTOR_RUNERROR_TMR){
                    CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)ERROR_STATUS;
                }
                else{
                    /* empty */
                }
            }
            else{
                /* empty */
            }
        }
        else if(u8_initmotorCW_stall_flag == (uint8_t)1){
            if(CoolValveInfo.sMotor.u8MotorInitMode == (uint8_t)ENABLED){
                u16_initmotor_stop_cnt++;
                if(u16_initmotor_stop_cnt >= (uint16_t)MOTOR_INITSTOP_CNT){
                    Motor_Stop();
                    u16_initmotor_stop_cnt = 0;
                    u8_initmotorCW_stall_flag = 0;
                    
                    u8_inithall_pulse_error = (uint8_t)MOTOR_REQ_STALL_CNT;
                    u8_initmotor_mode = (uint8_t)ENABLED;
                    CoolValveInfo.sMotor.u8HallValue = 0;
                    u8_initmotor_Rotation = 0;
                    CoolValveInfo.sMotor.u8MotorRunning = (uint8_t)MOVING_DONE;
                    CoolValveInfo.sMotor.u16MotorRunningCount = 0;
                }
            }
            else{
                u16_initmotor_stop_cnt++;
                CoolValveInfo.sMotor.u8MotorInitDone = (uint8_t)INIT_DLEAY;
                if(u16_initmotor_stop_cnt >= (uint16_t)MOTOR_INITSTOP_CNT){
                    Motor_Stop();
                    u16_initmotor_stop_cnt = 0;
                    u8_initmotorCW_stall_flag = 0;
                    
                    u8_init_retryCW_cnt = 0;
                    u8_init_retryCCW_cnt = 0;
                    u8_initmotor_mode = 0;
                    u8_initmotor_Rotation = 0;
                    CoolValveInfo.sMotor.u8HallValue = 0;
                    CoolValveInfo.sMotor.u8MotorRunning = (uint8_t)MOVING_DONE;
                    CoolValveInfo.sMotor.u16MotorRunningCount = 0;
                    CoolValveInfo.sMotor.u8MotorInitDone = (uint8_t)INIT_DONE;
                    CoolValveInfo.sMotor.u8MotorPositionFB = (uint8_t)MotorDirectionCW;
                }
            }
        }
        else{
            /* empty */
        }
    }
}
  
static void Motor_ReqRetry_Func(void)
{
    if (CoolValveInfo.sMotor.u8MotorCurrnetStatus == (uint8_t)ERROR_STATUS && CoolValveInfo.sMotor.u8MotorStallStatus == (uint8_t)ERROR_STATUS)
    {
        CoolValveInfo.sMotor.u8MotorCurrnetStatus = (uint8_t)NORMAL_STATUS;
        CoolValveInfo.sMotor.u8MotorStallStatus = (uint8_t)NORMAL_STATUS;
        Motor_Stop();
        CoolValveInfo.sMotor.u8ValveReqRetryMode = (uint8_t)ENABLED;
        CoolValveInfo.sMotor.u8SoftHallValue = CoolValveInfo.sMotor.u8HallValue - (MOTOR_TARGET_HALL - MOTOR_SOFTSTOP_HALL);
        CoolValveInfo.sMotor.u8SealHallValue = CoolValveInfo.sMotor.u8HallValue - (MOTOR_TARGET_HALL - MOTOR_SEAL_HALL);
        CoolValveInfo.sMotor.u8TargetHallValue = CoolValveInfo.sMotor.u8HallValue;
        CoolValveInfo.sMotor.u8HallValue = 0;

        if (CoolValveInfo.sLin.sReceive.C_3way_PosReq == (uint8_t)MotorDirectionCW)
        {
            CoolValveInfo.sLin.sReceive.C_3way_PosReq = (uint8_t)MotorDirectionCCW;
            CoolValveInfo.sMotor.u8ValveCWRetryCnt++;
            if (CoolValveInfo.sMotor.u8ValveCWRetryCnt > (uint8_t)REQ_RETRY_CNT)
            {
                CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)ERROR_STATUS;
                CoolValveInfo.sMotor.u8ValveReqRetryMode = (uint8_t)DISABLED;
                CoolValveInfo.sMotor.u8ValveCWRetryCnt = 0;
            }
        }
        else
        {
            CoolValveInfo.sLin.sReceive.C_3way_PosReq = (uint8_t)MotorDirectionCW;
            CoolValveInfo.sMotor.u8ValveCCWRetryCnt++;
            if (CoolValveInfo.sMotor.u8ValveCCWRetryCnt > (uint8_t)REQ_RETRY_CNT)
            {
                CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)ERROR_STATUS;
                CoolValveInfo.sMotor.u8ValveReqRetryMode = (uint8_t)DISABLED;
                CoolValveInfo.sMotor.u8ValveCCWRetryCnt = 0;
            }
        }
    }
    else
    {
        if (CoolValveInfo.sMotor.u16MotorRunningCount <= (uint8_t)MOTOR_RUNERROR_TMR)
        {
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)NORMAL_STATUS;
        }
        else
        {
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail = (uint8_t)ERROR_STATUS;
        }
    }
}

static void Motor_Request_Func(void)
{
    static uint8_t u8_motorCW_stall_flag = 0;
    static uint8_t u8_motorCCW_stall_flag = 0;
    static uint8_t u8_hall_pulse_sampling = 0;

    if((CoolValveInfo.sMotor.u8MotorMoveEnable == (uint8_t)ENABLED) || (CoolValveInfo.sMotor.u8ValveReqRetryMode == (uint8_t)ENABLED))
    {
        if(CoolValveInfo.sMotor.u8ValveReqRetryMode == (uint8_t)DISABLED)
        {
            CoolValveInfo.sMotor.u8SoftHallValue = MOTOR_SOFTSTOP_HALL;
            CoolValveInfo.sMotor.u8SealHallValue = MOTOR_SEAL_HALL;
            CoolValveInfo.sMotor.u8TargetHallValue = MOTOR_TARGET_HALL;
        }

        if(CoolValveInfo.sLin.sReceive.C_3way_PosReq == (uint8_t)MotorDirectionCW)
        {
            Motor_Direction(MotorDirectionCW);
            Motor_Start();
            u8_hall_pulse_sampling = Hall_Sampling();
            if(u8_hall_pulse_sampling == (uint8_t)1)
            {
                u8_hall_pulse_sampling = 0;
                CoolValveInfo.sMotor.u8HallValue++;
            }
            
            if(u8_motorCW_stall_flag == (uint8_t)0)
            {
                if(CoolValveInfo.sMotor.u8HallValue < (uint8_t)CoolValveInfo.sMotor.u8SoftHallValue)
                {
                    Motor_ReqRetry_Func();
                }
                else
                {
                    CoolValveInfo.sMotor.u8ValveSoftstopMode = (uint8_t)ENABLED;
                    if(CoolValveInfo.sMotor.u8HallValue < (uint8_t)CoolValveInfo.sMotor.u8SealHallValue)
                    {
                        Motor_ReqRetry_Func();
                    }
                    else
                    {
                        if(CoolValveInfo.sMotor.u8HallValue < (uint8_t)CoolValveInfo.sMotor.u8TargetHallValue)
                        {
                            if((CoolValveInfo.sMotor.u8MotorCurrnetStatus == (uint8_t)ERROR_STATUS) && (CoolValveInfo.sMotor.u8MotorStallStatus == (uint8_t)ERROR_STATUS))
                            {
                                u8_motorCW_stall_flag = 1;
                                CoolValveInfo.sMotor.u8MotorCurrnetStatus = (uint8_t)NORMAL_STATUS;
                                CoolValveInfo.sMotor.u8MotorStallStatus = (uint8_t)NORMAL_STATUS;
                            }
                        }
                        else
                        {
                            u8_motorCW_stall_flag = 1;
                        }
                    }
                }
            }
            else
            {
                Motor_Stop();
                CoolValveInfo.sMotor.u8HallValue = 0;
                CoolValveInfo.sMotor.u8ValveSoftstopMode = (uint8_t)DISABLED;
                CoolValveInfo.sMotor.u8ValveReqRetryMode = (uint8_t)DISABLED;
                u8_motorCW_stall_flag = 0;
                CoolValveInfo.sMotor.u8ValveCWRetryCnt = 0;
                CoolValveInfo.sMotor.u8MotorPositionFB = (uint8_t)MotorDirectionCW;
                CoolValveInfo.sMotor.u8MotorRunning = (uint8_t)MOVING_DONE;
                CoolValveInfo.sMotor.u16MotorRunningCount = 0;
            }
        }
        else
        {
            Motor_Direction(MotorDirectionCCW);
            Motor_Start();
            u8_hall_pulse_sampling = Hall_Sampling();
            if(u8_hall_pulse_sampling == (uint8_t)1)
            {
                u8_hall_pulse_sampling = 0;
                CoolValveInfo.sMotor.u8HallValue++;
            }
            
            if(u8_motorCCW_stall_flag == (uint8_t)0)
            {
                if(CoolValveInfo.sMotor.u8HallValue < (uint8_t)CoolValveInfo.sMotor.u8SoftHallValue)
                {
                    Motor_ReqRetry_Func();
                }
                else
                {
                    CoolValveInfo.sMotor.u8ValveSoftstopMode = (uint8_t)ENABLED;
                    if(CoolValveInfo.sMotor.u8HallValue < (uint8_t)CoolValveInfo.sMotor.u8SealHallValue)
                    {
                        Motor_ReqRetry_Func();
                    }
                    else
                    {
                        if(CoolValveInfo.sMotor.u8HallValue < (uint8_t)CoolValveInfo.sMotor.u8TargetHallValue)
                        {
                            if((CoolValveInfo.sMotor.u8MotorCurrnetStatus == (uint8_t)ERROR_STATUS) && (CoolValveInfo.sMotor.u8MotorStallStatus == (uint8_t)ERROR_STATUS))
                            {
                                u8_motorCCW_stall_flag = 1;
                                CoolValveInfo.sMotor.u8MotorCurrnetStatus = (uint8_t)NORMAL_STATUS;
                                CoolValveInfo.sMotor.u8MotorStallStatus = (uint8_t)NORMAL_STATUS;
                            }
                        }
                        else
                        {
                            u8_motorCCW_stall_flag = 1;
                        }
                    }
                }
            }
            else
            {
                Motor_Stop();
                CoolValveInfo.sMotor.u8HallValue = 0;
                CoolValveInfo.sMotor.u8ValveSoftstopMode = (uint8_t)DISABLED;
                CoolValveInfo.sMotor.u8ValveReqRetryMode = (uint8_t)DISABLED;
                u8_motorCCW_stall_flag = 0;
                CoolValveInfo.sMotor.u8ValveCCWRetryCnt = 0;
                CoolValveInfo.sMotor.u8MotorPositionFB = (uint8_t)MotorDirectionCCW;
                CoolValveInfo.sMotor.u8MotorRunning = (uint8_t)MOVING_DONE;
                CoolValveInfo.sMotor.u16MotorRunningCount = 0;
            }
        }
    }
    else
    {
        Motor_Stop();
        CoolValveInfo.sMotor.u8MotorRunning = (uint8_t)MOVING_DONE;
        CoolValveInfo.sMotor.u16MotorRunningCount = 0;
    }
}

static void Motor_Task(void)
{
    Motor_Wakeup_Check();
    if(CONTROLLER_CONDITION()){
        if((CoolValveInfo.sMotor.u8MotorInitDone != (uint8_t)INIT_DONE) && (CoolValveInfo.sMotor.u8ValveWakeupMode == (uint8_t)ENABLED)){
            Motor_Init_Func();
        }
        else if(CoolValveInfo.sMotor.u8MotorInitDone == (uint8_t)INIT_DONE){
            Motor_InitReq_Check();
            Motor_Request_Func();
        }
        else{
            /* empty */
        }
    }
    else{
        Motor_Stop();
        CoolValveInfo.sMotor.u8MotorRunning = (uint8_t)MOVING_DONE;
        CoolValveInfo.sMotor.u16MotorRunningCount = 0;
    }
}

void Main_Task(void)
{
    if(Timer_Count_Check.Flag_1ms == (uint8_t)1){
        Motor_Task();
        Sensing_Task();
        Timer_Count_Check.Flag_1ms = 0;
    }
    
    if(Timer_Count_Check.Flag_10ms == (uint8_t)1){
        Failsafety_Task();
        Lin_Task();
        Timer_Count_Check.Flag_10ms = 0;
    }
    
    if(Timer_Count_Check.Flag_100ms == (uint8_t)1){
        Motor_Reset_Release();
        Motor_RunCnt_Release();
        Timer_Count_Check.Flag_100ms = 0;
    }
}
