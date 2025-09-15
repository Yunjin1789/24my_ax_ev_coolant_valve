/**
 * @copyright 2020 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 * @file systemInit.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2018/09/20
 */

#include <systemInit.h>
#include <rugbyPro.h>
#include <buck_device.h>
#include <pwmAux_device.h>
#include <uart_device.h>
#include <linSlaveTask.h>
#include <linStackTask.h>
#include <timer_device.h>
#include <spi_device.h>
#include <softTimerTask.h>
#include <pmu_device.h>
#include <pdsTask.h>
#include <wdt_device.h>

void SYS_Init(void)
{
    /* Enable trim revise access enable*/
    HWCFG_TrimAccessUnlock();
    CRGA_SFRS->MODULERSTREQ.BYTE = 0xFFU;
    /* Init system clock */
    Clock_SystemMainClockInit(SYS_MAIN_CLOCK_DIV);
    PMU_Init();
    /* Init global timer engine for driving soft timer */
    SysTick_Init(SOFT_TIMER_INTERVAL *1000U * MAIN_CPU_CLOCK, SoftTimer_ExpireCallback);
#if WATCH_DOG_EN == 1U
    WDTA_Enable(WDTA_INTERVAL_256MS); /* 16s */
#endif
    /* Init gpios settings */
    GPIOs_Init();
    PWM_Module_Init();
#if _UNICK_COOL_
        FLASH_Read();
        Flash_EraseSector(FLASH_START_ADDRESS_USER_DATA);
        SPI_Init(SPI_CLK_DIV_64);
        SM1_SendChar(0xE9F8); //0xE9F8 전류제한 X,  0xEDF8 전류제한 O
#endif

#if (UART_MILKY_WAY_EN == 1U)
    (void)UART_Init(BAUDRATE_DIV_16MHz_1000000, BITSIZE_8BITS, PARITY_NONE,STOPS_1BITS);
    UART_RegisterIRQ(0,APPL_UART_ISR);
#elif (CODE_DEBUG_EN == 1U)
    (void)UART_Init(BAUDRATE_DIV_16MHz_115200, BITSIZE_8BITS, PARITY_NONE,STOPS_1BITS);
    UART_RegisterIRQ(0,NULL);
#endif

#if (UART_MILKY_WAY_EN == 1U)
#else
#endif 
    
    TM_PostTask(TASK_ID_SOFT_TIMER);
    TM_PostTask(TASK_ID_LINS);
    TM_PostTask(TASK_ID_PDS);
#if LIN_MASTER_EN == 1
    TM_PostTask(TASK_ID_LINM);
#endif
    TM_PostTask(TASK_ID_ADC_MEASURE);
}



