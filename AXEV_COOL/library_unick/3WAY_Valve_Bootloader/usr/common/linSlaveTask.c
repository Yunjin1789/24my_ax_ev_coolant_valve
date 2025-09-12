#include <linSlaveTask.h>
#include <linStackTask.h>
#include <applicationTask.h>
#include <linsNodeCfgIdentify.h>
#include <softTimerTask.h>
#include <taskManager.h>
#include <isr_redirection.h>
#include <pdsTask.h>

#define DIAG_DATA_BUFF_SIZE     (540U)
#define FID_COLOR_CTRL_INDEX    (0)
/* ******************internal function declarations****************************/
void DiagnosticSubscribedCmdsHandle(const DiagReqInfo_t * const frameInfo);

/* *******************global variants declarations*****************************/
static TaskState_t        linsTaskState = TASK_STATE_INIT;
static uint8_t diagDataRspBuff[32U];
//static uint8_t diagDataRecvBuff[DIAG_DATA_BUFF_SIZE];

static ls_LinsFramesCallback_t linsFramesCallback ={
  NULL,                                         /* received data from master */
  NULL,                                         /* send data to master, it's an interrupt function, please fill the data as fast as possible */
  NULL,                                         /* send data to master finished, it's an interrupt function, please fill the data as fast as possible  */
  DiagnosticSubscribedCmdsHandle,               /* Diagnostic sigle PDU and MultiPDU received data from master */
  NULL,                                         /* special functional NAD (0x7E) handler*/
  NULL,                                         /* Diagnostic sleep request from master */
  NULL,                                         /* Get the bus wake up result this would take effects after call API: ls_send_wake_up_bus_signal()*/
  NULL,                                         /* SAE J2602 -1 Configuration Messages */
  NULL,                                         /*  5.7.2.5 DNN Based Broadcast Messages */
};

/* **FID table declarations****/
static LIN_Device_Frame_t UnconditionalCmdsTable[DEFAULT_LINS_FID_SIZE] = {
  [FID_COLOR_CTRL_INDEX] = {
      .frame_id = FID_COLOR_CTRL,
      .msg_type = LIN_MSG_TYPE_RX,
      .checksum = LIN_CHECKSUM_ENHANCED,
      .length   = LIN_BUFF_SIZE,
      .frameIsValid = TRUE,
  },
};


/* please don't do any changes for it would be used by lin stack */
LIN_Device_Frame_t *LINS_GetUcndFramesTable(uint8_t *tableItemsCount)
{
    *tableItemsCount = (uint8_t)(sizeof(UnconditionalCmdsTable)/sizeof(LIN_Device_Frame_t));
    return UnconditionalCmdsTable;
}

/* 
  Diagnostic subscribed frame received from LIN master
*/
void DiagnosticSubscribedCmdsHandle(const DiagReqInfo_t *const diagReq)
{
    DiagRspInfo_t diagRsp={
      .sid      = diagReq->sid,
      .type     = diagReq->type,
      .payload  = diagDataRspBuff,
    };
    if (diagReq->type == PDU_TYPE_SINGLE_PDU){
        if (diagReq->sid >= LIN_SID_ASSIGN_NAD && diagReq->sid <= LIN_SID_ASSIGN_FRAME_ID_RANGE){
            /* handle Node configuration and Identification commands*/
            diagRsp.type = PDU_TYPE_SINGLE_PDU;
            if (LNCI_HandleNodeCfgIdentifyRequest(diagReq,&diagRsp)== TRUE){
                ls_handle_diagnostic_response(&diagRsp);
            }
        }
//        }else if (diagReq->sid == LIN_SID_APP_DIAG_BOOT_MODE){
            /*  prepare diagnostic single pdu, user defined diagnostic frame, here is for LED param configuration*/
//            if(APPL_HandleBootModeRequest(diagReq,&diagRsp) == TRUE){
//                ls_handle_diagnostic_response(&diagRsp);
//            }
//        }
    }
//    else{
        /*  handle diagnostic multi pdu received sigle pdu response, user defined diagnostic frame here   */
//        if (diagReq->sid == LIN_SID_APP_DIAG_BOOT_SEND_DATA_COUNTENT){
//            for (uint16_t i = 0; i < diagReq->frameDataLength; i++){
//                diagDataRecvBuff[i+diagReq->packCurrIndex] = diagReq->payload[i];
//            }
            /* Package received finished */
//            if ((diagReq->frameDataLength + diagReq->packCurrIndex)>=diagReq->packLength){
                /* handle finished data here  */
//                if ( APPL_HandleBootDataRequest(diagReq->sid,diagDataRecvBuff,diagReq->packLength,&diagRsp) == TRUE){
//                    ls_handle_diagnostic_response(&diagRsp);  
//                }
//            }
//        }
//    }
}


void LINS_TaskHandler(void)
{
    switch(linsTaskState){
    case TASK_STATE_ACTIVE:
      break;
    case TASK_STATE_INIT:
      ls_register_services(LIN_PROTOCOL_LIN2_2A,UnconditionalCmdsTable, (l_u8)(sizeof(UnconditionalCmdsTable)/sizeof(LIN_Device_Frame_t)), DIAG_DATA_BUFF_SIZE,&linsFramesCallback);
      (void)ls_set_tp_timeout(N_AS, N_CR);
      /* Note that please don't change the following glitch configuration!!!!*/
      (void)ls_set_lins_rx_glitch_filter_1st(0x00U,0x08U);
      (void)ls_set_lins_rx_glitch_filter_2nd(0x0AU,0x10U);
      (void)ls_set_lins_rx_glitch_filter_3rd(0x30U,0x30U);

      (void)l_sys_init();
      linsTaskState = TASK_STATE_ACTIVE;
      break;
    default:
      break;
    }
}