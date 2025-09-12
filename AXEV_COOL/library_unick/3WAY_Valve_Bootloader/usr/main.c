#include <appConfig.h>
#include <systemInit.h>
#include <pdsTask.h>
#include <applicationTask.h>

void main(void)
{
    /* if it's wakeup signal, program should jump to application directly */
    if (PUM_IsWakupSignal() == TRUE){
        APP_JumpToApplication();
    }
    /* !!!!!!!MUST BE called firstly here for initializing system parameters !!!!*/
    PDS_Init();
    /* System init for hardwre init */
    SYS_Init();
    /* system main infinite loop */
    for(;;){
        TM_RunTasks();
    }
}
