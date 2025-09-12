#ifndef _ISR_REDIRECTION_H_
#define _ISR_REDIRECTION_H_

#include <rugbyPro.h>

typedef enum
{
  INDEX_MSP = 0,
  INDEX_PROGRAM_START,
  INDEX_NMI_Handler,
  INDEX_HardFault_Handler,
  INDEX_MemManage_Handler,
  INDEX_BusFault_Handler,
  INDEX_UsageFault_Handler,
  INDEX_RESVD_0,
  INDEX_RESVD_1,
  INDEX_RESVD_2,
  INDEX_RESVD_3,
  INDEX_SVC_Handler,
  INDEX_DebugMon_Handler,
  INDEX_RESVED_4,
  INDEX_PendSV_Handler,
  INDEX_SysTick_Handler,
  
  INDEX_WULIN_Handler,
  INDEX_PWMAUX_Handler,
  INDEX_WUTIMER_Handler,
  INDEX_BOR_Handler,
  INDEX_VBAT_Handler,
  INDEX_OVTEMP_Handler,
  INDEX_WatchdogA_Handler,
  INDEX_GPIO_Handler,
  INDEX_LINS_Handler,
  INDEX_LINM_Handler,
  INDEX_UART_Handler,
  INDEX_ADC_Handler,
  INDEX_PWM_Handler,
  INDEX_BUCK_Handler,
  INDEX_SPI_Handler,
  INDEX_Lullaby_Handler,
  INDEX_Timer0_Handler,			// IRQ 10h Timer0	
  INDEX_Timer1_Handler,			// IRQ 11h Timer1
  INDEX_Timer2_Handler,			// IRQ 12h Timer2
  INDEX_Watchdog_Handler,		// IRQ 13h WDT
  INDEX_BTE_Handler,
  INDEX_SDIO_Handler,
}ISR_Index_t;

typedef void (*pFunction_t)(void);
#define getISR_Address(Index)   (pFunction_t)(*(volatile uint32_t *)(FLASH_START_ADDRESS_APPLICATION + (uint32_t)Index * 4U))

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);


void SysTick_Handler(void);
void WULIN_Handler(void);
void PWMAUX_Handler(void);
void WUTIMER_Handler(void);
void BOR_Handler(void);
void VBAT_Handler(void);
void OVTEMP_Handler(void);
void WatchdogA_Handler(void);
void GPIO_Handler(void);
void LINS_Handler(void);
void LINM_Handler(void);
void UART_Handler(void);
void ADC_Handler(void);
void PWM_Handler(void);
void BUCK_Handler(void);
void SPI_Handler(void);
void Lullaby_Handler(void);
void Timer0_Handler(void);
void Timer1_Handler(void);
void Timer2_Handler(void);
void Watchdog_Handler(void);
void BTE_Handler(void);
void SDIO_Handler(void);



void NMI_SoftHandler(void);
void HardFault_SoftHandler(void);
void MemManage_SoftHandler(void);
void BusFault_SoftHandler(void);
void UsageFault_SoftHandler(void);
void SVC_SoftHandler(void);
void DebugMon_SoftHandler(void);
void PendSV_SoftHandler(void);


void SysTick_SoftHandler(void);
void WULIN_SoftHandler(void);
void PWMAUX_SoftHandler(void);
void WUTIMER_SoftHandler(void);
void BOR_SoftHandler(void);
void VBAT_SoftHandler(void);
void OVTEMP_SoftHandler(void);
void WatchdogA_SoftHandler(void);
void GPIO_SoftHandler(void);
void LINS_SoftHandler(void);
void LINM_SoftHandler(void);
void UART_SoftHandler(void);
void ADC_SoftHandler(void);
void PWM_SoftHandler(void);
void BUCK_SoftHandler(void);
void SPI_SoftHandler(void);
void Lullaby_SoftHandler(void);
void Timer0_SoftHandler(void);
void Timer1_SoftHandler(void);
void Timer2_SoftHandler(void);
void Watchdog_SoftHandler(void);
void BTE_SoftHandler(void);
void SDIO_SoftHandler(void);


typedef enum{
  SYS_MODE_APPL = 0xAAAAAAAAU,
  SYS_MODE_BOOT = 0x55555555U,
}IsrSysMode_t;

void ISR_SetSysMode(IsrSysMode_t mode);

#endif