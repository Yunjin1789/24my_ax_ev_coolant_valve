/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file pwm_device.c
 */


#include <stdint.h>
#include <string.h>
#include <rugby_cl.h>
#include <errno.h>
#include <pwm_device.h>
#include <appConfig.h>
#include <isrfuncs.h>
#include <adc_device.h>

#if defined PWM_SFRS

/* 128us for PN voltage measurement*/
#define MAX_MEASURE_TIME_TICKS          (MAIN_CPU_CLOCK * 64U)   /* us  */
#define MAX_DEBUNCE_TIME_TICKS          (MAIN_CPU_CLOCK * 128U) /* us  */

#define MAX_PWM_CHN_DELAY_ON_TIME_TICKS (MAIN_CPU_CLOCK * 4U)   /* us  */

static uint16_t currPWM[24U];
static PwmIsrCallback_t pwmReloadCallback = NULL;

void PWM_Handler(void)
{
    if (pwmReloadCallback == NULL){
        pwmReloadCallback();
    }
}

/**
 * @brief The instance of PMW device
 */
void PWM_Init(PwmPrescaler_t divide, uint16_t period)
{
    IOCTRLA_SFRS->LEDPIN.HWMODE = 1U;
    PWM_SFRS->BASE.PRESCALESEL = (uint8_t)divide;
    PWM_SFRS->BASE.PERIOD = period;
    
    /* set duty cycle to 0*/
    for (uint8_t i = 0U; i < 24U; i++){
        PWM_SFRS->PULSE[i].PFALL = 0U;
        PWM_SFRS->PULSE[i].PRISE = 0U;
    }
}

/**
 * @brief The instance of PMW device
 */
void PWM_SetMatchValue(PwmChannel_t channel, uint16_t matchRisingValue, uint16_t matchFaillValue)
{
    PWM_SFRS->PULSE[channel].PRISE = matchRisingValue;
    PWM_SFRS->PULSE[channel].PFALL = matchFaillValue;
}

/**
 * @brief The instance of PMW device
 */
void PWM_StartAndUpdate(void)
{
    PWM_SFRS->ENAREQ.ENAREQALL = 1U;
    PWM_SFRS->UPDATE = 1U;
}



/**
 * @brief The instance of PWM_SetRGBValue
 */
