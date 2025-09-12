/**
 * @copyright 2020 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 * @file taskManager.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2018/09/20
 */

#include <taskManager.h>
#include <SoftTimerTask.h>
#include <linslaveTask.h>
#include <pdsTask.h>
#include <linStackTask.h>
#include <measureTask.h>
#include <wdt_device.h>
#include <app_motordriver.h>
#include <main.h>

static volatile uint16_t taskFlags = 0U;
static uint16_t tasksMask = 0xFFFFU;

typedef void (*taskHandler_t)(void);

static taskHandler_t taskHandlers[9] = {
    [TASK_ID_SOFT_TIMER]        = SoftTimer_TaskHandler,/* Global Timer task */
    [TASK_ID_LIN_STACK]         = LinStack_TaskHandler, /*  LIN stack task   */
    [TASK_ID_LINS]              = LINS_TaskHandler,     /*  LIN Slave task   */
    [TASK_ID_PDS]               = PDS_TaskHandler,      /*!< Task ID of persistance data storage*/
#if LIN_MASTER_EN == 1U
    [TASK_ID_LINM]              = LINM_TaskHandler,     /*  LIN Master task */
#endif
    [TASK_ID_ADC_MEASURE]       = MES_TaskHandler,      /* ADC measurement, chip temperature, Led PN volt, Battery Volt etc. */
    [TASK_ID_APPL]              = MotorControlTask,     /*  design for debug and demo    */
};

static void IdleTask(void)
{
    
  
}

void TM_RunTasks(void)
{
    uint8_t taskId;
    for (taskId = 0U; taskId < (uint8_t)(sizeof(taskHandlers) / sizeof(taskHandlers[0])); taskId++){
        if ( (((uint16_t)1UL << taskId) & (taskFlags & tasksMask)) != 0U ){
            __atomic_enter()
                taskFlags &= ~((uint16_t)1UL << taskId);
            __atomic_exit()
            if (taskHandlers[taskId] != NULL){
                taskHandlers[taskId]();
            }
            break;
        }
    }
    
#if WATCH_DOG_EN == 1U
    WDTA_Clear(); /* Feeding Watch dog */
#endif
    
    if (taskFlags == 0U){
        IdleTask();
    }
    MotorControlTask();
}

/*
 *  Disables specified task.
 *  @param [in] taskId - task to be disabled.
 *  @return none
 */
void TM_DisableTask(TM_TaskId_t taskId)
{
    tasksMask &= ~(uint16_t)(1UL << (unsigned int)taskId);
}

/*
 *  brief Enables specified task.
 *  @param [in] taskId - task to be enabled.
 *  @return none
 */
void TM_EnableTask(TM_TaskId_t taskId)
{
    tasksMask |= (uint16_t)(1UL << (unsigned int)taskId);
}

/*
 *  brief runs specified task.
 *  @param [in] taskId - task to be run.
 *  @return none
 */
void TM_PostTask(TM_TaskId_t taskId)
{
    __atomic_enter()
      taskFlags |= (uint16_t)(1UL << (unsigned int)taskId);
    __atomic_exit()
}

