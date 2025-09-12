#ifndef _APPLICATION_H
#define _APPLICATION_H
#include <appConfig.h>
#include <softTimerTask.h>
#include <systick_device.h>
#include <taskManager.h>

#include <lin_device.h>
#include <pmu_device.h>
#include <linStackTask.h>

#define BOOT_MODE_MAGIC_KEY_HANDSHAKE                   (0x5A5A55AAU)
#define BOOT_MODE_MAGIC_KEY_ENTER_UPGRADE_MODE          (0xAA55U)
#define BOOT_MODE_MAGIC_KEY_EXIT_UPGRADE_MODE           (0xA5A5A5A5U)
#define BOOT_MODE_MAGIC_ERASE_SYSTEM_MEMORY             (0xF5A5A5F5U)

#define BOOT_MODE_HANDSHAKE                             (0x00U)
#define BOOT_MODE_ENTER_UPGRADE_MODE                    (0x01U)
#define BOOT_MODE_EXIT_UPGRADE_MODE                     (0x02U)
#define BOOT_MODE_ERASE_SYSTEM_MEMORY                   (0x03U)
#define BOOT_MODE_SEND_START_ADDRESS                    (0xE0U)
#define BOOT_MODE_IMAGE_CRC_VALIDATION                  (0xE1U)

typedef enum{
  BOOT_MODE_IDLE = 0U,
  BOOT_MODE_WAITTING,
  BOOT_MODE_ACTIVE,
  BOOT_MODE_PROGRAMMING,
  BOOT_MODE_COMPLETED,
  BOOT_MODE_EXIT,
}BootMode_t;

typedef enum{
  ERROR_CODE_NONE = 0U,
  ERROR_CODE_INVALID_BOOT_MODE,
  ERROR_CODE_INVALID_ADDRESS_DATA_LENGTH,
  ERROR_CODE_INVALID_CRC,
}RspErrorCode_t;

BEGIN_PACK

typedef struct{
    uint8_t  command;
    union{
      struct{/* for Send Flash Start Address CMD */
          uint16_t dataLength;
          uint16_t startAddress;
      };
      struct{ /* for entering upgrade mode CMD*/
          uint16_t key;
          uint16_t imageHeadAddress;
      };
      uint32_t magicKey;/* for exit and handshake CMD*/
    };
}SIDBootMode_t;

typedef struct{
    uint8_t command;
    RspErrorCode_t result;
    uint8_t  dataMinSendInterval;
    uint16_t doImageCRCMinTimeExpense;
}SIDBootModeRsp_t;

typedef struct{
    uint8_t command;
    RspErrorCode_t result;
}SIDBootModeImageCRCRsp_t;

typedef struct{
    uint8_t command;
    RspErrorCode_t result;
}SIDBootModeEraseDataRsp_t;


typedef struct{
    uint8_t command;
    RspErrorCode_t result;
}SIDBootModeExitRsp_t;

typedef struct{
    uint8_t command;
    RspErrorCode_t result;
    uint16_t address;
}SIDBootModeInfoRsp_t;


typedef struct{
    RspErrorCode_t result;
}SIDBootModeSendDataRsp_t;

typedef struct{
    uint16_t datalength;
    uint16_t startAddress;
    uint32_t crcValue;
}SIDBootDataStruct_t;

END_PACK


typedef struct{
    uint16_t valid;                   //2
    uint16_t imageLength;             //2
    uint32_t imageCrc32;              //4
}BootImageHead_t;


void APPL_TaskHandler(void);
/* Response enable  */
uint8_t APPL_HandleBootModeRequest(const DiagReqInfo_t *const diagReq,DiagRspInfo_t *const diagRsp);
uint8_t APPL_HandleBootDataRequest(uint8_t reqSid,uint8_t *const dataBuff,uint16_t datalength,DiagRspInfo_t *const diagRsp);
void APP_JumpToApplication(void);

#endif