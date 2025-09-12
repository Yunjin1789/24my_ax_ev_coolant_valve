#include <systemInit.h>
#include <rugbyPro.h>
#include <buck_device.h>
#include <pwmAux_device.h>
#include <uart_device.h>
#include <linStackTask.h>

void SYS_Init(void)
{
    /* Init system clock */
    Clock_SystemMainClockInit(SYS_MAIN_CLOCK_DIV);
    /* Init global timer engine for driving soft timer */
    SysTick_Init(SOFT_TIMER_INTERVAL *1000U * MAIN_CPU_CLOCK, SoftTimer_ExpireCallback);
#if WATCH_DOG_EN == 1U
      WDTA_Enable(WDTA_INTERVAL_8S); /* 8s */
#endif
    /* tasks init must be called before use. */
    TM_PostTask(TASK_ID_SOFT_TIMER);
    TM_PostTask(TASK_ID_LINS);
    TM_PostTask(TASK_ID_PDS);
    TM_PostTask(TASK_ID_APPL);
    
}



