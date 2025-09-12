/*
 * DiagStatus.c
 *
 *  Created on: Apr 15, 2019
 *      Author: UN00518
 */
#include <Diagnosis.h>
#include <string.h>
#include <lin.h>
#include <lin_cfg.h>
#include <lin_hw_cfg.h>
#include <measureTask.h>

#define CHANNEL_HALLIC		1
#define CHANNEL_CURRENT		2
#define CHANNEL_IGN1		3

#define Slicing5ms_10min	120000
#define Slicing5ms_5min		600000
#define Slicing5ms_2min		240000
#define Slicing5ms_1min		120000
#define Slicing5ms_30sec	60000
#define Slicing5ms_10sec	20000
#define Slicing5ms_5sec		10000
#define Slicing5ms_500ms	1000
#define Slicing5ms_50ms		100

extern uint8_t u8t_PeriodFlag5;
static DiagStatusValue_t LocalDiagStatus_t;

static uint16_t GetIGN1Voltage(void);
static uint16_t GetHallICVoltage(void);
static uint16_t GetProductCurrent(void);

void DiagnosisSlicsingTask(uint16_t PeriodMs)
{
	static uint16_t u16t_PeriodCnt = 0;
		
	if(u16t_PeriodCnt > PeriodMs - 1){
		u16t_PeriodCnt = 0;
		u8t_PeriodFlag5 = 1;
	}
	else{}
	
	u16t_PeriodCnt++;
}


