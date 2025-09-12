#include <app_common.h>

static void Failsafety_Voltage(void)
{
    if((CoolValveInfo.sAdc.u32IgnVoltage > (uint32_t)IGN1_VOLT_FAIL_MAX) || (CoolValveInfo.sAdc.u32IgnVoltage < (uint32_t)IGN1_VOLT_FAIL_MIN)){
        if(CoolValveInfo.sAdc.u32IgnVoltage > (uint32_t)IGN1_VOLT_FAIL_MAX){
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverVoltage = (uint8_t)ERROR_STATUS;
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagUnderVoltage = (uint8_t)NORMAL_STATUS;
        }
        
        if(CoolValveInfo.sAdc.u32IgnVoltage < (uint32_t)IGN1_VOLT_FAIL_MIN){
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverVoltage = (uint8_t)NORMAL_STATUS;
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagUnderVoltage = (uint8_t)ERROR_STATUS;
        }
    }
    else if((CoolValveInfo.sAdc.u32IgnVoltage >= (uint32_t)IGN1_VOLT_NORM_MIN) && (CoolValveInfo.sAdc.u32IgnVoltage <= (uint32_t)IGN1_VOLT_NORM_MAX)){
        CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverVoltage = (uint8_t)NORMAL_STATUS;
        CoolValveInfo.uFaultFlag.bits.u8FaultFlagUnderVoltage = (uint8_t)NORMAL_STATUS;
    }
    else{
        /* empty */
    }
}

static void Failsafety_Temp(void)
{
    if((CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp == (uint8_t)NORMAL_STATUS) && (CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTempWarn == (uint8_t)NORMAL_STATUS)){
        if(CoolValveInfo.sAdc.u32McuTemperature >= (uint32_t)MCU_TEMP_FAIL_MAX){
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp = (uint8_t)ERROR_STATUS;
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTempWarn = (uint8_t)NORMAL_STATUS;
        }
        else if((CoolValveInfo.sAdc.u32McuTemperature >= (uint32_t)MCU_TEMP_WARN_MAX) && (CoolValveInfo.sAdc.u32McuTemperature < (uint32_t)MCU_TEMP_FAIL_MAX)){
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp = (uint8_t)NORMAL_STATUS;
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTempWarn = (uint8_t)ERROR_STATUS;
        }
        else if((CoolValveInfo.sAdc.u32McuTemperature >= (uint32_t)MCU_TEMP_NORMAL_MAX) && (CoolValveInfo.sAdc.u32McuTemperature < (uint32_t)MCU_TEMP_WARN_MAX)){
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp = (uint8_t)NORMAL_STATUS;
            CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTempWarn = (uint8_t)NORMAL_STATUS;
        }
        else{
            /* empty */
        }
    }
}

