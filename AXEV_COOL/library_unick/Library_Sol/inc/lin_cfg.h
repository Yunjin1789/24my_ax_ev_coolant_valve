/****************************************************************************** 
* 
* Freescale Semiconductor Inc. 
* (c) Copyright 2018-2019 Freescale Semiconductor, Inc. 
* ALL RIGHTS RESERVED. 
* 
****************************************************************************//*!  
* 
* @file      lin_cfg.h 
* 
* @author    FPT Software
*  
* @version   1.0 
*  
* @date      Wed Jul 10 14:35:10 KST 2019
*  
* @brief     Hardware configuration file
* 
******************************************************************************/
#ifndef    _LIN_CFG_H_  
#define    _LIN_CFG_H_  
#include "lin_hw_cfg.h" 
#include "linstackTask.h"
/* Define operating mode */
#define _MASTER_MODE_     0 
#define _SLAVE_MODE_      1 
#define LIN_MODE   _SLAVE_MODE_
/* Define protocol version */
#define PROTOCOL_21       0  
#define PROTOCOL_J2602    1  
#define PROTOCOL_20       2 
#define LIN_PROTOCOL    PROTOCOL_21

#define SCI_ADDR        SCI0_ADDR    /* For slave */ 

#define LIN_BAUD_RATE    19200    	 /*For slave*/ 
/**********************************************************************/
/***************          Diagnostic class selection  *****************/
/**********************************************************************/
#define _DIAG_CLASS_I_          0
#define _DIAG_CLASS_II_         1
#define _DIAG_CLASS_III_        2

#define _DIAG_CLASS_SUPPORT_    _DIAG_CLASS_I_
/**************** FRAME SUPPORT DEFINITION ******************/
#define _TL_SINGLE_FRAME_       0
#define _TL_MULTI_FRAME_        1

#define _TL_FRAME_SUPPORT_      _TL_SINGLE_FRAME_

/* frame buffer size */
#define LIN_FRAME_BUF_SIZE			24
#define LIN_FLAG_BUF_SIZE			4

/**********************************************************************/
/***************               Interfaces           *******************/
/**********************************************************************/
//typedef enum { 
//   LIN_Valve_ND
//}l_ifc_handle; 

/**********************************************************************/
/***************               Signals              *******************/
/**********************************************************************/
/* Number of signals */
#define LIN_NUM_OF_SIGS  17
/* List of signals */   
typedef enum {

   /* Interface_name = LIN_Valve_ND */

   LIN_Valve_ND_CR_Bms_cmdAngle_Valve

   , LIN_Valve_ND_CR_Bms_Angle_Valve_FB
  
   , LIN_Valve_ND_CR_Valve1_Res1_HeaderID
  
   , LIN_Valve_ND_CR_Valve1_Status
  
   , LIN_Valve_ND_CR_Valve1_AuxVolt
  
   , LIN_Valve_ND_CR_Valve1_OperAngle_FB
  
   , LIN_Valve_ND_CR_Valve_BmsCmdAngle
  
   , LIN_Valve_ND_CR_Valve1_Res2_HeaderID
  
   , LIN_Valve_ND_CR_Valve1_HWversion
  
   , LIN_Valve_ND_CR_Valve1_SWversion
  
   , LIN_Valve_ND_CF_Valve1_LowVoltageFail
  
   , LIN_Valve_ND_CF_Valve1_OverVoltageFail
  
   , LIN_Valve_ND_CF_Valve1_HallsensorFail
  
   , LIN_Valve_ND_CF_Valve1_LINCommFail
  
   , LIN_Valve_ND_CR_Bms_Req_HeaderID
  
   , LIN_Valve_ND_CF_Valve_Acting_now
  
   , LIN_Valve_ND_CR_Valve_Status_FB
  
  
} l_signal_handle; 
/**********************************************************************/
/*****************               Frame             ********************/
/**********************************************************************/
/* Number of frames */
#define LIN_NUM_OF_FRMS  5 
/* List of frames */
typedef enum {
/* All frames for master node */

   /* Interface_name = LIN_Valve_ND */

   LIN_Valve_ND_Valve1_Batt_Res1

   , LIN_Valve_ND_Valve1_Batt_Res2
  
   , LIN_Valve_ND_BMS_Act_Req_Valve1
  
   , LIN_Valve_ND_MasterReq
  
   , LIN_Valve_ND_SlaveResp
  
  
} l_frame_handle; 
 
/**********************************************************************/
/***************             Configuration          *******************/
/**********************************************************************/
/* Size of configuration in ROM and RAM used for interface: LI1 */
#define LIN_SIZE_OF_CFG  7 
#define LIN_CFG_FRAME_NUM  3 
/*********************************************************************
 * global macros
 *********************************************************************/
