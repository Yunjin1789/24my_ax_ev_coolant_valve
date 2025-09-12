#include <applicationTask.h>
#include <pdsTask.h>
#include <linStackTask.h>
#include <crc32.h>
#include <flash_device.h>
#include <isr_redirection.h>

static TaskState_t applState = TASK_STATE_INIT;
void bootTimeoutTimerExpired(SoftTimer_t *timer);
static BootMode_t bootMode = BOOT_MODE_IDLE;

typedef struct{
    uint16_t dataLength;
    uint16_t flashStartAddress;
}FlashParam_t;
FlashParam_t flashParam;

void APP_JumpToApplication(void)
{
    pFunction_t jumpToApplication;
    /* 1. Disable interrupt response.*/
    __disable_irq();
    /* reset all perif */
    CRGA_REG_MODULERSTREQ = 0xFFU; 
    
    ISR_SetSysMode(SYS_MODE_APPL);
    /*  2. Disable all enabled interrupts in NVIC. */
    NVIC->ICER[0] = 0xFFFFFFFFU;
    /* Clear all pending interrupt requests in NVIC.*/
    NVIC->ICPR[0] = 0xFFFFFFFFU;
    /* 4. Disable SysTick and clear its exception pending bit. */
    SysTick->CTRL = 0;
    SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;
    __set_MSP(*(__IO uint32_t*)FLASH_START_ADDRESS_APPLICATION); /* set SP */
    __set_CONTROL( 0 );
    
    jumpToApplication = getISR_Address(INDEX_PROGRAM_START);/* get start address of Application */
    jumpToApplication();
}


static SoftTimer_t bootTimeoutTimer = {
    .mode     = TIMER_ONE_SHOT_MODE,
    .interval = 50U,
    .handler  = bootTimeoutTimerExpired
};


void bootTimeoutTimerExpired(SoftTimer_t *timer)
{
    APP_JumpToApplication();
}

uint8_t isImageValid(void)
{
    uint8_t result = TRUE;
    BootImageHead_t *head = (BootImageHead_t *)(FLASH_START_ADDRESS_APPLICATION - sizeof(BootImageHead_t));
    if (head->imageLength <= IMAGE_MAX_SIZE){
        uint32_t activeImageCRC = CRC32_GetRunTimeCRC32((uint8_t *)FLASH_START_ADDRESS_APPLICATION,head->imageLength);
        if ( activeImageCRC != head->imageCrc32 ){
            result = FALSE;
        }
    }else{
        result = FALSE;
    }
    return result;
}


void APPL_TaskHandler(void)
{
    switch(applState){
    case TASK_STATE_INIT:
      if( (PUM_IsAppEnterBootModeCmd() != TRUE) && (isImageValid() == TRUE) ){
          SoftTimer_Start(&bootTimeoutTimer);
      }
      applState = TASK_STATE_ACTIVE;
      break;
    default:
      break;
    }
}

uint8_t IsValidAddressAndLengthRange(uint16_t startAddress, uint16_t length)
{
    uint8_t result = TRUE;
    if ((startAddress - ((startAddress >> 2U) << 2U)) != 0U){   /* 4bytes aligned? */
        result = FALSE;
    }else{
        if ((startAddress < (FLASH_START_ADDRESS_APPLICATION - sizeof(BootImageHead_t)) ) || (((uint32_t)startAddress + length) >= FLASH_START_ADDRESS_USER_DATA)){
            result = FALSE;
        }
    }
    return result;
}

uint8_t IsInSameSector(uint16_t startAddress, uint16_t length)
{
    uint8_t result = TRUE;
    uint16_t blockLength;
    blockLength = startAddress - ((startAddress >> FLASH_SECTOR_SIZE_POS) << FLASH_SECTOR_SIZE_POS) + length;
    
    if (blockLength > FLASH_SECTOR_SIZE){
        result = FALSE;
    }
    return result;
}

uint8_t IsStartOfSector(uint16_t startAddress)
{
    uint8_t result = TRUE;
    uint16_t startSectorIndex = startAddress >> FLASH_SECTOR_SIZE_POS;
    if ((startAddress - (startSectorIndex << FLASH_SECTOR_SIZE_POS)) != 0U){
        result = FALSE;
    }
    return result;
}