void DiagnosisManager(void)
{
	static uint16_t u16t_ValveOnCnt = 0;
	static uint16_t u16t_ValveOffCnt = 0;
	static uint16_t u16t_ValvePendingCnt = 0;
	static uint32_t u32t_IGN1HCnt = 0;
	static uint32_t u32t_IGN1LCnt = 0;
	static uint32_t u32t_IGN1IdleCnt = 0;
	static uint32_t u32t_LINCommFailCnt = 0;
	static uint32_t u32t_LINCommIdleCnt = 0;
	static uint16_t u16t_IGN1AvgCnt = 0;
	static uint16_t u16t_IGN1AvgCalc = 0;
	static uint16_t u16t_CurrentAvgCnt = 0;
	static uint16_t u16t_CurrentAvgCalc = 0;
	
	
//	LocalDiagStatus_t.u16t_IGN1ValueAvg500 = LocalDiagStatus_t.u16t_IGN1ValueOneShot = GetIGN1Voltage(); /* (uint32_t)(GetIGN1Voltage() >> 6)*100/204*52/100 + 7;*/		/* Resistor Voltage Allocation -> 1 : 5.2, Diode Voltage Drop : 0.7v */
//	u16t_IGN1AvgCalc += LocalDiagStatus_t.u16t_IGN1ValueOneShot;

	if(u8t_PeriodFlag5){
		u8t_PeriodFlag5 = 0;
		u16t_IGN1AvgCnt++;                                     
		 LocalDiagStatus_t.u16t_IGN1ValueAvg500 = LocalDiagStatus_t.u16t_IGN1ValueOneShot = GetIGN1Voltage(); /* (uint32_t)(GetIGN1Voltage() >> 6)*100/204*52/100 + 7;*/		/* Resistor Voltage Allocation -> 1 : 5.2, Diode Voltage Drop : 0.7v */
		 u16t_IGN1AvgCalc += LocalDiagStatus_t.u16t_IGN1ValueOneShot;

		if(u16t_IGN1AvgCnt > 199){
			u16t_IGN1AvgCnt = 0;
			u16t_IGN1AvgCalc += LocalDiagStatus_t.u16t_IGN1ValueOneShot;
			//LocalDiagStatus_t.u16t_IGN1ValueAvg500 = u16t_IGN1AvgCalc/200;
			u16t_IGN1AvgCalc = 0;
		}
		else{
			u16t_IGN1AvgCalc += LocalDiagStatus_t.u16t_IGN1ValueOneShot;
		}

		
		
		LocalDiagStatus_t.u16t_CurrentValueOneShot = GetProductCurrent();/* >> 6;*/
		LocalDiagStatus_t.u16t_CurrentValueAvg500 = u16t_CurrentAvgCalc = LocalDiagStatus_t.u16t_CurrentValueOneShot;		

		u16t_CurrentAvgCalc += LocalDiagStatus_t.u16t_CurrentValueOneShot;
		u16t_CurrentAvgCnt++;
		if(u16t_CurrentAvgCnt > 99){
			//LocalDiagStatus_t.u16t_CurrentValueAvg500 = u16t_CurrentAvgCalc/100;
			u16t_CurrentAvgCnt = 0;
			u16t_CurrentAvgCalc = 0;
		}
		else{}


		LocalDiagStatus_t.u16t_HallICValueOneShot = GetHallICVoltage(); /*(uint32_t)(GetHallICVoltage() >> 6)*100/204/10;*/
               /* LocalDiagStatus_t.u16t_CurrentValueAvg500 = 17; */
               /* LocalDiagStatus_t.u16t_HallICValueOneShot = 17;*/
		if(LocalDiagStatus_t.u16t_HallICValueOneShot > 3000){ /* Value  */
			u16t_ValveOnCnt++;
			u16t_ValveOffCnt = 0;
			u16t_ValvePendingCnt = 0;
			if(u16t_ValveOnCnt > Slicing5ms_50ms){
				u16t_ValveOnCnt = Slicing5ms_50ms+1;
				LocalDiagStatus_t.u8t_ValveLocation = 1;
			}
			else{}
		}
		else if(LocalDiagStatus_t.u16t_HallICValueOneShot < 1800){  /* Value */
			u16t_ValveOnCnt = 0;
			u16t_ValveOffCnt++;
			u16t_ValvePendingCnt = 0;
			if(u16t_ValveOffCnt > Slicing5ms_50ms){
				u16t_ValveOffCnt = Slicing5ms_50ms+1;
				LocalDiagStatus_t.u8t_ValveLocation = 0;
			}
			else{}
		}
		else{
			u16t_ValvePendingCnt++;
			u16t_ValveOnCnt = 0;
			u16t_ValveOffCnt = 0;
			if(u16t_ValvePendingCnt > Slicing5ms_50ms){
				u16t_ValvePendingCnt = Slicing5ms_50ms+1;
				LocalDiagStatus_t.u8t_ValveLocation = 2;
			}
			else{}
		}
		
		if(LocalDiagStatus_t.u16t_IGN1ValueAvg500 > 18000){ /* 3.3V   180 */
			u32t_IGN1HCnt++;
			u32t_IGN1LCnt = 0;
			u32t_IGN1IdleCnt = 0;
			if(u32t_IGN1HCnt > Slicing5ms_2min){
				u32t_IGN1HCnt = Slicing5ms_2min+1;
				if(LocalDiagStatus_t.u8t_ProductFailStatus&IGN1FAIL_LOW){
					LocalDiagStatus_t.u8t_ProductFailStatus|=IGN1FAIL_HIGH;
					LocalDiagStatus_t.u8t_ProductFailStatus&=~IGN1FAIL_LOW;
				}
				else{
					LocalDiagStatus_t.u8t_ProductFailStatus|=IGN1FAIL_HIGH;
				}
			}
			else{}
		}
		else if((LocalDiagStatus_t.u16t_IGN1ValueAvg500 > 7000) && (LocalDiagStatus_t.u16t_IGN1ValueAvg500 < 16000)){ /*90 165 */
			u32t_IGN1IdleCnt++;
			u32t_IGN1HCnt = 0;
			u32t_IGN1LCnt = 0;
			if(u32t_IGN1IdleCnt > Slicing5ms_30sec){
				u32t_IGN1IdleCnt = Slicing5ms_30sec+1;
				LocalDiagStatus_t.u8t_ProductFailStatus&=~(IGN1FAIL_HIGH | IGN1FAIL_LOW);
			}
			else{}
		}else if(LocalDiagStatus_t.u16t_IGN1ValueAvg500 < 6500){  /* 80 */
			u32t_IGN1LCnt++;
			u32t_IGN1HCnt = 0;
			u32t_IGN1IdleCnt = 0;
			if(u32t_IGN1LCnt > Slicing5ms_2min){
				u32t_IGN1LCnt = Slicing5ms_2min+1;
				if(LocalDiagStatus_t.u8t_ProductFailStatus&IGN1FAIL_HIGH){
					LocalDiagStatus_t.u8t_ProductFailStatus|=IGN1FAIL_LOW;
					LocalDiagStatus_t.u8t_ProductFailStatus&=~IGN1FAIL_HIGH;
				}
				else{
					LocalDiagStatus_t.u8t_ProductFailStatus|=IGN1FAIL_LOW;
				}
			}
			else{}
		}
		else{
			u32t_IGN1IdleCnt = 0;
			u32t_IGN1HCnt = 0;
			u32t_IGN1LCnt = 0;
		}
		
		/* LIN Comm Status Checking */
		if(l_bool_rd_LIN_Valve_ND_CF_Valve1_LINCommFail()){
			u32t_LINCommFailCnt++;
			u32t_LINCommIdleCnt=0;
			if(u32t_LINCommFailCnt>Slicing5ms_10sec){
				u32t_LINCommFailCnt = Slicing5ms_10sec + 1;
				LocalDiagStatus_t.u8t_ProductFailStatus|=LIN_COMM_FAIL;
			}
			else{}
		} 
		else{
			u32t_LINCommFailCnt = 0;
			u32t_LINCommIdleCnt++;
			if(u32t_LINCommIdleCnt > Slicing5ms_5sec){
				u32t_LINCommIdleCnt = Slicing5ms_5sec + 1;
				LocalDiagStatus_t.u8t_ProductFailStatus&=~LIN_COMM_FAIL;
			}
			else{}
		}
	}
	else{}
}