#define l_bool_rd(SIGNAL) l_bool_rd_##SIGNAL()
#define l_bool_wr(SIGNAL, A) l_bool_wr_##SIGNAL(A)
#define l_u8_rd(SIGNAL) l_u8_rd_##SIGNAL()
#define l_u8_wr(SIGNAL, A) l_u8_wr_##SIGNAL(A)
#define l_u16_rd(SIGNAL) l_u16_rd_##SIGNAL()
#define l_u16_wr(SIGNAL, A) l_u16_wr_##SIGNAL(A)
#define l_bytes_rd(SIGNAL, start, count, data)  l_bytes_rd_##SIGNAL(start, count, data)
#define l_bytes_wr(SIGNAL, start, count, data) l_bytes_wr_##SIGNAL(start, count, data)
#define l_flg_tst(FLAG) l_flg_tst_##FLAG()
#define l_flg_clr(FLAG) l_flg_clr_##FLAG()
#define LIN_TEST_BIT(A,B) ((l_bool)((((A) & (1U << (B))) != 0U) ? 1U : 0U))
#define LIN_SET_BIT(A,B)                      ((A) |= (l_u8) (1U << (B)))
#define LIN_CLEAR_BIT(A,B)               ((A) &= ((l_u8) (~(1U << (B)))))
#define LIN_BYTE_MASK  ((l_u16)(((l_u16)((l_u16)1 << CHAR_BIT)) - (l_u16)1))
#define LIN_FRAME_LEN_MAX                                             10U

/* Returns the low byte of the 32-bit value    */
#define BYTE_0(n)                              ((l_u8)((n) & (l_u8)0xFF))
/* Returns the second byte of the 32-bit value */
#define BYTE_1(n)                        ((l_u8)(BYTE_0((n) >> (l_u8)8)))
/* Returns the third byte of the 32-bit value  */
#define BYTE_2(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)16)))
/* Returns high byte of the 32-bit value       */
#define BYTE_3(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)24)))

/*
 * defines for signal access
 */


#define LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve    17
#define LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve    0
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Bms_cmdAngle_Valve    8
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve    2
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve    2
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve    2

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB    18
#define LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB    0
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Bms_Angle_Valve_FB    8
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB    2
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB    3
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB    2

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID    0
#define LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID    0
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_Res1_HeaderID    8
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID    0
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID    1
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID    0

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Status    1
#define LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Status    0
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_Status    3
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Status    0
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Status    2
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Status    0

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt    2
#define LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt    0
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_AuxVolt    8
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt    0
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt    4
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt    0

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB    4
#define LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB    0
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_OperAngle_FB    8
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB    0
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB    6
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB    0

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle    3
#define LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle    0
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve_BmsCmdAngle    8
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle    0
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle    5
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle    0

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID    8
#define LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID    0
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_Res2_HeaderID    8
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID    1
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID    1
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID    1

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion    9
#define LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion    0
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_HWversion    4
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion    1
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion    2
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion    1

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion    9
#define LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion    4
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_SWversion    4
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion    1
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion    3
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion    1

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail    10
#define LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail    0
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CF_Valve1_LowVoltageFail    1
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail    1
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail    4
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail    1

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail    10
#define LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail    1
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CF_Valve1_OverVoltageFail    1
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail    1
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail    5
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail    1

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail    10
#define LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail    2
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CF_Valve1_HallsensorFail    1
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail    1
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail    6
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail    1

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail    10
#define LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail    6
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CF_Valve1_LINCommFail    1
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail    1
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail    7
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail    1

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID    16
#define LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID    0
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Bms_Req_HeaderID    8
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID    2
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID    1
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID    2

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now    1
#define LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now    4
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CF_Valve_Acting_now    1
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now    0
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now    3
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now    0

#define LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB    22
#define LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB    0
#define LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve_Status_FB    3
#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB    2
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB    4
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB    2




#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_Valve1_Batt_Res1             0
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_Valve1_Batt_Res1              0

#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_Valve1_Batt_Res2             1
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_Valve1_Batt_Res2              0

#define LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_BMS_Act_Req_Valve1             2
#define LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_BMS_Act_Req_Valve1              0


/**********************************************************************/
/***************        Static API Functions        *******************/
/**********************************************************************/
/*
 * the static signal access macros
 */


/* static access macros for signal LIN_Valve_ND_CR_Bms_cmdAngle_Valve */
#define l_u8_rd_LIN_Valve_ND_CR_Bms_cmdAngle_Valve() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve] & \
    (((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Bms_cmdAngle_Valve) - 1) \
    << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve )) >> LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve))
