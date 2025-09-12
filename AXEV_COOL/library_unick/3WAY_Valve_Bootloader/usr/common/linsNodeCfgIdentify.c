#include <linStackTask.h>
#include <linsNodeCfgIdentify.h>
#include <linSlaveTask.h>
#include <pdsTask.h>

uint8_t LNCI_HandleNodeCfgIdentifyRequest(const DiagReqInfo_t *const diagReq,DiagRspInfo_t *const diagRsp)
{
  uint8_t response = FALSE;
  if (diagReq->sid == LIN_SID_READ_BY_ID && diagReq->packLength == sizeof(ReadByIdReq_t)){                /* Read by Identifier */
      ReadByIdReq_t *readById = (ReadByIdReq_t *)((void*)diagReq->payload);
      if ((readById->supplierID == LINS_SUPPLIER_ID || readById->supplierID == LIN_SUPPLIER_ID_WILDCARD)&&
          (readById->functionID == LINS_FUNCTION_ID || readById->functionID == LIN_FUNCTION_ID_WILDCARD)){
          /*
          4.2.6.1 Read by identifier
          It is possible to read the supplier identity and other properties from a slave node using
          the request in Table 4.18.
          */
          if (readById->identifier == 0U){
              /* LIN Product Identification */
              ReadByIdProductIdRsp_t *rsp = (ReadByIdProductIdRsp_t *)((void *)diagRsp->payload);
              rsp->supplierID = LINS_SUPPLIER_ID;
              rsp->functionID = LINS_FUNCTION_ID;
              rsp->variant = LINS_VARIANT;
              diagRsp->packLength = (uint16_t)sizeof(ReadByIdProductIdRsp_t);
              response = TRUE;
          }else if (readById->identifier == 1U){
              /* Serial number */
              ReadByIdSerialNoRsp_t *rsp = (ReadByIdSerialNoRsp_t *)((void *)diagRsp->payload);
              rsp->serialNo = LINS_SERIAL_NO;
              diagRsp->packLength = (uint16_t)sizeof(ReadByIdSerialNoRsp_t);
              response = TRUE;
          }else if (readById->identifier == 2U){
              /* Serial number */
              ReadByIdVersionRsp_t *rsp = (ReadByIdVersionRsp_t *)((void *)diagRsp->payload);
              rsp->fwVer   = LINS_FW_VERSION;
              rsp->hwVer   = LINS_HW_VERSION;
              rsp->variant = 1U;
              diagRsp->packLength = (uint16_t)sizeof(ReadByIdVersionRsp_t);
              response = TRUE;
          }else{
              /*
              If the slave is not supporting this request or could not process the request it will
              respond according to Table 4.21.
              */
              ReadByIdNakRsp_t *rsp = (ReadByIdNakRsp_t *)((void *)diagRsp->payload);
              diagRsp->sid = 0x7FU;
              rsp->sid = LIN_SID_READ_BY_ID;
              rsp->errorCode = 0x12U;
              diagRsp->packLength = (uint16_t)sizeof(ReadByIdNakRsp_t);
              response = TRUE;
          }
      }
  }
  return response;
}