#include <app_common.h>

TIMER_SET Timer_Count_Check;
static TaskState_t applState = TASK_STATE_INIT;

void M_DIS_PutVal(bool Val)
{
    mMC33HB2000_Port.mc33hb2000_DIS = Val;
    PWM_SetMatchValue(M_DIS, (uint16_t)PWM_DUTY62*(uint16_t)Val, (uint16_t)PWM_DUTY62);    
}

void M_ENBL_PutVal(bool Val)
{
    mMC33HB2000_Port.mc33hb2000_ENBL = Val;  
    PWM_SetMatchValue(M_ENLB, (uint16_t)PWM_DUTY62*(uint16_t)Val, (uint16_t)PWM_DUTY62);
}

void M_IN1_PutVal(bool Val)
{
    mMC33HB2000_Port.mc33hb2000_IN1 = Val;
    PWM_SetMatchValue(M_IN1, (uint16_t)PWM_DUTY62*(uint16_t)Val, (uint16_t)PWM_DUTY62);
    IOCTRLA_SFRS->LEDDATA = (unsigned int)IOCTRLA_SFRS->LEDDATA & ~(uint8_t)((unsigned int)Val<<(int8_t)3);
}

void M_FS_B_PutVal(bool Val)
{
    mMC33HB2000_Port.mc33hb2000_FS_B = Val;
}

void MotorDriver_GetStatus(void)
{
    static uint16_t u16_driver_status_recv = 0;
    static uint8_t u8_driver_status_read[3] = {0x02, 0x00, 0x00};

    SPI_WriteReadByte(u8_driver_status_read, 3);
    u16_driver_status_recv = (uint16_t)(u8_driver_status_read[1] << 8) | (uint16_t)u8_driver_status_read[2];

    CoolValveInfo.sMotorDriver.sMDStatusFlag.OT = (uint8_t)(u16_driver_status_recv) & (uint16_t)0x01;
    CoolValveInfo.sMotorDriver.sMDStatusFlag.TW = (uint8_t)((u16_driver_status_recv >> (uint16_t)1) & (uint16_t)0x01);
    CoolValveInfo.sMotorDriver.sMDStatusFlag.OC = (uint8_t)((u16_driver_status_recv >> (uint16_t)2) & (uint16_t)0x01);
    CoolValveInfo.sMotorDriver.sMDStatusFlag.OL = (uint8_t)((u16_driver_status_recv >> (uint16_t)3) & (uint16_t)0x01);
    CoolValveInfo.sMotorDriver.sMDStatusFlag.SCG1 = (uint8_t)((u16_driver_status_recv >> (uint16_t)4) & (uint16_t)0x01);
    CoolValveInfo.sMotorDriver.sMDStatusFlag.SCG2 = (uint8_t)((u16_driver_status_recv >> (uint16_t)5) & (uint16_t)0x01);
    CoolValveInfo.sMotorDriver.sMDStatusFlag.SCP1 = (uint8_t)((u16_driver_status_recv >> (uint16_t)6) & (uint16_t)0x01);
    CoolValveInfo.sMotorDriver.sMDStatusFlag.SCP2 = (uint8_t)((u16_driver_status_recv >> (uint16_t)7) & (uint16_t)0x01);
    CoolValveInfo.sMotorDriver.sMDStatusFlag.OV = (uint8_t)((u16_driver_status_recv >> (uint16_t)8) & (uint16_t)0x01);
    CoolValveInfo.sMotorDriver.sMDStatusFlag.UV = (uint8_t)((u16_driver_status_recv >> (uint16_t)9) & (uint16_t)0x01);
    CoolValveInfo.sMotorDriver.sMDStatusFlag.CP_U = (uint8_t)((u16_driver_status_recv >> (uint16_t)10) & (uint16_t)0x01);
    CoolValveInfo.sMotorDriver.sMDStatusFlag.FRM = (uint8_t)((u16_driver_status_recv >> (uint16_t)11) & (uint16_t)0x01);
    CoolValveInfo.sMotorDriver.sMDStatusFlag.SCGP12 = CoolValveInfo.sMotorDriver.sMDStatusFlag.SCG1 + CoolValveInfo.sMotorDriver.sMDStatusFlag.SCG2 + CoolValveInfo.sMotorDriver.sMDStatusFlag.SCP1 + CoolValveInfo.sMotorDriver.sMDStatusFlag.SCP2;

    u16_driver_status_recv = 0;
    SM1_SendChar(0xA000);
}

static void MotorControlTime0(void)
{
    Timer_Count_Check.Count_1ms++;
    Timer_Count_Check.Count_5ms++;
    Timer_Count_Check.Count_10ms++;
    Timer_Count_Check.Count_100ms++;
    
    if(Timer_Count_Check.Count_1ms >= (uint16_t)1){
        Timer_Count_Check.Count_1ms = 0;
        Timer_Count_Check.Flag_1ms = 1;
    }
    
    if(Timer_Count_Check.Count_5ms >= (uint16_t)5){
        Timer_Count_Check.Count_5ms = 0;
        Timer_Count_Check.Flag_5ms = 1;
    }
    
    if(Timer_Count_Check.Count_10ms >= (uint16_t)10){
      Timer_Count_Check.Count_10ms = 0;
      Timer_Count_Check.Flag_10ms = 1;
    }
	
    if(Timer_Count_Check.Count_100ms >= (uint16_t)100){
        Timer_Count_Check.Count_100ms = 0;
        Timer_Count_Check.Flag_100ms = 1;
    }
}

static void Main_Task_Init(void)
{
    TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, (1000U * 16U), MotorControlTime0);
}

void MotorControlTask(void)
{
    switch(applState){
    case TASK_STATE_INIT:
	  Main_Task_Init();
      applState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
	  Main_Task(); 
      break;
    default:
      break;
    }
}
