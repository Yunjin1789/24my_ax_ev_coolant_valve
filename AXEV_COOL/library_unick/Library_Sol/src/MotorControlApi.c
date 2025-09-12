/*
 * ValveControl.c
 *
 *  Created on: Apr 15, 2019
 *      Author: UN00518
 */

#include <MotorControlApi.h>
#include <Diagnosis.h>
#include "lin.h"
#include "lin_cfg.h"
#include "lin_hw_cfg.h"
#include "pwm_device.h"
//#include "lin_common_api.h"


#define Target_Current  	190     /* 855mA = 0.0045 * 190 */
#define Target_Hal			34      /*  153mA = 0.0045 * 34 */ 
#define Current_Set_Value   550     /* Sensing 0.045  2.2V*/

#define PWM_MAX 			PWM_DUTY100
#define PWM_MIN 			PWM_DUTY40

#define Slicing50ms_10min		12000
#define Slicing50ms_5min		6000
#define Slicing50ms_1min		1200
#define Slicing50ms_30sec		600
#define Slicing50ms_500ms		10
#define Valve_Retry_Cnt			30
#define ITERM_DEFAULT			60000

uint8_t u8t_PeriodFlag5 = 0;

static void ControlValveOn(uint16_t u16t_Current,uint16_t u16t_HallICValue);
static void ControlValveOff(uint16_t u16t_HallICValue);
static void FailSafeOn(void);
static void FailSafeOff(void);

extern lin_word_status_str current_lin_status; 
static float f32t_ITerm = ITERM_DEFAULT;
static float f32t_ITerm_Hal = PWM_DUTY30;

typedef enum{
	Idle = 0x00,
	ValveDiagRetry_On,
	ValveDiagRetry_Off,
	ValveDiagRetry_Check,
	ProductFail_Voltage,
	ProductFail_HallIC,
	LINComm_Fail
}ValveControlStatus_t;

typedef enum{
	OnReady = 0x00,
	OnImpulse,
	OnCurrentControl,
	OnIncline
}OnControlPhase;

typedef enum{
	OffReady = 0x10,
	OffLinearDecline,
	OffDone
}OffControlPhase;

typedef struct{
	OnControlPhase OnPhase;
	OffControlPhase OffPhase;
	uint16_t u16t_OnControlPhaseCnt;
	uint16_t u16t_OffControlPhaseCnt;
	const uint16_t OnReadyTime;
	const uint16_t OnImpulseTime;
	const uint16_t OffReadyTime;
	const uint16_t OnReadyDuty;
	const uint16_t OnImpulseDuty;
	const uint16_t OffReadyDuty;
	const uint16_t OffLinearDeclineUnit;
	const uint16_t OnLinearDeclineUnit;
	uint16_t u16t_PositionNotSetCnt;
}Control_t;


Control_t ValvePhaseControl = {OnReady,OffDone,0,0,40,100,20,PWM_DUTY30,PWM_DUTY100,PWM_DUTY35,100,75,0};

static ValveControlStatus_t ValveStatus_t = Idle;

void ValveControlSlicsingTask(uint16_t PeriodMs)
{
	static uint16_t u16t_PeriodCnt = 0;
		
	if(u16t_PeriodCnt > PeriodMs - 1){
		u16t_PeriodCnt = 0;
		u8t_PeriodFlag5 = 1;
	}
	else{}
	
	u16t_PeriodCnt++;
}

