#ifndef _LINS_TASK_H
#define _LINS_TASK_H

#include <appConfig.h>
#include <lin_device.h>


/**
* @brief A structure to represent LIN frame information.
*/

#define N_AS            (1000)
#define N_CR            (1000)


/*  *****************LIN configuration default value **************************/
#define INIT_LINS_NAD_ADDR              (0x01U)

#define LINS_FUNCTION_ID_SNPD_FORCE_SWITCH_ON     (0xAA00U)
#define LINS_FUNCTION_ID_SNPD_INIT                (0xAA01U)
#define LINS_FUNCTION_ID_SNPD_ASSIGN_NAD          (0xAA02U)
#define LINS_FUNCTION_ID_SNPD_STORAGE             (0xAA03U)
#define LINS_FUNCTION_ID_SNPD_EXIT                (0xAA04U)

#define LINS_SUPPLIER_ID                (0x0123U)
#define LINS_FUNCTION_ID                (0x0123U)
#define LINS_VARIANT                    (0x01U)
#define LINS_SERIAL_NO                  (0x01234567U)
#define LINS_FW_VERSION                 (0x0100U)
#define LINS_HW_VERSION                 (0x0001U)

#define DEFAULT_LINS_FID_SIZE           (0x01U)
#define DEFAULT_LINS_FID0               (0x20U)

#define FID_COLOR_CTRL                  DEFAULT_LINS_FID0


#define LIN_SID_APP_DIAG_BOOT_MODE                      (0x20U)
#define LIN_SID_APP_DIAG_BOOT_SEND_DATA_COUNTENT        (0x21U)

void LINS_TaskHandler(void);
LIN_Device_Frame_t *LINS_GetUcndFramesTable(uint8_t *tableItemsCount);


#endif