uint8_t blockIsFormatted(uint32_t address)
{
    uint8_t result = TRUE;
    for (uint16_t i = 0U; i < (FLASH_SECTOR_SIZE/4); i++){
        if ( (*(__IO   uint32_t *)(address + i*4U)) != 0xFFFFFFFFU){
            result = FALSE;
            break;
        }
    }
    return result;
}


uint8_t APPL_HandleBootModeRequest(const DiagReqInfo_t *const diagReq,DiagRspInfo_t *const diagRsp)
{
    uint8_t response = FALSE;
    SIDBootMode_t *const info       = (SIDBootMode_t *)((void *)diagReq->payload);
    SIDBootModeInfoRsp_t *const infoRsp = (SIDBootModeInfoRsp_t *)((void *)diagRsp->payload);
    
    diagRsp->sid = diagReq->sid;
    infoRsp->command = info->command;
    if ( (info->command == BOOT_MODE_HANDSHAKE) && (info->magicKey == BOOT_MODE_MAGIC_KEY_HANDSHAKE)){
        SoftTimer_Stop(&bootTimeoutTimer);
        bootMode = BOOT_MODE_WAITTING;
        SIDBootModeRsp_t *const BootModeRsp = (SIDBootModeRsp_t *)((void *)diagRsp->payload);
        BootModeRsp->command = info->command;
        BootModeRsp->result = ERROR_CODE_NONE;
        BootModeRsp->dataMinSendInterval = 20U;
        BootModeRsp->doImageCRCMinTimeExpense = 150U;
        diagRsp->packLength = (uint16_t)sizeof(SIDBootModeRsp_t);
        diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
        response = TRUE;/* response enable  */
    }else if ( (info->command == BOOT_MODE_ERASE_SYSTEM_MEMORY) && (info->magicKey == BOOT_MODE_MAGIC_ERASE_SYSTEM_MEMORY)){
        if ( bootMode >= BOOT_MODE_WAITTING ){
            if (blockIsFormatted(FLASH_START_ADDRESS_SYSTEM_DATA) != TRUE){
                Flash_EraseSector(FLASH_START_ADDRESS_SYSTEM_DATA);
            }
            if (blockIsFormatted(FLASH_START_ADDRESS_SYSTEM_DATA_BACK_UP) != TRUE){
                Flash_EraseSector(FLASH_START_ADDRESS_SYSTEM_DATA_BACK_UP);
            }
            infoRsp->result = ERROR_CODE_NONE;
        }else{
            infoRsp->result = ERROR_CODE_INVALID_BOOT_MODE;
        }
        diagRsp->packLength = (uint16_t)sizeof(SIDBootModeEraseDataRsp_t);
        diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
        response = TRUE;/* response enable  */
    }else if ( (info->command == BOOT_MODE_ENTER_UPGRADE_MODE) && (info->key == BOOT_MODE_MAGIC_KEY_ENTER_UPGRADE_MODE)){
        if ( (bootMode >= BOOT_MODE_WAITTING) && (info->imageHeadAddress == (FLASH_START_ADDRESS_APPLICATION - sizeof(BootImageHead_t)) ) ){
            bootMode = BOOT_MODE_ACTIVE;
            infoRsp->result = ERROR_CODE_NONE;
            flashParam.flashStartAddress = info->imageHeadAddress;
        }else{
            infoRsp->result = ERROR_CODE_INVALID_BOOT_MODE;
        }
        infoRsp->address = FLASH_START_ADDRESS_APPLICATION - sizeof(BootImageHead_t);
        diagRsp->packLength = (uint16_t)sizeof(SIDBootModeInfoRsp_t);
        diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
        response = TRUE;/* response enable  */
    }else if ( info->command == BOOT_MODE_SEND_START_ADDRESS){
        if (bootMode == BOOT_MODE_ACTIVE){
            if ((info->startAddress == flashParam.flashStartAddress) && (IsValidAddressAndLengthRange(info->startAddress, info->dataLength) == TRUE) && (IsInSameSector(info->startAddress, info->dataLength) == TRUE)){
                if (IsStartOfSector(info->startAddress) == TRUE){
                    Flash_EraseSector((uint32_t)info->startAddress);
                }
                flashParam.dataLength = info->dataLength;
                infoRsp->result = ERROR_CODE_NONE;
            }else{
                infoRsp->result = ERROR_CODE_INVALID_ADDRESS_DATA_LENGTH;
            }
        }else{
            infoRsp->result = ERROR_CODE_INVALID_ADDRESS_DATA_LENGTH;
        }
        infoRsp->address = flashParam.flashStartAddress;
        diagRsp->packLength = (uint16_t)sizeof(SIDBootModeInfoRsp_t);
        diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
        response = TRUE;/* response enable  */
    }
    else if ( info->command == BOOT_MODE_IMAGE_CRC_VALIDATION ){
        if (bootMode == BOOT_MODE_ACTIVE || bootMode == BOOT_MODE_COMPLETED){
            BootImageHead_t *head = (BootImageHead_t *)(FLASH_START_ADDRESS_APPLICATION - sizeof(BootImageHead_t));
            uint32_t activeImageCRC = CRC32_GetRunTimeCRC32((uint8_t *)FLASH_START_ADDRESS_APPLICATION ,head->imageLength);
            if ( (activeImageCRC == info->magicKey) && (activeImageCRC == head->imageCrc32) ){
                bootMode = BOOT_MODE_COMPLETED;
                infoRsp->result = ERROR_CODE_NONE;
            }else{
                bootMode = BOOT_MODE_ACTIVE;
                infoRsp->result = ERROR_CODE_INVALID_CRC;
            }
        }else{
            infoRsp->result = ERROR_CODE_INVALID_BOOT_MODE;
        }
        diagRsp->packLength = (uint16_t)sizeof(SIDBootModeImageCRCRsp_t);
        diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
        response = TRUE;/* response enable  */
    }else if ( (info->command == BOOT_MODE_EXIT_UPGRADE_MODE) && (info->magicKey == BOOT_MODE_MAGIC_KEY_EXIT_UPGRADE_MODE)){
        if ( bootMode == BOOT_MODE_COMPLETED ){
            SoftTimer_Stop(&bootTimeoutTimer);
            SoftTimer_Start(&bootTimeoutTimer);
            infoRsp->result = ERROR_CODE_NONE;
        }else{
            infoRsp->result = ERROR_CODE_INVALID_BOOT_MODE;
        }
        diagRsp->packLength = (uint16_t)sizeof(SIDBootModeExitRsp_t);
        diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
        response = TRUE;/* response enable  */
    }else{
        /*  reserved  */
    }
    return response;
}