#define l_u8_wr_LIN_Valve_ND_CR_Bms_cmdAngle_Valve(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Bms_cmdAngle_Valve) - 1) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Bms_cmdAngle_Valve) - 1) & (A)) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve] = 1;}

/* static access macros for signal LIN_Valve_ND_CR_Bms_Angle_Valve_FB */
#define l_u8_rd_LIN_Valve_ND_CR_Bms_Angle_Valve_FB() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB] & \
    (((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Bms_Angle_Valve_FB) - 1) \
    << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB )) >> LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB))
#define l_u8_wr_LIN_Valve_ND_CR_Bms_Angle_Valve_FB(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Bms_Angle_Valve_FB) - 1) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Bms_Angle_Valve_FB) - 1) & (A)) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB] = 1;}

/* static access macros for signal LIN_Valve_ND_CR_Valve1_Res1_HeaderID */
#define l_u8_rd_LIN_Valve_ND_CR_Valve1_Res1_HeaderID() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID] & \
    (((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_Res1_HeaderID) - 1) \
    << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID )) >> LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID))
#define l_u8_wr_LIN_Valve_ND_CR_Valve1_Res1_HeaderID(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_Res1_HeaderID) - 1) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_Res1_HeaderID) - 1) & (A)) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID] = 1;}

/* static access macros for signal LIN_Valve_ND_CR_Valve1_Status */
#define l_u8_rd_LIN_Valve_ND_CR_Valve1_Status() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Status] & \
    (((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_Status) - 1) \
    << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Status )) >> LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Status))
#define l_u8_wr_LIN_Valve_ND_CR_Valve1_Status(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Status] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Status] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_Status) - 1) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Status)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_Status) - 1) & (A)) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Status))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Status] = 1;}

/* static access macros for signal LIN_Valve_ND_CR_Valve1_AuxVolt */
#define l_u8_rd_LIN_Valve_ND_CR_Valve1_AuxVolt() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt] & \
    (((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_AuxVolt) - 1) \
    << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt )) >> LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt))
#define l_u8_wr_LIN_Valve_ND_CR_Valve1_AuxVolt(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_AuxVolt) - 1) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_AuxVolt) - 1) & (A)) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt] = 1;}

/* static access macros for signal LIN_Valve_ND_CR_Valve1_OperAngle_FB */
#define l_u8_rd_LIN_Valve_ND_CR_Valve1_OperAngle_FB() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB] & \
    (((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_OperAngle_FB) - 1) \
    << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB )) >> LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB))
#define l_u8_wr_LIN_Valve_ND_CR_Valve1_OperAngle_FB(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_OperAngle_FB) - 1) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_OperAngle_FB) - 1) & (A)) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB] = 1;}

/* static access macros for signal LIN_Valve_ND_CR_Valve_BmsCmdAngle */
#define l_u8_rd_LIN_Valve_ND_CR_Valve_BmsCmdAngle() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle] & \
    (((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve_BmsCmdAngle) - 1) \
    << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle )) >> LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle))
#define l_u8_wr_LIN_Valve_ND_CR_Valve_BmsCmdAngle(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve_BmsCmdAngle) - 1) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve_BmsCmdAngle) - 1) & (A)) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle] = 1;}

/* static access macros for signal LIN_Valve_ND_CR_Valve1_Res2_HeaderID */
#define l_u8_rd_LIN_Valve_ND_CR_Valve1_Res2_HeaderID() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID] & \
    (((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_Res2_HeaderID) - 1) \
    << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID )) >> LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID))
#define l_u8_wr_LIN_Valve_ND_CR_Valve1_Res2_HeaderID(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_Res2_HeaderID) - 1) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_Res2_HeaderID) - 1) & (A)) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID] = 1;}

/* static access macros for signal LIN_Valve_ND_CR_Valve1_HWversion */
#define l_u8_rd_LIN_Valve_ND_CR_Valve1_HWversion() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion] & \
    (((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_HWversion) - 1) \
    << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion )) >> LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion))
#define l_u8_wr_LIN_Valve_ND_CR_Valve1_HWversion(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_HWversion) - 1) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_HWversion) - 1) & (A)) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion] = 1;}

/* static access macros for signal LIN_Valve_ND_CR_Valve1_SWversion */
#define l_u8_rd_LIN_Valve_ND_CR_Valve1_SWversion() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion] & \
    (((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_SWversion) - 1) \
    << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion )) >> LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion))
