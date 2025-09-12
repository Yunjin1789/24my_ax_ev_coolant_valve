#include <appConfig.h>
#include <isr_redirection.h>
#include <crga_sfr.h>
#include "wdt_device.h"
static __INLINE void WDTA_Start(void);

#pragma location = ".bootModeSection"
static volatile IsrSysMode_t currSysMode = SYS_MODE_BOOT;

static pFunction_t jumpToApplication;

void ISR_SetSysMode(IsrSysMode_t mode)
{
    currSysMode = mode;
}

#pragma optimize=none
static __INLINE void WDTA_Start(void)
{
    WDTA_SFRS->CTRL.WDTA_TIMEOUT_SEL     = (uint8_t)WDTA_INTERVAL_8S;
    WDTA_SFRS->CLEAR = 0x3c574ad6U;
    WDTA_SFRS->STOP = 0U;/* start wdt*/
}


void NMI_Handler(void)
{ 
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_NMI_Handler);
        jumpToApplication();
    }else{
        WDTA_Start();
        for(;;){}
    }
}

void HardFault_Handler(void)
{ 
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_HardFault_Handler);
        jumpToApplication();
    }else{
        WDTA_Start();
        for(;;){}
    }
}

void MemManage_Handler(void)
{ 
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_MemManage_Handler);
        jumpToApplication();
    }else{
        WDTA_Start();
        for(;;){}
    }
}

void BusFault_Handler(void)
{ 
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_BusFault_Handler);
        jumpToApplication();
    }else{
        WDTA_Start();
        for(;;){}
    }
}

void UsageFault_Handler(void)
{ 
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_UsageFault_Handler);
        jumpToApplication();
    }else{
        WDTA_Start();
        for(;;){}
    }
}

void SVC_Handler(void)
{ 
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_SVC_Handler);
        jumpToApplication();
    }else{
        WDTA_Start();
        for(;;){}
    }
}

void DebugMon_Handler(void)
{ 
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_DebugMon_Handler);
        jumpToApplication();
    }else{
        WDTA_Start();
        for(;;){}
    }
}

void PendSV_Handler(void) 
{ 
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_PendSV_Handler);
        jumpToApplication();
    }else{
        WDTA_Start();
        for(;;){}
    }
}

void SysTick_Handler(void)
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_SysTick_Handler);
        jumpToApplication();
    }else{
        SysTick_SoftHandler();
    }
}

void WULIN_Handler(void)
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_WULIN_Handler);
        jumpToApplication();
    }
}

void PWMAUX_Handler(void)
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_PWMAUX_Handler);
        jumpToApplication();
    }
}


void WUTIMER_Handler(void)
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_WUTIMER_Handler);
        jumpToApplication();
    }
}

void BOR_Handler(void) 
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_BOR_Handler);
        jumpToApplication();
    }
}

void VBAT_Handler(void)
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_VBAT_Handler);
        jumpToApplication();
    }
}

void OVTEMP_Handler(void)
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_OVTEMP_Handler);
        jumpToApplication();
    }
}

void WatchdogA_Handler(void) 
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_WatchdogA_Handler);
        jumpToApplication();
    }
}

void GPIO_Handler(void) 
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_GPIO_Handler);
        jumpToApplication();
    }
}

void LINS_Handler(void) 
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_LINS_Handler);
        jumpToApplication();
    }else{
        LINS_SoftHandler();
    }
}

void LINM_Handler(void) 
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_LINM_Handler);
        jumpToApplication();
    }
}

void UART_Handler(void)
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_UART_Handler);
        jumpToApplication();
    }
}

void ADC_Handler(void) 
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_ADC_Handler);
        jumpToApplication();
    }
}

void PWM_Handler(void) 
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_PWM_Handler);
        jumpToApplication();
    }
}

void BUCK_Handler(void)
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_BUCK_Handler);
        jumpToApplication();
    }
}

void SPI_Handler(void)
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_SPI_Handler);
        jumpToApplication();
    }
}

void Lullaby_Handler(void)
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_Lullaby_Handler);
        jumpToApplication();
    }
}

void Timer0_Handler(void) 
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_Timer0_Handler);
        jumpToApplication();
    }
}

void Timer1_Handler(void) 
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_Timer1_Handler);
        jumpToApplication();
    }
}


void Timer2_Handler(void) 
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_Timer2_Handler);
        jumpToApplication();
    }
}

void Watchdog_Handler(void) 
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_Watchdog_Handler);
        jumpToApplication();
    }
}

void BTE_Handler(void)
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_BTE_Handler);
        jumpToApplication();
    }
}

void SDIO_Handler(void)
{
    if(currSysMode == SYS_MODE_APPL){
        jumpToApplication = getISR_Address(INDEX_SDIO_Handler);
        jumpToApplication();
    }
}