uint16_t DiagnosisGetIGN1Value(void)
{
	return LocalDiagStatus_t.u16t_IGN1ValueAvg500;
}

uint16_t DiagnosisGetCurrentValue(void)
{
	return LocalDiagStatus_t.u16t_CurrentValueAvg500;
}

uint16_t DiagnosisGetHallICValue(void)
{
	return LocalDiagStatus_t.u16t_HallICValueOneShot;
}

uint8_t DiagnosisGetProductFailState(void)
{
	return LocalDiagStatus_t.u8t_ProductFailStatus;
}

uint8_t DiagnosisGetValveLocation(void)
{
	return LocalDiagStatus_t.u8t_ValveLocation;
}


static uint16_t GetIGN1Voltage(void)
{
	uint16_t u16t_IGN1Value = 0;
	
	//AD1_Measure(1);
	//AD1_MeasureChan(1,CHANNEL_IGN1);
	AD1_GetChanValue16(CHANNEL_IGN1,&u16t_IGN1Value);	
	return u16t_IGN1Value;
}

static uint16_t GetHallICVoltage(void)
{
	uint16_t u16t_HallIcValue = 0;
	
	//AD1_Measure(1);
	//AD1_MeasureChan(1,CHANNEL_HALLIC);
	AD1_GetChanValue16(CHANNEL_HALLIC,&u16t_HallIcValue);

	return u16t_HallIcValue;
}

static uint16_t GetProductCurrent(void)
{
	uint16_t u16t_CurrentValue = 0;
	
	//AD1_Measure(1);
	//AD1_MeasureChan(1,CHANNEL_CURRENT);
	AD1_GetChanValue16(CHANNEL_CURRENT,&u16t_CurrentValue);
	
	return u16t_CurrentValue;
}