#define l_u8_wr_LIN_Valve_ND_CR_Valve1_SWversion(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_SWversion) - 1) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve1_SWversion) - 1) & (A)) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion] = 1;}

/* static access macros for signal LIN_Valve_ND_CF_Valve1_LowVoltageFail */
#define l_bool_rd_LIN_Valve_ND_CF_Valve1_LowVoltageFail() \
   	(LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail], \
   	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail))
#define l_bool_wr_LIN_Valve_ND_CF_Valve1_LowVoltageFail(A) \
	{(A) ? \
  	(LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail], \
  	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail)):\
  	(LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail], \
  	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail));\
  	lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail] = 1;}

/* static access macros for signal LIN_Valve_ND_CF_Valve1_OverVoltageFail */
#define l_bool_rd_LIN_Valve_ND_CF_Valve1_OverVoltageFail() \
   	(LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail], \
   	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail))
#define l_bool_wr_LIN_Valve_ND_CF_Valve1_OverVoltageFail(A) \
	{(A) ? \
  	(LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail], \
  	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail)):\
  	(LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail], \
  	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail));\
  	lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail] = 1;}

/* static access macros for signal LIN_Valve_ND_CF_Valve1_HallsensorFail */
#define l_bool_rd_LIN_Valve_ND_CF_Valve1_HallsensorFail() \
   	(LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail], \
   	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail))
#define l_bool_wr_LIN_Valve_ND_CF_Valve1_HallsensorFail(A) \
	{(A) ? \
  	(LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail], \
  	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail)):\
  	(LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail], \
  	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail));\
  	lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail] = 1;}

/* static access macros for signal LIN_Valve_ND_CF_Valve1_LINCommFail */
#define l_bool_rd_LIN_Valve_ND_CF_Valve1_LINCommFail() \
   	(LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail], \
   	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail))
#define l_bool_wr_LIN_Valve_ND_CF_Valve1_LINCommFail(A) \
	{(A) ? \
  	(LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail], \
  	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail)):\
  	(LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail], \
  	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail));\
  	lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail] = 1;}

/* static access macros for signal LIN_Valve_ND_CR_Bms_Req_HeaderID */
#define l_u8_rd_LIN_Valve_ND_CR_Bms_Req_HeaderID() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID] & \
    (((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Bms_Req_HeaderID) - 1) \
    << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID )) >> LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID))
#define l_u8_wr_LIN_Valve_ND_CR_Bms_Req_HeaderID(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Bms_Req_HeaderID) - 1) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Bms_Req_HeaderID) - 1) & (A)) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID] = 1;}

/* static access macros for signal LIN_Valve_ND_CF_Valve_Acting_now */
#define l_bool_rd_LIN_Valve_ND_CF_Valve_Acting_now() \
   	(LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now], \
   	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now))
#define l_bool_wr_LIN_Valve_ND_CF_Valve_Acting_now(A) \
	{(A) ? \
  	(LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now], \
  	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now)):\
  	(LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now], \
  	LIN_BIT_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now));\
  	lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now] = 1;}

/* static access macros for signal LIN_Valve_ND_CR_Valve_Status_FB */
#define l_u8_rd_LIN_Valve_ND_CR_Valve_Status_FB() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB] & \
    (((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve_Status_FB) - 1) \
    << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB )) >> LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB))
#define l_u8_wr_LIN_Valve_ND_CR_Valve_Status_FB(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve_Status_FB) - 1) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LIN_Valve_ND_CR_Valve_Status_FB) - 1) & (A)) << LIN_BIT_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB] = 1;}



/* Signal flag APIs */


#define l_flg_tst_LIN_Valve_ND_CR_Bms_cmdAngle_Valve_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve)
#define l_flg_clr_LIN_Valve_ND_CR_Bms_cmdAngle_Valve_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Bms_cmdAngle_Valve)

#define l_flg_tst_LIN_Valve_ND_CR_Bms_Angle_Valve_FB_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB)
#define l_flg_clr_LIN_Valve_ND_CR_Bms_Angle_Valve_FB_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Angle_Valve_FB)

#define l_flg_tst_LIN_Valve_ND_CR_Valve1_Res1_HeaderID_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID)
#define l_flg_clr_LIN_Valve_ND_CR_Valve1_Res1_HeaderID_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res1_HeaderID)

#define l_flg_tst_LIN_Valve_ND_CR_Valve1_Status_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Status],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Status)
#define l_flg_clr_LIN_Valve_ND_CR_Valve1_Status_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Status],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Status)

#define l_flg_tst_LIN_Valve_ND_CR_Valve1_AuxVolt_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt)
#define l_flg_clr_LIN_Valve_ND_CR_Valve1_AuxVolt_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_AuxVolt)