void ValveControlManager(void)
{
	static DiagStatusValue_t CheckStatusValue_t;
	/*static uint16_t u16t_FailCntIGN1 = 0;*/
	/*static uint8_t u8t_RetryOnOffValve = 0;*/
	static uint16_t u16t_OrderMissMatchCnt = 0;
	static uint16_t u16t_RetryCnt = 0;
	static uint16_t u16t_RetryTimeCnt = 0;
	uint8_t u8t_ValveOrder = 0;
	
	if(u8t_PeriodFlag5){
		u8t_PeriodFlag5 = 0;
		
		/* Status Update */
		CheckStatusValue_t.u16t_IGN1ValueOneShot = DiagnosisGetIGN1Value();
		CheckStatusValue_t.u16t_CurrentValueOneShot = DiagnosisGetCurrentValue();
		CheckStatusValue_t.u16t_HallICValueOneShot = DiagnosisGetHallICValue();
		CheckStatusValue_t.u8t_ProductFailStatus = DiagnosisGetProductFailState();
		CheckStatusValue_t.u8t_ValveLocation = DiagnosisGetValveLocation();
		
		l_u8_wr_LIN_Valve_ND_CR_Valve_BmsCmdAngle(CheckStatusValue_t.u16t_HallICValueOneShot);
		l_u8_wr_LIN_Valve_ND_CR_Valve1_AuxVolt(CheckStatusValue_t.u16t_IGN1ValueOneShot);
		u8t_ValveOrder = l_u8_rd_LIN_Valve_ND_CR_Bms_cmdAngle_Valve();
		l_u8_wr_LIN_Valve_ND_CR_Valve1_OperAngle_FB(u8t_ValveOrder);
		
		/* Check Current Voltage Diag Status */
		if(CheckStatusValue_t.u8t_ProductFailStatus != 0){
			if(CheckStatusValue_t.u8t_ProductFailStatus & IGN1FAIL_LOW){
				l_bool_wr_LIN_Valve_ND_CF_Valve1_LowVoltageFail(1);
				l_bool_wr_LIN_Valve_ND_CF_Valve1_OverVoltageFail(0);
			}
			else{}
			
			if(CheckStatusValue_t.u8t_ProductFailStatus & IGN1FAIL_HIGH){
				l_bool_wr_LIN_Valve_ND_CF_Valve1_OverVoltageFail(1);
				l_bool_wr_LIN_Valve_ND_CF_Valve1_LowVoltageFail(0);
			}
			else{}
			
			if((ValveStatus_t!=ProductFail_HallIC) && (ValveStatus_t!=LINComm_Fail)) ValveStatus_t = ProductFail_Voltage;
			else{}
		}
		else{
			l_bool_wr_LIN_Valve_ND_CF_Valve1_LowVoltageFail(0);
			l_bool_wr_LIN_Valve_ND_CF_Valve1_OverVoltageFail(0);	
		}
		
		if(CheckStatusValue_t.u8t_ProductFailStatus & LIN_COMM_FAIL){
			if(ValveStatus_t!=ProductFail_HallIC) ValveStatus_t = LINComm_Fail;
			else{}
		}
		else{}

		//ValveStatus_t = Idle;
		switch(ValveStatus_t){
			case Idle :
				if(u8t_ValveOrder){
					ControlValveOn(CheckStatusValue_t.u16t_CurrentValueOneShot,CheckStatusValue_t.u16t_HallICValueOneShot);
					if(u8t_ValveOrder!=CheckStatusValue_t.u8t_ValveLocation){
						u16t_OrderMissMatchCnt++;
						if(u16t_OrderMissMatchCnt>Slicing50ms_1min){
							ValveStatus_t = ValveDiagRetry_Off;
							u16t_OrderMissMatchCnt = 0;
						}
						else{}
					}
					else{
						u16t_OrderMissMatchCnt = 0;
					}
				}
				else{
					ControlValveOff(CheckStatusValue_t.u16t_HallICValueOneShot);
					if(u8t_ValveOrder!=CheckStatusValue_t.u8t_ValveLocation){
						u16t_OrderMissMatchCnt++;
						if(u16t_OrderMissMatchCnt>Slicing50ms_1min){
							ValveStatus_t = ValveDiagRetry_Off;
							u16t_OrderMissMatchCnt = 0;
						}
						else{}
					}
					else{
						u16t_OrderMissMatchCnt = 0;
					}
				}
				
				/* Apply Valve Location to lin data field */
				if(CheckStatusValue_t.u8t_ValveLocation == 1){
					l_u8_wr_LIN_Valve_ND_CR_Valve1_Status(1);
				}
				else if(CheckStatusValue_t.u8t_ValveLocation == 0){
					l_u8_wr_LIN_Valve_ND_CR_Valve1_Status(0);
				}
				else{}
				break;
				
			case ValveDiagRetry_Off :
				u16t_RetryTimeCnt++;
				l_u8_wr_LIN_Valve_ND_CR_Valve1_Status(3);
				FailSafeOff();	
				if(u16t_RetryTimeCnt > Slicing50ms_500ms){
					u16t_RetryTimeCnt = 0;
					u16t_RetryCnt++;
					if(u16t_RetryCnt>Valve_Retry_Cnt){
						ValveStatus_t = ValveDiagRetry_Check;
						u16t_OrderMissMatchCnt = 0;
						u16t_RetryCnt = 0;
					}
					else{
						ValveStatus_t = ValveDiagRetry_On;
					}
				}
				else{}
				break;
				
			case ValveDiagRetry_On :
				u16t_RetryTimeCnt++;
				l_u8_wr_LIN_Valve_ND_CR_Valve1_Status(3);
				FailSafeOn();
				if(u16t_RetryTimeCnt > Slicing50ms_500ms){
					u16t_RetryTimeCnt = 0;
					u16t_RetryCnt++;
					if(u16t_RetryCnt>Valve_Retry_Cnt){
						ValveStatus_t = ValveDiagRetry_Check;
						u16t_OrderMissMatchCnt = 0;
						u16t_RetryCnt = 0;
					}
					else{
						ValveStatus_t = ValveDiagRetry_Off;
					}
				}
				else{}
				break;
				
			case ValveDiagRetry_Check :
				l_u8_wr_LIN_Valve_ND_CR_Valve1_Status(3);
				if(u8t_ValveOrder==0){
					ControlValveOff(CheckStatusValue_t.u16t_HallICValueOneShot);
					if(CheckStatusValue_t.u8t_ValveLocation!=0){
						u16t_OrderMissMatchCnt++;
						if(u16t_OrderMissMatchCnt>Slicing50ms_1min){
							ValveStatus_t = ProductFail_HallIC;
							u16t_OrderMissMatchCnt = 0;
							l_bool_wr_LIN_Valve_ND_CF_Valve1_HallsensorFail(1);
						}
						else{}
					}
					else{
						ValveStatus_t = Idle;
						u16t_OrderMissMatchCnt = 0;
					}
				}
				else{
					ControlValveOn(CheckStatusValue_t.u16t_CurrentValueOneShot,CheckStatusValue_t.u16t_HallICValueOneShot);
					if(CheckStatusValue_t.u8t_ValveLocation!=1){
						u16t_OrderMissMatchCnt++;
						if(u16t_OrderMissMatchCnt>Slicing50ms_1min){
							ValveStatus_t = ProductFail_HallIC;
							u16t_OrderMissMatchCnt = 0;
							l_bool_wr_LIN_Valve_ND_CF_Valve1_HallsensorFail(1);
						}
						else{}
					}
					else{
						ValveStatus_t = Idle;
						u16t_OrderMissMatchCnt = 0;
					}
				}
				break;
				
			case ProductFail_Voltage :
				if(CheckStatusValue_t.u8t_ProductFailStatus & (IGN1FAIL_LOW | IGN1FAIL_HIGH)){
					ControlValveOff(CheckStatusValue_t.u16t_HallICValueOneShot);
					l_u8_wr_LIN_Valve_ND_CR_Valve1_Status(2);	
				}
				else{
					ValveStatus_t = Idle;
					l_u8_wr_LIN_Valve_ND_CR_Valve1_Status(0);					
				}
				break;
				
			case ProductFail_HallIC :
				ControlValveOff(CheckStatusValue_t.u16t_HallICValueOneShot);
				l_bool_wr_LIN_Valve_ND_CF_Valve1_HallsensorFail(1);
				l_u8_wr_LIN_Valve_ND_CR_Valve1_Status(7);	
				break;
				
			case LINComm_Fail :
				if(CheckStatusValue_t.u8t_ProductFailStatus & LIN_COMM_FAIL){
					ControlValveOff(CheckStatusValue_t.u16t_HallICValueOneShot);
					l_u8_wr_LIN_Valve_ND_CR_Valve1_Status(7);					
				}
				else{
					ValveStatus_t = Idle;
					l_u8_wr_LIN_Valve_ND_CR_Valve1_Status(0);						
				}
				break;
				
			default : 
				break;
		}
	}
	else{}
}

