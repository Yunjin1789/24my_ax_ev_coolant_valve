/*
 * DiagStatus.h
 *
 *  Created on: Apr 15, 2019
 *      Author: UN00518
 */

#ifndef DIAGNOSIS_H_
#define DIAGNOSIS_H_

#include "main.h"

#define PRODUCT_IDLE		0x00
#define IGN1FAIL_HIGH		0x01
#define IGN1FAIL_LOW		0x02
//#define CURRENTFAIL_HIGH	0x04
//#define CURRENTFAIL_LOW		0x08
#define LIN_COMM_FAIL		0x04


typedef struct{
	uint16_t u16t_CurrentValueAvg500;
	uint16_t u16t_HallICValueAvg500;
	uint16_t u16t_IGN1ValueAvg500;
	uint16_t u16t_IGN1ValueOneShot;
	uint16_t u16t_HallICValueOneShot;
	uint16_t u16t_CurrentValueOneShot;
	uint8_t u8t_ValveLocation;
	uint8_t u8t_ProductFailStatus;
}DiagStatusValue_t;

void DiagnosisSlicsingTask(uint16_t PeriodMs);
void DiagnosisManager(void);
uint16_t DiagnosisGetIGN1Value(void);
uint16_t DiagnosisGetCurrentValue(void);
uint16_t DiagnosisGetHallICValue(void);
uint8_t DiagnosisGetProductFailState(void);
uint8_t DiagnosisGetValveLocation(void);




#endif /* DIAGSTATUS_H_ */