void PWM_SetRGBValue(uint8_t ledNo, uint16_t red, uint16_t green, uint16_t blue)
{
    uint32_t maxDUTY;
    uint16_t r,g,b;
    maxDUTY = (1U << PWM_VALUE_MAX_POS) - MAX_MEASURE_TIME_TICKS - MAX_DEBUNCE_TIME_TICKS - 1U;
    
    r = (uint16_t)((red  *maxDUTY) >> 16);
    g = (uint16_t)((green*maxDUTY) >> 16);
    b = (uint16_t)((blue *maxDUTY) >> 16);
   
    currPWM[PHY_CHANNEL_R(ledNo)] = r;
    currPWM[PHY_CHANNEL_G(ledNo)] = g;
    currPWM[PHY_CHANNEL_B(ledNo)] = b;
    
    PWM_SetMatchValue(PHY_CHANNEL_R(ledNo),MAX_MEASURE_TIME_TICKS, r + MAX_MEASURE_TIME_TICKS);
    PWM_SetMatchValue(PHY_CHANNEL_G(ledNo),(((uint16_t)maxDUTY - g)>>1U), (uint16_t)((maxDUTY + (uint32_t)g) >> 1U));
    PWM_SetMatchValue(PHY_CHANNEL_B(ledNo),((uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS - b), (uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS);
    
    PWM_StartAndUpdate();
}

uint8_t PWM_UpdateFinished(void)
{
    uint8_t result = FALSE;
    uint8_t update = PWM_SFRS->UPDATE;
    if (update == 0U){
        result = TRUE;
    }
    return result;
}


uint8_t PWM_LedIsOn(uint8_t ledNo)
{
    uint8_t result = 1U;
    if ( (currPWM[PHY_CHANNEL_R(ledNo)] <= (PWM_VALUE_MAX/100U)) && 
         (currPWM[PHY_CHANNEL_G(ledNo)] == 0U) && 
         (currPWM[PHY_CHANNEL_B(ledNo)] <= (PWM_VALUE_MAX/100U)) ){
        result = 0U;
    }
    return result;
}

/**
 * @brief The instance of PMW device
 */
void PWM_RegisterPWMReloadIRQ(PwmIsrCallback_t callback)
{
    pwmReloadCallback = callback;
    PWM_SFRS->INTPWM.CLEAR.PERIOD = 1U;
    PWM_SFRS->INTPWM.IRQ.PERIOD = 1U;
    PWM_SFRS->UPDATE = 1U;
    NVIC_EnableIRQ(PWM_IRQn);
}

/**
 * @brief The instance of PMW device
 */
void PWM_UnRegisterPWMReloadIRQ(void)
{
    pwmReloadCallback = NULL;
    PWM_SFRS->INTPWM.CLEAR.PERIOD = 1U;
    PWM_SFRS->INTPWM.IRQ.PERIOD   = 0U;
    PWM_SFRS->UPDATE = 1U;
    NVIC_DisableIRQ(PWM_IRQn);
}


/**
 * @brief The instance of PMW device
 */
void PWM_EnableAllChannels(void)
{
    for (uint8_t i = 0U; i < 23U; i++){
        PWM_SFRS->PULSE[i].PRISE = 1U;
        PWM_SFRS->PULSE[i].PFALL = 0U;
    }
    PWM_REG_ENAREQ.ENAREQALL = 0x01U;
    PWM_SFRS->UPDATE = 1U;
}

/**
 * @brief The instance of PMW device
 */
void PWM_DisableAllChannels(void)
{
    PWM_SFRS->ENAREQ.CLRREQALL   = 0x1U;
    PWM_SFRS->ENAREQ.FORCEINACTIVE = 1U;
    PWM_SFRS->UPDATE = 1U;
}

/**
 * @brief The instance of PMW device
 */
void PWM_Disable(PwmChannel_t channel)
{
    PWM_SFRS->ENAREQ.ENAREQ &= ~(1UL << channel);
    PWM_SFRS->UPDATE = 1U;
}


/**
 * @brief The instance of PMW device
 */
void PWM_SetPrescaler(PwmPrescaler_t divide)
{
    PWM_SFRS->BASE.PRESCALESEL = (uint8_t)divide;
    PWM_SFRS->UPDATE = 1U;
}

/**
 * @brief The instance of PMW device
 */
void PWM_SetPeriod(uint16_t period)
{
    PWM_SFRS->BASE.PERIOD = period;
    PWM_SFRS->UPDATE = 1U;
}

/**
 * @brief The instance of PMW device
 */
void PWM_SetInvert(PwmChannel_t channel, uint8_t invertEn)
{
    PWM_SFRS->INVERT |= (uint32_t)((uint32_t)invertEn << channel);
    PWM_SFRS->UPDATE = 1U;
}

/**
 * @brief The instance of PMW device
 */
int8_t PWM_SetMaxChannelCurrent(CurrValue_t current,OffCurrValue_t offCurrent)
{
    /* Enable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0x0EU;
    SYSCTRLA_SFRS->TRIMLED = HWCFG_GetLEDTrimValue();
    SYSCTRLA_SFRS->TRIMVFW = HWCFG_GetOffLEDTrimValue();
    SYSCTRLA_SFRS->PMU_TRIM.RESISTOR_TRIM =  HWCFG_GetV2ITrimValue();
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0x00U; /* Disable trim access write enable */
    return 0;
}

void PWM_TurnOffChannelCurrent(void)
{
    /* Enable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY  = 0x0EU;
    SYSCTRLA_SFRS->TRIMLED              = 0U;
    SYSCTRLA_SFRS->TRIMVFW              = 0U;
    /* Disable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0x00U; 

}



#endif