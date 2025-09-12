#ifndef APP_MOTORDRIVER_H_
#define APP_MOTORDRIVER_H_

#include <softTimerTask.h>

typedef struct tag_sTimerSet
{
    uint16_t            Count_1ms;
    uint16_t            Count_5ms;
    uint16_t            Count_10ms;
    uint16_t            Count_100ms;
    uint16_t            Count_1000ms;
    uint8_t             Flag_1ms;
    uint8_t             Flag_5ms;
    uint8_t             Flag_10ms;
    uint8_t             Flag_100ms;
    uint8_t             Flag_1000ms;
}TIMER_SET;

typedef struct tag_sMDriverStatus
{
    uint8_t            OT;
    uint8_t            TW;
    uint8_t            OC;
    uint8_t            OL;
    uint8_t            SCG1;
    uint8_t            SCG2;
    uint8_t            SCP1;
    uint8_t            SCP2;
    uint8_t            OV;
    uint8_t            UV;
    uint8_t            CP_U;
    uint8_t            FRM;
    uint8_t            SCGP12;
    
    uint16_t           u16OTcount;
    uint16_t           u16TWcount;
    uint16_t           u16OCcount;
    uint16_t           u16OLcount;
}MDSTATUSFLAG;

typedef struct tag_sMDriverInformation
{
    MDSTATUSFLAG        sMDStatusFlag;
}MDRIVERINFO;

extern TIMER_SET Timer_Count_Check;
extern MC33HB2000_Port mMC33HB2000_Port;

void M_ENBL_PutVal(bool Val);
void M_DIS_PutVal(bool Val);
void M_IN1_PutVal(bool Val);
void M_FS_B_PutVal(bool Val);

void MotorDriver_GetStatus(void);
void MotorControlTask(void);

#endif