static void FailSafeOn(void)
{
	l_bool_wr_LIN_Valve_ND_CF_Valve_Acting_now(1);
	Motor_PWM_SetRatio16(PWM_DUTY100);
	ValvePhaseControl.OnPhase = OnReady;
	ValvePhaseControl.u16t_OnControlPhaseCnt = 0;
	ValvePhaseControl.u16t_PositionNotSetCnt = 0;
	f32t_ITerm = ITERM_DEFAULT;
	f32t_ITerm_Hal = PWM_DUTY30;
}

static void FailSafeOff(void)
{
	l_bool_wr_LIN_Valve_ND_CF_Valve_Acting_now(0);
	Motor_PWM_SetRatio16(0);
	ValvePhaseControl.OnPhase = OnReady;
	ValvePhaseControl.u16t_OnControlPhaseCnt = 0;
	ValvePhaseControl.u16t_PositionNotSetCnt = 0;
	f32t_ITerm = ITERM_DEFAULT;
	f32t_ITerm_Hal = PWM_DUTY30;
}

static void ControlValveOn(uint16_t u16t_Current,uint16_t u16t_HallICValue)
{
	
	/*static int16_t s16t_ErrorPre = 0;*/
	int16_t s16t_ErrorNow = 0;
	int32_t s32t_PWM_Output_Now = 0;
	
	l_bool_wr_LIN_Valve_ND_CF_Valve_Acting_now(1);
	ValvePhaseControl.OffPhase = OffReady;
	ValvePhaseControl.u16t_OffControlPhaseCnt = 0;
	
	switch(ValvePhaseControl.OnPhase){
		case OnReady :
			Motor_PWM_SetRatio16(ValvePhaseControl.OnReadyDuty);
			ValvePhaseControl.u16t_OnControlPhaseCnt++;
			if(ValvePhaseControl.u16t_OnControlPhaseCnt >= ValvePhaseControl.OnReadyTime){
				ValvePhaseControl.u16t_OnControlPhaseCnt = 0;
				ValvePhaseControl.OnPhase = OnIncline;
			}
			else{}
			break;
			
		case OnIncline :
			ValvePhaseControl.u16t_PositionNotSetCnt++;
			s16t_ErrorNow = (int16_t)(Target_Hal - u16t_HallICValue);
			f32t_ITerm_Hal += 20 * s16t_ErrorNow; 
			s32t_PWM_Output_Now = f32t_ITerm_Hal;
			if(s32t_PWM_Output_Now > PWM_MAX){
				s32t_PWM_Output_Now = PWM_MAX;
			}
			else if(s32t_PWM_Output_Now < PWM_DUTY10){
				s32t_PWM_Output_Now = PWM_DUTY10;
			}
			else{}
			Motor_PWM_SetRatio16(s32t_PWM_Output_Now);
			
			/* Hall IC Tolerance : 33(3.3v) ~ 35(3.5v)  */
			/* Detection Count : 2.5s */
			if((s16t_ErrorNow >= -1) && (s16t_ErrorNow <= 1)){
				ValvePhaseControl.u16t_OnControlPhaseCnt++;
				if(ValvePhaseControl.u16t_OnControlPhaseCnt >= 50){
					ValvePhaseControl.u16t_OnControlPhaseCnt = 0;
					ValvePhaseControl.OnPhase = OnImpulse;
					ValvePhaseControl.u16t_PositionNotSetCnt = 0;
				}
				else{}
			}
			else{}
			
			if(ValvePhaseControl.u16t_PositionNotSetCnt >= 140){
				ValvePhaseControl.u16t_PositionNotSetCnt = 0;
				ValvePhaseControl.OnPhase = OnImpulse;
			}
			else{}
			
			break;
			
		case OnImpulse :
			Motor_PWM_SetRatio16(ValvePhaseControl.OnImpulseDuty);
			ValvePhaseControl.u16t_OnControlPhaseCnt++;
			if(ValvePhaseControl.u16t_OnControlPhaseCnt >= ValvePhaseControl.OnImpulseTime){
				ValvePhaseControl.u16t_OnControlPhaseCnt = 0;
				ValvePhaseControl.OnPhase = OnCurrentControl;
			}
			else{}
			break;
			
		case OnCurrentControl :
			s16t_ErrorNow = (int16_t)(Target_Current - u16t_Current);
			f32t_ITerm += 5 * s16t_ErrorNow; 
			s32t_PWM_Output_Now = f32t_ITerm;
			if(s32t_PWM_Output_Now > PWM_MAX){
				s32t_PWM_Output_Now = PWM_MAX;
			}
			else if(s32t_PWM_Output_Now < PWM_MIN){
				s32t_PWM_Output_Now = PWM_MIN;
			}
			else{}
			Motor_PWM_SetRatio16(s32t_PWM_Output_Now);
			break;
		
		default :
			break;
	}
}

