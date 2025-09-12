/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file lin_device.c
 */

#include <stdint.h>
#include <string.h>

#include <appConfig.h>
#include <rugbyPro.h>
#include "timer_device.h"
#include "errno.h"
#include <isr_redirection.h>
#include <lin_device.h>

static lin_stack_cb_func_t lin_stack_callback = NULL;

typedef struct {
    uint16_t divider;
    uint16_t multiplier;
    uint8_t prescale;
} lin_speed_setting_t;

/**********************************************************************  
 *    Local definitions
 *********************************************************************/   

/**
 * At 16MHz clock, the clock divider settings for each LIN speed.
 */

#if SYS_MAIN_CLOCK_DIV == CLOCK_DIV_1   /* 16MHz */
  static const lin_speed_setting_t lins_speed_map[E_LIN_BAUDRATE_MAX] = {
      [E_LIN_BAUDRATE_20000] = {.divider = 200U, .multiplier = 0U, .prescale = 2U},
  };
#else
  /*#error MAIN_CPU_CLOCK MUST BE at 16MHz system colck!*/
#endif

void lins_SetTiming(LIN_BaudRate_t BaudRate);


 void lins_SetTiming(LIN_BaudRate_t BaudRate)
{
    LINS_SFRS->BTDIV07          = (uint8_t)lins_speed_map[BaudRate].divider & 0xFFU;
    LINS_SFRS->BITTIME.BTDIV8   = (uint8_t)lins_speed_map[BaudRate].divider >> 8U;
    LINS_SFRS->BITTIME.PRESCL   = (uint8_t)lins_speed_map[BaudRate].prescale;
}


void LINS_Init(void)
{
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0x0EU;
    LINS_REG_CTRL.SLEEP = 0U;                     /*  wake up lins when init if sleep  */
    /* Config GPIO to LIN mode, enable transmission */
    IOCTRLA_SFRS->LIN.LINS_HWMODE       = 1U;     /* Hardware Mode Enabled. LIN slave peripheral writes/read the LIN I/O pin.*/
    IOCTRLA_SFRS->LIN.LINS_PU30K_ENA    = 1U;     /* LIN 30K pullup enable.*/
    IOCTRLA_SFRS->LIN.LINS_TXENA        = 1U;     /* LIN transmit enable.*/
    IOCTRLA_SFRS->LIN.LINS_RXENA        = 1U;     /* LIN receive enable. */
    IOCTRLA_SFRS->LIN.SWON              = 1U;     /* 1: LIN slave auto addressing On*/
  
    LINS_SFRS->BUSTIME.BUSINACTIVE      = E_LIN_TIME_INACTIVE_SEC_4;
    LINS_SFRS->BUSTIME.WUPREPEAT        = E_LIN_TIME_WAKEUP_REPEAT_MS_200;
    LINS_SFRS->CTRL.RST_INT_ERR         = 0x03U;    /* reset error,reset interrupt */
    
    SYSCTRLA_SFRS->LIN.TXLINSRISESLOPE = 1;
    SYSCTRLA_SFRS->LIN.TXLINS_DR_SLOPE = LIN_TX_SLEW_RATE_4_9V_PER_US;
    
    lins_SetTiming(E_LIN_BAUDRATE_20000);       /* set the maximum supported baud rate */
    
    /* Enable wakeup detect and IRQ  when LINS is in sleep mode(LINS_REG_CTRL.SLEEP = 1U;)  */
    WICA_REG_CTRL.LINSIRQCLR = 1U;
    WICA_REG_CTRL.LINSIRQENA = 1U;
    WICA_REG_CTRL.LINSENA    = 1U;
    NVIC_EnableIRQ(WULIN_IRQn);
    
}


void LINS_RegisterLINStack_IRQ(lin_stack_cb_func_t callback)
{
    lin_stack_callback = callback;
    NVIC_EnableIRQ(LINS_IRQn);
}

void LINS_SetSlaveModuleToSleep(void)
{
   LINS_REG_CTRL.SLEEP = 1U;
}


void LINS_AutoAddressingSwitch(AutoaddressingSwitchState_t state)
{
   IOCTRLA_SFRS->LIN.SWON = (uint8_t)state;
}


void LINS_SoftHandler(void)
{
    if (lin_stack_callback != NULL){
        lin_stack_callback();
    }
}


void LINM_SoftHandler(void)
{

}


/* Please make sure here and don't delete these functions!!!! or it would cause severe error*/
void WULIN_SoftHandler(void)
{

}

/* Please make sure here and don't delete these functions!!!! or it would cause severe error*/
/* wakeup timer wake up interrupt ISR */
void WUTIMER_SoftHandler(void)
{

}