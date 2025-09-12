/* ###################################################################
**     Filename    : Events.h
**     Project     : CV_3Way_Coolant
**     Processor   : MC9S12ZVL32MLF
**     Component   : Events
**     Version     : Driver 01.04
**     Compiler    : CodeWarrior HCS12Z C Compiler
**     Date/Time   : 2020-03-05, 18:32, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.04
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */


#include "main.h"
#include <stdint.h>
#include <string.h>
#include <clock_device.h>
#include <gpio_device.h>
//#include "SOL_PWM.h"
//#include "SOL_FLT.h"
//#include "TI1.h"
//#include "WDog1.h"
//#include "VN7040_En.h"
//#include "VN7040_SEL0.h"
//#include "VN7040_SEL1.h"

//#pragma CODE_SEG DEFAULT


void MotorControlTime1(void);
/*
** ===================================================================
**     Event       :  MotorControlTime1 (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void MotorControlTime0(void);
/*
** ===================================================================
**     Event       :  MotorControlTime0 (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void MotorControlTask(void);

/*
** ===================================================================
**     Event       :  MotorControlInit (module Events)
**
**     Component   :  AD_Monitor [ADC]
**     Description :
**         This event is called when abort interupt occurs. The event
**         is available only when the <Interrupt service/event>
**         property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MotorControlInit(void);

/* END Events */
#endif /* __Events_H*/

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS12Z series of microcontrollers.
**
** ###################################################################
*/
