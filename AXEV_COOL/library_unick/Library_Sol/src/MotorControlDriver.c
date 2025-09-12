/* ###################################################################
**     Filename    : Events.c
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
** @file Events.c
** @version 01.04
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */


//#include "Cpu.h"
#include <main.h>
#include <MotorControlDriver.h>
#include <MotorControlApi.h>
#include <Diagnosis.h>
#include "timer_device.h"
#include "softTimerTask.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
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
int8_t idx = 0;
extern uint64_t PWM_Duty_Set;
uint64_t tick;

static TaskState_t applState = TASK_STATE_INIT;
void MotorExpired(SoftTimer_t *timer);

static SoftTimer_t ApplTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 1U,
    .handler  = MotorExpired

};

void MotorExpired(SoftTimer_t *timer)
{
    TM_PostTask(TASK_ID_APPL);
}

void MotorControlTime1(void)
{
  /* Write your code here ... */
  #if PWM_TO_GPIO == 0U  
 // 655365; 
  tick++;
  if(PWM_Duty_Set)
  {

  }

  switch(idx)
  {
     case 0:
      idx = 1;
      //GPIO_Set(GPIO5_SOL_PWM, GPIO_LOW);     
     break;
     case 1:
      //GPIO_Set(GPIO5_SOL_PWM, GPIO_HIGH);     
      idx = 0;     
     default:
     break;
  }  
#endif	
}


/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
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
void MotorControlTime0(void)
{
  /* Write your code here ... */    
	DiagnosisSlicsingTask(1);
	ValveControlSlicsingTask(100);
}



#if 1
uint32_t test_sol = 1;
uint32_t test_pwm_test = 5000;
#endif

void MotorControlInit(void)
{
    //Adc_Init();	
    /* Disable trim revise access until reset*/
    /*HWCFG_TrimAccessLockUntilReset();*/
    //400HZ
    TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, (1000U * 16U), MotorControlTime0);    //???600us/????y??????1us,  
#if PWM_TO_GPIO == 0U      
    TIMER_Init(TIMER1, HW_TIMER_PERIODIC_MODE, (1000U * 16U), MotorControlTime1);    //???600us/????y??????1us,  
#endif

}

void MotorControlTask(void)
{
    switch(applState){
    case TASK_STATE_INIT:
      //SoftTimer_Start(&ApplTimer);
      MotorControlInit();
      applState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
    DiagnosisManager();    
#if 0
    if(test_sol != test_pwm_test)
    {
        test_sol = test_pwm_test;
        Motor_PWM_SetRatio16(test_pwm_test);        
    }  
#else    
    ValveControlManager();
#endif        
      break;
    default:
      break;
    }
}

/* END Events */

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