#define l_flg_tst_LIN_Valve_ND_CR_Valve1_OperAngle_FB_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB)
#define l_flg_clr_LIN_Valve_ND_CR_Valve1_OperAngle_FB_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_OperAngle_FB)

#define l_flg_tst_LIN_Valve_ND_CR_Valve_BmsCmdAngle_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle)
#define l_flg_clr_LIN_Valve_ND_CR_Valve_BmsCmdAngle_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve_BmsCmdAngle)

#define l_flg_tst_LIN_Valve_ND_CR_Valve1_Res2_HeaderID_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID)
#define l_flg_clr_LIN_Valve_ND_CR_Valve1_Res2_HeaderID_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_Res2_HeaderID)

#define l_flg_tst_LIN_Valve_ND_CR_Valve1_HWversion_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion)
#define l_flg_clr_LIN_Valve_ND_CR_Valve1_HWversion_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_HWversion)

#define l_flg_tst_LIN_Valve_ND_CR_Valve1_SWversion_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion)
#define l_flg_clr_LIN_Valve_ND_CR_Valve1_SWversion_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve1_SWversion)

#define l_flg_tst_LIN_Valve_ND_CF_Valve1_LowVoltageFail_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail)
#define l_flg_clr_LIN_Valve_ND_CF_Valve1_LowVoltageFail_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LowVoltageFail)

#define l_flg_tst_LIN_Valve_ND_CF_Valve1_OverVoltageFail_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail)
#define l_flg_clr_LIN_Valve_ND_CF_Valve1_OverVoltageFail_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_OverVoltageFail)

#define l_flg_tst_LIN_Valve_ND_CF_Valve1_HallsensorFail_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail)
#define l_flg_clr_LIN_Valve_ND_CF_Valve1_HallsensorFail_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_HallsensorFail)

#define l_flg_tst_LIN_Valve_ND_CF_Valve1_LINCommFail_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail)
#define l_flg_clr_LIN_Valve_ND_CF_Valve1_LINCommFail_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve1_LINCommFail)

#define l_flg_tst_LIN_Valve_ND_CR_Bms_Req_HeaderID_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID)
#define l_flg_clr_LIN_Valve_ND_CR_Bms_Req_HeaderID_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Bms_Req_HeaderID)

#define l_flg_tst_LIN_Valve_ND_CF_Valve_Acting_now_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now)
#define l_flg_clr_LIN_Valve_ND_CF_Valve_Acting_now_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CF_Valve_Acting_now)

#define l_flg_tst_LIN_Valve_ND_CR_Valve_Status_FB_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB)
#define l_flg_clr_LIN_Valve_ND_CR_Valve_Status_FB_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_CR_Valve_Status_FB)



/* Frame flag APIs */

#define l_flg_tst_LIN_Valve_ND_Valve1_Batt_Res1_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_Valve1_Batt_Res1],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_Valve1_Batt_Res1)
#define l_flg_clr_LIN_Valve_ND_Valve1_Batt_Res1_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_Valve1_Batt_Res1],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_Valve1_Batt_Res1)

#define l_flg_tst_LIN_Valve_ND_Valve1_Batt_Res2_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_Valve1_Batt_Res2],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_Valve1_Batt_Res2)
#define l_flg_clr_LIN_Valve_ND_Valve1_Batt_Res2_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_Valve1_Batt_Res2],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_Valve1_Batt_Res2)

#define l_flg_tst_LIN_Valve_ND_BMS_Act_Req_Valve1_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_BMS_Act_Req_Valve1],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_BMS_Act_Req_Valve1)
#define l_flg_clr_LIN_Valve_ND_BMS_Act_Req_Valve1_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LIN_Valve_ND_BMS_Act_Req_Valve1],\
         LIN_FLAG_BIT_OFFSET_LIN_Valve_ND_BMS_Act_Req_Valve1)



/* INTERFACE MANAGEMENT */

#define l_ifc_init_LIN_Valve_ND() l_ifc_init(LIN_Valve_ND)



#define l_ifc_wake_up_LIN_Valve_ND() l_ifc_wake_up(LIN_Valve_ND)



#define l_ifc_rx_LIN_Valve_ND() l_ifc_rx(LIN_Valve_ND)



#define l_ifc_tx_LIN_Valve_ND() l_ifc_tx(LIN_Valve_ND)



#define l_ifc_aux_LIN_Valve_ND() l_ifc_aux(LIN_Valve_ND)



#define l_ifc_read_status_LIN_Valve_ND() l_ifc_read_status(LIN_Valve_ND)


#endif    /* _LIN_CFG_H_ */