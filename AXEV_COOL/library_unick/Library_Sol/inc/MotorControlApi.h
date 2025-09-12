/*
 * MotorControl.h
 *
 *  Created on: Apr 15, 2019
 *      Author: UN00518
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

//#include "PE_Types.h"
#include <stdint.h>
#include <string.h>

/* PWM DUTY 16Bit */
#define PWM_DUTY10			1000
#define PWM_DUTY15			1500
#define PWM_DUTY20			2000
#define PWM_DUTY25			2500
#define PWM_DUTY30			3000
#define PWM_DUTY35			3500
#define PWM_DUTY40			4000
#define PWM_DUTY45			4500
#define PWM_DUTY50			5000
#define PWM_DUTY60			6000
#define PWM_DUTY70			7000
#define PWM_DUTY80			8000
#define PWM_DUTY85			8500
#define PWM_DUTY90			9000
#define PWM_DUTY95			9500
#define PWM_DUTY100		10000

void ValveControlSlicsingTask(uint16_t PeriodMs);
void ValveControlManager(void);
void MotorControlApiMonSet(uint8_t Stall_MonF);
void MotorControlTask(void);
void MotorControlInit(void);
#endif /* VALVECONTROL_H_ */
