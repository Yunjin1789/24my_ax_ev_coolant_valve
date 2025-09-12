#include <pdsTask.h>
#include <crc32.h>
#include <rugbyPro.h>
#include <flash_device.h>
#include <linsNodeCfgIdentify.h>
#include <linStackTask.h>
#include <linSlaveTask.h>

static TaskState_t pdsState = TASK_STATE_INIT;

void PDS_Init(void)
{
    PdsStatus_t pdsStatus = PDS_STATUS_USING_DEFAULT;
    CRC32_Init();
    SystemParams_t *param = (SystemParams_t *)FLASH_START_ADDRESS_SYSTEM_DATA;
    uint32_t crcValue = CRC32_GetRunTimeCRC32((uint8_t *)&param->info ,(uint16_t)sizeof(param->info));
    if (crcValue == param->head.crc32){
        pdsStatus = PDS_STATUS_USING_NORMAL;
    }else{
        param = (SystemParams_t *)FLASH_START_ADDRESS_SYSTEM_DATA_BACK_UP;
        crcValue = CRC32_GetRunTimeCRC32((uint8_t *)&param->info ,(uint16_t)sizeof(param->info));
        if (crcValue == param->head.crc32){
            pdsStatus = PDS_STATUS_USING_BACKUP;
        }
    }
    
    if (pdsStatus == PDS_STATUS_USING_DEFAULT){
        ls_set_nad(INIT_LINS_NAD_ADDR);
    }else{
        ls_set_nad(param->info.linParams.nad);
    }
    ls_set_initialNad(INIT_LINS_NAD_ADDR);
    
}



void PDS_TaskHandler(void)
{
    switch(pdsState){
    case TASK_STATE_INIT:
      pdsState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
      break;
    case TASK_STATE_PROCESSING:
      break;
    default:
      break;
    }
}















