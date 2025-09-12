/* ###################################################################
**     Filename    : main.c
**     Project     : CV_3Way_Coolant
**     Processor   : MC9S12ZVL32MLF
**     Version     : Driver 01.14
**     Compiler    : CodeWarrior HCS12Z C Compiler
**     Date/Time   : 2020-03-05, 18:32, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.14
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include <appConfig.h>
#include <systemInit.h>
#include <pdsTask.h>
/* Include shared modules, which are used for whole project */
void main(void)
{
#if ENABLE_FUNCTION_VALIDATION == 1
    validation_main();
#endif
    /* !!!!!!!MUST BE called firstly here for initializing system parameters !!!!*/
    PDS_Init();
    SYS_Init();
	
    /* system main infinite loop */
  for(;;){
    TM_RunTasks();
  }
  
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