static void ControlValveOff(uint16_t u16t_HallICValue)
{
	l_bool_wr_LIN_Valve_ND_CF_Valve_Acting_now(0);
	f32t_ITerm = ITERM_DEFAULT;
	f32t_ITerm_Hal = PWM_DUTY30;
	ValvePhaseControl.OnPhase = OnReady;
	ValvePhaseControl.u16t_OnControlPhaseCnt = 0;
	ValvePhaseControl.u16t_PositionNotSetCnt = 0;
	
	switch(ValvePhaseControl.OffPhase){
		case OffReady :
			Motor_PWM_SetRatio16(ValvePhaseControl.OffReadyDuty);
			ValvePhaseControl.u16t_OffControlPhaseCnt++;
			if(ValvePhaseControl.u16t_OffControlPhaseCnt >= ValvePhaseControl.OffReadyTime){
				ValvePhaseControl.u16t_OffControlPhaseCnt = 0;
				ValvePhaseControl.OffPhase = OffLinearDecline;
			}
			else{}
			break;
			
		case OffLinearDecline :
			Motor_PWM_SetRatio16(ValvePhaseControl.OffReadyDuty - ValvePhaseControl.u16t_OffControlPhaseCnt * ValvePhaseControl.OffLinearDeclineUnit);
			ValvePhaseControl.u16t_OffControlPhaseCnt++;
			if((ValvePhaseControl.u16t_OffControlPhaseCnt * ValvePhaseControl.OffLinearDeclineUnit >= ValvePhaseControl.OffReadyDuty) || (u16t_HallICValue < 6)){
				ValvePhaseControl.u16t_OffControlPhaseCnt = 0;
				ValvePhaseControl.OffPhase = OffDone;
			}
			else{}
			break;
			
		case OffDone :
			Motor_PWM_SetRatio16(0);
			break;
	}
}