static void Failsafety_MotorDriver(void)
{
    static uint8_t u8_faultstatus_read = 0;
    
    u8_faultstatus_read = GPIO_Read(GPIO_PORT_7); 
    if(u8_faultstatus_read == (uint8_t)0){
        MotorDriver_GetStatus();
        if((CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp == (uint8_t)NORMAL_STATUS) && (CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTempWarn == (uint8_t)NORMAL_STATUS)){
            if(CoolValveInfo.sMotorDriver.sMDStatusFlag.OT == (uint8_t)ERROR_STATUS){
                CoolValveInfo.sMotorDriver.sMDStatusFlag.OT = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OTcount++;
                if(CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OTcount >= (uint8_t)MOTOR_FAULT_CNT){
                    if(CoolValveInfo.sAdc.u32McuTemperature >= (uint32_t)MOTOR_TEMP_FAIL_MAX){
                        CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OTcount = (uint8_t)NORMAL_STATUS;
                        CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp = (uint8_t)ERROR_STATUS;
                    }
                    else if((CoolValveInfo.sAdc.u32McuTemperature >= (uint32_t)MOTOR_TEMP_WARN_MAX) && (CoolValveInfo.sAdc.u32McuTemperature < (uint32_t)MOTOR_TEMP_FAIL_MAX)){
                        CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OTcount = (uint8_t)NORMAL_STATUS;
                        CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp = (uint8_t)NORMAL_STATUS;
                    }
                    else if((CoolValveInfo.sAdc.u32McuTemperature >= (uint32_t)MCU_TEMP_NORMAL_MAX) && (CoolValveInfo.sAdc.u32McuTemperature < (uint32_t)MOTOR_TEMP_WARN_MAX)){
                        CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OTcount = (uint8_t)NORMAL_STATUS;
                        CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp = (uint8_t)NORMAL_STATUS;
                    }
                    else{
                        /* empty */
                    }
                }
            }
            else{
                CoolValveInfo.sMotorDriver.sMDStatusFlag.OT = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OTcount = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp = (uint8_t)NORMAL_STATUS;
            }

            if(CoolValveInfo.sMotorDriver.sMDStatusFlag.TW == (uint8_t)ERROR_STATUS){
                CoolValveInfo.sMotorDriver.sMDStatusFlag.TW = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.sMotorDriver.sMDStatusFlag.u16TWcount++;
                if(CoolValveInfo.sMotorDriver.sMDStatusFlag.u16TWcount >= (uint8_t)MOTOR_FAULT_CNT){
                    if(CoolValveInfo.sAdc.u32McuTemperature >= (uint32_t)MOTOR_TEMP_FAIL_MAX){
                        CoolValveInfo.sMotorDriver.sMDStatusFlag.u16TWcount = (uint8_t)NORMAL_STATUS;
                        CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTempWarn = (uint8_t)NORMAL_STATUS;
                    }
                    else if((CoolValveInfo.sAdc.u32McuTemperature >= (uint32_t)MOTOR_TEMP_WARN_MAX) && (CoolValveInfo.sAdc.u32McuTemperature < (uint32_t)MOTOR_TEMP_FAIL_MAX)){
                        CoolValveInfo.sMotorDriver.sMDStatusFlag.u16TWcount = (uint8_t)NORMAL_STATUS;
                        CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTempWarn = (uint8_t)ERROR_STATUS;
                    }
                    else if((CoolValveInfo.sAdc.u32McuTemperature >= (uint32_t)MCU_TEMP_NORMAL_MAX) && (CoolValveInfo.sAdc.u32McuTemperature < (uint32_t)MOTOR_TEMP_WARN_MAX)){
                        SM1_SendChar(0xA000);
                        M_ENBL_PutVal(0);
                        M_DIS_PutVal(1);
                        CoolValveInfo.sMotorDriver.sMDStatusFlag.u16TWcount = (uint8_t)NORMAL_STATUS;
                        CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTempWarn = (uint8_t)NORMAL_STATUS;
                    }
                    else{
                        /* empty */
                    }
                }
            }
            else{
                CoolValveInfo.sMotorDriver.sMDStatusFlag.TW = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.sMotorDriver.sMDStatusFlag.u16TWcount = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTempWarn = (uint8_t)NORMAL_STATUS;
            }
        }
        
        if(CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorCoilShort == (uint8_t)NORMAL_STATUS){
            if(CoolValveInfo.sMotorDriver.sMDStatusFlag.OC == (uint8_t)ERROR_STATUS){
                CoolValveInfo.sMotorDriver.sMDStatusFlag.OC = (uint8_t)NORMAL_STATUS;
                if(CoolValveInfo.sMotorDriver.sMDStatusFlag.SCGP12 != (uint8_t)NORMAL_STATUS){
                    CoolValveInfo.sMotorDriver.sMDStatusFlag.SCG1 = (uint8_t)NORMAL_STATUS;
                    CoolValveInfo.sMotorDriver.sMDStatusFlag.SCG2 = (uint8_t)NORMAL_STATUS;
                    CoolValveInfo.sMotorDriver.sMDStatusFlag.SCP1 = (uint8_t)NORMAL_STATUS;
                    CoolValveInfo.sMotorDriver.sMDStatusFlag.SCP2 = (uint8_t)NORMAL_STATUS;
                    CoolValveInfo.sMotorDriver.sMDStatusFlag.SCGP12 = (uint8_t)NORMAL_STATUS;
                    CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OCcount++;
                    if(CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OCcount >= (uint8_t)MOTOR_FAULT_CNT){
                        if((CoolValveInfo.sAdc.u32MotorCurrent >= (uint32_t)MOTOR_SHORT_CUR_MIN) && (CoolValveInfo.sAdc.u32MotorCurrent <= (uint32_t)MOTOR_SHORT_CUR_MAX)){
                            CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OCcount = (uint8_t)NORMAL_STATUS;
                            CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorCoilShort = (uint8_t)ERROR_STATUS;
                        }
                        else{
                            CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OCcount = (uint8_t)NORMAL_STATUS;
                            CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorCoilShort = (uint8_t)NORMAL_STATUS;
                        }
                    }
                }
            }
            else{
                CoolValveInfo.sMotorDriver.sMDStatusFlag.OC = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OCcount = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorCoilShort = (uint8_t)NORMAL_STATUS;
            }
        }
        else{}
        
        if(CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorCoilOpen == (uint8_t)NORMAL_STATUS){
            if(CoolValveInfo.sMotorDriver.sMDStatusFlag.OL == (uint8_t)ERROR_STATUS){
                CoolValveInfo.sMotorDriver.sMDStatusFlag.OL = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OLcount++;
                if(CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OLcount >= (uint8_t)MOTOR_FAULT_CNT){
                    if((CoolValveInfo.sAdc.u32MotorCurrent <= (uint32_t)MOTOR_OPEN_CUR_MAX) || ((CoolValveInfo.sAdc.u32MotorCurrent >= (uint32_t)MOTOR_OPEN_CUR_MIN) && (CoolValveInfo.sAdc.u32MotorCurrent <= (uint32_t)MOTOR_ADC_CUR_MAX))){
                        CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OLcount = (uint8_t)NORMAL_STATUS;
                        CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorCoilOpen = (uint8_t)ERROR_STATUS;
                    }
                    else{
                        CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OLcount = (uint8_t)NORMAL_STATUS;
                        CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorCoilOpen = (uint8_t)NORMAL_STATUS;
                    }
                }
            }
            else{
                CoolValveInfo.sMotorDriver.sMDStatusFlag.OL = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.sMotorDriver.sMDStatusFlag.u16OLcount = (uint8_t)NORMAL_STATUS;
                CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorCoilOpen = (uint8_t)NORMAL_STATUS;
            }
        }
        else{}
        
        SM1_SendChar(0xA000);
        M_ENBL_PutVal(0);
        M_DIS_PutVal(1);
    }
    else{}
}

