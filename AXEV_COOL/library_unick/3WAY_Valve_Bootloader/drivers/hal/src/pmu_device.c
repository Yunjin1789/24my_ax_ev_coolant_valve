/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file pmu_device.c
 */

#include <stddef.h>
#include <pmu_device.h>
#include <errno.h>
#include <isr_redirection.h>
#include <gpio_device.h>
#include <lin_device.h>
#include <wica_sfr.h>
#include <pwm_device.h>
#include <buck_device.h>
#include <appConfig.h>

void BOR_SoftHandler(void) 
{
    
}
   
void PMU_BORInit(Bor1V5Thres_t lowThreshold, Bor3V3Thres_t highThreshold)
{
    CRGA_SFRS->BORCONFIG.BOR1V5THRESH = (uint8_t)lowThreshold;
    CRGA_SFRS->BORCONFIG.BOR3V3THRESH = (uint8_t)highThreshold;
    CRGA_SFRS->BORCONFIG.BORBIASOVERRIDESEL = 0U;
    CRGA_SFRS->BORACTION.VDD1V5 = (uint8_t)PMU_BROWNOUT_RESET;
    CRGA_SFRS->BORACTION.VDD3V3 = (uint8_t)PMU_BROWNOUT_RESET;
    
    CRGA_SFRS->BORACTION.BOR_1V5_LOCK = 1U;
    CRGA_SFRS->BORACTION.BOR_3V3_LOCK = 1U;
    
    EVTHOLD_SFRS->HOLD = 0U; /* clear  Lullaby_Handler after wake up  */
}

void PMU_WakeTimerInit(PMU_WAKEUP_TIMEER_MODE_t mode, PMU_WAKEUP_TIMEER_Interval_t interval)
{
  if (mode == WAKEUP_TIMEER_DISABLE){
      WICA_SFRS->CTRL.TIMERENA = 0U;
  }else{
      WICA_SFRS->CTRL.TIMERTAPSEL = (uint8_t)interval;
      WICA_SFRS->CTRL.TIMERENA    = 1U;
  }
}

void PMU_EnterToDeepSleepMode(void)
{
    BUCK_DisableBuck();                 /* disable buck*/
    CRGA_SFRS->MODULERSTREQ.BYTE = MODUE_SOFT_RESET_PWM;
    /*  Add GPIO function here for low power mode*/
    /* set gpios to input with power up resistor */
    for (uint8_t i = (uint8_t)GPIO_PORT_1; i<= (uint8_t)GPIO_PORT_9; i++ ){
        GPIO_Init((GpioPort_t)i, GPIO_DIR_INPUT,GPIO_PULL_UP);
    }
    /*  end of GPIO settigns   */
    CRGA_SFRS->OVTEMPCONFIG.OVTEMPENA = 0U;     /* disbale temperature sensor when sleep */
    CRGA_SFRS->OVTEMPCONFIG.DISTEMPSENS = 1U;
    
    /* clear all of wake up flags */
    /* Enable wakeup detect and IRQ  when LINS is in sleep mode(LINS_REG_CTRL.SLEEP = 1U;)  */
    WICA_REG_CTRL.LINSIRQCLR = 1U;
    WICA_REG_CTRL.LINSIRQENA = 1U;
    WICA_REG_CTRL.LINSENA    = 1U;
    NVIC_EnableIRQ(WULIN_IRQn);

    NVIC_EnableIRQ(Lullaby_IRQn);               /*  Enable  Lullaby interrupt*/
    EVTHOLD_SFRS->HOLD = 1U;                    /*  Trigger ISR then entering sleep in ISR */
    /* Waiting to enter sleep mode */
    for(;;){
    }
}

/* Please make sure here and don't delete these functions!!!! or it would cause severe error*/
/* hibernate wake up interrupt ISR */
void Lullaby_SoftHandler(void)
{
    /*  Enter sleep mode  */
    PMUA_SFRS->CTRL.HIBERNATE = 1U;
}


uint8_t PUM_IsWakupSignal(void)
{
    uint8_t status = SYSCTRLA_REG_RETAIN1;
    return status;
}

uint8_t PUM_IsAppEnterBootModeCmd(void)
{
    uint8_t result = FALSE;
    uint8_t status = SYSCTRLA_REG_RETAIN0;
    if ((status&0x0FU) == 0x05U){
        SYSCTRLA_REG_RETAIN0 = 0U;
        result = TRUE;
    }
    return result;
}
