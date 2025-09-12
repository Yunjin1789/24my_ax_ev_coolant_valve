/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file wdt_device.c
 */

#include <string.h>
#include <rugby_cl.h>
#include <buck_device.h>
#include <clock_device.h>
#include "errno.h"
#include <isrfuncs.h>

#define BUCK_FREQ_KHZ   (500U)

static BuckIsrCallback_t buckIsrCallback = NULL;

/* freqiency: KHz vBuckVolt: mV*/
void BUCK_Init( uint16_t vBuckVolt,BuckOverOKVoltThres_t overVolt, BuckOverOKVoltThres_t okVolt,BuckUnderVoltThres_t underVolt)
{
    BUCKCTRL_REG_CLOCK.PERIOD      = (uint8_t)(Clock_GetSystemClock()/BUCK_FREQ_KHZ -1U);
    BUCKCTRL_REG_CLOCK.SPREAD      = 0U; /* Enable frequency spread */

    /* Enable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0x0EU;
    BUCKCTRL_REG_VDACCODE.VDACTRIM = (uint8_t)(((uint32_t)vBuckVolt *256U *32U)/(11U*25000U) -1U); /* vBuckVolt: 6.25*1.375*(VDACTRIM+1)/256 */
    /* Disable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0U;
    BUCK_TRIM_REG_DATA4.SETILIM = 0x8;

    BUCKCTRL_REG_VBUCKCTRL.OVLEVEL_SEL  = (uint8_t)overVolt;     /* over volt monitor level */
    BUCKCTRL_REG_VBUCKCTRL.OKLEVEL_SEL  = (uint8_t)okVolt;       /* ok volt monitor level */
    BUCKCTRL_REG_VBUCKCTRL.UVLEVEL_SEL  = (uint8_t)underVolt;    /* under volt monitor level*/

    BUCKCTRL_REG_VBUCKCTRL.OV_MONITOR_ENA  = 1U;        /* enable over volt monitor */
    BUCKCTRL_REG_VBUCKCTRL.OK_MONITOR_ENA  = 1U;        /* enable OK volt monitor */
    BUCKCTRL_REG_VBUCKCTRL.UV_MONITOR_ENA  = 1U;        /* enable under volt monitor */

    BUCKCTRL_REG_VBUCKDBNC.BUCKSTRBDBNCSEL = 3U;        
    BUCKCTRL_REG_VBUCKDBNC.OVTHRES1 = 7U;               /* over volt debouncing settings  */
    BUCKCTRL_REG_VBUCKDBNC.UVTHRES1 = 7U;               /* under volt debouncing settings  */
    BUCKCTRL_REG_VBUCKDBNC.LFENA       = 0x00U;         /* Low frequency strobing enable for debouncing */
    BUCKCTRL_REG_VBUCKDBNC.BUCKDBNCENA = 0x0FU;         /* enable debouncing */
    BUCKCTRL_REG_VBUCKDBNC.EXTRADBNC   = 0x07U;

    BUCKCTRL_REG_VBUCKTIMEOUT.PREENADWELL       = 0x0FU;
    BUCKCTRL_REG_VBUCKTIMEOUT.DETLOWDWELL       = 0x08U;
    BUCKCTRL_REG_VBUCKTIMEOUT.STARTLDWELL       = 0x08U;
    BUCKCTRL_REG_VBUCKTIMEOUT.STARTDBNCDWELL    = 0x06U;
    BUCKCTRL_REG_VBUCKTIMEOUT.UVDBNCDWELL       = 0x01U;
    BUCKCTRL_REG_VBUCKTIMEOUT.OVDBNCDWELL       = 0x0FU;
    BUCKCTRL_REG_VBUCKTIMEOUT.HICCUPDWELL       = 0x0FU;
    BUCKCTRL_REG_CTRL.HICCUPENA  = 1U;
    BUCKCTRL_REG_CTRL.ENAREQ     = 1U;                  /* enable buck */
}

void BUCK_RegisterEmergencyIRQ(BuckIsrCallback_t callback)
{
    buckIsrCallback = callback;
    BUCKCTRL_REG_BUCKIRQ.ENABLE.BUCK_OV = 1U;
    BUCKCTRL_REG_BUCKIRQ.ENABLE.BUCK_UV = 1U;
    BUCKCTRL_REG_BUCKIRQ.ENABLE.OVERCUR = 1U;
    BUCKCTRL_REG_BUCKIRQ.ENABLE.START_ERROR = 1U;
    
    BUCKCTRL_REG_BUCKIRQ.ENABLE.DISABLED = 1U;
    NVIC_EnableIRQ(BUCK_IRQn);
}


void BUCK_Handler(void)
{
    uint8_t status = BUCKCTRL_REG_BUCKIRQ.STATUS.BYTE;
    if (buckIsrCallback != NULL){
        buckIsrCallback(status);
    }
    BUCKCTRL_REG_BUCKIRQ.CLEAR.BYTE = 0xFFU;
}


void BUCK_DisableBuck(void)
{
    BUCKCTRL_REG_CTRL.ENAREQ     = 0U;                  /* disable buck */
    while(BUCKCTRL_REG_STATUS.BUCKOFF == 0U){}           /* wait until disable finished */
}

void BUCK_EnableBuck(void)
{
    BUCKCTRL_REG_CTRL.ENAREQ     = 1U;                  /* enable buck */
}

void BUCK_RestartBuck(void)
{
    BUCKCTRL_REG_CTRL.ENAREQ     = 0U;                  /* disable buck */
    while(BUCKCTRL_REG_STATUS.BUCKOFF == 0U){}          /* wait until disable finished */
    BUCKCTRL_REG_CTRL.ENAREQ     = 1U;                  /* enable buck */
}










