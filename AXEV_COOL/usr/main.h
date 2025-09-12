/*
 * main.h
 *
 *  Created on: Aug 9, 2019
 *      Author: UN00427
 */

#ifndef MAIN_H_
#define MAIN_H_

/* Including needed modules to compile this module/procedure */
#include <clock_device.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;
typedef unsigned long dlong[2];
typedef uint8_t TPE_ErrCode;

typedef word SM1_TComData;           /* User type for communication. */


#define M_IN1 3
#define M_IN2 2
#define M_ENLB 1
#define M_DIS  0

#define ERR_OK           0U            /* OK */
#define ERR_SPEED        1U            /* This device does not work in the active speed mode. */
#define ERR_RANGE        2U            /* Parameter out of range. */
#define ERR_VALUE        3U            /* Parameter of incorrect value. */
#define ERR_OVERFLOW     4U            /* Timer overflow. */
#define ERR_MATH         5U            /* Overflow during evaluation. */
#define ERR_ENABLED      6U            /* Device is enabled. */
#define ERR_DISABLED     7U            /* Device is disabled. */
#define ERR_BUSY         8U            /* Device is busy. */
#define ERR_NOTAVAIL     9U            /* Requested value or method not available. */
#define ERR_RXEMPTY      10U           /* No data in receiver. */
#define ERR_TXFULL       11U           /* Transmitter is full. */
#define ERR_BUSOFF       12U           /* Bus not available. */
#define ERR_OVERRUN      13U           /* Overrun error is detected. */
#define ERR_FRAMING      14U           /* Framing error is detected. */
#define ERR_PARITY       15U           /* Parity error is detected. */
#define ERR_NOISE        16U           /* Noise error is detected. */
#define ERR_IDLE         17U           /* Idle error is detected. */
#define ERR_FAULT        18U           /* Fault error is detected. */
#define ERR_BREAK        19U           /* Break char is received during communication. */
#define ERR_CRC          20U           /* CRC error is detected. */
#define ERR_ARBITR       21U           /* A node losts arbitration. This error occurs if two nodes start transmission at the same time. */
#define ERR_PROTECT      22U           /* Protection error is detected. */
#define ERR_UNDERFLOW    23U           /* Underflow error is detected. */
#define ERR_UNDERRUN     24U           /* Underrun error is detected. */
#define ERR_COMMON       25U           /* Common error of a device. */
#define ERR_LINSYNC      26U           /* LIN synchronization error is detected. */
#define ERR_FAILED       27U           /* Requested functionality or process failed. */
#define ERR_QFULL        28U           /* Queue is full. */

//#pragma CODE_SEG DEFAULT
typedef struct{
	bool mc33hb2000_ENBL;
	bool mc33hb2000_DIS;
	bool mc33hb2000_IN1;
	uint8_t mc33hb2000_IN2;
	uint8_t mc33hb2000_FS_B;
	uint8_t mc33hb2000_CFB;        
	uint8_t mc33hb2000_CS_B;
	uint8_t mc33hb2000_SCLK;
	uint8_t mc33hb2000_MOSI;
	uint8_t	mc33hb2000_MISO;
        uint16_t mc33hb2000_PWM;
}MC33HB2000_Port;

#endif /* MAIN_H_ */
