#include <app_common.h>

static void Lin_Receive(void)
{
    static uint16_t u16_lin_receive_init_cnt = 0;
    static uint16_t u16_lin_receive_reset_cnt = 0;
    static uint16_t u16_lin_mpmode_cnt = 0;
    static uint16_t u16_lin_mpmode_reset_cnt = 0;
    
    if(CoolValveInfo.sMotor.u8MotorRunning == (uint8_t)MOVING_DONE){
        if(CoolValveInfo.sMotor.u8ValveReqRetryMode == (uint8_t)DISABLED){
            CoolValveInfo.sLin.sReceive.C_3way_PosReq = (uint8_t)l_u8_rd_LI0_C_3way_PosReq() & (uint8_t)0x01;
        }
    }
    else if(CoolValveInfo.sMotor.u8MotorRunning == (uint8_t)MOVING){
        if(CoolValveInfo.sMotor.u8MotorInitDone != (uint8_t)INIT_DONE){
            CoolValveInfo.sLin.sReceive.C_3way_PosReq = (uint8_t)l_u8_rd_LI0_C_3way_PosReq() & (uint8_t)0x01;
        }
    }
    else{
        /* empty */
    }
    
    CoolValveInfo.sLin.sReceive.C_3way_MoveEnable = (uint8_t)l_bool_rd_LI0_C_3way2_MoveEnable();
    if(CoolValveInfo.sMotor.u8MotorInitDone == (uint8_t)INIT_DONE){
        if(CoolValveInfo.sLin.sReceive.C_3way_MoveEnable == (uint8_t)ENABLED){
            if(CoolValveInfo.sMotor.u8MotorPositionFB != CoolValveInfo.sLin.sReceive.C_3way_PosReq){
                CoolValveInfo.sMotor.u8MotorMoveEnable = (uint8_t)ENABLED;
            }
            else{
                CoolValveInfo.sMotor.u8MotorMoveEnable = (uint8_t)DISABLED;
            }
        }
        else if(CoolValveInfo.sLin.sReceive.C_3way_MoveEnable == (uint8_t)DISABLED){
            if((CoolValveInfo.sMotor.u8MotorPositionFB != CoolValveInfo.sLin.sReceive.C_3way_PosReq) && (CoolValveInfo.sMotor.u8MotorRunning == (uint8_t)MOVING)){
                CoolValveInfo.sMotor.u8MotorMoveEnable = (uint8_t)ENABLED;
            }
            else{
                CoolValveInfo.sMotor.u8MotorMoveEnable = (uint8_t)DISABLED;
            }
        }
        else{
            /* empty */
        }
    }

    if(CoolValveInfo.sMotor.u8ValveMPMode == (uint8_t)DISABLED){
        if(CoolValveInfo.sMotor.u8MotorInitDone == (uint8_t)INIT_DONE){
            if(u16_lin_mpmode_cnt < (uint16_t)MPMODE_CNT){
                u16_lin_mpmode_cnt++;
                if(CoolValveInfo.sLin.sReceive.C_3way_MoveEnable == (uint8_t)ENABLED){
                    CoolValveInfo.sMotor.u8ValveMPMode = (uint8_t)ENABLED;
                }
                else{
                    CoolValveInfo.sMotor.u8ValveMPMode = (uint8_t)DISABLED;
                }
            }
        }
    }
    else if(CoolValveInfo.sMotor.u8ValveMPMode == (uint8_t)ENABLED){
        if(u16_lin_mpmode_reset_cnt < (uint16_t)MPMODE_RESET_CNT){
            u16_lin_mpmode_reset_cnt++;
            
            if(CoolValveInfo.sMotor.u8ValveMPCount >= 4){
                CoolValveInfo.sMotor.u8ValveMPMode = (uint8_t)DISABLED;
            }
        }
        else{
            CoolValveInfo.sMotor.u8ValveMPMode = (uint8_t)DISABLED;
        }
    }
    else{
        /* empty */
    }

    CoolValveInfo.sLin.sReceive.C_3way_Initial = (uint8_t)l_u8_rd_LI0_C_3way_Initial() & (uint8_t)0x01;
    if(CoolValveInfo.sLin.sReceive.C_3way_Initial == (uint8_t)ENABLED){
        u16_lin_receive_init_cnt++;
        if(u16_lin_receive_init_cnt >= (uint16_t)10){
            u16_lin_receive_init_cnt = 0;
            u16_lin_receive_reset_cnt = 0;
            CoolValveInfo.sMotor.u8MotorInitReq = (uint8_t)ENABLED;
        }
    }
    else if(CoolValveInfo.sLin.sReceive.C_3way_Initial == (uint8_t)DISABLED){
        u16_lin_receive_reset_cnt++;
        if(u16_lin_receive_reset_cnt >= (uint16_t)10){
            u16_lin_receive_reset_cnt = 0;
            u16_lin_receive_init_cnt = 0;
            CoolValveInfo.sMotor.u8MotorInitReq = (uint8_t)DISABLED;
            CoolValveInfo.sMotor.u8MotorInitReqFirst = (uint8_t)DISABLED;
        }
    }
    else{
        /* empty */
    }
       
    CoolValveInfo.sLin.sReceive.C_3way_LinError = (uint8_t)l_bool_rd_LI0_C_3way2_LinError();
}