uint8_t APPL_HandleBootDataRequest(uint8_t reqSid,uint8_t *const dataBuff,uint16_t datalength,DiagRspInfo_t *const diagRsp)
{
    uint8_t response = FALSE;
    uint8_t wordNum;
    SIDBootDataStruct_t *const info           = (SIDBootDataStruct_t *)((void *)dataBuff);
    SIDBootModeSendDataRsp_t *const infoRsp   = (SIDBootModeSendDataRsp_t *)((void *)diagRsp->payload);
    diagRsp->sid = reqSid;
    if (bootMode == BOOT_MODE_ACTIVE){
        if ( (flashParam.dataLength == (datalength - sizeof(SIDBootDataStruct_t) )) && (flashParam.dataLength == info->datalength) && (info->startAddress == flashParam.flashStartAddress)){
            uint32_t activeImageCRC = CRC32_GetRunTimeCRC32(dataBuff + 8U,flashParam.dataLength);
            if (activeImageCRC == info->crcValue){
                  uint32_t *wordBuff = (uint32_t *)(dataBuff + sizeof(SIDBootDataStruct_t));
                  wordNum = (flashParam.dataLength >> 2);
                  wordNum = wordNum + (uint8_t)((flashParam.dataLength - ( wordNum << 2)) != 0U);
                  for (uint16_t i = 0; i < wordNum; i++){
                      Flash_WriteWord( info->startAddress + i*4U,wordBuff[i]);
                  }
                  flashParam.flashStartAddress = info->startAddress + info->datalength;
                  flashParam.dataLength = 0U;
                  infoRsp->result = ERROR_CODE_NONE;
            }else{
                  infoRsp->result = ERROR_CODE_INVALID_CRC;
            }
        }else{
            infoRsp->result = ERROR_CODE_INVALID_ADDRESS_DATA_LENGTH;
        }
    }else{
        infoRsp->result = ERROR_CODE_INVALID_BOOT_MODE;
    }
    diagRsp->packLength = (uint16_t)sizeof(SIDBootModeSendDataRsp_t);
    diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
    response = TRUE;/* response enable  */
    return response;
}