static void CheckJTAGUnlock(void)
{
    static uint8_t u8_unlock_state = 0;
    
    if ( CoolValveInfo.sLin.sReceive.C_3way_DataFrame == UNLOCK_PASSWARD_1 )
    {
        u8_unlock_state = 1u;
    }
    else if ( CoolValveInfo.sLin.sReceive.C_3way_DataFrame == UNLOCK_PASSWARD_2 )
    {
        if ( u8_unlock_state == 1u )
        {
            u8_unlock_state = 2u;
        }
    }
    else if ( CoolValveInfo.sLin.sReceive.C_3way_DataFrame == UNLOCK_PASSWARD_3 )
    {
        if ( u8_unlock_state == 2u )
        {
            u8_unlock_state = 3u;
        }
    }
    else if ( CoolValveInfo.sLin.sReceive.C_3way_DataFrame == UNLOCK_PASSWARD_4 )
    {
        if ( u8_unlock_state == 3u )
        {
            WDTA_Disable();
            FLASH_CodeProtection_UnProtect();
            u8_unlock_state = 0u;
        }
    }
    else 
    {
        u8_unlock_state = 0u;
    }
}

void Failsafety_Task(void)
{
    static uint8_t u8_mcu_wakeup_done = 0;
    static uint16_t u16_mcu_wakeup_delay = 0;
    
    if(u8_mcu_wakeup_done == (uint8_t)0){
        u16_mcu_wakeup_delay++;
        if(u16_mcu_wakeup_delay >= (uint8_t)MCU_WAKEUP_TIMER){
            u16_mcu_wakeup_delay = 0;
            u8_mcu_wakeup_done = 1;
        }
    }
    else if(u8_mcu_wakeup_done == (uint8_t)1){
        Failsafety_Voltage(); 
        Failsafety_Temp();
        Failsafety_MotorDriver();
        CheckJTAGUnlock();
    }
    else{
        /* empty */
    }
}