static void Lin_Motor_Transmit(void)
{
    if(CoolValveInfo.sMotor.u8MotorInitDone == (uint8_t)INIT_NOT_DONE){
        l_u8_wr_LI0_C_3way2_C_InitialSta(0);
    }
    else if(CoolValveInfo.sMotor.u8MotorInitDone == (uint8_t)INIT_ING){
        l_u8_wr_LI0_C_3way2_C_InitialSta(1);
    }
    else if(CoolValveInfo.sMotor.u8MotorInitDone == (uint8_t)INIT_DONE){
        l_u8_wr_LI0_C_3way2_C_InitialSta(2);
    }
    else if(CoolValveInfo.sMotor.u8MotorInitDone == (uint8_t)INIT_DLEAY){
        l_u8_wr_LI0_C_3way2_C_InitialSta(2);
    }
    else{
        /* empty */
    }
    
    if(CoolValveInfo.sMotor.u8MotorRunning == (uint8_t)MOVING_DONE){
        l_bool_wr_LI0_C_3way2_C_Running(0);
    }
    else if(CoolValveInfo.sMotor.u8MotorRunning == (uint8_t)MOVING){
        l_bool_wr_LI0_C_3way2_C_Running(1);
    }
    else{
        /* empty */
    }
    
    if(CoolValveInfo.sMotor.u8MotorInitDone == (uint8_t)INIT_DONE){
        if(CoolValveInfo.sMotor.u8MotorRunning == (uint8_t)MOVING_DONE){
            if(CoolValveInfo.sMotor.u8MotorPositionFB == (uint8_t)MotorDirectionCW){
                l_u8_wr_LI0_C_3way2_C_PositionFB(0);
            }
            else if(CoolValveInfo.sMotor.u8MotorPositionFB == (uint8_t)MotorDirectionCCW){
                l_u8_wr_LI0_C_3way2_C_PositionFB(1);
            }
            else{
                /* empty */
            }
        }
    }
    
    l_u8_wr_LI0_C_3way2_C_SW_Ver(211);
    l_u8_wr_LI0_C_3way2_C_HW_Ver(100);
}

static void Lin_Fail_Transmit(void)
{
    if(CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorCoilShort == (uint8_t)ERROR_STATUS){
        l_u8_wr_LI0_C_3way2_C_Fault(1);
    }
    
    if(CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorCoilOpen == (uint8_t)ERROR_STATUS){
        l_u8_wr_LI0_C_3way2_C_Fault(2);
    }
    
    if(CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp == (uint8_t)ERROR_STATUS){
        l_u8_wr_LI0_C_3way2_C_Fault(3);
    }
    
    if(CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail == (uint8_t)ERROR_STATUS){
        l_u8_wr_LI0_C_3way2_C_Fault(5);
    }
    
    if((CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorCoilShort == (uint8_t)NORMAL_STATUS) && (CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorCoilOpen == (uint8_t)NORMAL_STATUS) &&
       (CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp == (uint8_t)NORMAL_STATUS) && (CoolValveInfo.uFaultFlag.bits.u8FaultFlagMotorStallFail == (uint8_t)NORMAL_STATUS)){
         l_u8_wr_LI0_C_3way2_C_Fault(0);
    }
    
    if((CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTempWarn == (uint8_t)ERROR_STATUS) && (CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverTemp == (uint8_t)NORMAL_STATUS)){
        l_u8_wr_LI0_C_3way2_C_OverTemp(1);
    }
    else{
        l_u8_wr_LI0_C_3way2_C_OverTemp(0);
    }
    
    if((CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverVoltage == (uint8_t)ERROR_STATUS) && (CoolValveInfo.uFaultFlag.bits.u8FaultFlagUnderVoltage == (uint8_t)NORMAL_STATUS)){
        l_u8_wr_LI0_C_3way2_C_Voltage(1);
    }
    else if((CoolValveInfo.uFaultFlag.bits.u8FaultFlagOverVoltage == (uint8_t)NORMAL_STATUS) && (CoolValveInfo.uFaultFlag.bits.u8FaultFlagUnderVoltage == (uint8_t)ERROR_STATUS)){
        l_u8_wr_LI0_C_3way2_C_Voltage(2);
    }
    else{
        l_u8_wr_LI0_C_3way2_C_Voltage(0);
    }
}

static void Lin_Transmit(void)
{
    Lin_Motor_Transmit();
    Lin_Fail_Transmit();
}

void Lin_Task(void)
{
    Lin_Receive();
    Lin_Transmit();
}
